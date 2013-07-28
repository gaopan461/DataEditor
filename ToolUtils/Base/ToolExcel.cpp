#include "ToolExcel.h"

BEGIN_NS_AC

const int cst_carry_num = 'Z' - 'A' + 1;

void MakeColumnName(int nCol,CString& strColName)
{
	ACCHECK(nCol > 0);

	if(nCol >= 1 && nCol <= cst_carry_num)
	{
		strColName.Insert(0,_T('A' + nCol - 1));
		return;
	}

	if(nCol % cst_carry_num == 0)
	{
		nCol = nCol - cst_carry_num;
		strColName.Insert(0,_T('Z'));
		nCol = nCol / cst_carry_num;
	}
	else
	{
		int nLow = nCol % cst_carry_num;
		strColName.Insert(0,_T('A' + nLow - 1));
		nCol = nCol / cst_carry_num;
	}

	MakeColumnName(nCol,strColName);
}

CString MakeCellName(int nRow,int nCol)
{
	ACCHECK(nRow > 0 && nCol > 0);

	CString strColName;
	MakeColumnName(nCol,strColName);
	CString strRowName;
	strRowName.Format(_T("%d"), nRow);
	return strColName + strRowName;
}

int TestMakeCellName()
{
	ACCHECK(MakeCellName(1000,1) == _T("A1000"));	// A1000
	ACCHECK(MakeCellName(1000,26) == _T("Z1000"));		// Z1000
	ACCHECK(MakeCellName(1000,27) == _T("AA1000"));		// AA1000
	ACCHECK(MakeCellName(1000,52) == _T("AZ1000"));		// AZ1000
	ACCHECK(MakeCellName(1000,53) == _T("BA1000"));		// BA1000
	ACCHECK(MakeCellName(1000,2 * 26 + 26) == _T("BZ1000")); // BZ1000
	ACCHECK(MakeCellName(1000,2 * 26 * 26 + 26 * 26 + 26) == _T("BZZ1000")); // BZZ1000
	return 0;
}

//-----------------------------------------------------------------

ExcelWorkbook::ExcelWorkbook(CString strPath,LPDISPATCH workbook)
: m_strPath(strPath)
{
	m_objWorkbook.AttachDispatch(workbook);
	m_objWorkSheets.AttachDispatch(m_objWorkbook.get_Worksheets());
}

ExcelWorkbook::~ExcelWorkbook()
{
	m_objWorkSheets.DetachDispatch();
	m_objWorkbook.DetachDispatch();
}

int ExcelWorkbook::GetSheetCount()
{
	return m_objWorkSheets.get_Count();
}

int ExcelWorkbook::GetUsedRowCount(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_UsedRange());
	range.AttachDispatch(range.get_Rows());
	return range.get_Count();
}

int ExcelWorkbook::GetUsedColumnCount(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_UsedRange());
	range.AttachDispatch(range.get_Columns());
	return range.get_Count();
}

CString ExcelWorkbook::GetCellText(int sheetidx,int row,int col)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetUsedRowCount(sheetidx));
	ACCHECK(col >= 0 && col < GetUsedColumnCount(sheetidx));
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_Cells());
	return range.get_Item(COleVariant((short)(row+1)),COleVariant((short)(col+1)));
}

void ExcelWorkbook::SetCellText(int sheetidx,int row,int col,CString val)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetUsedRowCount(sheetidx));
	ACCHECK(col >= 0 && col < GetUsedColumnCount(sheetidx));
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_Cells());
	range.put_Item(COleVariant((short)(row+1)),COleVariant((short)(col+1)),COleVariant(val));
}

void ExcelWorkbook::SortAllSheetByColumn(int sortByCol,int startRow)
{
	int nSheetTotal = GetSheetCount();
	for(int nSheet = 0; nSheet < nSheetTotal; ++nSheet)
	{
		if(GetUsedRowCount(nSheet) <= startRow)
			continue;

		ACCHECK(startRow >= 0 && startRow < GetUsedRowCount(nSheet));
		ACCHECK(sortByCol >= 0 && sortByCol < GetUsedColumnCount(nSheet));
		CWorksheet sheet;
		sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(nSheet+1))));
		CString strCell1 = MakeCellName(startRow,1);
		CString strCell2 = MakeCellName(GetUsedRowCount(nSheet),GetUsedColumnCount(nSheet));
		CRange range;
		range.AttachDispatch(sheet.get_Range(COleVariant(strCell1),COleVariant(strCell2)));
		VARIANT key;
		V_VT(&key) = VT_DISPATCH;
		V_DISPATCH(&key) = range.get_Range(COleVariant(_T("A1")),COleVariant(_T("A1")));
		range.Sort(key,xlAscending,covOptional,covOptional,xlAscending,covOptional,
			xlAscending,xlYes,covOptional,COleVariant((long)0),xlSortColumns,xlPinYin,0,0,0);
	}
}

