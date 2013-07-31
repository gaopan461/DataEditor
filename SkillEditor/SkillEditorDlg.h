
// SkillEditorDlg.h : 头文件
//

#pragma once

#include "ToolApp.h"
#include "afxcmn.h"

#include "MagicWindow.h"
#include "EffectCommonWindow.h"

USE_NS_AC


// CSkillEditorDlg 对话框
class CSkillEditorDlg : public CDialog, public ToolApp
{
// 构造
public:
	CSkillEditorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SKILLEDITOR_DIALOG };

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
	virtual CWnd* GetMainWnd();
protected:
	CTabCtrl m_objMainTab;
	MagicWindow m_objMagicWindow;
	EffectCommonWindow m_objEffectCommonWindow;
protected:
	void InitializeTab();
	void OpenExcelDBs();
public:
	void OnLoadFromDB(MapCNameToValueT& mapValues);
	void OnSaveToDB(MapCNameToValueT& mapValues);
public:
	afx_msg void OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult);
};
