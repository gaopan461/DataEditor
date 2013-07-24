#include "ToolDef.h"
#include "ACLog.h"
#include "ACString.h"
#include "ToolTree.h"

BEGIN_NS_AC

int GetCellContent(const BasicExcelCell* pCell, CString& strContent)
{
	ACCHECK(pCell);
	switch(pCell->Type())
	{
	case BasicExcelCell::UNDEFINED:
		strContent = _T("");
		return -1;
		break;
	case BasicExcelCell::INT:
		strContent.Format(_T("%d"),pCell->GetInteger());
		break;
	case BasicExcelCell::DOUBLE:
		strContent.Format(_T("%.6lf"),pCell->GetDouble());
		break;
	case BasicExcelCell::STRING:
		{
			std::string stlstr = pCell->GetString();
			strContent = StlStringToCString(stlstr);
		}
		break;
	case BasicExcelCell::WSTRING:
		strContent = pCell->GetWString();
		break;
	}

	strContent.Trim();
	return 0;
}

int SetCellContent(BasicExcelCell* pCell, const CString& strContent)
{
	ACCHECK(pCell);
	if(strContent.IsEmpty())
		pCell->EraseContents();
	else
		pCell->Set(strContent);
	return 0;
	switch(pCell->Type())
	{
	case BasicExcelCell::UNDEFINED:
		return -1;
		break;
	case BasicExcelCell::INT:
		{
			int nVal = 0;
			if(stricmp(CStringToStlString(strContent).c_str(),"false") == 0)
				nVal = 0;
			else if(stricmp(CStringToStlString(strContent).c_str(),"true") == 0)
				nVal = 1;
			else
				nVal = atoi(CStringToStlString(strContent).c_str());
			pCell->Set(nVal);
		}
		break;
	case BasicExcelCell::DOUBLE:
		{
			float fVal = 0;
			if(stricmp(CStringToStlString(strContent).c_str(),"false") == 0)
				fVal = 0;
			else if(stricmp(CStringToStlString(strContent).c_str(),"true") == 0)
				fVal = 1;
			else
				fVal = atof(CStringToStlString(strContent).c_str());
			pCell->Set(fVal);
		}
		break;
	case BasicExcelCell::STRING:
		pCell->Set(CStringToStlString(strContent).c_str());
		break;
	case BasicExcelCell::WSTRING:
		pCell->Set(strContent);
		break;
	}
	
	return 0;
}

//-----------------------------------------------------

SItemDB::SItemDB(int type,const CString& path,const CString& key,const CString& des)
{
	nDBType = type;
	strFilePath = path;
	strKeyCName = key;
	strDesCName = des;
}

SItemExcelDB::SItemExcelDB(const CString& path,const CString& key, const CString& des,int headRow,int dataRow)
: SItemDB(DB_EXCEL,path,key,des)
{
	pExcel = new BasicExcel();
	if(!pExcel->Load(CStringToStlString(path).c_str()))
	{
		CString str;
		str.Format(_T("Load excel faild! excel:%s"),path);
		AfxMessageBox(str);
		ExitProcess(-1);
	}

	nHeadRow = headRow;
	nDataRow = dataRow;

	mapCNameToColumn.clear();
	InitMapNameToColumn();
	mapKeyToTreeInfo.clear();
	InitMapKeyToTreeInfo();
}

SItemExcelDB::~SItemExcelDB()
{
	SortDB();
	SaveDB();
	_safe_delete(pExcel);
	mapCNameToColumn.clear();
}

int SItemExcelDB::InitMapNameToColumn()
{
	ACCHECK(pExcel->GetTotalWorkSheets() > 0);

	size_t nSheet1Index = 0;
	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(nSheet1Index);
	ACCHECK(pSheet);

	if(pSheet->GetTotalRows() < nHeadRow)
	{
		ERROR_MSG("Require head,excel:%s",CStringToStlString(strFilePath).c_str());
		return -1;
	}

	for(size_t nCol = 0; nCol < pSheet->GetTotalCols(); ++nCol)
	{
		BasicExcelCell* pCell = pSheet->Cell(nHeadRow-1,nCol);
		ACCHECK(pCell);
		CString str;
		if(GetCellContent(pCell,str) != 0)
			INFO_MSG("Read cell failed,excel:%s,row:%d,col:%d",CStringToStlString(strFilePath).c_str(),nHeadRow,nCol);

		if(str.IsEmpty())
			continue;

		mapCNameToColumn.insert(std::make_pair(str,nCol));
	}

	return 0;
}

