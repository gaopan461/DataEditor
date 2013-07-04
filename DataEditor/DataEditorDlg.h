// DataEditorDlg.h : ͷ�ļ�
//

#pragma once

#include "ACDef.h"
#include "ToolBase.h"
#include "ToolTree.h"
#include "afxcmn.h"

#include <vector>
#include "TabItemDialog1.h"
#include "TabItemDialog2.h"

USE_NS_AC

// CDataEditorDlg �Ի���
class CDataEditorDlg : public CDialog, public ToolBase
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
	void InitTab();
private:
	ToolTree m_objMainTree;
public:
	CTabCtrl m_objMainTab;
private:
	TabItemDialog1 m_objTabItem1;
	TabItemDialog2 m_objTabItem2;
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
