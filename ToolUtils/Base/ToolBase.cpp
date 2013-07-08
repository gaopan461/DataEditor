#include "stdafx.h"
#include "ToolBase.h"
#include "ACThreadGuard.h"
#include "ACString.h"

BEGIN_NS_AC

#define IDC_MAIN_TAB 2000
#define IDC_MAIN_TREE 2001
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
	LoadConfig(strAppName);

	CreateLogWnd(pParent, strAppName);
	CreateMainTab(pParent);
	CreateMainTree(pParent);

	INFO_MSG("--------------------------------------------");
	INFO_MSG("               %s Start             ",strAppName.c_str());
	INFO_MSG("--------------------------------------------");

	return 0;
}

void ToolBase::CreateLogWnd(CWnd* pParent, const std::string& strAppName)
{
	CRect prect,rect;
	pParent->GetClientRect(&prect);
	rect.left = prect.left + 20 + MAIN_TREE_WIDTH;
	rect.top = prect.bottom - 10 - MAIN_LOG_HEIGHT;
	rect.right = prect.right - 10;
	rect.bottom = prect.bottom - 10;

	if(!m_objLogWnd.Create(WS_CHILD|WS_VISIBLE|WS_BORDER, rect, pParent, IDC_LOG))
	{
		AfxMessageBox(_T("Create log window failed!"));
		ExitProcess(-1);
	}

	std::string strLogFileName = strAppName + ".txt";
	Log::Instance().AddFacility(new LogFacilityFile(strLogFileName));
	Log::Instance().AddFacility(new LogFacilityMFC());
	LogMFC::Instance().OpenLog(m_objLogWnd.m_hWnd);
}

void ToolBase::CreateMainTree(CWnd* pParent)
{
	CRect prect,rect;
	pParent->GetClientRect(&prect);
	rect.left = prect.left + 10;
	rect.top = prect.top + 10;
	rect.right = prect.left + MAIN_TREE_WIDTH;
	rect.bottom = prect.bottom - 10;

	if(!m_objMainTree.Create(rect, pParent, IDC_MAIN_TREE))
	{
		AfxMessageBox(_T("Create main tree failed!"));
		ExitProcess(-1);
	}
}

void ToolBase::CreateMainTab(CWnd* pParent)
{
	CRect prect,rect;
	pParent->GetClientRect(&prect);
	rect.left = prect.left + 20 + MAIN_TREE_WIDTH;
	rect.top = prect.top + 10;
	rect.right = prect.right - 10;
	rect.bottom = prect.bottom - 20 - MAIN_LOG_HEIGHT;

	if(!m_objMainTab.Create(WS_CHILD|WS_VISIBLE,rect,pParent,IDC_MAIN_TAB))
	{
		AfxMessageBox(_T("Create main tab ctrl failed!"));
		ExitProcess(-1);
	}

	m_objMainTab.InitTabItems(m_objConfig);
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