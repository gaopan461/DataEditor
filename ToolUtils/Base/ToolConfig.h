#ifndef _TOOL_CONFIG_H_
#define _TOOL_CONFIG_H_

#include "ToolDef.h"
#include "ToolApp.h"

BEGIN_NS_AC

class ToolConfig : Module<ToolApp>
{
public:
	ToolConfig(ToolApp* app);
public:
	int Load(const CString& strAppName);
private:
	void LoadEditorConfig();
	void LoadEditorDBConfig(SItemTab* pTab);
	void LoadEditorCtrlConfig(CWnd* pCtrlParent);
public:
	void pfnLoadEditorItem(void* ctx);
	void pfnLoadEditorCtrlItem(void* ctx);
private:
	LuaConfig m_objLua;
};

END_NS_AC

#endif//_TOOL_CONFIG_H_