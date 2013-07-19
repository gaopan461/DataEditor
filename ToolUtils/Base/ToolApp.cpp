#include "stdafx.h"
#include "ToolApp.h"
#include "ACString.h"

#include "ToolConfig.h"
#include "ToolTree.h"
#include "ToolTab.h"
#include "ToolLog.h"
#include "ToolLayout.h"

BEGIN_NS_AC

//-----------------------------------------------------------

VectorItemTabsT g_vtItemTabs;

//-----------------------------------------------------------

ToolApp* ToolApp::m_pInstance = NULL;

ToolApp::ToolApp()
{
	m_pInstance = this;
	m_pConfig = new ToolConfig(this);
	m_pTree = new ToolTree(this);
	m_pTab = new ToolTab(this);
	m_pLog = new ToolLog(this);
	m_pLayout = new ToolLayout(this);
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
	m_pLog->Create(strAppName);
	m_pTab->Create();
	m_pTree->Create();
	m_pLayout->Init();
	m_pConfig->Load(strAppName);
	m_pTab->ChangeTab(0);

	INFO_MSG("--------------------------------------------");
	INFO_MSG("               %s Start             ",CStringToStlString(strAppName).c_str());
	INFO_MSG("--------------------------------------------");

	return 0;
}

int ToolApp::FinalizeTool()
{
	for(size_t i = 0; i < g_vtItemTabs.size(); ++i)
		_safe_delete(g_vtItemTabs[i]);

	g_vtItemTabs.clear();
	return 0;
}

int ToolApp::Update()
{
	m_pLog->Update();
	return 0;
}

END_NS_AC