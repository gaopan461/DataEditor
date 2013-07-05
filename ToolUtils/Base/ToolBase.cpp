#include "stdafx.h"
#include "ToolBase.h"
#include "ACThreadGuard.h"

BEGIN_NS_AC

LogMFC* LogMFC::m_pInstance = 0;
long LogMFC::m_nSingletonGuard = 0;
bool LogMFC::m_bSingletonDestroyed = false;
long LogMFC::m_nLogGuard = 0;

LogMFC::LogMFC()
	: m_bIsClosed(true)
{}

LogMFC::~LogMFC()
{
	m_bSingletonDestroyed = true;
	m_pInstance = 0;
}

void LogMFC::AddLog(const std::string& str)
{
	ThreadGuard guard(&m_nLogGuard);
	m_vtLogCache.push_back(str);
}

int LogMFC::Update()
{
	ThreadGuard guard(&m_nLogGuard);
	if(m_bIsClosed)
		return 0;

	if(!m_vtLogCache.empty())
	{
		std::string str = m_vtLogCache.front();
		str = str + '\n';
		m_vtLogCache.pop_front();
		::SendMessage(m_lpPrintHwnd, LB_ADDSTRING, 0, (LPARAM)str.c_str());
		::SendMessage(m_lpPrintHwnd, WM_VSCROLL, SB_PAGEDOWN, NULL);
	}

	return 0;
}

void LogMFC::OpenLog(HWND lpPrintHwnd)
{
	ThreadGuard guard(&m_nLogGuard);
	if(!m_bIsClosed)
		return;

	m_bIsClosed = false;
	m_lpPrintHwnd = lpPrintHwnd;
}

void LogMFC::CloseLog()
{
	ThreadGuard guard(&m_nLogGuard);
	if(m_bIsClosed)
		return;

	m_bIsClosed = true;
	m_lpPrintHwnd = NULL;
}

//-----------------------------------------------------------

void LogFacilityMFC::Output(const std::string& str)
{
	LogMFC::Instance().AddLog(str);
}

//-----------------------------------------------------------

ToolBase* ToolBase::m_pInstance = NULL;

ToolBase::ToolBase()
{
	m_pInstance = this;
}

ToolBase::~ToolBase()
{
	m_pInstance = NULL;
}

int ToolBase::InitTool(const std::string& filename, HWND lpPrintHwnd)
{
	//初始化日志
	std::string strLogFileName = filename + ".txt";
	Log::Instance().AddFacility(new LogFacilityFile(strLogFileName));
	Log::Instance().AddFacility(new LogFacilityMFC());
	LogMFC::Instance().OpenLog(lpPrintHwnd);

	//lua相关初始化
	std::string strConfigFileName = filename + ".lua";
	m_pLuaConfig = new LuaConfig(strConfigFileName);

	return 0;
}

int ToolBase::Update()
{
	LogMFC::Instance().Update();
	return 0;
}

void ToolBase::DeInitTool()
{
	LogMFC::Instance().CloseLog();

	delete m_pLuaConfig;
}

END_NS_AC