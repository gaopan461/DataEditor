#include "ToolConfig.h"
#include "ACString.h"
#include "ToolCtrl.h"
#include "ToolTab.h"
#include "ToolLayout.h"
#include "ACLog.h"

BEGIN_NS_AC

ToolConfig::ToolConfig(ToolApp* app)
: Module<ToolApp>(app)
{}

int ToolConfig::Load(const CString& strAppName)
{
	CString cstrFileName = strAppName + ".lua";
	std::string stlstrFileName = CStringToStlString(cstrFileName);
	m_objLua.Load(stlstrFileName);

	LoadEditorConfig();

	return 0;
}

void ToolConfig::LoadEditorConfig()
{
	m_objLua.IterTable<ToolConfig>("/EditorConfig",this,&ToolConfig::pfnLoadEditorItem);
}

void ToolConfig::pfnLoadEditorItem(void* ctx)
{
	ACCHECK(m_objLua.IsTopTable());

	std::string stlstr;
	SItemTab* pTab = new SItemTab;
	stlstr = m_objLua.GetString("./Name");
	pTab->strName = StlStringToCString(stlstr);
	stlstr = m_objLua.GetString("./CName");

	CWnd* pWnd = m_pOwner->GetMainTab()->AddTabItem(pTab->strName);

	g_vtItemTabs.push_back(pTab);

	LoadEditorDBConfig(pTab);
	LoadEditorCtrlConfig(pWnd);
}

void ToolConfig::LoadEditorDBConfig(SItemTab* pTab)
{
	ACCHECK(pTab);
	int type = m_objLua.GetInteger("./DB/DBType");
	CString path = StlStringToCString(m_objLua.GetString("./DB/Path"));
	CString key = StlStringToCString(m_objLua.GetString("./DB/Key"));
	CString des = StlStringToCString(m_objLua.GetString("./DB/Des"));

	switch(type)
	{
	case DB_EXCEL:
		{
			int headRow = m_objLua.GetInteger("./DB/HeadRow");
			int dataRow = m_objLua.GetInteger("./DB/DataRow");
			pTab->pDB = new SItemExcelDB(path,key,des,headRow,dataRow);
		}
		break;
	default:
		ERROR_MSG("LoadEditorDBConfig,unknown db type:%d",type);
	}
}

void ToolConfig::LoadEditorCtrlConfig(CWnd* pCtrlParent)
{
	m_pOwner->GetLayout()->Reset();
	m_objLua.IterTable("./Items",this,&ToolConfig::pfnLoadEditorCtrlItem,pCtrlParent);
}

void ToolConfig::pfnLoadEditorCtrlItem(void* ctx)
{
	ACCHECK(m_objLua.IsTopTable());
	CWnd* pParent = (CWnd*)ctx;
	ACCHECK(pParent);

	SCtrl* pCtrl = NULL;
	int ctrl = m_objLua.GetInteger("./Ctrl");
	switch(ctrl)
	{
	case CTRL_EDIT:
		pCtrl = new SEdit;
		break;
	case CTRL_CHECK:
		pCtrl = new SCheck;
		break;
	case CTRL_STATIC:
		pCtrl = new SStatic;
		break;
	case CTRL_COMBOBOX:
		pCtrl = new SCombobox;
		break;
	case CTRL_CHECKCOMBO:
		pCtrl = new SCheckCombo;
		break;
	default:
		ERROR_MSG("Unknown ctrl:%d",ctrl);
		return;
	}

	pCtrl->Init(m_objLua,pParent);

	SItemTab* pLastItem = g_vtItemTabs.back();
	pLastItem->vtCtrls.push_back(pCtrl);
}

END_NS_AC