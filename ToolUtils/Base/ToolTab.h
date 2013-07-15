#pragma once

#include <vector>
#include "ToolDef.h"
#include "ToolApp.h"

using namespace YExcel;

BEGIN_NS_AC

// ToolTab

// 下面的多继承必须将CTabCtrl放前面，否则BEGIN_MESSAGE_MAP时
// 会报警，运行时会出现指针转换导致出错

class ToolTab : public CTabCtrl, public Module<ToolApp>
{
	DECLARE_DYNAMIC(ToolTab)

public:
	ToolTab(ToolApp* app);
	virtual ~ToolTab();
public:
	int Create();

public:
	CWnd* AddTabItem(const CString& strName);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	std::vector<CWnd*> m_vtTabWnds;

private:
	int m_nTabCurrent;
};

END_NS_AC
