#include "ToolCtrl.h"
#include "ACString.h"
#include "ToolApp.h"
#include "ToolLayout.h"

BEGIN_NS_AC

int SComboItem::Init(LuaConfig& rLuaConfig)
{
	std::string str = rLuaConfig.GetString("./Name");
	strName = StlStringToCString(str);
	nValue = rLuaConfig.GetInteger("./Value");
	bDefault = rLuaConfig.GetBoolean("./Default");
	return 0;
}

//---------------------------------------------------------

int SCtrl::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	std::string str = rLuaConfig.GetString("./Name");
	strName = StlStringToCString(str);
	str = rLuaConfig.GetString("./CName");
	strCName = StlStringToCString(str);
	nCtrl = rLuaConfig.GetInteger("./Ctrl");
	nWidth1 = rLuaConfig.GetInteger("./Width1");
	bNewline = rLuaConfig.GetBoolean("./Newline");
	return 0;
}

void SCtrl::CreateStatic(CWnd* pParent)
{
	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth1,false);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pStatic = new CStatic;
	pStatic->Create(strName,WS_CHILD|WS_VISIBLE|SS_CENTER,rect,pParent,id);
	pStatic->SetFont(&ToolApp::Instance().m_objFont);
}

//---------------------------------------------------------

int SEdit::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	nType = rLuaConfig.GetInteger("./Type");
	nWidth2 = rLuaConfig.GetInteger("./Width2");
	std::string str = rLuaConfig.GetString("./Default");
	strDefault = StlStringToCString(str);

	CreateStatic(pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth2,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pCtrl = new CEdit;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER,rect,pParent,id);
	pCtrl->SetFont(&ToolApp::Instance().m_objFont);
	return 0;
}

int SEdit::LoadDefaultValue()
{
	pCtrl->SetWindowText(strDefault);
	return 0;
}

int SEdit::DataToCtrl(CString& data)
{
	if(nType == DATA_INT)
	{
		int nData = atoi(CStringToStlString(data).c_str());
		data.Format(_T("%d"),nData);
	}

	pCtrl->SetWindowText(data);
	return 0;
}

int SEdit::CtrlToData(CString& data)
{
	CString strCtrlVal;
	pCtrl->GetWindowText(strCtrlVal);
	if(nType == DATA_INT)
	{
		int nData = atoi(CStringToStlString(strCtrlVal).c_str());
		data.Format(_T("%d"),nData);
	}
	else if(nType == DATA_FLOAT)
	{
		double fData = atof(CStringToStlString(strCtrlVal).c_str());
		data.Format(_T("%.6lf"),fData);
	}
	else
		data = strCtrlVal;

	return 0;
}

//---------------------------------------------------------

int SCheck::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	bDefault = rLuaConfig.GetBoolean("./Default");

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth1,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pCtrl = new CButton;
	pCtrl->Create(strName,WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX,rect,pParent,id);
	pCtrl->SetFont(&ToolApp::Instance().m_objFont);
	return 0;
}

int SCheck::LoadDefaultValue()
{
	pCtrl->SetCheck(bDefault);
	return 0;
}

int SCheck::DataToCtrl(CString& data)
{
	bool val = false;
	if(_stricmp(CStringToStlString(data).c_str(),"false") == 0)
		val = false;
	else if(_stricmp(CStringToStlString(data).c_str(),"true") == 0)
		val = true;
	else
		val = atoi(CStringToStlString(data).c_str()) ? true : false;

	pCtrl->SetCheck(val);
	return 0;
}

int SCheck::CtrlToData(CString& data)
{
	int bCheck = pCtrl->GetCheck();
	data = bCheck ? _T("true") : _T("false");
	return 0;
}

//---------------------------------------------------------

int SStatic::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth1,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pStatic = new CStatic;
	pStatic->Create(strName,WS_CHILD|WS_VISIBLE,rect,pParent,id);
	pStatic->SetFont(&ToolApp::Instance().m_objFont);
	return 0;
}

//---------------------------------------------------------

int SCombobox::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	nWidth2 = rLuaConfig.GetInteger("./Width2");

	CreateStatic(pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth2,bNewline);
	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pCtrl = new CComboBox;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,rect,pParent,id);
	pCtrl->SetFont(&ToolApp::Instance().m_objFont);

	rLuaConfig.IterTable<SCombobox>("./Confs",this,&SCombobox::pfnAddComboItem,&rLuaConfig);
	return 0;
}

