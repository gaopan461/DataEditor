#include "ToolCtrl.h"
#include "ACString.h"

BEGIN_NS_AC

int SComboItem::Init(LuaConfig& rLuaConfig)
{
	std::string str = rLuaConfig.GetString("./Name");
	strName = StlStringToCString(str);
	nValue = rLuaConfig.GetInteger("./Value");
	bChecked = rLuaConfig.GetBoolean("./Checked");
	return 0;
}

int SCtrl::Init(LuaConfig& rLuaConfig)
{
	std::string str = rLuaConfig.GetString("./Name");
	strName = StlStringToCString(str);
	str = rLuaConfig.GetString("./CName");
	strCName = StlStringToCString(str);
	nCtrl = rLuaConfig.GetInteger("./Ctrl");
	nWidth1 = rLuaConfig.GetInteger("./Width1");
	return 0;
}

int SEdit::Init(LuaConfig& rLuaConfig)
{
	SCtrl::Init(rLuaConfig);
	nType = rLuaConfig.GetInteger("./Type");
	nWidth2 = rLuaConfig.GetInteger("./Width2");
	std::string str = rLuaConfig.GetString("./Default");
	strDefault = StlStringToCString(str);

	pCtrl = new CEdit();
	//pCtrl->Create();
	//Set();
	return 0;
}

int SCheck::Init(LuaConfig& rLuaConfig)
{
	SCtrl::Init(rLuaConfig);
	bChecked = rLuaConfig.GetBoolean("./Checked");

	pCtrl = new CButton();

	return 0;
}

int SStatic::Init(LuaConfig& rLuaConfig)
{
	SCtrl::Init(rLuaConfig);

	pCtrl = new CStatic();

	return 0;
}

int SCombobox::Init(LuaConfig& rLuaConfig)
{
	SCtrl::Init(rLuaConfig);
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

int SCheckCombo::Init(LuaConfig& rLuaConfig)
{
	SCtrl::Init(rLuaConfig);
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