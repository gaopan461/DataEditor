#include "stdafx.h"
#include "ToolLayout.h"
#include "ToolTab.h"

BEGIN_NS_AC

ToolLayout::ToolLayout(ToolApp* app)
: Module<ToolApp>(app)
, m_nID(IDC_CTRL)
, m_nStartX(0)
, m_nStartY(0)
{
}

ToolLayout::~ToolLayout()
{
}

int ToolLayout::Init()
{
	ToolTab* pTab = ToolApp::Instance().GetMainTab();
	ACCHECK(pTab);

	CRect rect;
	pTab->GetClientRect(&rect);
	m_nStartX = rect.left;
	m_nStartY = rect.top + 15;
	m_nX = m_nStartX;
	m_nY = m_nStartY;
	return 0;
}

int ToolLayout::Reset()
{
	m_nX = m_nStartX;
	m_nY = m_nStartY;
	return 0;
}

CRect ToolLayout::GetNextRect(int width,bool br)
{
	CRect rect;
	rect.left = m_nX;
	rect.top = m_nY;
	rect.right = m_nX + width;
	rect.bottom = m_nY + CTRL_HEIGHT;

	if(br)
	{
		m_nX = m_nStartX;
		m_nY = m_nY + CTRL_HEIGHT + CTRL_HEIGHT_INTER;
	}
	else
	{
		m_nX = m_nX + width;
	}

	return rect;
}

int ToolLayout::GetNextID()
{
	return m_nID++;
}

END_NS_AC