void ExcelWorkbook::DeleteRow(int sheetidx,int row)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetUsedRowCount(sheetidx));
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_UsedRange());
	CString strCell = MakeCellName(row+1,1);
	range.AttachDispatch(range.get_Range(COleVariant(strCell),COleVariant(strCell)));
	range.AttachDispatch(range.get_EntireRow());
	range.Delete(COleVariant((long)xlShiftUp));
}

void ExcelWorkbook::GetRowText(int sheetidx,int row,std::vector<CString>& vtStr)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetUsedRowCount(sheetidx));
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_UsedRange());
	int nColTotal = GetUsedColumnCount(sheetidx);
	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		vtStr.push_back(range.get_Item(COleVariant((short)(row+1)),COleVariant((short)(nCol))));
	}
}

void ExcelWorkbook::InsertRow(int sheetidx,int row,std::vector<CString>& vtStr)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0);
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	for(size_t nCol = 0; nCol < vtStr.size(); ++nCol)
	{
		//获取单元格
		CString strCell = MakeCellName(row+1,nCol+1);
		range.AttachDispatch(sheet.get_Range(COleVariant(strCell),covOptional));

		//当前位置插入单元格
		range.Insert(COleVariant((short)xlShiftDown),covOptional);

		//设置单元格内容
		range.AttachDispatch(sheet.get_Range(COleVariant(strCell),covOptional));
		range.put_Value2(COleVariant(vtStr[nCol]));

		//自动列宽
		range.AttachDispatch(range.get_EntireColumn());
		range.AutoFit();
	}
}

void ExcelWorkbook::InsertEmptyRow(int sheetidx,int row)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0);
	int nColTotal = GetUsedColumnCount(sheetidx);
	std::vector<CString> vtStr;
	for(int nCol = 0; nCol < nColTotal; ++nCol)
		vtStr.push_back(_T(" "));

	InsertRow(sheetidx,row,vtStr);
}

void ExcelWorkbook::AppendEmptyRow(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	int nRowTotal = GetUsedRowCount(sheetidx);
	int nColTotal = GetUsedColumnCount(sheetidx);
	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));
	CRange range;
	range.AttachDispatch(sheet.get_Cells());

	for(int nCol = 0; nCol < nColTotal; ++nCol)
	{
		range.put_Item(COleVariant((short)(nRowTotal+1)),COleVariant((short)(nCol+1)),COleVariant(_T(" ")));
	}
}

void ExcelWorkbook::Save()
{
	m_objWorkbook.Save();
	m_objWorkbook.put_Saved(TRUE);
}

void ExcelWorkbook::Close()
{
	m_objWorkbook.put_Saved(TRUE);
	m_objWorkbook.Close(covOptional,COleVariant(m_strPath),covOptional);
}

//--------------------------------------------------------

ToolExcel::ToolExcel(ToolApp* app)
: Module<ToolApp>(app)
{
	CreateExcelServer();
}

ToolExcel::~ToolExcel()
{
	DestroyExcelServer();
}

int ToolExcel::CreateExcelServer()
{
	if(!AfxOleInit())
	{
		ErrorMessageBox(_T("Error! Ole initial failed!"));
		ExitProcess(-1);
	}

	if(!m_objApplication.CreateDispatch(_T("Excel.Application")))
	{
		ErrorMessageBox(_T("Error! Creat excel application server failed!"));
		ExitProcess(-1);
	}

	m_objApplication.put_AlertBeforeOverwriting(FALSE);
	m_objApplication.put_DisplayAlerts(FALSE);

	m_objWorkbooks.AttachDispatch(m_objApplication.get_Workbooks());
	return 0;
}

int ToolExcel::DestroyExcelServer()
{
	for(MapNameToWorkbookT::iterator iter = m_mapWorkbooks.begin(); iter != m_mapWorkbooks.end(); ++iter)
	{
		ExcelWorkbook& rBook = iter->second;
		rBook.Close();
	}

	m_mapWorkbooks.clear();
	m_objWorkbooks.Close();
	m_objWorkbooks.DetachDispatch();
	m_objApplication.DetachDispatch();
	m_objApplication.Quit();
	return 0;
}

ExcelWorkbook* ToolExcel::OpenWorkbook(CString strPath)
{
	int pos = strPath.ReverseFind('\\');
	CString strFileName = strPath.Right(strPath.GetLength()-pos-1);
	ACCHECK(strFileName.Right(4) == _T(".xls") || strFileName.Right(5) == _T(".xlsx"));

	MapNameToWorkbookT::iterator iter = m_mapWorkbooks.find(strFileName);
	if(iter != m_mapWorkbooks.end())
		return &iter->second;

	CWorkbook workbook;
	workbook.AttachDispatch(m_objWorkbooks.Open(strPath,covOptional,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional));

	iter = m_mapWorkbooks.insert(std::make_pair(strFileName,ExcelWorkbook(strPath,workbook))).first;
	return &iter->second;
}

END_NS_AC