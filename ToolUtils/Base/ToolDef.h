#ifndef _TOOL_DEF_H_
#define _TOOL_DEF_H_

#include <afxwin.h>
#include <afxcmn.h>
#include <map>
#include <vector>

#include "ACDef.h"
#include "BasicExcel.h"

#define MAIN_TREE_WIDTH 180
#define MAIN_LOG_HEIGHT 150

#define IDC_MAIN_TAB	2000
#define IDC_MAIN_TREE	2001
#define IDC_MAIN_LOG	2002

#define IDC_TAB_ITEM	2100
#define IDC_CTRL		2200

using namespace YExcel;

BEGIN_NS_AC

enum ECtrlType
{
	CTRL_EDIT = 0,
	CTRL_CHECK,
	CTRL_STATIC,
	CTRL_COMBOBOX,
	CTRL_CHECKCOMBO,
};

struct SCtrl;

struct SItemTab
{
	CString strName;
	CString strCName;
	CString strKey;
	CString strDes;
	std::vector<SCtrl*> vtCtrls;

	SItemTab()
	{
		strName = _T("");
		strCName = _T("");
		strKey = _T("");
		strDes = _T("");
		vtCtrls.clear();
	}
};

typedef std::map<CString,int> MapCNameToColumnT;

struct SItemDB
{
	virtual int Save(SItemTab* pItemTab){}
	virtual int Load(SItemTab* pItemTab){}
};
struct SItemExcelDB : public SItemDB
{
	BasicExcel* pExcel;
	MapCNameToColumnT mapCNameToColumn;
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
	int nHeadColumn;
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