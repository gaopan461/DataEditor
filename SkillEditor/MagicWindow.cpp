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


void MagicWindow::OnLoadFromDB(MapCNameToValueT& mapValues)
{

}

void MagicWindow::OnSaveToDB(MapCNameToValueT& mapValues)
{

}
