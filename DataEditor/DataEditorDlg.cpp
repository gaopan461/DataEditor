// DataEditorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataEditor.h"
#include "DataEditorDlg.h"

#include "ACString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDataEditorDlg 对话框




CDataEditorDlg::CDataEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDataEditorDlg::~CDataEditorDlg()
{
}

void CDataEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDataEditorDlg 消息处理程序

BOOL CDataEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	_tsetlocale(LC_ALL, _T(""));

	// 设置心跳，10ms一次
	::SetTimer(m_hWnd,1,10,NULL);

	// 初始化工具
	InitTool(_T("DataEditor"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDataEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDataEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDataEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDataEditorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nIDEvent == 1)
	{
		this->Update();
	}

	__super::OnTimer(nIDEvent);
}

CWnd* CDataEditorDlg::GetMainWnd()
{
	return this;
}

/*void CDataEditorDlg::InitTab()
{
	//设置页面的位置在m_tab控件范围内 
	CRect rect; 
	m_objMainTab.GetClientRect(&rect); 
	rect.top += 28; 
	rect.bottom -= 4; 
	rect.left += 4; 
	rect.right -= 4; 

	int index = 0;
	std::string str;
	CString cstr;

	str = m_pLuaConfig->GetString("/Config/MagicType/Name");
	cstr = StlStringToCString(str);
	m_objMainTab.InsertItem(index,cstr);
	m_objTabItem1.Create(IDD_TAB_DIALOG1,&m_objMainTab);
	m_objTabItem1.MoveWindow(&rect);

	index++;
	str = m_pLuaConfig->GetString("/Config/AuraEffectType/Name");
	cstr = StlStringToCString(str);
	m_objMainTab.InsertItem(index,cstr);
	m_objTabItem2.Create(IDD_TAB_DIALOG2,&m_objMainTab);
	m_objTabItem2.MoveWindow(&rect);

	m_objTabItem1.ShowWindow(TRUE);
	m_objTabItem2.ShowWindow(FALSE);
	m_objMainTab.SetCurSel(0);
}*/
