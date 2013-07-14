#include "ToolConfig.h"
#include "ACString.h"

BEGIN_NS_AC

ToolConfig::ToolConfig(ToolApp* app)
: Module<ToolApp>(app)
{}

int ToolConfig::Initial(const CString& strAppName)
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

void ToolConfig::pfnLoadEditorItem()
{
	ACCHECK(m_objLua.IsTopTable());

	std::string stlstr;
	SItemTab itemTab;
	stlstr = m_objLua.GetString("./Name");
	itemTab.strName = StlStringToCString(stlstr);
	stlstr = m_objLua.GetString("./CName");
	itemTab.strCName = StlStringToCString(stlstr);
	stlstr = m_objLua.GetString("./Key");
	itemTab.strKey = StlStringToCString(stlstr);
	stlstr = m_objLua.GetString("./Des");
	itemTab.strDes = StlStringToCString(stlstr);

	LoadEditorCtrlConfig();
}

void ToolConfig::LoadEditorCtrlConfig()
{
	m_objLua.IterTable("./Items",this,&ToolConfig::pfnLoadEditorCtrlItem);
}

void ToolConfig::pfnLoadEditorCtrlItem()
{
	ACCHECK(m_objLua.IsTopTable());
}

END_NS_AC