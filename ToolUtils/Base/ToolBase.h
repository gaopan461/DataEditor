#ifndef _TOOL_BASE_H_
#define _TOOL_BASE_H_

#include "ACLog.h"
#include <Windows.h>
#include <deque>
#include <afxwin.h>
#include "ACThreadGuard.h"
#include "ACLuaUtils.h"

BEGIN_NS_AC

/*
 *	MFC��־��ʵ�������־�ĵط�����־�������LISTBOX
 *  ÿ��Updateʱ�ӻ�����ȡ��һ����־���LISTBOXĩβ
 */
class LogMFC
{
	static void CreateInstance();
	static LogMFC* m_pInstance;
	static long m_nSingletonGuard;
	static bool m_bSingletonDestroyed;
protected:
	static long m_nLogGuard;
public:
	static LogMFC& Instance();
	LogMFC();
	virtual ~LogMFC();
public:
	/*
	 *	�Ƚ���־��¼��������
	 */
	void AddLog(const std::string& str);
	
	/*
	 *	����������־���µ�LISTBOX��ʵ�������־�ĵط���
	 */
	int Update();

	/*
	 *	����־���
	 */
	void OpenLog(HWND lpPrintHwnd);

	/*
	 *	�ر���־���
	 */
	void CloseLog();
private:
	HWND m_lpPrintHwnd;						//������־�Ĵ��ڣ�LISTBOX��
	std::deque<std::string> m_vtLogCache;	//��־������
	bool m_bIsClosed;						//�����־�Ƿ�����
};

inline LogMFC& LogMFC::Instance()
{
	if(!m_pInstance)
		CreateInstance();

	return *m_pInstance;
}

inline void LogMFC::CreateInstance()
{
	acoral::ThreadGuard guard(&m_nSingletonGuard);
	if(m_pInstance)
		return;

	ACCHECK(!m_bSingletonDestroyed);
	static LogMFC obj;
	m_pInstance = &obj;
}

//-----------------------------------------------------------

/*
 *	MFC��־��ʩ
 */
class LogFacilityMFC : public LogFacilityBase
{
public:
	LogFacilityMFC(){}
	LOG_INTERFACE ~LogFacilityMFC(){}
public:
	LOG_INTERFACE void Output(const std::string& str);
};

//-----------------------------------------------------------

class ToolBase
{
public:
	ToolBase();
	virtual ~ToolBase();
public:
	virtual int InitTool(const std::string& filename, HWND lpPrintHwnd);
	virtual int Update();
	virtual void DeInitTool();
public:
	lua_State* GetLuaState()
	{
		return m_pLuaConfig->GetLuaState();
	}
public:
	static ToolBase& Instance()
	{
		return *m_pInstance;
	}
private:
	static ToolBase* m_pInstance;
protected:
	LuaConfig* m_pLuaConfig;
};

END_NS_AC

#endif//_TOOL_BASE_H_