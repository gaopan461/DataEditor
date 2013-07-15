#ifndef _TOOL_CTRL_H_
#define _TOOL_CTRL_H_

#include <afxwin.h>
#include <afxcmn.h>

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

struct SCtrl
{
	CString strName;
	CString strCName;
	int nCtrl;
	int nWidth1;
	int bNewline;

	CStatic* pStatic;

	SCtrl()
	{
		pStatic = NULL;
	}
	virtual ~SCtrl()
	{
		_safe_delete(pStatic);
	}

	virtual int Set(){return 0;}
	virtual int Get(){return 0;}

	virtual int Init(LuaConfig& rLuaConfig,CWnd* pParent);

	void CreateStatic(CWnd* pParent);
};

struct SEdit : public SCtrl
{
	int nType;
	int nWidth2;
	CString strDefault;

	CEdit* pCtrl;

	SEdit()
	{
		pCtrl = NULL;
	}
	virtual ~SEdit()
	{
		_safe_delete(pCtrl);
	}

	virtual int Init(LuaConfig& rLuaConfig,CWnd* pParent);
};

struct SCheck : public SCtrl
{
	bool bChecked;

	CButton* pCtrl;

	SCheck()
	{
		pCtrl = NULL;
	}
	virtual ~SCheck()
	{
		_safe_delete(pCtrl);
	}

	virtual int Init(LuaConfig& rLuaConfig,CWnd* pParent);
};

struct SStatic : public SCtrl
{
	virtual ~SStatic()
	{}

	virtual int Init(LuaConfig& rLuaConfig,CWnd* pParent);
};

struct SCombobox : public SCtrl
{
	int nWidth2;
	std::vector<SComboItem> vtItems;

	CComboBox* pCtrl;

	SCombobox()
	{
		pCtrl = NULL;
	}
	virtual ~SCombobox()
	{
		_safe_delete(pCtrl);
	}

	virtual int Init(LuaConfig& rLuaConfig,CWnd* pParent);

	void pfnAddComboItem(void* ctx);
};

struct SCheckCombo : public SCtrl
{
	int nWidth2;
	std::vector<SComboItem> vtItems;

	CCheckComboBox* pCtrl;

	SCheckCombo()
	{
		pCtrl = NULL;
	}
	virtual ~SCheckCombo()
	{
		_safe_delete(pCtrl);
	}

	virtual int Init(LuaConfig& rLuaConfig,CWnd* pParent);

	void pfnAddComboItem(void* ctx);
};

END_NS_AC

#endif//_TOOL_CTRL_H_