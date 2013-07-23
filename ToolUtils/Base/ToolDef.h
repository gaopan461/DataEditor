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

#define EXCEL_ARRAY_DELIMITER _T(",")

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

	virtual int CtrlToDB(SItemTab* pItemTab,int key){return 0;}
	virtual int DBToCtrl(SItemTab* pItemTab,int key){return 0;}

	virtual int DBToTree(ToolTree* pTree){return 0;}

	virtual bool InsertNewKey(int key){return false;}
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

	virtual int CtrlToDB(SItemTab* pItemTab,int key);
	virtual int DBToCtrl(SItemTab* pItemTab,int key);

	virtual int DBToTree(ToolTree* pTree);

	virtual bool InsertNewKey(int key);

	int InitMapNameToColumn();

	int Find(int key,int& row,int& sheet);

	int DataToEdit(SEdit* pCtrl,CString data);
	int DataToCheck(SCheck* pCtrl,CString data);
	int DataToCombobox(SCombobox* pCtrl,CString data);
	int DataToCheckCombo(SCheckCombo* pCtrl,CString data);

	int EditToData(SEdit* pCtrl,CString& data);
	int CheckToData(SCheck* pCtrl,CString& data);
	int ComboboxToData(SCombobox* pCtrl,CString& data);
	int CheckComboToData(SCheckCombo* pCtrl,CString& data);
};

//-----------------------------------------------------------

struct SItemTab
{
	CString strName;
	CString strCName;
	SItemDB* pDB;
	std::vector<SCtrl*> vtCtrls;
	CWnd* pWnd;		// tab项window，每个tab项是一个window
	CEdit* pKeyWnd;	// key对应控件的window

	SItemTab();
	virtual ~SItemTab();

	SItemDB* GetDB(){return pDB;}
	CEdit* GetKeyWnd(){return pKeyWnd;}
	int LoadDefaultValues();
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