int SItemExcelDB::InitMapKeyToTreeInfo()
{
	MapCNameToColumnT::iterator iter = mapCNameToColumn.find(strKeyCName);
	if(iter == mapCNameToColumn.end())
	{
		ERROR_MSG("DBToTree failed,excel:%s,key:%s",CStringToStlString(strFilePath).c_str(),CStringToStlString(strKeyCName).c_str());
		return -1;
	}
	size_t nKeyCol = iter->second;

	size_t nDesCol = 1;
	iter = mapCNameToColumn.find(strDesCName);
	if(iter == mapCNameToColumn.end())
	{
		WARN_MSG("DBToTree no des,excel:%s,des:%s",CStringToStlString(strFilePath).c_str(),CStringToStlString(strDesCName).c_str());
	}
	else
		nDesCol = iter->second;


	for(size_t sheet = 0; sheet < pExcel->GetTotalWorkSheets(); ++sheet)
	{
		BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(sheet);
		ACCHECK(pSheet);

		for(size_t nRow = nDataRow; nRow < pSheet->GetTotalRows(); ++nRow)
		{
			BasicExcelCell* pCellKey = pSheet->Cell(nRow,nKeyCol);
			ACCHECK(pCellKey);
			CString str;
			if(GetCellContent(pCellKey,str) != 0)
			{
				//INFO_MSG("Unknown cell type,excel:%s,row:%d,col:%d",CStringToStlString(strFilePath).c_str(),nRow,nKeyCol);
				continue;
			}

			int nKey = atoi(CStringToStlString(str).c_str());

			BasicExcelCell* pCellDes = pSheet->Cell(nRow,nDesCol);
			ACCHECK(pCellDes);
			if(GetCellContent(pCellDes,str) != 0)
			{
				//INFO_MSG("Unknown cell type,excel:%s,row:%d,col:%d",CStringToStlString(strFilePath).c_str(),nRow,nDesCol);
				str = _T("");
			}

			mapKeyToTreeInfo.insert(std::make_pair(nKey,STreeItemInfo(nKey,str,sheet,nRow)));
		}
	}
	return 0;
}

int SItemExcelDB::CtrlToDB(SItemTab* pItemTab,int key)
{
	MapKeyToTreeInfoT::iterator iter = mapKeyToTreeInfo.find(key);
	if(iter == mapKeyToTreeInfo.end())
	{
		ERROR_MSG("CtrlToDB,can't find key:%d",key);
		return -1;
	}

	STreeItemInfo& rTreeItemInfo = iter->second;
	int nSheet = rTreeItemInfo.nSheet;
	int nRow = rTreeItemInfo.nRow;

	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(nSheet);
	ACCHECK(pSheet);

	for(size_t i = 0; i < pItemTab->vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = pItemTab->vtCtrls[i];
		ACCHECK(pCtrl);

		int nCtrlCol = mapCNameToColumn[pCtrl->strCName];
		BasicExcelCell* pCell = pSheet->Cell(nRow,nCtrlCol);
		ACCHECK(pCell);

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

		if(SetCellContent(pCell,strDBVal) != 0)
			INFO_MSG("Write cell failed,excel:%s,row:%d,col:%d",CStringToStlString(strFilePath).c_str(),nRow,nCtrlCol);

		if(pCtrl->strCName == strDesCName)
		{
			rTreeItemInfo.strDes = strDBVal;
		}
	}

	return SaveDB();
}

