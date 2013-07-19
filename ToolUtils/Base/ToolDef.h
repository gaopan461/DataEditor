#ifndef _TOOL_DEF_H_
#define _TOOL_DEF_H_

#include <afxwin.h>
#include <afxcmn.h>
#include <map>
#include <vector>

#include "ACDef.h"
#include "BasicExcel.h"
#include "ToolCtrl.h"

#define MAIN_TREE_WIDTH 180
#define MAIN_LOG_HEIGHT 150

#define CTRL_HEIGHT			25	//控件默认高度
#define CTRL_HEIGHT_INTER	5	//控件纵向间隔

#define IDC_MAIN_TAB	2000
#define IDC_MAIN_TREE	2001
#define IDC_MAIN_LOG	2002

#define IDC_TAB_ITEM	2100
#define IDC_CTRL		2200

using namespace YExcel;

BEGIN_NS_AC

class ToolTree;

int GetCellContent(const BasicExcelCell* pCell, CString& strContent);
int SetCellContent(BasicExcelCell* pCell, const CString& strContent);

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

//-----------------------------------------------------------

struct SItemTab;

struct SItemDB
{
	int nDBType;
	CString strFilePath;
	CString strKeyCName;
	CString strDesCName;

	SItemDB(int type,const CString& path,const CString& key,const CString& des);
	virtual ~SItemDB(){}

	virtual int CtrlToDB(SItemTab* pItemTab){return 0;}
	virtual int DBToCtrl(SItemTab* pItemTab){return 0;}

	virtual int DBToTree(ToolTree* pTree){return 0;}
};

typedef std::map<CString,size_t> MapCNameToColumnT;

struct SItemExcelDB : public SItemDB
{
	int nHeadRow;
	int nDataRow;
	BasicExcel* pExcel;
	MapCNameToColumnT mapCNameToColumn;

	SItemExcelDB(const CString& path,const CString& key, const CString& des,int headRow,int dataRow);
	virtual ~SItemExcelDB();

	virtual int DBToTree(ToolTree* pTree);

	int InitMapNameToColumn();
};

//-----------------------------------------------------------

struct SItemTab
{
	CString strName;
	CString strCName;
	SItemDB* pDB;
	std::vector<SCtrl*> vtCtrls;

	SItemTab();
	virtual ~SItemTab();
};

typedef std::vector<SItemTab*> VectorItemTabsT;

extern VectorItemTabsT g_vtItemTabs;

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