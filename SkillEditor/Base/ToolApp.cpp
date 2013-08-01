#include "stdafx.h"
#include "ToolApp.h"
#include "ACString.h"

#include "ToolTree.h"
#include "ToolLog.h"
#include "ToolExcel.h"

BEGIN_NS_AC

ToolApp* ToolApp::m_pInstance = NULL;

ToolApp::ToolApp()
{
	m_pInstance = this;
	m_pTree = new ToolTree(this);
	m_pLog = new ToolLog(this);
	m_pExcel = new ToolExcel(this);
}

ToolApp::~ToolApp()
{
	FinalizeTool();
	_safe_delete(m_pTree);
	_safe_delete(m_pLog);
	_safe_delete(m_pExcel);
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
	m_pTree->Create();

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
	for(std::map<int,CWnd*>::iterator iter = m_mapCheckCombos.begin(); iter != m_mapCheckCombos.end(); ++iter)
	{
		CWnd* pCheckCombo = iter->second;
		_safe_delete(pCheckCombo);
	}
	m_mapCheckCombos.clear();

	m_pMenu->DestroyMenu();
	delete m_pMenu;
	m_objFont.DeleteObject();
	return 0;
}

int ToolApp::Update()
{
	m_pLog->Update();
	return 0;
}

int ToolApp::LoadFromDB(int key)
{
	CString strCurrentDB = m_pTree->GetCurrentDB();
	ExcelDB* pExcelDB = m_pExcel->GetWorkbook(strCurrentDB);
	ACCHECK(pExcelDB);

	MapCNameToValueT mapValues;
	pExcelDB->ReadDBRecord(key,mapValues);
	OnLoadFromDB(mapValues);
	return 0;
}

int ToolApp::SaveToDB(int key)
{
	CString strCurrentDB = m_pTree->GetCurrentDB();
	ExcelDB* pExcelDB = m_pExcel->GetWorkbook(strCurrentDB);
	ACCHECK(pExcelDB);

	MapCNameToValueT mapValues;
	OnSaveToDB(mapValues);
	pExcelDB->WriteDBRecord(key,mapValues);
	return 0;
}

int ToolApp::MenuNew()
{
	m_pTree->EnableWindow(FALSE);
	return 0;
}

int ToolApp::MenuSave()
{
	return 0;
}

int ToolApp::MenuDelete()
{
	return 0;
}

int ToolApp::MenuCopy()
{
	return 0;
}

int ToolApp::MenuCancel()
{
	return 0;
}

int ToolApp::GetUnusedKey()
{
	CString strCurrentDB = m_pTree->GetCurrentDB();
	ExcelDB* pExcelDB = m_pExcel->GetWorkbook(strCurrentDB);
	ACCHECK(pExcelDB);

	return pExcelDB->GetUnusedKey();
}

int ToolApp::InsertByKey(int key, MapCNameToValueT& mapValues)
{
	CString strCurrentDB = m_pTree->GetCurrentDB();
	ExcelDB* pExcelDB = m_pExcel->GetWorkbook(strCurrentDB);
	ACCHECK(pExcelDB);

	return pExcelDB->InsertByKey(key,mapValues);
}

void ToolApp::InsertCheckCombo(int nDlgID, CWnd* pCheckCombo)
{
	m_mapCheckCombos.insert(std::make_pair(nDlgID,pCheckCombo));
}

CWnd* ToolApp::FindCheckCombo(int nDlgID)
{
	std::map<int,CWnd*>::iterator iter = m_mapCheckCombos.find(nDlgID);
	if(iter != m_mapCheckCombos.end())
		return iter->second;

	return NULL;
}

END_NS_AC