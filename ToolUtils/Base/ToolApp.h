#ifndef _TOOL_APP_H_
#define _TOOL_APP_H_

#include "ToolDef.h"
#include "ACLuaUtils.h"

BEGIN_NS_AC

class ToolConfig;
class ToolTree;
class ToolTab;
class ToolLog;
class ToolLayout;

class ToolApp
{
public:
	ToolApp();
	virtual ~ToolApp();
public:
	virtual int InitializeTool(const CString& strAppName);
	virtual int Update();
	virtual int FinalizeTool();
public:
	int MenuNew();
	int MenuSave();
public:
	virtual CWnd* GetMainWnd()
	{
		return NULL;
	}
public:
	static ToolApp& Instance()
	{
		return *m_pInstance;
	}
public:
	ToolConfig* GetMainConfig()
	{
		return m_pConfig;
	}
	ToolTree* GetMainTree()
	{
		return m_pTree;
	}
	ToolTab* GetMainTab()
	{
		return m_pTab;
	}
	ToolLog* GetMainLog()
	{
		return m_pLog;
	}
	ToolLayout* GetLayout()
	{
		return m_pLayout;
	}
	bool IsNewing()
	{
		return m_bIsNewing;
	}
private:
	static ToolApp* m_pInstance;
protected:
	ToolConfig* m_pConfig;
	ToolTree* m_pTree;
	ToolTab* m_pTab;
	ToolLog* m_pLog;
	ToolLayout* m_pLayout;
	CMenu* m_pMenu;
public:
	CFont m_objFont;
private:
	bool m_bIsNewing;	// 正在新建一项数据
};

END_NS_AC

#endif//_TOOL_APP_H_