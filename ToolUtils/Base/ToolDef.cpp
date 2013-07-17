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
	pCell->Set(strContent);
	return 0;
}

SItemTab::SItemTab()
{
	strName = _T("");
	strCName = _T("");
	strKeyCName = _T("");
	strDesCName = _T("");
	vtCtrls.clear();
}

SItemTab::~SItemTab()
{
	for(size_t i = 0; i < vtCtrls.size(); ++i)
		_safe_delete(vtCtrls[i]);

	vtCtrls.clear();
}

SItemExcelDB::SItemExcelDB(const char* filename)
{
	pExcel = new BasicExcel();
	if(!pExcel->Load(filename))
	{
		CString str;
		str.Format(_T("Load excel faild! excel:%s"),filename);
		AfxMessageBox(str);
		ExitProcess(-1);
	}

	strFileName = filename;
	mapCNameToColumn.clear();
	Init();
}

SItemExcelDB::~SItemExcelDB()
{
	_safe_delete(pExcel);
	mapCNameToColumn.clear();
}

int SItemExcelDB::Init()
{
	ACCHECK(pExcel->GetTotalWorkSheets() > 0);

	size_t nSheet1Index = 0;
	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(nSheet1Index);
	ACCHECK(pSheet);

	size_t nHeadRow = g_objPlatformConfig.objExcelConfig.nHeadRow;
	if(pSheet->GetTotalRows() < nHeadRow)
	{
		ERROR_MSG("Require head,excel:%s",strFileName.c_str());
		return -1;
	}

	for(size_t nCol = 0; nCol < pSheet->GetTotalCols(); ++nCol)
	{
		BasicExcelCell* pCell = pSheet->Cell(nHeadRow-1,nCol);
		ACCHECK(pCell);
		CString str;
		if(GetCellContent(pCell,str) != 0)
			INFO_MSG("Unknown cell type,excel:%s,row:%d,col:%d",strFileName.c_str(),nHeadRow,nCol);

		if(str.IsEmpty())
			continue;

		mapCNameToColumn.insert(std::make_pair(str,nCol));
	}

	return 0;
}

int SItemExcelDB::DBToTree(ToolTree* pTree)
{
	pTree->DeleteAllItems();
	pTree->InsertUndefinedRoot();

	MapCNameToColumnT::iterator iter = mapCNameToColumn.find(strKeyCName);
	if(iter == mapCNameToColumn.end())
	{
		ERROR_MSG("DBToTree failed,excel:%s,key:%s",strFileName.c_str(),CStringToStlString(strKeyCName).c_str());
		return -1;
	}
	size_t nKeyCol = iter->second;

	size_t nDesCol = 1;
	iter = mapCNameToColumn.find(strDesCName);
	if(iter == mapCNameToColumn.end())
	{
		WARN_MSG("DBToTree no des,excel:%s,des:%s",strFileName.c_str(),CStringToStlString(strDesCName).c_str());
	}
	else
		nDesCol = iter->second;


	for(size_t sheet = 0; sheet < pExcel->GetTotalWorkSheets(); ++sheet)
	{
		BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(sheet);
		ACCHECK(pSheet);

		size_t nDataStartRow = g_objPlatformConfig.objExcelConfig.nDataStartRow;
		for(size_t nRow = nDataStartRow; nRow < pSheet->GetTotalRows(); ++nRow)
		{
			BasicExcelCell* pCellKey = pSheet->Cell(nRow,nKeyCol);
			ACCHECK(pCellKey);
			CString str;
			if(GetCellContent(pCellKey,str) != 0)
			{
				INFO_MSG("Unknown cell type,excel:%s,row:%d,col:%d",strFileName.c_str(),nRow,nKeyCol);
				continue;
			}

			int nKey = atoi(CStringToStlString(str).c_str());

			BasicExcelCell* pCellDes = pSheet->Cell(nRow,nDesCol);
			ACCHECK(pCellDes);
			if(GetCellContent(pCellDes,str) != 0)
			{
				INFO_MSG("Unknown cell type,excel:%s,row:%d,col:%d",strFileName.c_str(),nRow,nDesCol);
				str = _T("");
			}

			pTree->InsertItem(nKey,str);
		}
	}

	pTree->ExpandAllItems();
	pTree->UpdatedItems();
	return 0;
}

END_NS_AC