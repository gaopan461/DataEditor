// ToolLog.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolLog.h"
#include "ToolApp.h"
#include "ACString.h"

BEGIN_NS_AC


// ToolLog

IMPLEMENT_DYNAMIC(ToolLog, CListBox)

ToolLog::ToolLog(ToolApp* app)
: Module<ToolApp>(app)
{
}

ToolLog::~ToolLog()
{
}


BEGIN_MESSAGE_MAP(ToolLog, CListBox)
END_MESSAGE_MAP()


// ToolLog 消息处理程序


void ToolLog::Create(const CString& strAppName)
{
	CWnd* pParent = m_pOwner->GetMainWnd();
	ACCHECK(pParent);

	CRect prect,rect;
	pParent->GetClientRect(&prect);
	rect.left = prect.left + 20 + MAIN_TREE_WIDTH;
	rect.top = prect.bottom - 10 - MAIN_LOG_HEIGHT;
	rect.right = prect.right - 10;
	rect.bottom = prect.bottom - 10;

	if(!CListBox::Create(WS_CHILD|WS_VISIBLE|WS_BORDER, rect, pParent, IDC_MAIN_LOG))
	{
		ErrorMessageBox(_T("Create log window failed!"));
		ExitProcess(-1);
	}

	SetFont(&ToolApp::Instance().m_objFont);

	CString cstrLogFileName = strAppName + _T(".txt");
	std::string stlstrLogFileName = CStringToStlString(cstrLogFileName);
	Log::Instance().AddFacility(new LogFacilityFile(stlstrLogFileName));
	Log::Instance().AddFacility(new LogFacilityMFC());
}

int ToolLog::Update()
{
	if(!m_vtLogCache.empty())
	{
		CString cstr = m_vtLogCache.front();
		m_vtLogCache.pop_front();
		SendMessage(LB_ADDSTRING, 0, (LPARAM)cstr.GetBuffer());
		SendMessage(WM_VSCROLL, SB_PAGEDOWN, NULL);
	}

	return 0;
}

void ToolLog::AddLog(const CString& str)
{
	m_vtLogCache.push_back(str);
}

//-----------------------------------------------------------

void LogFacilityMFC::Output(const std::string& str)
{
	CString cstr = StlStringToCString(str);
	ToolApp::Instance().GetMainLog()->AddLog(cstr);
}

END_NS_AC