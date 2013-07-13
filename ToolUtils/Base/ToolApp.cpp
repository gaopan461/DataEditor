#include "stdafx.h"
#include "ToolApp.h"
#include "ACString.h"

#include "ToolConfig.h"
#include "ToolTree.h"
#include "ToolTab.h"
#include "ToolLog.h"

BEGIN_NS_AC

//-----------------------------------------------------------

VectorItemTabsT g_vtItemTabs;
VectorItemDBsT g_vtItemDBs;

//-----------------------------------------------------------

ToolApp* ToolApp::m_pInstance = NULL;

ToolApp::ToolApp()
{
	m_pInstance = this;
}

ToolApp::~ToolApp()
{
	_safe_delete(m_pConfig);
	_safe_delete(m_pTree);
	_safe_delete(m_pTab);
	_safe_delete(m_pLog);
	m_pInstance = NULL;
}

int ToolApp::InitTool(const CString& strAppName)
{
	m_pConfig = new ToolConfig(this);
	m_pTree = new ToolTree(this);
	m_pTab = new ToolTab(this);
	m_pLog = new ToolLog(this);

	m_pLog->Initial(strAppName);
	m_pConfig->Initial(strAppName);
	m_pTab->Initial();
	m_pTree->Initial();

	INFO_MSG("--------------------------------------------");
	INFO_MSG("               %s Start             ",CStringToStlString(strAppName).c_str());
	INFO_MSG("--------------------------------------------");

	return 0;
}

int ToolApp::Update()
{
	m_pLog->Update();
	return 0;
}

END_NS_AC