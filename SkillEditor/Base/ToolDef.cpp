#include "stdafx.h"
#include "ToolDef.h"
#include "ToolApp.h"
#include "ToolExcel.h"
#include "ToolTree.h"
#include "ACLog.h"
#include "CheckComboBox.h"

BEGIN_NS_AC

void CreateCheckCombo(CWnd* pWnd, int nDlgID, VectorComboItemT& vtConfs)
{
	CRect rect;
	pWnd->GetDlgItem(nDlgID)->GetWindowRect(&rect);
	pWnd->ScreenToClient(rect);
	int nCtrlHeight = rect.bottom - rect.top;
	rect.bottom += nCtrlHeight * vtConfs.size();

	CCheckComboBox* pCheckCombo = new CCheckComboBox;
	pCheckCombo->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,rect,pWnd,nDlgID);

	for(size_t i = 0; i < vtConfs.size(); ++i)
		pCheckCombo->AddString(vtConfs[i].m_strName);

	ToolApp::Instance().PushCheckCombo(pCheckCombo);
}

void CreateCombobox(CWnd* pWnd, int nDlgID, VectorComboItemT& vtConfs)
{
	CComboBox* pCheckCombo = (CComboBox*)pWnd->GetDlgItem(nDlgID);

	for(size_t i = 0; i < vtConfs.size(); ++i)
		pCheckCombo->AddString(vtConfs[i].m_strName);
}

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

void DeclareCheckbox(bool bSaveOrLoad,int nDlgID,const CString& strCName)
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

	CButton* pCheck = (CButton*)pWnd->GetDlgItem(nDlgID);
	ACCHECK(pCheck);

	if(bSaveOrLoad)
	{
		MapCNameToValueT::iterator iter = mapValues.find(strCName);
		if(iter != mapValues.end())
		{
			WARN_MSG("Save failed! exist cname:%s",CStringToStlString(strCName).c_str());
			return;
		}

		CString strValue = pCheck->GetCheck() ? _T("true" : _T("false"));
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

		CString strValue = iter->second;
		strValue.MakeLower();
		if(strValue == _T("true") || strValue == _T("1"))
			pCheck->SetCheck(true);
		else if(strValue == _T("false") || strValue == _T("0"))
			pCheck->SetCheck(false);
		else
		{
			WARN_MSG("Load failed! require bool,value:%s",CStringToStlString(strValue).c_str());
			pCheck->SetCheck(false);
		}
	}
}

void DeclareComboboxStr(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{

}

void DeclareComboboxInt(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{

}

void DeclareComboboxFloat(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{

}

void DeclareCheckComboStrArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{

}

void DeclareCheckComboIntArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{

}

void DeclareCheckComboFloatArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{

}

END_NS_AC