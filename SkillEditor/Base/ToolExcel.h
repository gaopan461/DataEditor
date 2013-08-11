#ifndef _TOOL_EXCEL_H_
#define _TOOL_EXCEL_H_

#include "ToolDef.h"
#include "ToolApp.h"

#include "ToolExcelDef.h"

#include "stdafx.h"
#include "CApplication.h"
#include "CWorkbooks.h"
#include "CWorkbook.h"
#include "CWorksheets.h"
#include "CWorksheet.h"
#include "CRange.h"

BEGIN_NS_AC

// ���к��к�ת��Cell�����֣���(1000,52) = "AZ1000"
CString MakeCellName(int nRow,int nCol);

/**
* @class ExcelWorkbook
* @author gaopan
* @date 2013��08��10��
* @file ToolExcel.h
* @brief ����һ��Excel���࣬���е�sheet,column,row��0��ʼ���; COleSafeArray�����д�1��ʼ���
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

typedef std::map<CString,size_t> MapCNameToColumnT;

struct STreeItemInfo
{
	int m_nKey;
	CString m_strDes;
	std::vector<CString> m_vtLayers;
	int m_nSheet;
};

typedef std::vector<STreeItemInfo> VectorTreeItemInfoT;
typedef std::vector<int> VectorDataTypeT;

typedef std::pair<bool,VectorTreeItemInfoT::iterator> PairTreeInfoFoundT;

class ExcelDB : public ExcelWorkbook
{
public:
	ExcelDB(LPDISPATCH pWorkbook, SExcelConfig& rExcelConfig);
	virtual ~ExcelDB();
public:
	virtual int ReadDBRecord(int key, MapCNameToValueT& mapValues);
	virtual int WriteDBRecord(int key, MapCNameToValueT& mapValues);

	virtual void SetDBDefaultValue(MapCNameToValueT& mapDefault);
	virtual void GetDBDefaultValue(MapCNameToValueT& mapDefault);

	virtual int DBToTree(ToolTree* pTree);
	virtual int SortDB();

	virtual int InsertByKey(int key, MapCNameToValueT& mapValues);
	virtual int DeleteByKey(int key);
	virtual int GetUnusedKey();

	int InitMapNameToColumn();

	int InitTreeItemInfos();
	STreeItemInfo GetTreeItemInfo(int nSheet,int nRow);
	int UpdateTreeItemInfo(STreeItemInfo& rTreeItemInfo,MapCNameToValueT& mapValues,bool bForcedUpdateTree = false);
	PairTreeInfoFoundT FindTreeInfoByKey(int key);
public:
	int InitVectorDataTypes();
public:
	int GetLastSelectKey()
	{
		return m_nLastSelectKey;
	}
	void SetLastSelectKey(int key)
	{
		m_nLastSelectKey = key;
	}
private:
	CString m_strFilePath;
	CString m_strKeyCName;
	CString m_strDesCName;

	std::vector<CString> m_vtLayerCName;// ���������ؼ��㼶�Ŀؼ���

	int m_nHeadRow;
	int m_nTypeRow;
	int m_nDataRow;
	MapCNameToColumnT m_mapCNameToColumn;
	VectorTreeItemInfoT m_vtTreeItemInfos;
	MapCNameToValueT m_mapDefaultValue;
	VectorDataTypeT m_vtDataTypes;
private:
	int m_nLastSelectKey;
};

//--------------------------------------------------------------

class ToolExcel : public Module<ToolApp>
{
public:
	ToolExcel(ToolApp* app);
	virtual ~ToolExcel();
public:
	int CreateExcelServer();
	int DestroyExcelServer();
public:
	ExcelDB* OpenWorkbook(SExcelConfig& rExcelConfig);
	ExcelDB* GetWorkbook(CString strExcelCName);
protected:
	typedef std::map<CString,ExcelDB*> MapNameToWorkbookT;
private:
	CApplication m_objApplication;
	CWorkbooks m_objWorkbooks;
	MapNameToWorkbookT m_mapWorkbooks;
};

END_NS_AC

#endif//_TOOL_EXCEL_H_