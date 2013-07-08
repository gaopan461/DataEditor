// ACTabCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ACTabCtrl.h"
#include "BasicExcel.h"
#include "ACString.h"

BEGIN_NS_AC

#define IDC_MAIN_TAB 2000
#define IDC_TAB_ITEM 2100

// ACTabCtrl

IMPLEMENT_DYNAMIC(ACTabCtrl, CTabCtrl)

ACTabCtrl::ACTabCtrl()
{
	m_nTabCurrent = 0;
}

ACTabCtrl::~ACTabCtrl()
{
	for(std::vector<STabItem>::iterator it = m_vtTabItems.begin(),ed = m_vtTabItems.end(); it != ed; ++it)
	{
		STabItem& rTabItem = *it;
		delete rTabItem.m_pWnd;
		delete rTabItem.m_pExcel;
	}

	m_vtTabItems.clear();
}

BEGIN_MESSAGE_MAP(ACTabCtrl, CTabCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void ACTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTabCtrl::OnLButtonDown(nFlags, point);

	if (m_nTabCurrent != GetCurFocus())
	{
		m_vtTabItems[m_nTabCurrent].m_pWnd->ShowWindow(SW_HIDE);
		m_nTabCurrent = GetCurFocus();
		m_vtTabItems[m_nTabCurrent].m_pWnd->ShowWindow(SW_SHOW);
		m_vtTabItems[m_nTabCurrent].m_pWnd->SetFocus();
	}
}

void ACTabCtrl::Init(CWnd* pParent, LuaConfig& rConfig)
{
	Create(pParent);

	InitTabItems(rConfig);
}

void ACTabCtrl::InitTabItems(LuaConfig& rConfig)
{
	// 初始化tab选项卡
	AddTabItem(_T("技能"), _T("技能.xls"));
	AddTabItem(_T("Buff"), _T("Buff.xls"));
}

void ACTabCtrl::Create(CWnd* pParent)
{
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
}

void ACTabCtrl::AddTabItem(const CString& strName, const CString& strExcel)
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

	BasicExcel* pExcel = new BasicExcel();
	if(!pExcel->Load(CStringToStlString(strExcel).c_str()))
	{
		CString strErr;
		strErr.Format(_T("AddTabItem,load excel failed : %s"),strExcel);
		AfxMessageBox(strErr.GetBuffer());
		ExitProcess(-1);
	}

	InsertItem(GetItemCount(),strName);
	m_vtTabItems.push_back(STabItem(pWnd,pExcel));
}


// ACTabCtrl 消息处理程序


END_NS_AC