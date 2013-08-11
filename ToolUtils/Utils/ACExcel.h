#ifndef _AC_EXCEL_H_
#define _AC_EXCEL_H_

#include "CApplication.h"
#include "CWorkbooks.h"
#include "CWorkbook.h"
#include "CWorksheets.h"
#include "CWorksheet.h"
#include "CRange.h"

#include "ACDef.h"
#include "ACExcelDef.h"

#include <map>
#include <vector>

BEGIN_NS_AC

// 将行号列好转成Cell的名字，如(1000,52) = "AZ1000"
CString MakeCellName(int nRow,int nCol);

// 根据值和数据类型数组，生产一个COleSafeArray，用于对excel中整行的读写
void MakeCOleSafeArray(COleSafeArray& rSA,std::vector<CString>& vtValues,std::vector<int>& vtTypes);

/**
* @class ExcelWorkbook
* @author gaopan
* @date 2013年08月10日
* @file ToolExcel.h
* @brief 操作一个Excel的类，其中的sheet,column,row从0开始编号; COleSafeArray的行列从1开始编号
*/
class ExcelWorkbook
{
public:
	ExcelWorkbook(LPDISPATCH workbook);
	virtual ~ExcelWorkbook();
public:
	int GetSheetCount();
	int GetUsedRowCount(int sheetidx);
	int GetUsedColumnCount(int sheetidx);
public:
	CString GetCellText(int sheetidx,int row,int col);
	void SetCellText(int sheetidx,int row,int col,const CString& val);
public:
	void SortAllSheetByColumn(int sortByCol,int startRow);
public:
	void DeleteRow(int sheetidx,int row);

	void GetRowText(int sheetidx,int row,std::vector<CString>& vtStr);
	void GetRowText(int sheetidx,int row,COleSafeArray& rSA);

	void SetRowText(int sheetidx,int row,std::vector<CString>& vtStr);
	void SetRowText(int sheetidx,int row,COleSafeArray& rSA);

	void InsertRow(int sheetidx,int row,std::vector<CString>& vtStr);
	void InsertRow(int sheetidx,int row,COleSafeArray& rSA);

	void InsertEmptyRow(int sheetidx,int row);
public:
	void SaveWorkbook();
	void CloseWorkbook();
private:
	LPDISPATCH _GetRowRange(int sheetidx,int row);
private:
	CWorkbook m_objWorkbook;
	CWorksheets m_objWorkSheets;
};

//--------------------------------------------------------------

class ACExcel
{
public:
	ACExcel();
	virtual ~ACExcel();
public:
	int CreateExcelServer();
	int DestroyExcelServer();
public:
	ExcelWorkbook* OpenWorkbook(const CString& strExcelName,const CString& strExcelPath);
	ExcelWorkbook* GetWorkbook(const CString& strExcelName);
protected:
	typedef std::map<CString,ExcelWorkbook*> MapNameToWorkbookT;
private:
	CApplication m_objApplication;
	CWorkbooks m_objWorkbooks;
	MapNameToWorkbookT m_mapWorkbooks;
};

END_NS_AC

#endif//_AC_EXCEL_H_