#include "ToolDef.h"
#include "ACLog.h"
#include "ACString.h"
#include "ToolTree.h"
#include "ToolExcel.h"

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
	m_mapKeyToTreeInfo.clear();
	InitMapKeyToTreeInfo();
}

SItemExcelDB::~SItemExcelDB()
{
	SortDB();
	SaveDB();
	m_mapCNameToColumn.clear();
}

int SItemExcelDB::InitMapNameToColumn()
{
	ACCHECK(m_pExcel->GetSheetCount() > 0);

	int nFirstSheet = 0;
	if(m_pExcel->GetRowCount(nFirstSheet) < m_nHeadRow)
	{
		ERROR_MSG("Require head,excel:%s",CStringToStlString(m_strFilePath).c_str());
		return -1;
	}

	int nColTotal = m_pExcel->GetColumnCount(nFirstSheet);
	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		CString str = m_pExcel->GetCell(nFirstSheet,m_nHeadRow,nCol);
		if(str.IsEmpty())
			continue;

		m_mapCNameToColumn.insert(std::make_pair(str,nCol));
	}

	return 0;
}

int SItemExcelDB::InitMapKeyToTreeInfo()
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
		int nRowTotal = m_pExcel->GetRowCount(nSheet);
		for(int nRow = m_nDataRow; nRow < nRowTotal; ++nRow)
		{
			CString str = m_pExcel->GetCell(nSheet,nRow,nKeyCol);
			int nKey = atoi(CStringToStlString(str).c_str());
			str = m_pExcel->GetCell(nSheet,nRow,nDesCol);
			m_mapKeyToTreeInfo.insert(std::make_pair(nKey,STreeItemInfo(nKey,str,nSheet,nRow)));
		}
	}
	return 0;
}

int SItemExcelDB::CtrlToDB(SItemTab* pItemTab,int key)
{
	MapKeyToTreeInfoT::iterator iter = m_mapKeyToTreeInfo.find(key);
	if(iter == m_mapKeyToTreeInfo.end())
	{
		ERROR_MSG("CtrlToDB,can't find key:%d",key);
		return -1;
	}

	STreeItemInfo& rTreeItemInfo = iter->second;
	int nSheet = rTreeItemInfo.m_nSheet;
	int nRow = rTreeItemInfo.m_nRow;

	for(size_t i = 0; i < pItemTab->m_vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = pItemTab->m_vtCtrls[i];
		ACCHECK(pCtrl);

		int nCtrlCol = m_mapCNameToColumn[pCtrl->strCName];

		CString strDBVal;
		switch(pCtrl->nCtrl)
		{
		case CTRL_EDIT:
			EditToData((SEdit*)pCtrl,strDBVal);
			break;
		case CTRL_CHECK:
			CheckToData((SCheck*)pCtrl,strDBVal);
			break;
		case CTRL_COMBOBOX:
			ComboboxToData((SCombobox*)pCtrl,strDBVal);
			break;
		case CTRL_CHECKCOMBO:
			CheckComboToData((SCheckCombo*)pCtrl,strDBVal);
			break;
		default:
			continue;
		}

		m_pExcel->SetCell(nSheet,nRow,nCtrlCol,strDBVal);

		if(pCtrl->strCName == m_strDesCName)
		{
			rTreeItemInfo.m_strDes = strDBVal;
		}
	}

	return SaveDB();
}

int SItemExcelDB::DBToCtrl(SItemTab* pItemTab,int key)
{
	MapKeyToTreeInfoT::iterator iter = m_mapKeyToTreeInfo.find(key);
	if(iter == m_mapKeyToTreeInfo.end())
	{
		ERROR_MSG("DBToCtrl,can't find key:%d",key);
		return -1;
	}

	int nSheet = iter->second.m_nSheet;
	int nRow = iter->second.m_nRow;

	for(size_t i = 0; i < pItemTab->m_vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = pItemTab->m_vtCtrls[i];
		ACCHECK(pCtrl);

		int nCtrlCol = m_mapCNameToColumn[pCtrl->strCName];
		CString strDBVal = m_pExcel->GetCell(nSheet,nRow,nCtrlCol);

		switch(pCtrl->nCtrl)
		{
		case CTRL_EDIT:
			DataToEdit((SEdit*)pCtrl,strDBVal);
			break;
		case CTRL_CHECK:
			DataToCheck((SCheck*)pCtrl,strDBVal);
			break;
		case CTRL_COMBOBOX:
			DataToCombobox((SCombobox*)pCtrl,strDBVal);
			break;
		case CTRL_CHECKCOMBO:
			DataToCheckCombo((SCheckCombo*)pCtrl,strDBVal);
			break;
		default:
			continue;
		}
	}

	return 0;
}

int SItemExcelDB::DataToEdit(SEdit* pCtrl,CString data)
{
	if(pCtrl->nType == DATA_INT)
	{
		int nData = atoi(CStringToStlString(data).c_str());
		data.Format(_T("%d"),nData);
	}

	pCtrl->pCtrl->SetWindowText(data);
	return 0;
}

int SItemExcelDB::EditToData(SEdit* pCtrl,CString& data)
{
	CString strCtrlVal;
	pCtrl->pCtrl->GetWindowText(strCtrlVal);
	if(pCtrl->nType == DATA_INT)
	{
		int nData = atoi(CStringToStlString(strCtrlVal).c_str());
		data.Format(_T("%d"),nData);
	}
	else if(pCtrl->nType == DATA_FLOAT)
	{
		double fData = atof(CStringToStlString(strCtrlVal).c_str());
		data.Format(_T("%.6lf"),fData);
	}
	else
		data = strCtrlVal;

	return 0;
}

