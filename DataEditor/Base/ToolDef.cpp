#include "stdafx.h"
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
	int nDesCol = m_mapCNameToColumn[m_strDesCName];

	std::vector<int> vtLayerCol;
	for(size_t i = 0; i < m_vtLayerCName.size(); ++i)
	{
		vtLayerCol.push_back(m_mapCNameToColumn[m_vtLayerCName[i]]);
	}

	int nSheetTotal = m_pExcel->GetSheetCount();
	for(int nSheet = 0; nSheet < nSheetTotal; ++nSheet)
	{
		int nRowTotal = m_pExcel->GetUsedRowCount(nSheet);
		for(int nRow = m_nDataRow; nRow < nRowTotal; ++nRow)
		{
			STreeItemInfo info;
			CString strKey = m_pExcel->GetCellText(nSheet,nRow,nKeyCol);
			info.m_nKey = atoi(CStringToStlString(strKey).c_str());
			info.m_strDes = m_pExcel->GetCellText(nSheet,nRow,nDesCol);
			info.m_nSheet = nSheet;

			for(size_t i = 0; i < vtLayerCol.size(); ++i)
			{
				CString strLayerValue = m_pExcel->GetCellText(nSheet,nRow,vtLayerCol[i]);
				if(strLayerValue.IsEmpty())
					continue;

				info.m_vtLayers.push_back(strLayerValue);
			}

			m_vtTreeItemInfos.push_back(info);
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
	STreeItemInfo info;
	info.m_nKey = key;
	result.second = std::lower_bound(m_vtTreeItemInfos.begin(),m_vtTreeItemInfos.end(),info,CompareTreeInfo());

	if(result.second == m_vtTreeItemInfos.end() || result.second->m_nKey != key)
		result.first = false;
	else
		result.first = true;

	return result;
}

int SItemExcelDB::Save(int key, MapCNameToValueT& mapValues)
{
	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(!findResult.first)
	{
		ERROR_MSG("Save,can't find key:%d",key);
		return -1;
	}

	STreeItemInfo& rTreeItemInfo = *(findResult.second);
	int nSheet = rTreeItemInfo.m_nSheet;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;

	for(MapCNameToValueT::iterator iter = mapValues.begin(); iter != mapValues.end(); ++iter)
	{
		int nCtrlCol = m_mapCNameToColumn[iter->first];
		m_pExcel->SetCellText(nSheet,nRow,nCtrlCol,iter->second);
	}

	SaveDB();

	// 更新tree
	STreeItemInfo infoNew = GetTreeItemInfo(nSheet,nRow);
	ACCHECK(infoNew.m_nKey == rTreeItemInfo.m_nKey);

	if(infoNew.m_vtLayers != rTreeItemInfo.m_vtLayers)
	{
		ToolApp::Instance().GetMainTree()->DeleteItemByKey(infoNew.m_nKey);
		ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
		ToolApp::Instance().GetMainTree()->SelectKey(key);
	}
	else if(infoNew.m_strDes != rTreeItemInfo.m_strDes)
	{
		ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
	}

	rTreeItemInfo = infoNew;
	return 0;
}

int SItemExcelDB::UpdateTreeItemInfo(STreeItemInfo& rTreeItemInfo,MapCNameToValueT& mapValues,bool bForcedUpdateTree /* = false */)
{
	bool bChanged = false;
	for(MapCNameToValueT::iterator iter = mapValues.begin(); iter != mapValues.end(); ++iter)
	{
		// 更新描述信息
		if(iter->first == m_strDesCName)
		{
			if(rTreeItemInfo.m_strDes == iter->second)
				bChanged = true;

			rTreeItemInfo.m_strDes = iter->second;
		}

		// 更新层级信息
	}

	ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(rTreeItemInfo.m_nKey,rTreeItemInfo.m_strDes,rTreeItemInfo.m_vtLayers);
	return 0;
}

int SItemExcelDB::Load(int key, MapCNameToValueT& mapValues)
{
	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(!findResult.first)
	{
		ERROR_MSG("Load,can't find key:%d",key);
		return -1;
	}

	STreeItemInfo& rTreeItemInfo = *(findResult.second);
	int nSheet = rTreeItemInfo.m_nSheet;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;

	for(MapCNameToColumnT::iterator iter = m_mapCNameToColumn.begin(); iter != m_mapCNameToColumn.end(); ++iter)
	{
		CString strDBVal = m_pExcel->GetCellText(nSheet,nRow,iter->second);
		mapValues.insert(std::make_pair(iter->first,strDBVal));
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

	//pTree->ExpandAllItems();
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

int SItemExcelDB::InsertByKey(int key, MapCNameToValueT& mapValues)
{
	if(key <= 0)
		return -1;

	PairTreeInfoFoundT findResult = FindTreeInfoByKey(key);
	if(findResult.first)
		return -1;

	int nSheet = 0;
	int nRow = findResult.second - m_vtTreeItemInfos.begin() + m_nDataRow;

	std::vector<CString> vtValues;
	int nColTotal = m_pExcel->GetUsedRowCount(nSheet);
	vtValues.reserve(nColTotal);
	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		vtValues.push_back(_T(" "));
	}

	for(MapCNameToValueT::iterator iter = mapValues.begin(); iter != mapValues.end(); ++iter)
	{
		MapCNameToColumnT::iterator iterCNToCol = m_mapCNameToColumn.find(iter->first);
		if(iterCNToCol == m_mapCNameToColumn.end())
			continue;

		int nCol = iterCNToCol->second;
		ACCHECK(nCol >= 0 && nCol < nColTotal);
		vtValues[nCol] = iter->second;
	}

	m_pExcel->InsertRow(nSheet,nRow,vtValues);
	SaveDB();

	STreeItemInfo infoNew = GetTreeItemInfo(nSheet,nRow);
	m_vtTreeItemInfos.insert(findResult.second,infoNew);

	// 更新tree
	ToolApp::Instance().GetMainTree()->UpdateOrInsertItemByKey(infoNew.m_nKey,infoNew.m_strDes,infoNew.m_vtLayers);
	ToolApp::Instance().GetMainTree()->SelectKey(key);

	return key;
}

STreeItemInfo SItemExcelDB::GetTreeItemInfo(int nSheet,int nRow)
{
	int nKeyCol = m_mapCNameToColumn[m_strKeyCName];
	int nDesCol = m_mapCNameToColumn[m_strDesCName];

	std::vector<int> vtLayerCol;
	for(size_t i = 0; i < m_vtLayerCName.size(); ++i)
	{
		vtLayerCol.push_back(m_mapCNameToColumn[m_vtLayerCName[i]]);
	}

	STreeItemInfo info;
	CString strKey = m_pExcel->GetCellText(nSheet,nRow,nKeyCol);
	info.m_nKey = atoi(CStringToStlString(strKey).c_str());
	info.m_strDes = m_pExcel->GetCellText(nSheet,nRow,nDesCol);
	info.m_nSheet = nSheet;

	for(size_t i = 0; i < vtLayerCol.size(); ++i)
	{
		CString strLayerValue = m_pExcel->GetCellText(nSheet,nRow,vtLayerCol[i]);
		if(strLayerValue.IsEmpty())
			continue;

		info.m_vtLayers.push_back(strLayerValue);
	}

	return info;
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

	int nNextKey = -1;
	VectorTreeItemInfoT::iterator iterNext = m_vtTreeItemInfos.erase(findResult.second);
	if(iterNext != m_vtTreeItemInfos.end())
		nNextKey = iterNext->m_nKey;

	// 更新tree
	ToolApp::Instance().GetMainTree()->DeleteItemByKey(key);
	ToolApp::Instance().GetMainTree()->SelectKey(nNextKey);

	return nNextKey;
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

int SItemTab::CtrlToDB(int key)
{
	MapCNameToValueT mapValues;
	GetAllCtrlValues(mapValues);
	m_pDB->Save(key,mapValues);
	return 0;
}

int SItemTab::DBToCtrl(int key)
{
	MapCNameToValueT mapValues;
	m_pDB->Load(key,mapValues);
	SetAllCtrlValues(mapValues);
	return 0;
}

int SItemTab::GetAllCtrlValues(MapCNameToValueT& mapValues)
{
	for(size_t i = 0; i < m_vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = m_vtCtrls[i];
		CString strValue;
		pCtrl->CtrlToData(strValue);
		mapValues.insert(std::make_pair(pCtrl->strCName,strValue));
	}
	return 0;
}

int SItemTab::SetAllCtrlValues(MapCNameToValueT& mapValues)
{
	for(size_t i = 0; i < m_vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = m_vtCtrls[i];
		MapCNameToValueT::iterator iter = mapValues.find(pCtrl->strCName);
		if(iter != mapValues.end())
		{
			pCtrl->DataToCtrl(iter->second);
		}
	}
	return 0;
}

END_NS_AC