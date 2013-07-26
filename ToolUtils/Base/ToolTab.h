#pragma once

#include <vector>
#include "ToolDef.h"
#include "ToolApp.h"

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
	SItemTab* GetCurrentItem();
	int ChangeTab(int nSel);
	int DBToCtrl(int key);
	int CtrlToDB(int key);
	int DBToTree();
public:
	bool EnableKeyWindow(bool enable);
	int LoadDefaultValues();
	int RestoreLastSelect();
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
