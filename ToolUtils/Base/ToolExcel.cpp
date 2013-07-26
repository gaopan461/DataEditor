#include "ToolExcel.h"

BEGIN_NS_AC

ExcelWorkbook::ExcelWorkbook(CString strPath,CWorkbook workbook)
: m_strPath(strPath)
, m_objWorkbook(workbook)
{
	m_objWorkSheets = m_objWorkbook.get_Worksheets();
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

int ExcelWorkbook::GetRowCount(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	CWorksheet sheet = m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1)));
	CRange range = sheet.get_UsedRange();
	range = range.get_Rows();
	return range.get_Count();
}

int ExcelWorkbook::GetColumnCount(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	CWorksheet sheet = m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1)));
	CRange range = sheet.get_UsedRange();
	range = range.get_Columns();
	return range.get_Count();
}

CString ExcelWorkbook::GetCell(int sheetidx,int row,int col)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetRowCount(sheetidx));
	ACCHECK(col >= 0 && col < GetColumnCount(sheetidx));
	CWorksheet sheet = m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1)));
	CRange range = sheet.get_Cells();
	return range.get_Item(COleVariant((short)(row+1)),COleVariant((short)(col+1)));
}

void ExcelWorkbook::SetCell(int sheetidx,int row,int col,CString val)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetRowCount(sheetidx));
	ACCHECK(col >= 0 && col < GetColumnCount(sheetidx));
	CWorksheet sheet = m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1)));
	CRange range = sheet.get_Cells();
	range.put_Item(COleVariant((short)(row+1)),COleVariant((short)(col+1)),COleVariant(val));
}

void ExcelWorkbook::SortAllSheetByColumn(int sortByCol,int startRow)
{
	int nSheetTotal = GetSheetCount();
	for(int nSheet = 0; nSheet < nSheetTotal; ++nSheet)
	{
		if(GetRowCount(nSheet) <= startRow)
			continue;

		ACCHECK(startRow >= 0 && startRow < GetRowCount(nSheet));
		ACCHECK(sortByCol >= 0 && sortByCol < GetColumnCount(nSheet));
		CWorksheet sheet = m_objWorkSheets.get_Item(COleVariant((short)(nSheet+1)));
		CString strCell1,strCell2;
		strCell1.Format(_T("A%d"),startRow);
		strCell2.Format(_T("%c%d"),'A'+GetColumnCount(nSheet)-1,GetRowCount(nSheet));
		CRange range = sheet.get_Range(COleVariant(strCell1),COleVariant(strCell2));
		VARIANT key;
		V_VT(&key) = VT_DISPATCH;
		V_DISPATCH(&key) = range.get_Range(COleVariant(_T("A1")),COleVariant(_T("A1")));
		range.Sort(key, xlAscending, covOptional, covOptional, xlAscending, covOptional,
			xlAscending,xlHeader,covOptional,xlIgnoreCase,xlTopToBottom,xlPinYin,0,0,0);
	}
}

void ExcelWorkbook::SaveAllSheet()
{
	m_objWorkbook.Save();
	m_objWorkbook.put_Saved(TRUE);
}

void ExcelWorkbook::DeleteRow(int sheetidx,int row)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < GetRowCount(sheetidx));
	CWorksheet sheet = m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1)));
	CRange range = sheet.get_UsedRange();
	CString str;
	str.Format(_T("A%d"),row+1);
	range = range.get_Range(COleVariant(str),COleVariant(str));
	range = range.get_EntireRow();
	range.Delete(COleVariant((long)xlShiftUp));
}

int ExcelWorkbook::AddRow(int sheetidx)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	int nRowTotal = GetRowCount(sheetidx);
	CWorksheet sheet = m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1)));
	CRange range = sheet.get_Cells();
	range.put_Item(COleVariant((short)(nRowTotal+1)),COleVariant((short)(1)),COleVariant(_T("0")));
	return nRowTotal;

	CString str;
	str.Format(_T("A%d"),nRowTotal+1);
	range = range.get_Range(COleVariant(str),COleVariant(str));
	range = range.get_EntireRow();
	range.Insert(COleVariant((short)xlShiftDown),covOptional);
	return nRowTotal;
}

void ExcelWorkbook::Close()
{
	m_objWorkbook.Close(covTrue,covOptional,covOptional);
	//m_objWorkbook.put_Saved(true);
}

//--------------------------------------------------------

ToolExcel::ToolExcel(ToolApp* app)
: Module<ToolApp>(app)
{}

ToolExcel::~ToolExcel()
{
	Finalize();
}

int ToolExcel::Initialize()
{
	if(!AfxOleInit())
	{
		AfxMessageBox(_T("Error! Ole initial failed!"));
		ExitProcess(-1);
	}

	if(!m_objApplication.CreateDispatch(_T("Excel.Application")))
	{
		AfxMessageBox(_T("Error! Creat excel application server failed!"));
		ExitProcess(-1);
	}

	m_objApplication.put_AlertBeforeOverwriting(FALSE);
	m_objApplication.put_DisplayAlerts(FALSE);

	m_objWorkbooks = m_objApplication.get_Workbooks();
	return 0;
}

int ToolExcel::Finalize()
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

	CWorkbook workbook = m_objWorkbooks.Open(strPath,covOptional,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional);

	iter = m_mapWorkbooks.insert(std::make_pair(strFileName,ExcelWorkbook(strPath,workbook))).first;
	return &iter->second;
}

END_NS_AC