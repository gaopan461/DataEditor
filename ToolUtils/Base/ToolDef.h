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

enum ECtrlType
{
	CTRL_EDIT = 0,
	CTRL_CHECK,
	CTRL_STATIC,
	CTRL_COMBOBOX,
	CTRL_CHECKCOMBO,
};

struct SItemTab
{
	CString strName;
	CString strCName;
	CString strKeyCName;
	CString strDesCName;
	std::vector<SCtrl*> vtCtrls;

	SItemTab();
	virtual ~SItemTab();
};

struct SItemDB
{
	CString strKeyCName;
	CString strDesCName;

	SItemDB()
	{
		strKeyCName = _T("");
		strDesCName = _T("");
	}
	virtual ~SItemDB(){}

	virtual int CtrlToDB(SItemTab* pItemTab){return 0;}
	virtual int DBToCtrl(SItemTab* pItemTab){return 0;}

	virtual int DBToTree(ToolTree* pTree){return 0;}
};

typedef std::map<CString,size_t> MapCNameToColumnT;

struct SItemExcelDB : public SItemDB
{
	BasicExcel* pExcel;
	MapCNameToColumnT mapCNameToColumn;
	std::string strFileName;

	SItemExcelDB(const char* filename);
	virtual ~SItemExcelDB();

	virtual int DBToTree(ToolTree* pTree);

	int Init();
};

typedef std::vector<SItemTab*> VectorItemTabsT;
typedef std::vector<SItemDB*> VectorItemDBsT;

extern VectorItemTabsT g_vtItemTabs;
extern VectorItemDBsT g_vtItemDBs;

//-----------------------------------------------------------

enum EPlatformType
{
	PLATFORM_TYPE_EXCEL = 0,
	PLATFORM_TYPE_LUA,
	PLATFORM_TYPE_PYTHON,
};

struct SExcelPlatformConfig
{
	int nHeadRow;
	int nDataStartRow;
};

struct SPlatformConfig
{
	int nPlatformType;
	union
	{
		SExcelPlatformConfig objExcelConfig;
	};
};

extern SPlatformConfig g_objPlatformConfig;

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