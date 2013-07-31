// EffectCommonWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "SkillEditor.h"
#include "EffectCommonWindow.h"


// EffectCommonWindow 对话框

IMPLEMENT_DYNAMIC(EffectCommonWindow, CDialog)

EffectCommonWindow::EffectCommonWindow(CWnd* pParent /*=NULL*/)
	: CDialog(EffectCommonWindow::IDD, pParent)
{

}

EffectCommonWindow::~EffectCommonWindow()
{
}

void EffectCommonWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EffectCommonWindow, CDialog)
END_MESSAGE_MAP()


// EffectCommonWindow 消息处理程序

BOOL EffectCommonWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	VectorComboItemT vtConfs;
	vtConfs.push_back(SComboItem(_T("选项一"),0));
	vtConfs.push_back(SComboItem(_T("选项二"),1));
	vtConfs.push_back(SComboItem(_T("选项三"),2));
	CreateCheckCombo(this,IDC_EFFECT_ARRAY,vtConfs);
	CreateCombobox(this,IDC_EFFECT_ENUM,vtConfs);
	return TRUE;
}

void EffectCommonWindow::OnLoadFromDB(MapCNameToValueT& mapValues)
{
	InitDeclare(this,mapValues);
	DeclareID(false,IDC_EFFECT_ID,_T("ID"));
	DeclareEditStr(false,IDC_EFFECT_DES,_T("Des"));
	DeclareEditInt(false,IDC_EFFECT_INT,_T("TestInt"));
	DeclareEditFloat(false,IDC_EFFECT_FLOAT,_T("TestFloat"));

	DeclareCheckbox(false,IDC_EFFECT_BOOL,_T("TestBool"));
}

void EffectCommonWindow::OnSaveToDB(MapCNameToValueT& mapValues)
{
	InitDeclare(this,mapValues);
	DeclareID(true,IDC_EFFECT_ID,_T("ID"));
	DeclareEditStr(true,IDC_EFFECT_DES,_T("Des"));
	DeclareEditInt(true,IDC_EFFECT_INT,_T("TestInt"));
	DeclareEditFloat(true,IDC_EFFECT_FLOAT,_T("TestFloat"));

	DeclareCheckbox(true,IDC_EFFECT_BOOL,_T("TestBool"));
}
