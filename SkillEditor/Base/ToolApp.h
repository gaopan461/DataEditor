#ifndef _TOOL_APP_H_
#define _TOOL_APP_H_

#include "ToolDef.h"
#include "ACLuaUtils.h"

BEGIN_NS_AC

class ToolTree;
class ToolLog;
class ToolExcel;

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
	int MenuDelete();
	int MenuCopy();
	int MenuCancel();
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
	ToolTree* GetMainTree()
	{
		return m_pTree;
	}
	ToolLog* GetMainLog()
	{
		return m_pLog;
	}
	ToolExcel* GetExcelTool()
	{
		return m_pExcel;
	}
	bool IsNewing()
	{
		return m_bIsNewing;
	}
private:
	static ToolApp* m_pInstance;
protected:
	ToolTree* m_pTree;
	ToolLog* m_pLog;
	ToolExcel* m_pExcel;
public:
	CFont m_objFont;
private:
	bool m_bIsNewing;	// �����½�һ������
};

END_NS_AC

#endif//_TOOL_APP_H_