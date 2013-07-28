#include "ToolDef.h"
#include "ACLog.h"
#include "ACString.h"
#include "ToolTree.h"
#include "ToolExcel.h"
#include <algorithm>

BEGIN_NS_AC

//-----------------------------------------------------

SItemDB::SItemDB(int type,const CString& path,const CString& key,const CString& des)
{
	m_nDBType = type;
	m_strFilePath = path;
	m_strKeyCName = key;
	m_strDesCName = des;
}

SItemExcelDB::SItemExcelDB(const CString& path,const CString& key, const CString& des,int headRow,int dataRow)
: SItemDB(DB_EXCEL,path,key,des)
{
	m_pExcel = ToolApp::Instance().GetExcelTool()->OpenWorkbook(path);
	ACCHECK(m_pExcel);

	m_nHeadRow = headRow-1;
	m_nDataRow = dataRow-1;

	m_mapCNameToColumn.clear();
	InitMapNameToColumn();

	SortDB();

	m_vtTreeItemInfos.clear();
	InitTreeItemInfos();
}

SItemExcelDB::~SItemExcelDB()
{
	SaveDB();
	m_mapCNameToColumn.clear();
}

int SItemExcelDB::InitMapNameToColumn()
{
	ACCHECK(m_pExcel->GetSheetCount() > 0);

	int nFirstSheet = 0;
	if(m_pExcel->GetUsedRowCount(nFirstSheet) < m_nHeadRow)
	{
		ERROR_MSG("Require head,excel:%s",CStringToStlString(m_strFilePath).c_str());
		return -1;
	}

	int nColTotal = m_pExcel->GetUsedColumnCount(nFirstSheet);
	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		CString str = m_pExcel->GetCellText(nFirstSheet,m_nHeadRow,nCol);
		if(str.IsEmpty())
			continue;

		m_mapCNameToColumn.insert(std::make_pair(str,nCol));
	}

	return 0;
}

int SItemExcelDB::InitTreeItemInfos()
{
	MapCNameToColumnT::iterator iter = m_mapCNameToColumn.find(m_strKeyCName);
	if(iter == m_mapCNameToColumn.end())
	{
		ERROR_MSG("DBToTree failed,excel:%s,key:%s",CStringToStlString(m_strFilePath).c_str(),CStringToStlString(m_strKeyCName).c_str());
		return -1;
	}
	int nKeyCol = iter->second;

	int nDesCol = 1;
	iter = m_mapCNameToColumn.find(m_strDesCName);
	if(iter == m_mapCNameToColumn.end())
	{
		WARN_MSG("DBToTree no des,excel:%s,des:%s",CStringToStlString(m_strFilePath).c_str(),CStringToStlString(m_strDesCName).c_str());
	}
	else
		nDesCol = iter->second;

	int nSheetTotal = m_pExcel->GetSheetCount();
	for(int nSheet = 0; nSheet < nSheetTotal; ++nSheet)
	{
		int nRowTotal = m_pExcel->GetUsedRowCount(nSheet);
		for(int nRow = m_nDataRow; nRow < nRowTotal; ++nRow)
		{
			CString str = m_pExcel->GetCellText(nSheet,nRow,nKeyCol);
			int nKey = atoi(CStringToStlString(str).c_str());
			str = m_pExcel->GetCellText(nSheet,nRow,nDesCol);
			m_vtTreeItemInfos.push_back(STreeItemInfo(nKey,str,nSheet));
		}
	}
	return 0;
}

struct CompareTreeInfo
{
	bool operator()(const STreeItemInfo& lh,const STreeItemInfo& rh)
	{
		return lh.m_nKey < rh.m_nKey;
	}
};

PairTreeInfoFoundT SItemExcelDB::FindTreeInfoByKey(int key)
{
	PairTreeInfoFoundT result;
	result.second = std::lower_bound(m_vtTreeItemInfos.begin(),m_vtTreeItemInfos.end(),STreeItemInfo(key,_T(""),-1),CompareTreeInfo());

	if(result.second == m_vtTreeItemInfos.end() || result.second->m_nKey != key)
		result.first = false;
	else
		result.first = true;

	return result;
}

int SItemExcelDB::CtrlToDB(SItemTab* pItemTab,int key)
{
	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(!findResult.first)
	{
		ERROR_MSG("CtrlToDB,can't find key:%d",key);
		return -1;
	}

	STreeItemInfo& rTreeItemInfo = *(findResult.second);
	int nSheet = rTreeItemInfo.m_nSheet;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;

	for(size_t i = 0; i < pItemTab->m_vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = pItemTab->m_vtCtrls[i];
		ACCHECK(pCtrl);

		int nCtrlCol = m_mapCNameToColumn[pCtrl->strCName];

		CString strDBVal;
		pCtrl->CtrlToData(strDBVal);

		m_pExcel->SetCellText(nSheet,nRow,nCtrlCol,strDBVal);
	}

	SaveDB();
	UpdateTreeItemInfo(pItemTab,rTreeItemInfo);
	return 0;
}

