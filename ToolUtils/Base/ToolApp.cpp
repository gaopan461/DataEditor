#include "stdafx.h"
#include "ToolApp.h"
#include "ACString.h"

#include "ToolConfig.h"
#include "ToolTree.h"
#include "ToolTab.h"
#include "ToolLog.h"
#include "ToolLayout.h"

BEGIN_NS_AC

ToolApp* ToolApp::m_pInstance = NULL;

ToolApp::ToolApp()
{
	m_pInstance = this;
	m_pConfig = new ToolConfig(this);
	m_pTree = new ToolTree(this);
	m_pTab = new ToolTab(this);
	m_pLog = new ToolLog(this);
	m_pLayout = new ToolLayout(this);
	m_bIsNewing = false;
}

ToolApp::~ToolApp()
{
	FinalizeTool();
	_safe_delete(m_pConfig);
	_safe_delete(m_pTree);
	_safe_delete(m_pTab);
	_safe_delete(m_pLog);
	_safe_delete(m_pLayout);
	m_pInstance = NULL;
}

int ToolApp::InitializeTool(const CString& strAppName)
{
	m_objFont.CreateFont(20, // Height
		0, // Width
		0, // Escapement
		0, // Orientation
		FW_BLACK, // Weight
		FALSE, // Italic
		FALSE, // Underline
		0, // StrikeOut
		ANSI_CHARSET, // CharSet
		OUT_DEFAULT_PRECIS, // OutPrecision
		CLIP_DEFAULT_PRECIS, // ClipPrecision
		DEFAULT_QUALITY, // Quality
		DEFAULT_PITCH | FF_SWISS, // PitchAndFamily
		_T("微软雅黑")); // Facename

	m_pLog->Create(strAppName);
	m_pTab->Create();
	m_pTree->Create();
	m_pLayout->Init();
	m_pConfig->Load(strAppName);
	m_pTab->ChangeTab(0);

	m_pMenu = new CMenu;
	m_pMenu->CreatePopupMenu();
	m_pMenu->AppendMenu(MF_STRING,ID_MENU_NEW,_T("新建"));
	m_pMenu->AppendMenu(MF_STRING,ID_MENU_SAVE,_T("保存"));
	m_pMenu->AppendMenu(MF_STRING,ID_MENU_DELETE,_T("删除"));
	m_pMenu->AppendMenu(MF_STRING,ID_MENU_COPY,_T("复制"));
	m_pMenu->AppendMenu(MF_STRING,ID_MENU_CANCEL,_T("取消"));
	SetMenu(GetMainWnd()->GetSafeHwnd(),m_pMenu->GetSafeHmenu());

	INFO_MSG("--------------------------------------------");
	INFO_MSG("               %s Start             ",CStringToStlString(strAppName).c_str());
	INFO_MSG("--------------------------------------------");

	return 0;
}

int ToolApp::FinalizeTool()
{
	m_objFont.DeleteObject();
	m_pMenu->DestroyMenu();
	delete m_pMenu;
	return 0;
}

int ToolApp::Update()
{
	m_pLog->Update();
	return 0;
}

int ToolApp::MenuNew()
{
	m_bIsNewing = true;
	m_pTree->EnableWindow(FALSE);
	m_pTab->EnableKeyWindow(TRUE);
	m_pTab->LoadDefaultValues();
	return 0;
}

int ToolApp::MenuSave()
{
	SItemTab* pTabItem = m_pTab->GetCurrentItem();
	ACCHECK(pTabItem);

	int nKey = -1;
	if(m_bIsNewing)
	{
		m_bIsNewing = false;
		m_pTree->EnableWindow(TRUE);
		m_pTab->EnableKeyWindow(FALSE);

		CEdit* pCtrlKey = pTabItem->GetKeyWnd();
		ACCHECK(pCtrlKey);

		CString strKey;
		pCtrlKey->GetWindowText(strKey);
		nKey = atoi(CStringToStlString(strKey).c_str());

		if(!pTabItem->GetDB()->InsertNewKey(nKey))
		{
			m_pTree->SelectKey(m_pTree->GetSelectKey(),true);
			AfxMessageBox(_T("Key invalid"));
			return -1;
		}
	}
	else
	{
		nKey = m_pTree->GetSelectKey();
	}
	
	m_pTab->CtrlToDB(nKey);
	m_pTab->DBToTree();
	m_pTree->SetFocusedKey(nKey);
	return 0;
}

int ToolApp::MenuCancel()
{
	if(m_bIsNewing)
	{
		m_bIsNewing = false;
		m_pTree->EnableWindow(TRUE);
		m_pTab->EnableKeyWindow(FALSE);
		m_pTree->SelectKey(m_pTree->GetSelectKey(),true);
	}
	return 0;
}

END_NS_AC