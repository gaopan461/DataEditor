#include "ToolLayout.h"

BEGIN_NS_AC

ToolLayout::ToolLayout(ToolApp* app)
: Module<ToolApp>(app)
, m_nID(IDC_CTRL)
{
}

ToolLayout::~ToolLayout()
{
}

CRect ToolLayout::GetNextRect(int width,bool br)
{
	return CRect(0,0,0,0);
}

int ToolLayout::GetNextID()
{
	return m_nID++;
}

END_NS_AC
