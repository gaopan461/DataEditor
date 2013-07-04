// TabItemDialog2.cpp : 实现文件
//

#include "stdafx.h"
#include "DataEditor.h"
#include "TabItemDialog2.h"


// TabItemDialog2 对话框

IMPLEMENT_DYNAMIC(TabItemDialog2, CDialog)

TabItemDialog2::TabItemDialog2(CWnd* pParent /*=NULL*/)
	: CDialog(TabItemDialog2::IDD, pParent)
{

}

TabItemDialog2::~TabItemDialog2()
{
}

void TabItemDialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TabItemDialog2, CDialog)
END_MESSAGE_MAP()


// TabItemDialog2 消息处理程序
