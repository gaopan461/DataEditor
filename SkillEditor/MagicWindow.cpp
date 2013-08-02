// MagicWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SkillEditor.h"
#include "MagicWindow.h"


// MagicWindow �Ի���

IMPLEMENT_DYNAMIC(MagicWindow, CDialog)

MagicWindow::MagicWindow(CWnd* pParent /*=NULL*/)
	: CDialog(MagicWindow::IDD, pParent)
{

}

MagicWindow::~MagicWindow()
{
}

void MagicWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MagicWindow, CDialog)
END_MESSAGE_MAP()


// MagicWindow ��Ϣ�������

BOOL MagicWindow::OnInitDialog()
{
	VectorComboItemT vtAttackTypeConf;
	vtAttackTypeConf.push_back(SComboItem(_T("����"),0));
	vtAttackTypeConf.push_back(SComboItem(_T("����"),1));
	CreateCombobox(this,IDC_MAGIC_ATTACKTYPE,vtAttackTypeConf);

	CDialog::OnInitDialog();
	VectorComboItemT vtProfessionConf;
	vtProfessionConf.push_back(SComboItem(_T("սʿ"),0));
	vtProfessionConf.push_back(SComboItem(_T("������"),1));
	vtProfessionConf.push_back(SComboItem(_T("��ʦ"),2));
	CreateCheckCombo(this,IDC_MAGIC_PROFESSION,vtProfessionConf);
	return TRUE;
}


void MagicWindow::OnLoadFromDB(MapCNameToValueT& mapValues)
{
	InitDeclare(this,mapValues);
	DeclareID(false,IDC_MAGIC_ID,_T("ID"));
	DeclareEditStr(false,IDC_MAGIC_DES,_T("Des"));
	
	VectorComboItemT vtAttackTypeConf;
	vtAttackTypeConf.push_back(SComboItem(_T("����"),0));
	vtAttackTypeConf.push_back(SComboItem(_T("����"),1));
	DeclareComboboxInt(false,IDC_MAGIC_ATTACKTYPE,_T("AttackType"),vtAttackTypeConf);

	VectorComboItemT vtProfessionConf;
	vtProfessionConf.push_back(SComboItem(_T("սʿ"),0));
	vtProfessionConf.push_back(SComboItem(_T("������"),1));
	vtProfessionConf.push_back(SComboItem(_T("��ʦ"),2));
	DeclareCheckComboIntArray(false,IDC_MAGIC_PROFESSION,_T("Profession"),vtProfessionConf);

	DeclareCheckbox(false,IDC_MAGIC_HITEDBREAK,_T("HitedBreak"));
	DeclareEditStr(false,IDC_MAGIC_LAYER1,_T("Layer1"));
	DeclareEditStr(false,IDC_MAGIC_LAYER2,_T("Layer2"));
	DeclareEditStr(false,IDC_MAGIC_LAYER3,_T("Layer3"));
	DeclareEditStr(false,IDC_MAGIC_LAYER4,_T("Layer4"));
}

void MagicWindow::OnSaveToDB(MapCNameToValueT& mapValues)
{
	InitDeclare(this,mapValues);
	DeclareID(true,IDC_MAGIC_ID,_T("ID"));
	DeclareEditStr(true,IDC_MAGIC_DES,_T("Des"));

	VectorComboItemT vtAttackTypeConf;
	vtAttackTypeConf.push_back(SComboItem(_T("����"),0));
	vtAttackTypeConf.push_back(SComboItem(_T("����"),1));
	DeclareComboboxInt(true,IDC_MAGIC_ATTACKTYPE,_T("AttackType"),vtAttackTypeConf);

	VectorComboItemT vtProfessionConf;
	vtProfessionConf.push_back(SComboItem(_T("սʿ"),0));
	vtProfessionConf.push_back(SComboItem(_T("������"),1));
	vtProfessionConf.push_back(SComboItem(_T("��ʦ"),2));
	DeclareCheckComboIntArray(true,IDC_MAGIC_PROFESSION,_T("Profession"),vtProfessionConf);

	DeclareCheckbox(true,IDC_MAGIC_HITEDBREAK,_T("HitedBreak"));
	DeclareEditStr(true,IDC_MAGIC_LAYER1,_T("Layer1"));
	DeclareEditStr(true,IDC_MAGIC_LAYER2,_T("Layer2"));
	DeclareEditStr(true,IDC_MAGIC_LAYER3,_T("Layer3"));
	DeclareEditStr(true,IDC_MAGIC_LAYER4,_T("Layer4"));
}
