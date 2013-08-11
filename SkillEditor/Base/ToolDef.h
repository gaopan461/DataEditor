#ifndef _TOOL_DEF_H_
#define _TOOL_DEF_H_

#include <afxwin.h>
#include <afxcmn.h>
#include <map>
#include <vector>

#include "ACDef.h"

#define ID_MENU_NEW		5000
#define ID_MENU_SAVE	5001
#define ID_MENU_DELETE	5002
#define ID_MENU_COPY	5003
#define ID_MENU_CANCEL	5004

#define ARRAY_DELIMITER _T(",")

BEGIN_NS_AC

enum EDataType
{
	DATA_INT = 0,
	DATA_FLOAT,
	DATA_STRING,
	DATA_BOOL,
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
	int m_nTypeRow;				// Excel�б�ͷ�������ڵ���(������0��ʼ)
	int m_nDataRow;				// Excel��������ʼ����(������0��ʼ)
};

//-----------------------------------------------------------

struct SComboItem
{
	CString m_strName;
	CString m_strValue;
	int m_nValue;
	float m_fValue;
	SComboItem(const CString& strName,const CString& strValue)
	{
		m_strName = strName;
		m_strValue = strValue;
		m_nValue = 0;
		m_fValue = 0.0f;
	}
	SComboItem(const CString& strName,int nValue)
	{
		m_strName = strName;
		m_strValue = _T("");
		m_nValue = nValue;
		m_fValue = 0.0f;
	}
	SComboItem(const CString& strName,float fValue)
	{
		m_strName = strName;
		m_strValue = _T("");
		m_nValue = 0;
		m_fValue = fValue;
	}
};

typedef std::vector<SComboItem> VectorComboItemT;

void CreateCheckCombo(CWnd* pWnd, int nDlgID, VectorComboItemT& vtConfs);
void CreateCombobox(CWnd* pWnd, int nDlgID, VectorComboItemT& vtConfs);

void InitDeclare(CWnd* pWnd,MapCNameToValueT& mapValues);

void DeclareID(bool bSaveOrLoad,int nDlgID,const CString& strCName);

void DeclareEditStr(bool bSaveOrLoad,int nDlgID,const CString& strCName);
void DeclareEditInt(bool bSaveOrLoad,int nDlgID,const CString& strCName);
void DeclareEditFloat(bool bSaveOrLoad,int nDlgID,const CString& strCName);

void DeclareCheckbox(bool bSaveOrLoad,int nDlgID,const CString& strCName);

void DeclareComboboxStr(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs);
void DeclareComboboxInt(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs);
void DeclareComboboxFloat(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs);

void DeclareCheckComboStrArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs);
void DeclareCheckComboIntArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs);
void DeclareCheckComboFloatArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs);

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