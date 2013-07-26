#ifndef _TOOL_EXCEL_H_
#define _TOOL_EXCEL_H_

#include "ToolDef.h"
#include "ToolApp.h"

#include "stdafx.h"
#include "CApplication.h"
#include "CWorkbooks.h"
#include "CWorkbook.h"
#include "CWorksheets.h"
#include "CWorksheet.h"
#include "CRange.h"

BEGIN_NS_AC

#define covTrue COleVariant((short)TRUE)
#define covFalse COleVariant((short)FALSE)
#define covOptional COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR)

#define xlPinYin ((long)1) // this is the default
#define xlTopToBottom ((long)1)
#define xlIgnoreCase COleVariant((long)0)
#define xlHeader ((long)1)

enum
{
	xlAscending  = 1,
	xlDescending  = 2,
};

enum
{
	xlShiftToLeft = -4159,
	xlShiftUp = -4162,
	xlShiftDown = -4121,
};

class ExcelWorkbook
{
public:
	ExcelWorkbook(CString strPath,CWorkbook workbook);
	virtual ~ExcelWorkbook();
public:
	int GetSheetCount();
	int GetRowCount(int sheetidx);
	int GetColumnCount(int sheetidx);
	CString GetCell(int sheetidx,int row,int col);
	void SetCell(int sheetidx,int row,int col,CString val);
public:
	void SortAllSheetByColumn(int sortByCol,int startRow);
	void SaveAllSheet();
public:
	void DeleteRow(int sheetidx,int row);
	int AddRow(int sheetidx);
public:
	void Close();
private:
	CString m_strPath;
	CWorkbook m_objWorkbook;
	CWorksheets m_objWorkSheets;
};

class ToolExcel : public Module<ToolApp>
{
public:
	ToolExcel(ToolApp* app);
	virtual ~ToolExcel();
public:
	int Initialize();
	int Finalize();
public:
	ExcelWorkbook* OpenWorkbook(CString strPath);
protected:
	typedef std::map<CString,ExcelWorkbook> MapNameToWorkbookT;
private:
	CApplication m_objApplication;
	CWorkbooks m_objWorkbooks;
	MapNameToWorkbookT m_mapWorkbooks;
};

END_NS_AC

#endif//_TOOL_EXCEL_H_