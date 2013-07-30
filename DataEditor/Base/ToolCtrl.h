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
	CString strName;	// �ؼ���ʾ���֣�CStatic������
	CString strCName;	// �ؼ����ݴ�dbʱ���֣���db��key
	int nCtrl;			// �ؼ�������
	int nWidth1;		// ���ƿؼ��Ŀ��
	bool bNewline;		// ��һ���ؼ��Ƿ�����ʾ

	CStatic* pStatic;	// ���ƿؼ�

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
	int nType;			// �༭�������ݵ�����
	int nWidth2;		// �ؼ��Ŀ��
	CString strDefault;	// �ؼ�Ĭ��ֵ

	CEdit* pCtrl;		// �ؼ�

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