// COptionTree
//
// License
// -------
// This code is provided "as is" with no expressed or implied warranty.
// 
// You may use this code in a commercial product with or without acknowledgement.
// However you may not sell this code or any modification of this code, this includes 
// commercial libraries and anything else for profit.
//
// I would appreciate a notification of any bugs or bug fixes to help the control grow.
//
// History:
// --------
//	See License.txt for full history information.
//
//
// Copyright (c) 1999-2002 
// ComputerSmarts.net 
// mattrmiller@computersmarts.net
#include "stdafx.h"
#include "OptionTreeItemEdit.h"

// Added Headers
#include "OptionTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemEdit

COptionTreeItemEdit::COptionTreeItemEdit()
{
	// Initialize variables
	m_bFocus = FALSE;
	m_dwOptions = 0;	

	// Set item type
	SetItemType(OT_ITEM_EDIT);
}

COptionTreeItemEdit::~COptionTreeItemEdit()
{
}


BEGIN_MESSAGE_MAP(COptionTreeItemEdit, CEdit)
	//{{AFX_MSG_MAP(COptionTreeItemEdit)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionTreeItemEdit message handlers

void COptionTreeItemEdit::OnActivate()
{
	// Make sure window is valid
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_SHOW);

		// -- Set selection
		SetSel(0, 0);

		// -- Set window position
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());

		// -- Set focus
		SetFocus();
	}
}

UINT COptionTreeItemEdit::OnGetDlgCode() 
{
	return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void COptionTreeItemEdit::DrawAttribute(CDC *pDC, const RECT &rcRect)
{
	// If we don't have focus, text is drawn.
	if (m_bFocus == TRUE)
	{
		return;
	}

	// Make sure options aren't NULL
	if (m_otOption == NULL)
	{
		return;
	}

	// Make sure there is a window
	if (!IsWindow(GetSafeHwnd()))
	{
		return;
	}

	// Set window position
	if (IsWindow(GetSafeHwnd()))
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());
	}

	// Declare variables
	HGDIOBJ hOld;
	COLORREF crOld;
	int nOldBack;
	CRect rcText;
	CString strText, strWindowText;
	COLORREF crOldBack;

	// Get window text
	GetWindowText(strWindowText);

	// Make text
	// -- Password
	if (GetOption(OT_EDIT_PASSWORD) == TRUE)
	{
		strText = "";
		for (int i = 0; i < strWindowText.GetLength(); i++)
		{
			strText += _T("*");
		}
	}
	// -- No password
	else
	{
		strText = strWindowText;
	}

	// Select font
	hOld = pDC->SelectObject(m_otOption->GetNormalFont());
	
	// Set text color
	if (IsReadOnly() == TRUE || m_otOption->IsWindowEnabled() == FALSE)
	{
		crOld = pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	}
	else
	{
		crOld = pDC->SetTextColor(GetTextColor());
	}

	// Set background mode
	nOldBack = pDC->SetBkMode(TRANSPARENT);

	// Set background color
	crOldBack = pDC->SetBkColor(GetBackgroundColor());	

	// Get rectangle
	rcText = rcRect;

	// Draw text
	// -- Single line
	if (GetOption(OT_EDIT_MULTILINE) == FALSE)
	{
		pDC->DrawText(strText, rcText, DT_SINGLELINE | DT_VCENTER);
	}
	// -- Mutliline
	else
	{
		pDC->DrawText(strText, rcText, DT_TOP);
	}

	// Restore GDI ojects
	pDC->SelectObject(hOld);
	pDC->SetTextColor(crOld);
	pDC->SetBkMode(nOldBack);
	pDC->SetBkColor(crOldBack);
}

void COptionTreeItemEdit::OnCommit()
{
	// Hide edit control
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}

}

void COptionTreeItemEdit::OnRefresh()
{
	// Set the window text
	if (IsWindow(GetSafeHwnd()))
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());
	}
}

