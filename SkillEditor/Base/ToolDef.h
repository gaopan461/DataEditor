#ifndef _TOOL_DEF_H_
#define _TOOL_DEF_H_

#include <afxwin.h>
#include <afxcmn.h>
#include <map>
#include <vector>

#include "ACDef.h"

#define IDC_TAB_ITEM	2100
#define IDC_CTRL		2200

#define ARRAY_DELIMITER _T(",")

BEGIN_NS_AC

class ToolTree;
class ExcelWorkbook;
class ToolExcel;

//-----------------------------------------------------------

enum EDataType
{
	DATA_INT = 0,
	DATA_FLOAT,
	DATA_STRING,
};

enum ECtrlType
{
	CTRL_EDIT = 0,
	CTRL_CHECK,
	CTRL_STATIC,
	CTRL_COMBOBOX,
	CTRL_CHECKCOMBO,
};

enum EDBType
{
	DB_EXCEL = 0,
	DB_LUA,
	DB_PYTHON,
};

typedef std::map<CString,CString> MapCNameToValueT;

//-----------------------------------------------------------

struct SItemTab;

struct SItemDB
{
	int m_nDBType;
	CString m_strFilePath;
	CString m_strKeyCName;
	CString m_strDesCName;

	std::vector<CString> m_vtLayerCName;// 控制主树控件层级的控件名

	SItemDB(int type,const CString& path,const CString& key,const CString& des);
	virtual ~SItemDB(){}

	virtual int Save(int key, MapCNameToValueT& mapValues){return 0;}
	virtual int Load(int key, MapCNameToValueT& mapValues){return 0;}

	virtual int DBToTree(ToolTree* pTree){return 0;}
	virtual int SortDB(){return 0;}
	virtual int SaveDB(){return 0;}

	virtual int InsertByKey(int key, MapCNameToValueT& mapValues){return -1;}
	virtual int DeleteByKey(int key){return -1;}
};

typedef std::map<CString,size_t> MapCNameToColumnT;

struct STreeItemInfo
{
	int m_nKey;
	CString m_strDes;
	std::vector<CString> m_vtLayers;
	int m_nSheet;
};

typedef std::vector<STreeItemInfo> VectorTreeItemInfoT;

typedef std::pair<bool,VectorTreeItemInfoT::iterator> PairTreeInfoFoundT;

struct SItemExcelDB : public SItemDB
{
	int m_nHeadRow;
	int m_nDataRow;
	ExcelWorkbook* m_pExcel;
	MapCNameToColumnT m_mapCNameToColumn;

	VectorTreeItemInfoT m_vtTreeItemInfos;

	SItemExcelDB(const CString& path,const CString& key, const CString& des,int headRow,int dataRow);
	virtual ~SItemExcelDB();

	virtual int Save(int key, MapCNameToValueT& mapValues);
	virtual int Load(int key, MapCNameToValueT& mapValues);

	virtual int DBToTree(ToolTree* pTree);
	virtual int SortDB();
	virtual int SaveDB();

	virtual int InsertByKey(int key, MapCNameToValueT& mapValues);
	virtual int DeleteByKey(int key);

	int InitMapNameToColumn();

	int InitTreeItemInfos();
	STreeItemInfo GetTreeItemInfo(int nSheet,int nRow);
	int UpdateTreeItemInfo(STreeItemInfo& rTreeItemInfo,MapCNameToValueT& mapValues,bool bForcedUpdateTree = false);
	PairTreeInfoFoundT FindTreeInfoByKey(int key);

	int GetKeyInExcel(int sheet,int row);
};

//-----------------------------------------------------------

template <class T>
class Module
{
public:
	Module(T* pOwner)
		: m_pOwner(pOwner)
	{}
	virtual ~Module()
	{
		m_pOwner = NULL;
	}
protected:
	T* m_pOwner;
};

END_NS_AC

#endif//_TOOL_DEF_H_