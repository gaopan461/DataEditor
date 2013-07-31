#include "stdafx.h"
#include "ToolDef.h"
#include "ToolApp.h"
#include "ToolExcel.h"
#include "ToolTree.h"
#include "ACLog.h"

BEGIN_NS_AC

struct{
	CWnd* m_pWnd;
	MapCNameToValueT* m_pMapValues;
}g_objDeclareInfo;

void InitDeclare(CWnd* pWnd,MapCNameToValueT& mapValues)
{
	ACCHECK(pWnd);
	g_objDeclareInfo.m_pWnd = pWnd;
	g_objDeclareInfo.m_pMapValues = &mapValues;
}

void DeclareID(bool bSaveOrLoad,int nDlgID,const CString& strCName)
{
	CWnd* pMainWnd = ToolApp::Instance().GetMainWnd();
	ACCHECK(pMainWnd);

	ToolTree* pMainTree = ToolApp::Instance().GetMainTree();
	ACCHECK(pMainTree);

	ToolExcel* pExcelTool = ToolApp::Instance().GetExcelTool();
	ACCHECK(pExcelTool);

	ExcelDB* pExcelDB = pExcelTool->GetWorkbook(pMainTree->GetCurrentDB());
	ACCHECK(pExcelDB);

	MapCNameToValueT& mapValues = *(g_objDeclareInfo.m_pMapValues);
	CWnd* pWnd = g_objDeclareInfo.m_pWnd;

	CEdit* pEdit = (CEdit*)pWnd->GetDlgItem(nDlgID);
	ACCHECK(pEdit);

	if(bSaveOrLoad)
	{
		CString strValue;
		pEdit->GetWindowText(strValue);
		int nKey = atoi(CStringToStlString(strValue).c_str());
		if(nKey != pExcelDB->GetLastSelectKey())
		{
			WarningMessageBox(_T("ID invalid!"));
			return ;
		}
	}
	else
	{
		MapCNameToValueT::iterator iter = mapValues.find(strCName);
		if(iter == mapValues.end())
		{
			WARN_MSG("ID not exist!");
			return;
		}

		pEdit->SetWindowText(iter->second);
	}
}

void _DeclareEdit(bool bSaveOrLoad,int nDlgID,const CString& strCName,int nCastType)
{
	CWnd* pMainWnd = ToolApp::Instance().GetMainWnd();
	ACCHECK(pMainWnd);

	ToolTree* pMainTree = ToolApp::Instance().GetMainTree();
	ACCHECK(pMainTree);

	ToolExcel* pExcelTool = ToolApp::Instance().GetExcelTool();
	ACCHECK(pExcelTool);

	ExcelDB* pExcelDB = pExcelTool->GetWorkbook(pMainTree->GetCurrentDB());
	ACCHECK(pExcelDB);

	MapCNameToValueT& mapValues = *(g_objDeclareInfo.m_pMapValues);
	CWnd* pWnd = g_objDeclareInfo.m_pWnd;

	CEdit* pEdit = (CEdit*)pWnd->GetDlgItem(nDlgID);
	ACCHECK(pEdit);

	if(bSaveOrLoad)
	{
		CString strValue;
		pEdit->GetWindowText(strValue);
		MapCNameToValueT::iterator iter = mapValues.find(strCName);
		if(iter != mapValues.end())
		{
			WARN_MSG("Save failed! exist cname:%s",CStringToStlString(strCName).c_str());
			return;
		}

		if(nCastType == DATA_INT)
		{
			int nValue = atoi(CStringToStlString(strValue).c_str());
			strValue.Format(_T("%d"),nValue);
		}
		else if(nCastType == DATA_FLOAT)
		{
			double fValue = atof(CStringToStlString(strValue).c_str());
			strValue.Format(_T("%.6lf"),fValue);
		}
		
		mapValues.insert(std::make_pair(strCName,strValue));
	}
	else
	{
		MapCNameToValueT::iterator iter = mapValues.find(strCName);
		if(iter == mapValues.end())
		{
			WARN_MSG("Load failed! not find cname:%s",CStringToStlString(strCName).c_str());
			return;
		}
		pEdit->SetWindowText(iter->second);
	}
}

void DeclareEditStr(bool bSaveOrLoad,int nDlgID,const CString& strCName)
{
	_DeclareEdit(bSaveOrLoad,nDlgID,strCName,DATA_STRING);
}

void DeclareEditInt(bool bSaveOrLoad,int nDlgID,const CString& strCName)
{
	_DeclareEdit(bSaveOrLoad,nDlgID,strCName,DATA_INT);
}

void DeclareEditFloat(bool bSaveOrLoad,int nDlgID,const CString& strCName)
{
	_DeclareEdit(bSaveOrLoad,nDlgID,strCName,DATA_FLOAT);
}

END_NS_AC