int SCombobox::LoadDefaultValue()
{
	for(size_t i = 0; i < vtItems.size(); ++i)
	{
		if(vtItems[i].bDefault)
		{
			pCtrl->SetCurSel(TRUE);
			break;
		}
	}
	return 0;
}

int SCombobox::DataToCtrl(CString& data)
{
	int dbVal = atoi(CStringToStlString(data).c_str());
	for(size_t ctlItem = 0; ctlItem < vtItems.size(); ++ctlItem)
	{
		if(dbVal == vtItems[ctlItem].nValue)
		{
			pCtrl->SetCurSel(ctlItem);
			break;
		}
	}
	return 0;
}

int SCombobox::CtrlToData(CString& data)
{
	int curSel = pCtrl->GetCurSel();
	data.Format(_T("%d"),vtItems[curSel].nValue);
	return 0;
}

void SCombobox::pfnAddComboItem(void* ctx)
{
	LuaConfig* pLuaConfig = (LuaConfig*)ctx;
	ACCHECK(pLuaConfig);

	SComboItem comboItem;
	comboItem.Init(*pLuaConfig);

	pCtrl->AddString(comboItem.strName);

	vtItems.push_back(comboItem);
}

//---------------------------------------------------------

int SCheckCombo::Init( LuaConfig& rLuaConfig,CWnd* pParent )
{
	SCtrl::Init(rLuaConfig,pParent);
	nWidth2 = rLuaConfig.GetInteger("./Width2");

	CreateStatic(pParent);

	CRect rect = ToolApp::Instance().GetLayout()->GetNextRect(nWidth2,bNewline);
	int tableLen = rLuaConfig.GetTableLen("./Confs");
	rect.bottom += tableLen * CTRL_HEIGHT;

	int id = ToolApp::Instance().GetLayout()->GetNextID();
	pCtrl = new CCheckComboBox;
	pCtrl->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,rect,pParent,id);
	pCtrl->SetFont(&ToolApp::Instance().m_objFont);

	rLuaConfig.IterTable<SCheckCombo>("./Confs",this,&SCheckCombo::pfnAddComboItem,&rLuaConfig);

	return 0;
}

int SCheckCombo::LoadDefaultValue()
{
	for(size_t i = 0; i < vtItems.size(); ++i)
	{
		pCtrl->SetCheck(i,vtItems[i].bDefault);
	}
	return 0;
}

int SCheckCombo::DataToCtrl(CString& data)
{
	std::vector<CString> vtDataItems;
	CString strItemText;
	int nPos = 0;
	//数组通过,隔开
	strItemText = data.Tokenize(ARRAY_DELIMITER,nPos);
	while (strItemText != _T(""))
	{
		vtDataItems.push_back(strItemText);
		strItemText = data.Tokenize(ARRAY_DELIMITER, nPos);
	};

	pCtrl->SelectAll(FALSE);

	for(size_t dbItem = 0; dbItem < vtDataItems.size(); ++dbItem)
	{
		int dbVal = atoi(CStringToStlString(vtDataItems[dbItem]).c_str());
		for(size_t ctlItem = 0; ctlItem < vtItems.size(); ++ctlItem)
		{
			if(dbVal == vtItems[ctlItem].nValue)
				pCtrl->SetCheck(ctlItem,true);
		}
	}
	return 0;
}

int SCheckCombo::CtrlToData(CString& data)
{
	bool bFirst = true;
	for(size_t ctrlItem = 0; ctrlItem < vtItems.size(); ++ctrlItem)
	{
		if(pCtrl->GetCheck(ctrlItem))
		{
			if(bFirst)
				bFirst = false;
			else
				data.Append(ARRAY_DELIMITER);

			data.AppendFormat(_T("%d"),vtItems[ctrlItem].nValue);
		}
	}
	return 0;
}

void SCheckCombo::pfnAddComboItem(void* ctx)
{
	LuaConfig* pLuaConfig = (LuaConfig*)ctx;
	ACCHECK(pLuaConfig);

	SComboItem comboItem;
	comboItem.Init(*pLuaConfig);

	((CCheckComboBox*)pCtrl)->AddString(comboItem.strName);

	vtItems.push_back(comboItem);
}

END_NS_AC