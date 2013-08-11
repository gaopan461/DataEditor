#ifndef _TOOL_APP_H_
#define _TOOL_APP_H_

#include "ToolDef.h"
#include "ACLuaUtils.h"

BEGIN_NS_AC

class ToolTree;
class ToolLog;
class ACExcel;
class ExcelDB;

class ToolApp
{
public:
	ToolApp();
	virtual ~ToolApp();
public:
	virtual int InitializeTool(const CString& strAppName);
	virtual int Update();
	virtual int FinalizeTool();
public:
	int LoadFromDB(int key);
	int SaveToDB(int key);
public:
	virtual void OnLoadFromDB(MapCNameToValueT& mapValues){}
	virtual void OnSaveToDB(MapCNameToValueT& mapValues){}
public:
	int MenuNew();
	int MenuSave();
	int MenuDelete();
	int MenuCopy();
	int MenuCancel();
public:
	int InsertByKey(int key);
public:
	ExcelDB* OpenExcelDB(const SExcelConfig& rConfig);
public:
	void SetCurrentDB(const CString& strDBName);
	CString GetCurrentDB(){return m_strCurrentDB;}
public:
	virtual CWnd* GetMainWnd()
	{
		return NULL;
	}
	virtual CWnd* GetCurrentKeyWindow()
	{
		return NULL;
	}
public:
	static ToolApp& Instance()
	{
		return *m_pInstance;
	}
public:
	ToolLog* GetMainLog()
	{
		return m_pLog;
	}
	ACExcel* GetExcelServer()
	{
		return m_pExcelServer;
	}
public:
	bool IsNewing()
	{
		return m_bIsNewing;
	}
private:
	bool m_bIsNewing;
private:
	static ToolApp* m_pInstance;
protected:
	ToolLog* m_pLog;
	ACExcel* m_pExcelServer;
public:
	CFont m_objFont;
	CMenu* m_pMenu;
public:
	void InsertCheckCombo(int nDlgID, CWnd* pCheckCombo);
	CWnd* FindCheckCombo(int nDlgID);
protected:
	std::map<int,CWnd*> m_mapCheckCombos;
	std::map<CString,ExcelDB*> m_mapExcelDBs;
	CString m_strCurrentDB;
};

END_NS_AC

#endif//_TOOL_APP_H_