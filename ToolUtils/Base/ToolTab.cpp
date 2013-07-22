// ToolTab.cpp : ʵ���ļ�
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
	for(size_t i = 0; i < m_vtItemTabs.size(); ++i)
		_safe_delete(m_vtItemTabs[i]);

	m_vtItemTabs.clear();
}

BEGIN_MESSAGE_MAP(ToolTab, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void ToolTab::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_pOwner->IsNewing())
	{
		AfxMessageBox(_T("��ǰ�����л�"));
		return;
	}
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
		m_vtItemTabs[m_nTabCurrent]->pWnd->ShowWindow(SW_HIDE);

	m_nTabCurrent = nSel;
	m_vtItemTabs[m_nTabCurrent]->pWnd->ShowWindow(SW_SHOW);
	m_vtItemTabs[m_nTabCurrent]->pWnd->SetFocus();

	m_vtItemTabs[m_nTabCurrent]->pDB->DBToTree(pTree);
	return 0;
}

int ToolTab::DBToCtrl( int key )
{
	SItemTab* pTabItem = m_vtItemTabs[m_nTabCurrent];
	ACCHECK(pTabItem && pTabItem->pDB);
	
	pTabItem->GetDB()->DBToCtrl(pTabItem,key);
	return 0;
}

int ToolTab::CtrlToDB(int key)
{
	SItemTab* pTabItem = m_vtItemTabs[m_nTabCurrent];
	ACCHECK(pTabItem && pTabItem->pDB);

	pTabItem->GetDB()->CtrlToDB(pTabItem,key);
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

	SetFont(&ToolApp::Instance().m_objFont);

	return 0;
}

SItemTab* ToolTab::AddTabItem(const CString& strName)
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

	SItemTab* pTab = new SItemTab;
	pTab->pWnd = pWnd;

	m_vtItemTabs.push_back(pTab);
	return pTab;
}

SItemTab* ToolTab::GetCurrentItem()
{
	return m_vtItemTabs[m_nTabCurrent];
}

bool ToolTab::EnableKeyWindow(bool enable)
{
	CEdit* pCtrlKey = m_vtItemTabs[m_nTabCurrent]->GetKeyWnd();
	ACCHECK(pCtrlKey);

	return pCtrlKey->EnableWindow(enable);
}

int ToolTab::LoadDefaultValues()
{
	m_vtItemTabs[m_nTabCurrent]->LoadDefaultValues();
	return 0;
}


// ToolTab ��Ϣ�������


END_NS_AC