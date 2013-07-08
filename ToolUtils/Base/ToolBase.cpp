#include "stdafx.h"
#include "ToolBase.h"
#include "ACThreadGuard.h"
#include "ACString.h"

BEGIN_NS_AC

#define IDC_LOG 2002

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
		m_vtLogCache.pop_front();
		CString cstr = StlStringToCString(str);
		::SendMessage(m_lpPrintHwnd, LB_ADDSTRING, 0, (LPARAM)cstr.GetBuffer());
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
	DeInitTool();
	m_pInstance = NULL;
}

int ToolBase::InitTool(CWnd* pParent, const std::string& strAppName)
{
	InitLog(pParent, strAppName);
	LoadConfig(strAppName);
	m_objMainTab.Init(pParent,m_objConfig);
	return 0;
}

void ToolBase::InitLog(CWnd* pParent, const std::string& strAppName)
{
	CRect prect,rect;
	pParent->GetClientRect(&prect);
	rect.left = prect.left + 20 + MAIN_TREE_WIDTH;
	rect.top = prect.bottom - 10 - MAIN_LOG_HEIGHT;
	rect.right = prect.right - 10;
	rect.bottom = prect.bottom - 10;

	m_objLogWnd.Create(WS_CHILD|WS_VISIBLE|WS_BORDER, rect, pParent, IDC_LOG);

	std::string strLogFileName = strAppName + ".txt";
	Log::Instance().AddFacility(new LogFacilityFile(strLogFileName));
	Log::Instance().AddFacility(new LogFacilityMFC());
	LogMFC::Instance().OpenLog(m_objLogWnd.m_hWnd);
}

void ToolBase::LoadConfig(const std::string& strAppName)
{
	std::string strConfigFileName = strAppName + ".lua";
	m_objConfig.Load(strConfigFileName);
}

int ToolBase::Update()
{
	LogMFC::Instance().Update();
	return 0;
}

void ToolBase::DeInitTool()
{
	LogMFC::Instance().CloseLog();
}

END_NS_AC