void COptionTreeItemEdit::OnMove()
{
	// Set window position
	if (IsWindow(GetSafeHwnd()))
	{
		MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());
	}

	// Hide window
	if (m_bFocus == FALSE && IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// If return is pressed and not multiline, commit
/**  RuralMouse Modified 2011-03-24
	if (IsWindow(GetSafeHwnd()))
	{
		if (nChar == VK_RETURN && (GetStyle() & ES_WANTRETURN) == FALSE)
		{
			// -- Commit changes
			CommitChanges();

			return;
		}
	}
*/
	// If return is pressed and not multiline, commit
	if (IsWindow(GetSafeHwnd()))	
	{		
		//Send message to parent if its tab (always) or up/down (always for single line,		
		//only if we're at the beginning / ending line for up and down. For left/right		
		//only if we're at the beginning / end of all of the text.		
		int nKeyToSendToParent = 0;		
		int nStartSel=0, nEndSel=0;		
		GetSel(nStartSel, nEndSel);		
		DWORD dwStyle = GetStyle();		
		if (nChar == VK_RETURN && (dwStyle & ES_WANTRETURN) == FALSE)		
		{			
			// -- Commit changes -- uncomment to restore original behavior.			
			// CommitChanges();			
			// -- I wanted it to activate the next item as well.			
			// -- Comment out the next line (and uncomment out the first line)			
			//    if you don't want this behavior.			
			nKeyToSendToParent = VK_TAB;		
		}		
		else if (VK_TAB == nChar)
		{ 
			nKeyToSendToParent = nChar; 
		}		
		else if (VK_UP == nChar)		
		{			
			if (!(dwStyle & ES_WANTRETURN))
			{ 
				nKeyToSendToParent = nChar; 
			} //Always for single line			
			else //Multi-line controls only if we're at the top.			
			{				
				if (0 == LineFromChar())
				{ 
					nKeyToSendToParent = nChar; 
				}			
			}		
		}		
		else if (VK_DOWN == nChar)		
		{			
			if (!(dwStyle & ES_WANTRETURN))
			{ 
				nKeyToSendToParent = nChar; 
			}		//Always for single line			
			else	//Multi-line controls only if we're at the bottom.			
			{				
				if ((GetLineCount()-1) == LineFromChar())
				{
					nKeyToSendToParent = nChar; 
				}			
			}		
		}		
		//Only if we're at the very beginning and no selection.		
		else if (VK_LEFT == nChar && 0 == nStartSel && (nStartSel == nEndSel))		
		{			
			nKeyToSendToParent = VK_UP;		
			//Use VK_UP to move to previous control.		
		}		
		//Only if we're at the very end and no selection.		
		else if (VK_RIGHT == nChar && GetWindowTextLength() == nEndSel && nStartSel == nEndSel)		
		{			
			nKeyToSendToParent = VK_DOWN;	//Use VK_DOWN to move to previous control.		
		}		
		if (0 != nKeyToSendToParent)		
		{			
			//Send this to the parent so that the focus can properly be shifted (keycode was possibly modified).			
			const MSG* pCurMsg = GetCurrentMessage();			
			CEdit::GetParent()->SendMessage(pCurMsg->message, nKeyToSendToParent, pCurMsg->lParam);			
			return;		
		}	
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COptionTreeItemEdit::OnKillFocus(CWnd* pNewWnd) 
{
	// Validate
	if (m_otOption == NULL)
	{
		CEdit::OnKillFocus(pNewWnd);
		return;
	}

	// See if new window is tree of list
	if (m_otOption->IsChild(pNewWnd) == TRUE)
	{
		// -- Mark focus
		m_bFocus = FALSE;

		// -- Commit changes
		CommitChanges();
	}
	
	CEdit::OnKillFocus(pNewWnd);	
}

BOOL COptionTreeItemEdit::IsStringNumeric(CString strString)
{
	// See if string is numeric or not
	if (strString.FindOneOf(_T("1234567890")) == -1 || strString.FindOneOf(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`~[]{}-_=+\\|'/?>,<")) != -1)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL COptionTreeItemEdit::GetEditFloat(float &fReturn)
{
	// Declare variables
	CString strWindowText;

	// Set blank
	fReturn = 0;

	// Get window text
	if (IsWindow(GetSafeHwnd()))
	{
		GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// First see if we are in numerical mode
	if (GetOption(OT_EDIT_NUMERICAL) == FALSE)
	{
		return FALSE;
	}

	// See if string is numeric
	if (IsStringNumeric(strWindowText) == FALSE)
	{
		return FALSE;
	}

	// Convert string
    fReturn = (float)_tstof(strWindowText);

	return TRUE;
}

BOOL COptionTreeItemEdit::GetEditDouble(double &dReturn)
{
	// Declare variables
	CString strWindowText;

	// Set blank
	dReturn = 0;

	// Get window text
	if (IsWindow(GetSafeHwnd()))
	{
		GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// First see if we are in numerical mode
	if (GetOption(OT_EDIT_NUMERICAL) == FALSE)
	{
		return FALSE;
	}

	// See if string is numeric
	if (IsStringNumeric(strWindowText) == FALSE)
	{
		return FALSE;
	}

	// Convert string
    dReturn = _tstof(strWindowText);

	return TRUE;
}

BOOL COptionTreeItemEdit::GetEditLong(long &lReturn)
{
	// Declare variables
	CString strWindowText;

	// Set blank
	lReturn = 0;

	// Get window text
	if (IsWindow(GetSafeHwnd()))
	{
		GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// First see if we are in numerical mode
	if (GetOption(OT_EDIT_NUMERICAL) == FALSE)
	{
		return FALSE;
	}

	// See if string is numeric
	if (IsStringNumeric(strWindowText) == FALSE)
	{
		return FALSE;
	}

	// Convert string
	lReturn = _tstol(strWindowText);

	return TRUE;
}

BOOL COptionTreeItemEdit::GetEditInt(int &nReturn)
{
	// Declare variables
	CString strWindowText;

	// Set blank
	nReturn = 0;

	// Get window text
	if (IsWindow(GetSafeHwnd()))
	{
		GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// First see if we are in numerical mode
	if (GetOption(OT_EDIT_NUMERICAL) == FALSE)
	{
		return FALSE;
	}

	// See if string is numeric
	if (IsStringNumeric(strWindowText) == FALSE)
	{
		return FALSE;
	}

	// Convert string
	nReturn = _tstoi(strWindowText);

	return TRUE;
}

BOOL COptionTreeItemEdit::GetEditDword(DWORD &dwReturn)
{
	// Declare variables
	CString strWindowText;

	// Set blank
	dwReturn = 0;

	// Get window text
	if (IsWindow(GetSafeHwnd()))
	{
		GetWindowText(strWindowText);
	}
	else
	{
		return FALSE;
	}

	// First see if we are in numerical mode
	if (GetOption(OT_EDIT_NUMERICAL) == FALSE)
	{
		return FALSE;
	}

	// See if string is numeric
	if (IsStringNumeric(strWindowText) == FALSE)
	{
		return FALSE;
	}

	// Convert string
	dwReturn = (DWORD) _tstoi(strWindowText);

	return TRUE;
}

void COptionTreeItemEdit::CleanDestroyWindow()
{
	// Destroy window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Destroy window
		DestroyWindow();
	}
}

void COptionTreeItemEdit::OnSetFocus(CWnd* pOldWnd) 
{
	// Mark focus
	m_bFocus = TRUE;
	
	CEdit::OnSetFocus(pOldWnd);	
}

BOOL COptionTreeItemEdit::CreateEditItem(DWORD dwOptions, DWORD dwAddStyle)
{
	// Declare variables
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
	BOOL bRet = FALSE;


	// Make sure options is not NULL
	if (m_otOption == NULL)
	{
		return FALSE;
	}

	// Create edit control
	if (!IsWindow(GetSafeHwnd()))
	{
		// -- Create edit style
		// -- -- Multiline
		if (dwOptions & OT_EDIT_MULTILINE)
		{
			dwStyle |= ES_MULTILINE;
		}
		// -- -- Password
		if (dwOptions & OT_EDIT_PASSWORD)
		{
			dwStyle |= ES_PASSWORD;
		}
		// -- -- Numeical
		if (dwOptions & OT_EDIT_NUMERICAL)
		{
			dwStyle |= ES_NUMBER;
		}
		// -- -- Additional style
		if (dwAddStyle != 0)
		{
			dwStyle |= dwAddStyle;
		}

		// -- Create the edit view
		bRet = Create(dwStyle, m_rcAttribute, m_otOption->GetCtrlParent(), GetCtrlID());

		// -- Setup edit
		if (bRet == TRUE)
		{
			// -- -- Set font
			SetFont(m_otOption->GetNormalFont(), TRUE);

			// -- -- Modify style
			ModifyStyleEx(0, WS_EX_CLIENTEDGE, SWP_FRAMECHANGED);

			// -- -- Save options
			m_dwOptions = dwOptions;

			// -- -- Set selection
			SetSel(0, 0);

			// -- -- Set multiline
			if (GetOption(OT_EDIT_MULTILINE) == TRUE)
			{
				SetItemHeight(OT_EDIT_MLHEIGHT);
			}

			// -- -- Set window position
			MoveWindow(m_rcAttribute.left, m_rcAttribute.top, m_rcAttribute.Width(), m_rcAttribute.Height());

			// -- -- Hide window
			ShowWindow(SW_HIDE);
		}
	}

	return bRet;
}

void COptionTreeItemEdit::SetEditDouble(double dValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format(_T("%.0f"), dValue);

	// Modify style
	if (::IsWindow(GetSafeHwnd()))
	{
		SetWindowText(strText);
	}
}

void COptionTreeItemEdit::SetEditInt(int nValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format(_T("%d"), nValue);

	// Modify style
	if (::IsWindow(GetSafeHwnd()))
	{
		SetWindowText(strText);
	}
}

void COptionTreeItemEdit::SetEditFloat(float fValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format(_T("%.0f"), fValue);

	// Modify style
	if (::IsWindow(GetSafeHwnd()))
	{
		SetWindowText(strText);
	}
}

void COptionTreeItemEdit::SetEditDword(DWORD dwValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format(_T("%d"), dwValue);

	// Modify style
	if (::IsWindow(GetSafeHwnd()))
	{
		SetWindowText(strText);
	}
}

void COptionTreeItemEdit::SetEditLong(long lValue)
{
	// Declare variables
	CString strText;

	// Convert string
	strText.Format(_T("%.0f"), lValue);

	// Modify style
	if (::IsWindow(GetSafeHwnd()) == TRUE)
	{
		SetWindowText(strText);
	}
}

void COptionTreeItemEdit::OnDeSelect()
{
	// Hide window
	if (IsWindow(GetSafeHwnd()))
	{
		// -- Show window
		ShowWindow(SW_HIDE);
	}
}

void COptionTreeItemEdit::OnSelect()
{
	// Do nothing here
}

BOOL COptionTreeItemEdit::GetOption(DWORD dwOption)
{
	// Return option
	return (m_dwOptions & dwOption) ? TRUE : FALSE;
}

void COptionTreeItemEdit::SetOption(DWORD dwOption, BOOL bSet)
{
	// Set option
	if (bSet == TRUE)
	{
		m_dwOptions |= dwOption;
	}
	else
	{
		m_dwOptions &= ~dwOption;
	}

	// Options changed
	OptionsChanges();
}

void COptionTreeItemEdit::OptionsChanges()
{
	// Multiline
	SetDrawMultiline(GetOption(OT_EDIT_MULTILINE));
	if (GetOption(OT_EDIT_MULTILINE) == TRUE)
	{
		SetItemHeight(OT_EDIT_MLHEIGHT);

		if (IsWindow(GetSafeHwnd()))
		{
			ModifyStyle(0, ES_MULTILINE, 0);
		}
	}
	else
	{
		SetItemHeight(OT_DEFHEIGHT);

		if (IsWindow(GetSafeHwnd()))
		{
			ModifyStyle(ES_MULTILINE, 0, 0);
		}
	}

	// Password
	if (GetOption(OT_EDIT_PASSWORD) == TRUE)
	{
		if (IsWindow(GetSafeHwnd()))
		{
			ModifyStyle(0, ES_PASSWORD, 0);
		}
	}
	else
	{
		if (IsWindow(GetSafeHwnd()))
		{
			ModifyStyle(ES_PASSWORD, 0, 0);
		}
	}

	// Numerical
	SetDrawMultiline(GetOption(OT_EDIT_NUMERICAL));
	if (GetOption(OT_EDIT_NUMERICAL) == TRUE)
	{
		SetItemHeight(OT_EDIT_MLHEIGHT);

		if (IsWindow(GetSafeHwnd()))
		{
			ModifyStyle(0, ES_MULTILINE, 0);
		}
	}
	else
	{
		SetItemHeight(OT_DEFHEIGHT);

		if (IsWindow(GetSafeHwnd()))
		{
			ModifyStyle(ES_MULTILINE, 0, 0);
		}
	}
}

void COptionTreeItemEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	// Ignore special characters, since these are handled in WM_KEYDOWN	
	// This avoids a ding sound when these keys are pressed!	
	if (VK_TAB == nChar || (VK_RETURN == nChar && !(GetStyle() & ES_MULTILINE)))
	{ 
		return; 
	}	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