int SItemExcelDB::DataToCheck(SCheck* pCtrl,CString data)
{
	bool val = false;
	if(stricmp(CStringToStlString(data).c_str(),"false") == 0)
		val = false;
	else if(stricmp(CStringToStlString(data).c_str(),"true") == 0)
		val = true;
	else
		val = atoi(CStringToStlString(data).c_str());

	pCtrl->pCtrl->SetCheck(val);
	return 0;
}

int SItemExcelDB::CheckToData(SCheck* pCtrl,CString& data)
{
	bool val = pCtrl->pCtrl->GetCheck();
	data = val ? _T("true") : _T("false");
	return 0;
}

int SItemExcelDB::DataToCombobox(SCombobox* pCtrl,CString data)
{
	int dbVal = atoi(CStringToStlString(data).c_str());
	for(size_t ctlItem = 0; ctlItem < pCtrl->vtItems.size(); ++ctlItem)
	{
		if(dbVal == pCtrl->vtItems[ctlItem].nValue)
		{
			pCtrl->pCtrl->SetCurSel(ctlItem);
			break;
		}
	}
	return 0;
}

int SItemExcelDB::ComboboxToData(SCombobox* pCtrl,CString& data)
{
	int curSel = pCtrl->pCtrl->GetCurSel();
	data.Format(_T("%d"),pCtrl->vtItems[curSel].nValue);
	return 0;
}

int SItemExcelDB::DataToCheckCombo(SCheckCombo* pCtrl,CString data)
{
	std::vector<CString> vtItems;
	CString strItemText;
	int nPos = 0;
	//数组通过,隔开
	strItemText = data.Tokenize(EXCEL_ARRAY_DELIMITER,nPos);
	while (strItemText != _T(""))
	{
		vtItems.push_back(strItemText);
		strItemText = data.Tokenize(EXCEL_ARRAY_DELIMITER, nPos);
	};

	pCtrl->pCtrl->SelectAll(FALSE);

	for(size_t dbItem = 0; dbItem < vtItems.size(); ++dbItem)
	{
		int dbVal = atoi(CStringToStlString(vtItems[dbItem]).c_str());
		for(size_t ctlItem = 0; ctlItem < pCtrl->vtItems.size(); ++ctlItem)
		{
			if(dbVal == pCtrl->vtItems[ctlItem].nValue)
				pCtrl->pCtrl->SetCheck(ctlItem,true);
		}
	}
	return 0;
}

int SItemExcelDB::CheckComboToData(SCheckCombo* pCtrl,CString& data)
{
	bool bFirst = true;
	for(size_t ctrlItem = 0; ctrlItem < pCtrl->vtItems.size(); ++ctrlItem)
	{
		if(pCtrl->pCtrl->GetCheck(ctrlItem))
		{
			if(bFirst)
				bFirst = false;
			else
				data.Append(EXCEL_ARRAY_DELIMITER);

			data.AppendFormat(_T("%d"),pCtrl->vtItems[ctrlItem].nValue);
		}
	}
	return 0;
}

int SItemExcelDB::DBToTree(ToolTree* pTree)
{
	pTree->DeleteAllItems();
	pTree->InsertUndefinedRoot();

	MapKeyToTreeInfoT::iterator it,ed;
	for(it = m_mapKeyToTreeInfo.begin(),ed = m_mapKeyToTreeInfo.end(); it != ed; ++it)
	{
		STreeItemInfo& rTreeItemInfo = it->second;
		pTree->InsertItem(rTreeItemInfo.m_nKey,rTreeItemInfo.m_strDes);
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
	m_pExcel->SaveAllSheet();
	return 0;
}

int SItemExcelDB::GetKeyInExcel(int sheet,int row)
{
	int nKeyCol = m_mapCNameToColumn[m_strKeyCName];
	CString strKey = m_pExcel->GetCell(sheet,row,nKeyCol);
	return atoi(CStringToStlString(strKey).c_str());
}

int SItemExcelDB::InsertNewKey(int key)
{
	if(key <= 0)
		return -1;

	if(m_mapKeyToTreeInfo.find(key) != m_mapKeyToTreeInfo.end())
		return -1;

	int nSheet = 0;
	int nRow = m_pExcel->AddRow(nSheet);

	m_mapKeyToTreeInfo.insert(std::make_pair(key,STreeItemInfo(key,_T(""),nSheet,nRow)));

	return key;
}

int SItemExcelDB::DeleteByKey(int key)
{
	if(key <= 0)
		return -1;

	MapKeyToTreeInfoT::iterator iter = m_mapKeyToTreeInfo.find(key);
	if(iter == m_mapKeyToTreeInfo.end())
		return -1;

	STreeItemInfo& rTreeItemInfo = iter->second;
	m_pExcel->DeleteRow(rTreeItemInfo.m_nSheet,rTreeItemInfo.m_nRow);

	SaveDB();

	int nKey = -1;
	MapKeyToTreeInfoT::iterator iterNext = m_mapKeyToTreeInfo.erase(iter);
	if(iterNext != m_mapKeyToTreeInfo.end())
		nKey = iterNext->first;

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