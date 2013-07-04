#pragma once


// TabItemDialog1 对话框

class TabItemDialog1 : public CDialog
{
	DECLARE_DYNAMIC(TabItemDialog1)

public:
	TabItemDialog1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TabItemDialog1();

// 对话框数据
	enum { IDD = IDD_TAB_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
