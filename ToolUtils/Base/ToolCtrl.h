#ifndef _TOOL_CTRL_H_
#define _TOOL_CTRL_H_

#include "ACDef.h"
#include "ACLuaUtils.h"
#include "CheckComboBox.h"

BEGIN_NS_AC

struct SComboItem
{
	CString strName;
	int nValue;
	bool bChecked;

	virtual int Init(LuaConfig& rLuaConfig);
};

struct SCtrlBase
{
	CString strName;
	int nCtrl;
	int nColumn;
	int nWidth1;

	virtual int Set(){}
	virtual int Get(){}

	virtual int Init(LuaConfig& rLuaConfig);
};

struct SEdit : public SCtrlBase
{
	int nType;
	int nWidth2;
	CString strDefault;

	CEdit* pCtrl;

	virtual int Init(LuaConfig& rLuaConfig);
};

struct SCheck : public SCtrlBase
{
	bool bChecked;

	CButton* pCtrl;

	virtual int Init(LuaConfig& rLuaConfig);
};

struct SStatic : public SCtrlBase
{
	CStatic* pCtrl;

	virtual int Init(LuaConfig& rLuaConfig);
};

struct SCombobox : public SCtrlBase
{
	int nWidth2;
	std::vector<SComboItem> vtItems;

	CComboBox* pCtrl;

	virtual int Init(LuaConfig& rLuaConfig);
};

struct SCheckCombo : public SCtrlBase
{
	int nWidth2;
	std::vector<SComboItem> vtItems;

	CCheckComboBox* pCtrl;

	virtual int Init(LuaConfig& rLuaConfig);
};

END_NS_AC

#endif//_TOOL_CTRL_H_