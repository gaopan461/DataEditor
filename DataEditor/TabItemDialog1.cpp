// TabItemDialog1.cpp : 实现文件
//

#include "stdafx.h"
#include "DataEditor.h"
#include "TabItemDialog1.h"


// TabItemDialog1 对话框

IMPLEMENT_DYNAMIC(TabItemDialog1, CDialog)

TabItemDialog1::TabItemDialog1(CWnd* pParent /*=NULL*/)
	: CDialog(TabItemDialog1::IDD, pParent)
{

}

TabItemDialog1::~TabItemDialog1()
{
}

void TabItemDialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TabItemDialog1, CDialog)
END_MESSAGE_MAP()


// TabItemDialog1 消息处理程序
