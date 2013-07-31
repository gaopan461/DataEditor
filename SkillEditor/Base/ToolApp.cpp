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
	m_bIsNewing = false;
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
		_T("Î¢ÈíÑÅºÚ")); // Facename

	m_pLog->Create(strAppName);
	m_pTree->Create();

	INFO_MSG("--------------------------------------------");
	INFO_MSG("               %s Start             ",CStringToStlString(strAppName).c_str());
	INFO_MSG("--------------------------------------------");

	return 0;
}

int ToolApp::FinalizeTool()
{
	for(size_t i = 0; i < m_vtCheckCombos.size(); ++i)
	{
		_safe_delete(m_vtCheckCombos[i]);
	}
	m_vtCheckCombos.clear();

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

void ToolApp::PushCheckCombo(CWnd* pCheckCombo)
{
	m_vtCheckCombos.push_back(pCheckCombo);
}

END_NS_AC