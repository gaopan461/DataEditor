// DataEditorDlg.h : ͷ�ļ�
//

#pragma once

#include "ToolApp.h"

USE_NS_AC

// CDataEditorDlg �Ի���
class CDataEditorDlg : public CDialog, public ToolApp
{
// ����
public:
	CDataEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CDataEditorDlg();

// �Ի�������
	enum { IDD = IDD_DATAEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	virtual CWnd* GetMainWnd();
};
