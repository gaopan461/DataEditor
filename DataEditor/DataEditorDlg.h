// DataEditorDlg.h : 头文件
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

// CDataEditorDlg 对话框
class CDataEditorDlg : public CDialog, public ToolBase
{
// 构造
public:
	CDataEditorDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CDataEditorDlg();

// 对话框数据
	enum { IDD = IDD_DATAEDITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
