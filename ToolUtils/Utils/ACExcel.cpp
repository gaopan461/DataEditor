#include "stdafx.h"
#include "ACExcel.h"
#include "ACString.h"
#include "ACLog.h"

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

CString ConvertVarintToCString(VARIANT& var)
{
	if(var.vt == VT_BOOL)
	{
		return var.boolVal == VARIANT_TRUE ? _T("true") : _T("false");
	}

	return CString(var);
}

void MakeCOleSafeArray(COleSafeArray& rSA,std::vector<CString>& vtValues,std::vector<int>& vtTypes)
{
	ACCHECK(vtValues.size() == vtTypes.size());

	SAFEARRAYBOUND bound[2] = {{1,1},{vtValues.size(),1}};
	rSA.Create(VT_VARIANT,2,bound);

	long index[2] = {1,1};
	VARIANT var;
	for(size_t nCol = 0; nCol < vtValues.size(); ++nCol)
	{
		index[1] = nCol+1;
		VariantInit(&var);
		switch(vtTypes[nCol])
		{
		case EXCEL_DATA_INT:
			var.vt = VT_I4;
			var.lVal = atoi(CStringToStlString(vtValues[nCol]).c_str());
			rSA.PutElement(index,&var);
			break;
		case EXCEL_DATA_FLOAT:
			var.vt = VT_R8;
			var.dblVal = atof(CStringToStlString(vtValues[nCol]).c_str());
			rSA.PutElement(index,&var);
			break;
		case EXCEL_DATA_BOOL:
			{
				var.vt = VT_BOOL;
				CString strValue = vtValues[nCol];
				strValue.MakeLower();
				if(strValue == _T("true"))
					var.boolVal = VARIANT_TRUE;
				else if(strValue == _T("false"))
					var.boolVal = VARIANT_FALSE;
				else
				{
					int nValue = atoi(CStringToStlString(strValue).c_str());
					var.boolVal = nValue ? VARIANT_TRUE : VARIANT_FALSE;
				}
				rSA.PutElement(index,&var);
			}
			break;
		case EXCEL_DATA_STRING:
			var.vt = VT_BSTR;
			var.bstrVal = SysAllocString(vtValues[nCol].GetBuffer());
			rSA.PutElement(index,&var);
			SysFreeString(var.bstrVal);
			break;
		}
		VariantClear(&var);
	}
}

//-----------------------------------------------------------------

ExcelWorkbook::ExcelWorkbook(LPDISPATCH workbook)
{
	m_objWorkbook.AttachDispatch(workbook);
	m_objWorkSheets.AttachDispatch(m_objWorkbook.get_Worksheets());
}

ExcelWorkbook::~ExcelWorkbook()
{
	m_objWorkSheets.ReleaseDispatch();
	m_objWorkbook.ReleaseDispatch();
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
	ACCHECK(row >= 0 && row < EXCEL_ROW_MAX);
	ACCHECK(col >= 0 && col < EXCEL_COLUMN_MAX);

	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));

	CRange range;
	range.AttachDispatch(sheet.get_Cells());
	VARIANT var = range.get_Item(COleVariant((short)(row+1)),COleVariant((short)(col+1)));
	return ConvertVarintToCString(var);
}

void ExcelWorkbook::SetCellText(int sheetidx,int row,int col,const CString& val)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < EXCEL_ROW_MAX);
	ACCHECK(col >= 0 && col < EXCEL_COLUMN_MAX);

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
		int nRowTotal = GetUsedRowCount(nSheet);
		int nColTotal = GetUsedColumnCount(nSheet);

		if(startRow >= nRowTotal)
			continue;

		ACCHECK(startRow >= 0 && startRow < nRowTotal);
		ACCHECK(sortByCol >= 0 && sortByCol < nColTotal);

		CWorksheet sheet;
		sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(nSheet+1))));

		CString strCell1 = MakeCellName(startRow,1);
		CString strCell2 = MakeCellName(nRowTotal,nColTotal);
		CRange range;
		range.AttachDispatch(sheet.get_Range(COleVariant(strCell1),COleVariant(strCell2)));

		VARIANT key;
		key.vt = VT_DISPATCH;
		key.pdispVal = range.get_Range(COleVariant(_T("A1")),COleVariant(_T("A1")));
		range.Sort(key,xlAscending,covOptional,covOptional,xlAscending,covOptional,
			xlAscending,xlYes,covOptional,COleVariant((long)0),xlSortColumns,xlPinYin,0,0,0);
	}
}

void ExcelWorkbook::DeleteRow(int sheetidx,int row)
{
	CRange range;
	range.AttachDispatch(_GetRowRange(sheetidx,row));

	range.Delete(COleVariant((long)xlShiftUp));
}

void ExcelWorkbook::GetRowText(int sheetidx,int row,std::vector<CString>& vtStr)
{
	CRange range;
	range.AttachDispatch(_GetRowRange(sheetidx,row));

	COleSafeArray sa = range.get_Value2();
	long nColBound;
	sa.GetUBound(2,&nColBound);

	long index[2] = {1,1};
	VARIANT var;
	for(int nCol = 0; nCol < nColBound; ++nCol)
	{
		index[1] = nCol+1;
		VariantInit(&var);
		sa.GetElement(index,&var);
		CString strVal = ConvertVarintToCString(var);
		vtStr.push_back(strVal);
		VariantClear(&var);
	}
}

void ExcelWorkbook::GetRowText(int sheetidx,int row,COleSafeArray& rSA)
{
	CRange range;
	range.AttachDispatch(_GetRowRange(sheetidx,row));

	SAFEARRAYBOUND bound[2] = {{1,1},{range.get_Count(),1}};
	rSA.Create(VT_VARIANT,2,bound);

	rSA = range.get_Value2();
}

