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
	int Initial(const CString& strAppName);
private:
	void LoadPlatformConfig();
	void LoadEditorConfig();
	void LoadEditorCtrlConfig();
public:
	void pfnLoadEditorItem();
	void pfnLoadEditorCtrlItem();
private:
	LuaConfig m_objLua;
};

END_NS_AC

#endif//_TOOL_CONFIG_H_