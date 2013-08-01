#ifndef _TOOL_APP_H_
#define _TOOL_APP_H_

#include "ToolDef.h"
#include "ACLuaUtils.h"

BEGIN_NS_AC

class ToolTree;
class ToolLog;
class ToolExcel;

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
	int GetUnusedKey();
	int InsertByKey(int key, MapCNameToValueT& mapValues);
	int ModifyKey(int oldKey,int newKey);
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
	ToolTree* GetMainTree()
	{
		return m_pTree;
	}
	ToolLog* GetMainLog()
	{
		return m_pLog;
	}
	ToolExcel* GetExcelTool()
	{
		return m_pExcel;
	}
public:
	bool IsNewing();
private:
	bool m_bIsNewing;
private:
	static ToolApp* m_pInstance;
protected:
	ToolTree* m_pTree;
	ToolLog* m_pLog;
	ToolExcel* m_pExcel;
public:
	CFont m_objFont;
	CMenu* m_pMenu;
public:
	void InsertCheckCombo(int nDlgID, CWnd* pCheckCombo);
	CWnd* FindCheckCombo(int nDlgID);
protected:
	std::map<int,CWnd*> m_mapCheckCombos;
};

END_NS_AC

#endif//_TOOL_APP_H_