#pragma once


// TabItemDialog2 �Ի���

class TabItemDialog2 : public CDialog
{
	DECLARE_DYNAMIC(TabItemDialog2)

public:
	TabItemDialog2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TabItemDialog2();

// �Ի�������
	enum { IDD = IDD_TAB_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
