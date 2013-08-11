#include "stdafx.h"
#include "ToolTree.h"
#include "ToolApp.h"
#include "Resource.h"

BEGIN_NS_AC

ToolTree::ToolTree()
: m_nLastSelKey(-1)
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
	CWnd* pParent = ToolApp::Instance().GetMainWnd();
	ACCHECK(pParent);

	CRect rect;
	pParent->GetDlgItem(IDC_MAIN_TREE)->GetWindowRect(&rect);
	pParent->ScreenToClient(&rect);

	if(!Create(rect, pParent, IDC_MAIN_TREE))
	{
		ErrorMessageBox(_T("Create main tree failed!"));
		ExitProcess(-1);
	}

	SetColumn(80);

	return 0;
}

int ToolTree::UpdateOrInsertItemByKey(int key,CString& strDes,std::vector<CString>& vtLayers)
{
	CString strKey;
	strKey.Format(_T("%d"), key);

	COptionTreeItem* pInsertPosition = FindOrCreateLayers(vtLayers);
	COptionTreeItemStaticEx* pOptItem = (COptionTreeItemStaticEx*)FindItemByInfoText(pInsertPosition,strKey);
	if(pOptItem == NULL)
	{
		pOptItem = (COptionTreeItemStaticEx*)COptionTree::InsertItem(new COptionTreeItemStaticEx(),pInsertPosition);
		pOptItem->CreateStaticItem(0);
	}

	ACCHECK(pOptItem);

	pOptItem->SetLabelText(strKey);
	pOptItem->SetInfoText(strKey);
	pOptItem->SetStaticText(strDes);

	return 0;
}

int ToolTree::DeleteItemByKey(int key)
{
	COptionTreeItem* pFoundItem = FindItemByKey(key);
	if(pFoundItem)
	{
		COptionTree::DeleteItem(pFoundItem);
	}
	return 0;
}

void ToolTree::OnSelect(int key)
{
	if(key <= 0 || key == m_nLastSelKey)
		return;

	m_nLastSelKey = key;

	ToolApp::Instance().LoadFromDB(key);
}

void ToolTree::ResetSelectKey()
{
	m_nLastSelKey = -1;
}

void ToolTree::SelectKey(int key)
{
	if(key <= 0)
		return;

	COptionTreeItem* pTreeItem = FindItemByKey(key);
	if(!pTreeItem)
		return;

	SelectItems(NULL,FALSE);
	SelectItems(pTreeItem,TRUE);
	SetFocusedItem(pTreeItem);
}

int ToolTree::InsertUndefinedRoot()
{
	m_pUndefinedRoot = COptionTree::InsertItem(new COptionTreeItem());
	ACCHECK(m_pUndefinedRoot);

	m_pUndefinedRoot->SetLabelText(_T("未分类"));
	m_pUndefinedRoot->SetInfoText(_T("未分类"));
	return 0;
}

COptionTreeItem* ToolTree::FindOrCreateLayers(std::vector<CString>& vtLayers)
{
	if(vtLayers.empty())
		return m_pUndefinedRoot;

	COptionTreeItem* pCurrentRoot = &m_otiRoot;
	for(std::vector<CString>::iterator iter = vtLayers.begin(); iter != vtLayers.end(); ++iter)
	{
		COptionTreeItem* pFoundItem = FindItemByInfoText(pCurrentRoot,*iter);
		if(pFoundItem == NULL)
		{
			pFoundItem = COptionTree::InsertItem(new COptionTreeItem(),pCurrentRoot);
			ACCHECK(pFoundItem);

			pFoundItem->SetLabelText(*iter);
			pFoundItem->SetInfoText(*iter);
		}

		pCurrentRoot = pFoundItem;
	}

	return pCurrentRoot;
}

COptionTreeItem* ToolTree::FindItemByKey(int key)
{
	CString strKey;
	strKey.Format(_T("%d"),key);
	return FindItemByInfoText(&m_otiRoot,strKey);
}

COptionTreeItem* ToolTree::FindItemByInfoText(COptionTreeItem* otiRoot, const CString& strInfoText)
{
	// Mark found as NULL
	m_otiFound = NULL;

	// Enumerate items
	EnumItems(otiRoot, EnumFindItemByInfoText, (LPARAM)(&strInfoText));

	return m_otiFound;
}

BOOL CALLBACK ToolTree::EnumFindItemByInfoText(COptionTree* otProp, COptionTreeItem* otiItem, LPARAM lParam)
{
	// Validate items
	if (otiItem == NULL)
	{
		return FALSE;
	}

	CString* pInfoText = (CString*)lParam;
	if (pInfoText == NULL)
	{
		return FALSE;
	}

	if (otiItem->GetInfoText() == *pInfoText)
	{
		m_otiFound = otiItem;

		return FALSE;
	}

	return TRUE;
}

END_NS_AC