void ExcelWorkbook::SetRowText(int sheetidx,int row,std::vector<CString>& vtStr)
{
	CRange range;
	range.AttachDispatch(_GetRowRange(sheetidx,row));

	int nColTotal = range.get_Count();

	COleSafeArray sa;
	SAFEARRAYBOUND bound[2] = {{1,1},{nColTotal,1}};
	sa.Create(VT_VARIANT,2,bound);

	long index[2] = {1,1};
	VARIANT var;
	for(size_t nCol = 0; nCol < vtStr.size(); ++nCol)
	{
		if(nCol >= nColTotal)
			break;

		index[1] = nCol+1;
		VariantInit(&var);
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(vtStr[nCol].GetBuffer());
		sa.PutElement(index,&var);
		SysFreeString(var.bstrVal);
		VariantClear(&var);
	}

	range.put_Value2(sa);
}

void ExcelWorkbook::SetRowText(int sheetidx,int row,COleSafeArray& rSA)
{
	CRange range;
	range.AttachDispatch(_GetRowRange(sheetidx,row));

	int nColTotal = range.get_Count();

	long nColBound;
	rSA.GetUBound(2,&nColBound);
	ACCHECK(nColBound == nColTotal);

	range.put_Value2(rSA);
}

void ExcelWorkbook::InsertRow(int sheetidx,int row,std::vector<CString>& vtStr)
{
	CRange range;
	range.AttachDispatch(_GetRowRange(sheetidx,row));

	range.Insert(COleVariant((short)xlShiftDown),covOptional);

	SetRowText(sheetidx,row,vtStr);
}

void ExcelWorkbook::InsertRow(int sheetidx,int row,COleSafeArray& rSA)
{
	CRange range;
	range.AttachDispatch(_GetRowRange(sheetidx,row));

	range.Insert(COleVariant((short)xlShiftDown),covOptional);

	SetRowText(sheetidx,row,rSA);
}

void ExcelWorkbook::InsertEmptyRow(int sheetidx,int row)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	int nColTotal = GetUsedColumnCount(sheetidx);
	std::vector<CString> vtStr;
	for(int nCol = 0; nCol < nColTotal; ++nCol)
		vtStr.push_back(_T(""));

	InsertRow(sheetidx,row,vtStr);
}

LPDISPATCH ExcelWorkbook::_GetRowRange(int sheetidx,int row)
{
	ACCHECK(sheetidx >= 0 && sheetidx < GetSheetCount());
	ACCHECK(row >= 0 && row < EXCEL_ROW_MAX);

	CWorksheet sheet;
	sheet.AttachDispatch(m_objWorkSheets.get_Item(COleVariant((short)(sheetidx+1))));

	int nColTotal = GetUsedColumnCount(sheetidx);

	CRange range;
	CString strCell1 = MakeCellName(row+1,1);
	CString strCell2 = MakeCellName(row+1,nColTotal);
	return sheet.get_Range(COleVariant(strCell1),COleVariant(strCell2));
}

void ExcelWorkbook::SaveWorkbook()
{
	m_objWorkbook.Save();
	m_objWorkbook.put_Saved(TRUE);
}

void ExcelWorkbook::CloseWorkbook()
{
	m_objWorkbook.put_Saved(TRUE);
	m_objWorkbook.Close(covOptional,covOptional,covOptional);
}

//--------------------------------------------------------

ACExcel::ACExcel()
{
	CreateExcelServer();
}

ACExcel::~ACExcel()
{
	DestroyExcelServer();
}

int ACExcel::CreateExcelServer()
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

	//m_objApplication.put_AlertBeforeOverwriting(FALSE);
	//m_objApplication.put_DisplayAlerts(FALSE);

	m_objApplication.put_Visible(TRUE);

	m_objWorkbooks.AttachDispatch(m_objApplication.get_Workbooks());
	return 0;
}

int ACExcel::DestroyExcelServer()
{
	for(MapNameToWorkbookT::iterator iter = m_mapWorkbooks.begin(); iter != m_mapWorkbooks.end(); ++iter)
	{
		ExcelWorkbook* pBook = iter->second;
		pBook->CloseWorkbook();
		_safe_delete(pBook);
	}

	m_mapWorkbooks.clear();
	m_objWorkbooks.Close();
	m_objWorkbooks.ReleaseDispatch();
	m_objApplication.Quit();
	m_objApplication.ReleaseDispatch();
	return 0;
}

ExcelWorkbook* ACExcel::OpenWorkbook(const CString& strExcelName,const CString& strExcelPath)
{
	MapNameToWorkbookT::iterator iter = m_mapWorkbooks.find(strExcelName);
	if(iter != m_mapWorkbooks.end())
		return iter->second;

	LPDISPATCH workbook = m_objWorkbooks.Open(strExcelPath,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional,covOptional,
		covOptional,covOptional,covOptional,covOptional,covOptional,covOptional);

	ExcelWorkbook* pWorkbook = new ExcelWorkbook(workbook);
	m_mapWorkbooks.insert(std::make_pair(strExcelName,pWorkbook));
	return pWorkbook;
}

ExcelWorkbook* ACExcel::GetWorkbook(const CString& strExcelName)
{
	MapNameToWorkbookT::iterator iter = m_mapWorkbooks.find(strExcelName);
	if(iter != m_mapWorkbooks.end())
		return iter->second;

	return NULL;
}

END_NS_AC