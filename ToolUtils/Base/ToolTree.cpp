#include "ToolTree.h"
#include "ToolApp.h"

BEGIN_NS_AC

ToolTree::ToolTree(ToolApp* app)
: Module<ToolApp>(app)
, m_nLastSelID(-1)
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
		AfxMessageBox(_T("Create main tree failed!"));
		ExitProcess(-1);
	}

	return 0;
}

void ToolTree::SelectID(int id)
{
	if(id == m_nLastSelID || id <= 0)
		return;
}

END_NS_AC