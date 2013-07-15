#include "ToolConfig.h"
#include "ACString.h"
#include "ToolCtrl.h"
#include "ToolTab.h"
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

	LoadPlatformConfig();
	LoadEditorConfig();

	return 0;
}

void ToolConfig::LoadPlatformConfig()
{
	g_objPlatformConfig.nPlatformType = m_objLua.GetInteger("/PlatformConfig/PlatformType");
	if(g_objPlatformConfig.nPlatformType == PLATFORM_TYPE_EXCEL)
		g_objPlatformConfig.objExcelConfig.nHeadColumn = m_objLua.GetInteger("/PlatformConfig/ExcelHead");
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
	pTab->strCName = StlStringToCString(stlstr);
	stlstr = m_objLua.GetString("./Key");
	pTab->strKey = StlStringToCString(stlstr);
	stlstr = m_objLua.GetString("./Des");
	pTab->strDes = StlStringToCString(stlstr);

	CWnd* pWnd = m_pOwner->GetMainTab()->AddTabItem(pTab->strName);

	g_vtItemTabs.push_back(pTab);

	LoadEditorCtrlConfig(pWnd);
}

void ToolConfig::LoadEditorCtrlConfig(CWnd* pCtrlParent)
{
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
}

END_NS_AC