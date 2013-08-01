
// SkillEditorDlg.h : ͷ�ļ�
//

#pragma once

#include "ToolApp.h"
#include "afxcmn.h"

#include "MagicWindow.h"
#include "EffectCommonWindow.h"

USE_NS_AC


// CSkillEditorDlg �Ի���
class CSkillEditorDlg : public CDialog, public ToolApp
{
	BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(pMsg->message == WM_RBUTTONDOWN)
		{
			m_pMenu->TrackPopupMenu(TPM_LEFTALIGN,pMsg->pt.x,pMsg->pt.y,this);
			return TRUE;
		}
		else if(pMsg->message == WM_COMMAND && pMsg->wParam == ID_MENU_NEW)
		{
			MenuNew();
			return TRUE;
		}
		else if(pMsg->message == WM_COMMAND && pMsg->wParam == ID_MENU_SAVE)
		{
			MenuSave();
			return TRUE;
		}
		else if(pMsg->message == WM_COMMAND && pMsg->wParam == ID_MENU_DELETE)
		{
			MenuDelete();
			return TRUE;
		}
		else if(pMsg->message == WM_COMMAND && pMsg->wParam == ID_MENU_COPY)
		{
			MenuCopy();
			return TRUE;
		}
		else if(pMsg->message == WM_COMMAND && pMsg->wParam == ID_MENU_CANCEL)
		{
			MenuCancel();
			return TRUE;
		}
		return CDialog::PreTranslateMessage(pMsg);
	}
// ����
public:
	CSkillEditorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SKILLEDITOR_DIALOG };

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
	virtual CWnd* GetCurrentKeyWindow();
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
	virtual int MenuNew();
public:
	afx_msg void OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult);
};