int SItemExcelDB::DBToCtrl(SItemTab* pItemTab,int key)
{
	MapKeyToTreeInfoT::iterator iter = mapKeyToTreeInfo.find(key);
	if(iter == mapKeyToTreeInfo.end())
	{
		ERROR_MSG("DBToCtrl,can't find key:%d",key);
		return -1;
	}

	int nSheet = iter->second.nSheet;
	int nRow = iter->second.nRow;

	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(nSheet);
	ACCHECK(pSheet);

	for(size_t i = 0; i < pItemTab->vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = pItemTab->vtCtrls[i];
		ACCHECK(pCtrl);

		int nCtrlCol = mapCNameToColumn[pCtrl->strCName];
		BasicExcelCell* pCell = pSheet->Cell(nRow,nCtrlCol);
		ACCHECK(pCell);
		CString strDBVal;
		GetCellContent(pCell,strDBVal);

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
		float fData = atof(CStringToStlString(strCtrlVal).c_str());
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
	for(it = mapKeyToTreeInfo.begin(),ed = mapKeyToTreeInfo.end(); it != ed; ++it)
	{
		STreeItemInfo& rTreeItemInfo = it->second;
		pTree->InsertItem(rTreeItemInfo.nKey,rTreeItemInfo.strDes);
	}

	pTree->ExpandAllItems();
	pTree->UpdatedItems();
	pTree->ResetSelectKey();
	return 0;
}

int SItemExcelDB::SortDB()
{
	std::vector<int> vtRowIdx;
	for(size_t i = 0; i < pExcel->GetTotalWorkSheets(); ++i)
	{
		vtRowIdx.push_back(nDataRow);
	}

	for(MapKeyToTreeInfoT::iterator iter1 = mapKeyToTreeInfo.begin(); iter1 != mapKeyToTreeInfo.end(); ++iter1)
	{
		STreeItemInfo& rTreeItemInfo1 = iter1->second;
		int nRow1 = rTreeItemInfo1.nRow;
		int nSheet = rTreeItemInfo1.nSheet;
		int nRow2 = vtRowIdx[nSheet];
		int nKey2 = GetKeyInExcel(nSheet,nRow2);

		SwapExcelRow(nSheet,nRow1,nRow2);
		rTreeItemInfo1.nRow = nRow2;

		MapKeyToTreeInfoT::iterator iter2 = mapKeyToTreeInfo.find(nKey2);
		if(iter2 != mapKeyToTreeInfo.end())
		{
			STreeItemInfo& rTreeItemInfo2 = iter2->second;
			ACCHECK(rTreeItemInfo2.nSheet == nSheet);
			rTreeItemInfo2.nRow = nRow1;
		}

		vtRowIdx[nSheet]++;
	}

	return 0;
}

int SItemExcelDB::SaveDB()
{
	if(!pExcel->Save())
	{
		ERROR_MSG("Save excel failed,excel:%s",CStringToStlString(strFilePath));
		return -1;
	}

	return 0;
}

int SItemExcelDB::GetKeyInExcel(int sheet,int row)
{
	int nKeyCol = mapCNameToColumn[strKeyCName];
	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(sheet);
	ACCHECK(pSheet);

	BasicExcelCell* pCell = pSheet->Cell(row,nKeyCol);
	ACCHECK(pSheet);

	CString strKey;
	GetCellContent(pCell,strKey);
	return atoi(CStringToStlString(strKey).c_str());
}

int SItemExcelDB::SwapExcelRow(int sheet,int row1,int row2)
{
	if(row1 == row2)
		return 0;

	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(sheet);
	ACCHECK(pSheet);

	CString strTmp1,strTmp2;
	for(size_t nCol = 0; nCol < pSheet->GetTotalCols(); ++nCol)
	{
		BasicExcelCell* pCell1 = pSheet->Cell(row1,nCol);
		ACCHECK(pCell1);
		BasicExcelCell* pCell2 = pSheet->Cell(row2,nCol);
		ACCHECK(pCell2);

		GetCellContent(pCell1,strTmp1);
		GetCellContent(pCell2,strTmp2);
		SetCellContent(pCell1,strTmp2);
		SetCellContent(pCell2,strTmp1);

	}
	return 0;
}

int SItemExcelDB::InsertNewKey(int key)
{
	if(key <= 0)
		return -1;

	if(mapKeyToTreeInfo.find(key) != mapKeyToTreeInfo.end())
		return -1;

	int nSheet = 0;
	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(nSheet);
	ACCHECK(pSheet);

	int nRow = mapKeyToTreeInfo.size() + nDataRow;

	mapKeyToTreeInfo.insert(std::make_pair(key,STreeItemInfo(key,_T(""),nSheet,nRow)));

	return key;
}

int SItemExcelDB::DeleteByKey(int key)
{
	if(key <= 0)
		return -1;

	MapKeyToTreeInfoT::iterator iter = mapKeyToTreeInfo.find(key);
	if(iter == mapKeyToTreeInfo.end())
		return -1;

	STreeItemInfo& rTreeItemInfo = iter->second;
	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(rTreeItemInfo.nSheet);
	ACCHECK(pSheet);

	for(int nCol = 0; nCol < pSheet->GetTotalCols(); ++nCol)
	{
		BasicExcelCell* pCell = pSheet->Cell(rTreeItemInfo.nRow,nCol);
		ACCHECK(pCell);

		pCell->EraseContents();
	}

	SaveDB();

	int nKey = -1;
	MapKeyToTreeInfoT::iterator iterNext = mapKeyToTreeInfo.erase(iter);
	if(iterNext != mapKeyToTreeInfo.end())
		nKey = iterNext->first;

	return nKey;
}

//-----------------------------------------------------

SItemTab::SItemTab()
{
	strName = _T("");
	strCName = _T("");
	pDB = NULL;
	vtCtrls.clear();
	pWnd = NULL;
	pKeyWnd = NULL;
	nLastSelKey = -1;
}

SItemTab::~SItemTab()
{
	_safe_delete(pDB);
	_safe_delete(pWnd);
	for(size_t i = 0; i < vtCtrls.size(); ++i)
		_safe_delete(vtCtrls[i]);

	vtCtrls.clear();
}

int SItemTab::LoadDefaultValues()
{
	for(size_t i = 0; i < vtCtrls.size(); ++i)
	{
		SCtrl* pCtrl = vtCtrls[i];
		pCtrl->LoadDefaultValue();
	}
	return 0;
}

END_NS_AC