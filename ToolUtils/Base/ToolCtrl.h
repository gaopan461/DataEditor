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
	bool bDefault;

	virtual int Init(LuaConfig& rLuaConfig);
};

struct SCtrl
{
	CString strName;	// 控件显示名字，CStatic的名字
	CString strCName;	// 控件数据存db时名字，存db的key
	int nCtrl;			// 控件的类型
	int nWidth1;		// 名称控件的宽度
	bool bNewline;		// 下一个控件是否换行显示

	CStatic* pStatic;	// 名称控件

	SCtrl()
	{
		pStatic = NULL;
	}
	virtual ~SCtrl()
	{
		_safe_delete(pStatic);
	}

	virtual int Init(LuaConfig& rLuaConfig,CWnd* pParent);
	virtual int LoadDefaultValue(){return 0;}

	virtual int DataToCtrl(CString& data){return 0;}
	virtual int CtrlToData(CString& data){return 0;}

	void CreateStatic(CWnd* pParent);
};

struct SEdit : public SCtrl
{
	int nType;			// 编辑框中数据的类型
	int nWidth2;		// 控件的宽度
	CString strDefault;	// 控件默认值

	CEdit* pCtrl;		// 控件

	SEdit()
	{
		pCtrl = NULL;
	}
	virtual ~SEdit()
	{
		_safe_delete(pCtrl);
	}

	virtual int Init(LuaConfig& rLuaConfig,CWnd* pParent);
	virtual int LoadDefaultValue();

	virtual int DataToCtrl(CString& data);
	virtual int CtrlToData(CString& data);
};

struct SCheck : public SCtrl
{
	bool bDefault;

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
	virtual int LoadDefaultValue();

	virtual int DataToCtrl(CString& data);
	virtual int CtrlToData(CString& data);
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
	virtual int LoadDefaultValue();

	virtual int DataToCtrl(CString& data);
	virtual int CtrlToData(CString& data);

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
	virtual int LoadDefaultValue();

	virtual int DataToCtrl(CString& data);
	virtual int CtrlToData(CString& data);

	void pfnAddComboItem(void* ctx);
};

END_NS_AC

#endif//_TOOL_CTRL_H_