#ifndef _TOOL_DEF_H_
#define _TOOL_DEF_H_

#include <afxwin.h>
#include <afxcmn.h>
#include <map>
#include <vector>

#include "ACDef.h"

#define IDC_TAB_ITEM	2100
#define IDC_CTRL		2200

#define ARRAY_DELIMITER _T(",")

BEGIN_NS_AC

enum EDataType
{
	DATA_INT = 0,
	DATA_FLOAT,
	DATA_STRING,
};

enum ECtrlType
{
	CTRL_EDIT = 0,
	CTRL_CHECK,
	CTRL_STATIC,
	CTRL_COMBOBOX,
	CTRL_CHECKCOMBO,
};

enum EDBType
{
	DB_EXCEL = 0,
	DB_LUA,
	DB_PYTHON,
};

typedef std::map<CString,CString> MapCNameToValueT;

struct SExcelConfig
{
	CString m_strExcelPath;		// Excel������·��(�����ļ���)
	CString m_strExcelCName;	// Excel������
	CString m_strKeyCName;		// Excel�д���Key���еı�ͷ����
	CString m_strDesCName;		// Excel�д���Des���еı�ͷ����
	std::vector<CString> m_vtLayerCName; // Excel�д���Layer���еı�ͷ����
	int m_nHeadRow;				// Excel�б�ͷ���ڵ���(������0��ʼ)
	int m_nDataRow;				// Excel��������ʼ����(������0��ʼ)
};

//-----------------------------------------------------------

template <class T>
class Module
{
public:
	Module(T* pOwner)
		: m_pOwner(pOwner)
	{}
	virtual ~Module()
	{
		m_pOwner = NULL;
	}
protected:
	T* m_pOwner;
};

END_NS_AC

#endif//_TOOL_DEF_H_