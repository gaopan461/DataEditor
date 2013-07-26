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

class ExcelWorkbook
{
public:
	ExcelWorkbook(CString strPath,CWorkbook workbook);
	virtual ~ExcelWorkbook();
private:
	CString m_strPath;
	CWorkbook m_objWorkbook;
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