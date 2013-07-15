#pragma once

#include <vector>
#include "ToolDef.h"
#include "ToolApp.h"

using namespace YExcel;

BEGIN_NS_AC

// ToolTab

// ����Ķ�̳б��뽫CTabCtrl��ǰ�棬����BEGIN_MESSAGE_MAPʱ
// �ᱨ��������ʱ�����ָ��ת�����³���

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
