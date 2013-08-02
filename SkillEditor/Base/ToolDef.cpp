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

	ToolApp::Instance().InsertCheckCombo(nDlgID,pCheckCombo);
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
	MapCNameToValueT& mapValues = *(g_objDeclareInfo.m_pMapValues);
	CWnd* pWnd = g_objDeclareInfo.m_pWnd;

	CEdit* pEdit = (CEdit*)pWnd->GetDlgItem(nDlgID);
	ACCHECK(pEdit);

	if(bSaveOrLoad)
	{
		CString strValue;
		pEdit->GetWindowText(strValue);
		int nKey = atoi(CStringToStlString(strValue).c_str());
		if(nKey <= 0)
		{
			WarningMessageBox(_T("ID invalid!"));
			return ;
		}
		mapValues.insert(std::make_pair(strCName,strValue));
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
			WARN_MSG("Load failed! require bool,%s=%s",CStringToStlString(strCName).c_str(),CStringToStlString(strValue).c_str());
			pCheck->SetCheck(false);
		}
	}
}

void _DeclareCombobox(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs,int nDataType)
{
	MapCNameToValueT& mapValues = *(g_objDeclareInfo.m_pMapValues);
	CWnd* pWnd = g_objDeclareInfo.m_pWnd;

	CComboBox* pCombobox = (CComboBox*)pWnd->GetDlgItem(nDlgID);
	ACCHECK(pCombobox);

	if(bSaveOrLoad)
	{
		MapCNameToValueT::iterator iter = mapValues.find(strCName);
		if(iter != mapValues.end())
		{
			WARN_MSG("Save failed! exist cname:%s",CStringToStlString(strCName).c_str());
			return;
		}

		int nSel = pCombobox->GetCurSel();
		if(nSel < 0)
		{
			ErrorMessageBox(_T("Combobox, Must select a item!"));
			return;
		}

		ACCHECK(nSel < (int)vtConfs.size());
		CString strValue;
		switch(nDataType)
		{
		case DATA_INT:
			strValue.Format(_T("%d"),vtConfs[nSel].m_nValue);
			break;
		case DATA_FLOAT:
			strValue.Format(_T("%.6lf"),vtConfs[nSel].m_fValue);
			break;
		case DATA_STRING:
			strValue = vtConfs[nSel].m_strValue;
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

		size_t i;
		for(i = 0; i < vtConfs.size(); ++i)
		{
			CString strValue;
			switch(nDataType)
			{
			case DATA_INT:
				strValue.Format(_T("%d"),vtConfs[i].m_nValue);
				break;
			case DATA_FLOAT:
				strValue.Format(_T("%.6lf"),vtConfs[i].m_fValue);
				break;
			case DATA_STRING:
				strValue = vtConfs[i].m_strValue;
				break;
			}

			if(strValue == iter->second)
			{
				pCombobox->SetCurSel(i);
				break;
			}
		}
		if(i >= vtConfs.size())
		{
			ERROR_MSG("Combobox,Unknow %s=%s",CStringToStlString(strCName).c_str(),CStringToStlString(iter->second).c_str());
			pCombobox->SetCurSel(0);
		}
	}
}

void DeclareComboboxStr(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{
	_DeclareCombobox(bSaveOrLoad,nDlgID,strCName,vtConfs,DATA_STRING);
}

void DeclareComboboxInt(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{
	_DeclareCombobox(bSaveOrLoad,nDlgID,strCName,vtConfs,DATA_INT);
}

void DeclareComboboxFloat(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{
	_DeclareCombobox(bSaveOrLoad,nDlgID,strCName,vtConfs,DATA_FLOAT);
}

void _DeclareCheckCombo(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs,int nDataType)
{
	MapCNameToValueT& mapValues = *(g_objDeclareInfo.m_pMapValues);
	CWnd* pWnd = g_objDeclareInfo.m_pWnd;

	CCheckComboBox* pCheckCombo = (CCheckComboBox*)ToolApp::Instance().FindCheckCombo(nDlgID);
	ACCHECK(pCheckCombo);

	if(bSaveOrLoad)
	{
		MapCNameToValueT::iterator iter = mapValues.find(strCName);
		if(iter != mapValues.end())
		{
			WARN_MSG("Save failed! exist cname:%s",CStringToStlString(strCName).c_str());
			return;
		}

		CString strValue;
		bool bFirst = true;
		ACCHECK(pCheckCombo->GetCount() == vtConfs.size());
		for(int i = 0; i < pCheckCombo->GetCount(); ++i)
		{
			if(pCheckCombo->GetCheck(i))
			{
				if(!bFirst)
					strValue.Append(_T(","));

				CString strItemValue;
				switch(nDataType)
				{
				case DATA_INT:
					strItemValue.Format(_T("%d"),vtConfs[i].m_nValue);
					break;
				case DATA_FLOAT:
					strItemValue.Format(_T("%.6lf"),vtConfs[i].m_fValue);
					break;
				case DATA_STRING:
					strItemValue = vtConfs[i].m_strValue;
					break;
				}
				strValue.Append(strItemValue);
				bFirst = false;
			}
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

		std::vector<CString> vtItemValues;
		CString strItemValue;
		int nPos = 0;
		//数组通过,隔开
		strItemValue = iter->second.Tokenize(ARRAY_DELIMITER,nPos);
		while (strItemValue != _T(""))
		{
			vtItemValues.push_back(strItemValue);
			strItemValue = iter->second.Tokenize(ARRAY_DELIMITER, nPos);
		};

		pCheckCombo->SelectAll(FALSE);

		for(size_t dbItem = 0; dbItem < vtItemValues.size(); ++dbItem)
		{
			size_t ctlItem;
			for(ctlItem = 0; ctlItem < vtConfs.size(); ++ctlItem)
			{
				CString strValue;
				switch(nDataType)
				{
				case DATA_INT:
					strValue.Format(_T("%d"),vtConfs[ctlItem].m_nValue);
					break;
				case DATA_FLOAT:
					strValue.Format(_T("%.6lf"),vtConfs[ctlItem].m_fValue);
					break;
				case DATA_STRING:
					strValue = vtConfs[ctlItem].m_strValue;
				}

				if(strValue == vtItemValues[dbItem])
				{
					pCheckCombo->SetCheck(ctlItem,TRUE);
					break;
				}
			}

			if(ctlItem >= vtConfs.size())
				ERROR_MSG("CheckCombo,Unknow %s=%s",CStringToStlString(strCName).c_str(),CStringToStlString(vtItemValues[dbItem]).c_str());
		}
	}
}

void DeclareCheckComboStrArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{
	_DeclareCheckCombo(bSaveOrLoad,nDlgID,strCName,vtConfs,DATA_STRING);
}

void DeclareCheckComboIntArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{
	_DeclareCheckCombo(bSaveOrLoad,nDlgID,strCName,vtConfs,DATA_INT);
}

void DeclareCheckComboFloatArray(bool bSaveOrLoad,int nDlgID,const CString& strCName,VectorComboItemT& vtConfs)
{
	_DeclareCheckCombo(bSaveOrLoad,nDlgID,strCName,vtConfs,DATA_FLOAT);
}

END_NS_AC