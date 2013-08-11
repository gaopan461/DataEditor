#ifndef _EXCEL_DB_H_
#define _EXCEL_DB_H_

#include "ToolDef.h"
#include "ToolApp.h"

#include "ACExcel.h"

BEGIN_NS_AC

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

class ExcelDB
{
public:
	ExcelDB();
	virtual ~ExcelDB();
public:
	int OpenDB(const SExcelConfig& rExcelConfig);
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
	PairTreeInfoFoundT FindTreeInfoByKey(int key);
public:
	int InitVectorDataTypes();
public:
	inline ToolTree* GetTree()
	{
		return m_pTree;
	}
private:
	CString m_strFilePath;
	CString m_strKeyCName;
	CString m_strDesCName;

	std::vector<CString> m_vtLayerCName;// 控制主树控件层级的控件名

	int m_nHeadRow;
	int m_nTypeRow;
	int m_nDataRow;
	MapCNameToColumnT m_mapCNameToColumn;
	VectorTreeItemInfoT m_vtTreeItemInfos;
	MapCNameToValueT m_mapDefaultValue;
	VectorDataTypeT m_vtDataTypes;
private:
	ExcelWorkbook* m_pWorkbook;
	ToolTree* m_pTree;
};

END_NS_AC

#endif//_EXCEL_DB_H_