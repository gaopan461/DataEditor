#pragma once


// TabItemDialog1 �Ի���

class TabItemDialog1 : public CDialog
{
	DECLARE_DYNAMIC(TabItemDialog1)

public:
	TabItemDialog1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~TabItemDialog1();

// �Ի�������
	enum { IDD = IDD_TAB_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
