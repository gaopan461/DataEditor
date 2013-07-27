#include "ToolTree.h"
#include "ToolApp.h"
#include "ToolTab.h"

BEGIN_NS_AC

ToolTree::ToolTree(ToolApp* app)
: Module<ToolApp>(app)
, m_nLastSelKey(-1)
, m_pUndefinedRoot(NULL)
{
}

ToolTree::~ToolTree()
{}

BOOL ToolTree::Create(RECT rcRect, CWnd* pWnd, UINT nID)
{
	DWORD dwStyle, dwOptions;

	// Setup the window style
	dwStyle = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	// Setup the tree options 
	// OT_OPTIONS_SHOWINFOWINDOW
	dwOptions = OT_OPTIONS_SHADEEXPANDCOLUMN | OT_OPTIONS_SHADEROOTITEMS;

	// Create tree options
	BOOL ret = COptionTree::Create(dwStyle, rcRect, pWnd, dwOptions, nID);

	// Want to be notified
	SetNotify(TRUE, this);
	return ret;
}

int ToolTree::Create()
{
	CWnd* pParent = m_pOwner->GetMainWnd();
	ACCHECK(pParent);

	CRect prect,rect;
	pParent->GetClientRect(&prect);
	rect.left = prect.left + 10;
	rect.top = prect.top + 10;
	rect.right = prect.left + MAIN_TREE_WIDTH;
	rect.bottom = prect.bottom - 10;

	if(!Create(rect, pParent, IDC_MAIN_TREE))
	{
		ErrorMessageBox(_T("Create main tree failed!"));
		ExitProcess(-1);
	}

	SetColumn(80);

	return 0;
}

int ToolTree::InsertItem( int key, const CString& strDes )
{
	CString strKey;
	strKey.Format(_T("%d"), key);

	COptionTreeItemStaticEx* pOptItem = (COptionTreeItemStaticEx*)COptionTree::InsertItem(new COptionTreeItemStaticEx(),m_pUndefinedRoot);
	ACCHECK(pOptItem);

	pOptItem->SetLabelText(strKey);
	pOptItem->SetInfoText(strKey);
	pOptItem->SetStaticText(strDes);
	pOptItem->CreateStaticItem(0);

	return 0;
}

void ToolTree::OnSelect(int key)
{
	if(key <= 0 || key == m_nLastSelKey)
		return;

	m_nLastSelKey = key;

	m_pOwner->GetMainTab()->DBToCtrl(key);
}

void ToolTree::ResetSelectKey()
{
	m_nLastSelKey = -1;
}

int ToolTree::GetSelectKey()
{
	return m_nLastSelKey;
}

void ToolTree::SelectKey(int key)
{
	if(key <= 0)
		return;

	COptionTreeItem* pTreeItem = FindItemByKey(key);
	if(!pTreeItem)
		return;

	SetFocusedItem(pTreeItem);
	pTreeItem->Select(TRUE);
	UpdatedItems();
}

int ToolTree::InsertUndefinedRoot()
{
	m_pUndefinedRoot = COptionTree::InsertItem(new COptionTreeItem());
	ACCHECK(m_pUndefinedRoot);

	m_pUndefinedRoot->SetLabelText(_T("未分类"));
	m_pUndefinedRoot->SetInfoText(_T("未分类"));
	return 0;
}

COptionTreeItem* ToolTree::FindItemByKey(int key)
{
	// Mark found as NULL
	m_otiFound = NULL;

	// Enumerate items
	EnumItems(&m_otiRoot, EnumFindItemByKey, (LPARAM)key);

	return m_otiFound;
}

BOOL CALLBACK ToolTree::EnumFindItemByKey(COptionTree* otProp, COptionTreeItem* otiItem, LPARAM lParam)
{
	// Validate items
	if (otiItem == NULL)
	{
		return FALSE;
	}

	// Find item
	int nKey = (int)lParam;
	CString strKey;
	strKey.Format(_T("%d"),nKey);
	if (otiItem->GetInfoText() == strKey)
	{
		m_otiFound = otiItem;

		return FALSE;
	}

	return TRUE;
}

END_NS_AC