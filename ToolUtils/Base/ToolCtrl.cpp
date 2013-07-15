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
	pStatic->Create(strName,WS_CHILD|WS_VISIBLE,rect,pParent,id);
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
	//Set();
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

	return 0;
}

int SStatic::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth1,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pStatic = new CStatic;
	pStatic->Create(strName,WS_CHILD|WS_VISIBLE,rect,pParent,id);

	return 0;
}

int SCombobox::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	nWidth2 = rLuaConfig.GetInteger("./Width2");

	rLuaConfig.PushTable("./Confs");

	lua_State* pLua = rLuaConfig.GetLuaState();
	ACCHECK(pLua);

	lua_pushnil(pLua);
	while(lua_next(pLua,-2) != 0)
	{
		SComboItem item;
		item.Init(rLuaConfig);
		vtItems.push_back(item);
		lua_pop(pLua,1);
	}
	lua_pop(pLua,1);

	pCtrl = new CComboBox();

	for(size_t i = 0; i < vtItems.size(); ++i)
	{
		pCtrl->InsertString(i, vtItems[i].strName);
	}

	return 0;
}

int SCheckCombo::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	nWidth2 = rLuaConfig.GetInteger("./Width2");

	rLuaConfig.PushTable("./Confs");

	lua_State* pLua = rLuaConfig.GetLuaState();
	ACCHECK(pLua);

	lua_pushnil(pLua);
	while(lua_next(pLua,-2) != 0)
	{
		SComboItem item;
		item.Init(rLuaConfig);
		vtItems.push_back(item);
		lua_pop(pLua,1);
	}
	lua_pop(pLua,1);

	pCtrl = new CCheckComboBox();

	for(size_t i = 0; i < vtItems.size(); ++i)
	{
		pCtrl->InsertString(i, vtItems[i].strName);
	}

	return 0;
}

END_NS_AC