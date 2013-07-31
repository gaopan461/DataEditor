// MagicWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "SkillEditor.h"
#include "MagicWindow.h"


// MagicWindow 对话框

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


// MagicWindow 消息处理程序


void MagicWindow::OnLoadFromDB(MapCNameToValueT& mapValues)
{

}

void MagicWindow::OnSaveToDB(MapCNameToValueT& mapValues)
{

}