int SItemExcelDB::UpdateTreeItemInfo(SItemTab* pItemTab,STreeItemInfo& rTreeItemInfo)
{
	for(size_t i = 0; i < pItemTab->m_vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = pItemTab->m_vtCtrls[i];
		ACCHECK(pCtrl);

		CString strDBVal;
		pCtrl->CtrlToData(strDBVal);
		if(pCtrl->strCName == m_strDesCName)
		{
			rTreeItemInfo.m_strDes = strDBVal;
		}
	}

	ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(rTreeItemInfo.m_nKey,rTreeItemInfo.m_strDes,rTreeItemInfo.m_vtLayers);
	return 0;
}

int SItemExcelDB::DBToCtrl(SItemTab* pItemTab,int key)
{
	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(!findResult.first)
	{
		ERROR_MSG("DBToCtrl,can't find key:%d",key);
		return -1;
	}

	STreeItemInfo& rTreeItemInfo = *(findResult.second);
	int nSheet = rTreeItemInfo.m_nSheet;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;

	for(size_t i = 0; i < pItemTab->m_vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = pItemTab->m_vtCtrls[i];
		ACCHECK(pCtrl);

		int nCtrlCol = m_mapCNameToColumn[pCtrl->strCName];
		CString strDBVal = m_pExcel->GetCellText(nSheet,nRow,nCtrlCol);
		pCtrl->DataToCtrl(strDBVal);
	}

	return 0;
}

int SItemExcelDB::DBToTree(ToolTree* pTree)
{
	pTree->DeleteAllItems();
	pTree->InsertUndefinedRoot();

	VectorTreeItemInfoT::iterator it,ed;
	for(it = m_vtTreeItemInfos.begin(),ed = m_vtTreeItemInfos.end(); it != ed; ++it)
	{
		STreeItemInfo& rTreeItemInfo = *it;
		pTree->UpdateOrInsertItemByKey(rTreeItemInfo.m_nKey,rTreeItemInfo.m_strDes,rTreeItemInfo.m_vtLayers);
	}

	pTree->ExpandAllItems();
	pTree->UpdatedItems();
	pTree->ResetSelectKey();
	return 0;
}

int SItemExcelDB::SortDB()
{
	int nKeyCol = m_mapCNameToColumn[m_strKeyCName];
	m_pExcel->SortAllSheetByColumn(nKeyCol,m_nDataRow);
	return 0;
}

int SItemExcelDB::SaveDB()
{
	m_pExcel->Save();
	return 0;
}

int SItemExcelDB::GetKeyInExcel(int sheet,int row)
{
	int nKeyCol = m_mapCNameToColumn[m_strKeyCName];
	CString strKey = m_pExcel->GetCellText(sheet,row,nKeyCol);
	return atoi(CStringToStlString(strKey).c_str());
}

int SItemExcelDB::InsertNewKey(int key)
{
	if(key <= 0)
		return -1;

	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(findResult.first)
		return -1;

	int nSheet = 0;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;
	m_pExcel->InsertEmptyRow(nSheet,nRow);
	m_vtTreeItemInfos.insert(findResult.second,STreeItemInfo(key,_T(""),nSheet));

	return key;
}

int SItemExcelDB::DeleteByKey(int key)
{
	if(key <= 0)
		return -1;

	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(!findResult.first)
		return -1;

	STreeItemInfo& rTreeItemInfo = *(findResult.second);
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;
	m_pExcel->DeleteRow(rTreeItemInfo.m_nSheet,nRow);

	SaveDB();

	int nKey = -1;
	VectorTreeItemInfoT::iterator iterNext = m_vtTreeItemInfos.erase(findResult.second);
	if(iterNext != m_vtTreeItemInfos.end())
		nKey = iterNext->m_nKey;

	// ¸üÐÂtree
	ToolApp::Instance().GetMainTree()->DeleteItemByKey(key);

	return nKey;
}

//-----------------------------------------------------

SItemTab::SItemTab()
{
	m_strName = _T("");
	m_strCName = _T("");
	m_pDB = NULL;
	m_vtCtrls.clear();
	m_pWnd = NULL;
	m_pKeyWnd = NULL;
	m_nLastSelKey = -1;
}

SItemTab::~SItemTab()
{
	_safe_delete(m_pDB);
	_safe_delete(m_pWnd);
	for(size_t i = 0; i < m_vtCtrls.size(); ++i)
		_safe_delete(m_vtCtrls[i]);

	m_vtCtrls.clear();
}

int SItemTab::LoadDefaultValues()
{
	for(size_t i = 0; i < m_vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = m_vtCtrls[i];
		pCtrl->LoadDefaultValue();
	}
	return 0;
}

END_NS_AC