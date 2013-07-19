#include "ToolCtrl.h"
#include "ACString.h"
#include "ToolApp.h"
#include "ToolLayout.h"

BEGIN_NS_AC

int SComboItem::Init(LuaConfig& rLuaConfig)
{
	std::string str = rLuaConfig.GetString("./Name");
	strName = StlStringToCString(str);
	nValue = rLuaConfig.GetInteger("./Value");
	bChecked = rLuaConfig.GetBoolean("./Checked");
	return 0;
}

int SCtrl::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	std::string str = rLuaConfig.GetString("./Name");
	strName = StlStringToCString(str);
	str = rLuaConfig.GetString("./CName");
	strCName = StlStringToCString(str);
	nCtrl = rLuaConfig.GetInteger("./Ctrl");
	nWidth1 = rLuaConfig.GetInteger("./Width1");
	bNewline = rLuaConfig.GetBoolean("./Newline");
	return 0;
}

void SCtrl::CreateStatic(CWnd* pParent)
{
	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth1,false);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pStatic = new CStatic;
	pStatic->Create(strName,WS_CHILD|WS_VISIBLE|SS_CENTER,rect,pParent,id);
	pStatic->SetFont(&ToolApp::Instance().m_objFont);
}

int SEdit::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	nType = rLuaConfig.GetInteger("./Type");
	nWidth2 = rLuaConfig.GetInteger("./Width2");
	std::string str = rLuaConfig.GetString("./Default");
	strDefault = StlStringToCString(str);

	CreateStatic(pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth2,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pCtrl = new CEdit;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,rect,pParent,id);
	pCtrl->SetFont(&ToolApp::Instance().m_objFont);
	return 0;
}

int SCheck::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	bChecked = rLuaConfig.GetBoolean("./Checked");

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth1,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pCtrl = new CButton;
	pCtrl->Create(strName,WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,rect,pParent,id);
	pCtrl->SetFont(&ToolApp::Instance().m_objFont);
	return 0;
}

int SStatic::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth1,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pStatic = new CStatic;
	pStatic->Create(strName,WS_CHILD|WS_VISIBLE,rect,pParent,id);
	pStatic->SetFont(&ToolApp::Instance().m_objFont);
	return 0;
}

int SCombobox::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	nWidth2 = rLuaConfig.GetInteger("./Width2");

	CreateStatic(pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth2,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pCtrl = new CComboBox;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,rect,pParent,id);
	pCtrl->SetFont(&ToolApp::Instance().m_objFont);

	rLuaConfig.IterTable<SCombobox>("./Confs",this,&SCombobox::pfnAddComboItem,&rLuaConfig);
	return 0;
}

void SCombobox::pfnAddComboItem(void* ctx)
{
	LuaConfig* pLuaConfig = (LuaConfig*)ctx;
	ACCHECK(pLuaConfig);

	SComboItem comboItem;
	comboItem.Init(*pLuaConfig);

	pCtrl->AddString(comboItem.strName);

	vtItems.push_back(comboItem);
}

int SCheckCombo::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	nWidth2 = rLuaConfig.GetInteger("./Width2");

	CreateStatic(pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth2,bNewline);
	int tableLen = rLuaConfig.GetTableLen("./Confs");
	rect.bottom += tableLen * CTRL_HEIGHT;

	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pCtrl = new CCheckComboBox;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,rect,pParent,id);
	pCtrl->SetFont(&ToolApp::Instance().m_objFont);

	rLuaConfig.IterTable<SCheckCombo>("./Confs",this,&SCheckCombo::pfnAddComboItem,&rLuaConfig);

	return 0;
}

void SCheckCombo::pfnAddComboItem(void* ctx)
{
	LuaConfig* pLuaConfig = (LuaConfig*)ctx;
	ACCHECK(pLuaConfig);

	SComboItem comboItem;
	comboItem.Init(*pLuaConfig);

	((CCheckComboBox*)pCtrl)->AddString(comboItem.strName);

	vtItems.push_back(comboItem);
}

END_NS_AC