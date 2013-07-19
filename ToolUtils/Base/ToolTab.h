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
	SItemTab* AddTabItem(const CString& strName);
	int ChangeTab(int nSel);
	int DBToCtrl(int key);
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
protected:
	typedef std::vector<SItemTab*> VectorItemTabsT;
private:
	int m_nTabCurrent;
	VectorItemTabsT m_vtItemTabs;
};

END_NS_AC
