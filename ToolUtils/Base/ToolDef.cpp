#include "ToolDef.h"
#include "ACLog.h"
#include "ACString.h"

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
	strKey = _T("");
	strDes = _T("");
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

	int nSheet1Index = 0;
	BasicExcelWorksheet* pSheet = pExcel->GetWorksheet(nSheet1Index);
	ACCHECK(pSheet);

	int nHeadRow = g_objPlatformConfig.objExcelConfig.nHeadRow;
	if(pSheet->GetTotalRows() < nHeadRow)
	{
		ERROR_MSG("Require head,excel:%s",strFileName.c_str());
		return -1;
	}

	int nMaxCol = pSheet->GetTotalCols();
	for(int nCol = 0; nCol < nMaxCol; ++nCol)
	{
		BasicExcelCell* pCell = pSheet->Cell(nHeadRow-1,nCol);
		CString str;
		if(GetCellContent(pCell,str) != 0)
			INFO_MSG("Unknown cell type,excel:%s,row:%d,col:%d",strFileName.c_str(),nHeadRow,nCol);

		if(str.IsEmpty())
			continue;

		mapCNameToColumn.insert(std::make_pair(str,nCol));
	}

	return 0;
}

END_NS_AC