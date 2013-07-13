#ifndef _TOOL_APP_H_
#define _TOOL_APP_H_

#include "ToolDef.h"
#include "ACLuaUtils.h"

BEGIN_NS_AC

class ToolConfig;
class ToolTree;
class ToolTab;
class ToolLog;

class ToolApp
{
public:
	ToolApp();
	virtual ~ToolApp();
public:
	virtual int InitTool(const CString& strAppName);
	virtual int Update();
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
private:
	static ToolApp* m_pInstance;
protected:
	ToolConfig* m_pConfig;
	ToolTree* m_pTree;
	ToolTab* m_pTab;
	ToolLog* m_pLog;
};

END_NS_AC

#endif//_TOOL_APP_H_