#ifndef _TOOL_LAYOUT_H_
#define _TOOL_LAYOUT_H_

#include "ToolDef.h"
#include "ToolApp.h"

BEGIN_NS_AC

class ToolLayout : public Module<ToolApp>
{
public:
	ToolLayout(ToolApp* app);
	~ToolLayout();
public:
	CRect GetNextRect(int width,bool br);
	int GetNextID();
private:
	int m_nID;
	int m_nX;
	int m_nY;
};

END_NS_AC

#endif//_TOOL_LAYOUT_H_
