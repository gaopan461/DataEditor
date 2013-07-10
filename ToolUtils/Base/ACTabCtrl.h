#pragma once

#include <vector>
#include "BasicExcel.h"
#include "ACDef.h"
#include "ACLuaUtils.h"
#include "ToolCtrl.h"

using namespace YExcel;

BEGIN_NS_AC

// ACTabCtrl

class ACTabCtrl : public CTabCtrl
{
	struct STabItem
	{
		CWnd* m_pWnd;
		BasicExcel* m_pExcel;
		std::vector<SCtrlBase*> m_vtCtrlItems;
		
		STabItem(CWnd* pWnd, BasicExcel* pExcel)
			: m_pWnd(pWnd)
			, m_pExcel(pExcel)
		{}
	};

	DECLARE_DYNAMIC(ACTabCtrl)

public:
	ACTabCtrl();
	virtual ~ACTabCtrl();

public:
	void InitTabItems(LuaConfig& rConfig);

protected:
	
	void AddTabItem(const CString& strName, const CString& strExcel);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

private:
	std::vector<STabItem> m_vtTabItems;
	int m_nTabCurrent;
};

END_NS_AC
