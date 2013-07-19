// ToolTab.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolTab.h"
#include "ACString.h"

BEGIN_NS_AC

// ToolTab

IMPLEMENT_DYNAMIC(ToolTab, CTabCtrl)

ToolTab::ToolTab(ToolApp* app)
: Module<ToolApp>(app)
{
	m_nTabCurrent = -1;
}

ToolTab::~ToolTab()
{
	for(std::vector<CWnd*>::iterator it = m_vtTabWnds.begin(),ed = m_vtTabWnds.end(); it != ed; ++it)
	{
		delete *it;
	}

	m_vtTabWnds.clear();
}

BEGIN_MESSAGE_MAP(ToolTab, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void ToolTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);
	ChangeTab(GetCurFocus());
}

int ToolTab::ChangeTab(int nSel)
{
	if(m_nTabCurrent == nSel)
		return 0;

	if(nSel < 0 || nSel >= GetItemCount())
		return -1;

	ToolTree* pTree = ToolApp::Instance().GetMainTree();
	ACCHECK(pTree);

	if(m_nTabCurrent != -1)
		m_vtTabWnds[m_nTabCurrent]->ShowWindow(SW_HIDE);

	m_nTabCurrent = nSel;
	m_vtTabWnds[m_nTabCurrent]->ShowWindow(SW_SHOW);
	m_vtTabWnds[m_nTabCurrent]->SetFocus();

	g_vtItemTabs[m_nTabCurrent]->pDB->DBToTree(pTree);
	return 0;
}

int ToolTab::Create()
{
	CWnd* pParent = m_pOwner->GetMainWnd();
	ACCHECK(pParent);

	CRect prect,rect;
	pParent->GetClientRect(&prect);
	rect.left = prect.left + 20 + MAIN_TREE_WIDTH;
	rect.top = prect.top + 10;
	rect.right = prect.right - 10;
	rect.bottom = prect.bottom - 20 - MAIN_LOG_HEIGHT;

	if(!CTabCtrl::Create(WS_CHILD|WS_VISIBLE,rect,pParent,IDC_MAIN_TAB))
	{
		AfxMessageBox(_T("Create main tab ctrl failed!"));
		ExitProcess(-1);
	}

	return 0;
}

CWnd* ToolTab::AddTabItem(const CString& strName)
{
	CRect rect;
	GetClientRect(&rect);
	rect.top += 20;
	rect.left += 20;
	rect.bottom -= 20;
	rect.right -= 20;

	CWnd* pWnd = new CWnd();
	if(!pWnd->Create(NULL, NULL, WS_CHILD, rect, this, IDC_TAB_ITEM + GetItemCount()))
	{
		AfxMessageBox(_T("AddTabItem,create tab item windown failed!"));
		ExitProcess(-1);
	}

	InsertItem(GetItemCount(),strName);
	m_vtTabWnds.push_back(pWnd);
	return pWnd;
}


// ToolTab 消息处理程序


END_NS_AC