#ifndef _TOOL_DEF_H_
#define _TOOL_DEF_H_

#include <afxwin.h>
#include <afxcmn.h>
#include <map>
#include <vector>

#include "ACDef.h"
#include "ToolCtrl.h"

#define MAIN_TREE_WIDTH 180
#define MAIN_LOG_HEIGHT 150

#define CTRL_HEIGHT			25	//控件默认高度
#define CTRL_HEIGHT_INTER	5	//控件纵向间隔

#define ID_MENU_NEW		1900
#define ID_MENU_SAVE	1901
#define ID_MENU_DELETE	1902
#define ID_MENU_COPY	1903
#define ID_MENU_CANCEL	1904

#define IDC_MAIN_TAB	2000
#define IDC_MAIN_TREE	2001
#define IDC_MAIN_LOG	2002

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

	STreeItemInfo(int key,const CString& des,int sheet)
	{
		m_nKey = key;
		m_strDes = des;
		m_nSheet = sheet;
	}
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
	int UpdateTreeItemInfo(STreeItemInfo& rTreeItemInfo,MapCNameToValueT& mapValues);
	PairTreeInfoFoundT FindTreeInfoByKey(int key);

	int GetKeyInExcel(int sheet,int row);
};

//-----------------------------------------------------------

struct SItemTab
{
	CString m_strName;		// Tab标签页显示名字
	CString m_strCName;		// db名字
	SItemDB* m_pDB;			// db指针
	std::vector<CString> m_vtLayers;// 控制主树控件层级的控件名
	std::vector<SCtrl*> m_vtCtrls;	// 该Tab标签页的全部控件

	CWnd* m_pWnd;		// tab项window，每个tab项是一个window
	CEdit* m_pKeyWnd;	// key对应控件的window
	int m_nLastSelKey;	// 上次选择的key

	SItemTab();
	virtual ~SItemTab();

	SItemDB* GetDB(){return m_pDB;}
	CEdit* GetKeyWnd(){return m_pKeyWnd;}
	int LoadDefaultValues();

	int CtrlToDB(int key);
	int DBToCtrl(int key);

	int GetAllCtrlValues(MapCNameToValueT& mapValues);
	int SetAllCtrlValues(MapCNameToValueT& mapValues);
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