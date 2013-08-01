
// SkillEditorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SkillEditor.h"
#include "SkillEditorDlg.h"

#include "ToolExcel.h"
#include "ToolTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSkillEditorDlg �Ի���




CSkillEditorDlg::CSkillEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSkillEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_TAB, m_objMainTab);
}

BEGIN_MESSAGE_MAP(CSkillEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, &CSkillEditorDlg::OnTcnSelchangeMainTab)
END_MESSAGE_MAP()


// CSkillEditorDlg ��Ϣ�������

BOOL CSkillEditorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	_tsetlocale(LC_ALL, _T(""));

	// ����������10msһ��
	::SetTimer(m_hWnd,1,10,NULL);

	// ��ʼ������
	InitializeTool(_T("SkillEditor"));

	// ��ʼ����Tab
	InitializeTab();

	// ��ExcelDB
	OpenExcelDBs();

	m_pTree->SetCurrentDB(_T("MagicType"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSkillEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSkillEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSkillEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSkillEditorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(nIDEvent == 1)
	{
		this->Update();
	}

	__super::OnTimer(nIDEvent);
}

CWnd* CSkillEditorDlg::GetMainWnd()
{
	return this;
}

void CSkillEditorDlg::InitializeTab()
{
	m_objMainTab.InsertItem(0,_T("����"));
	m_objMainTab.InsertItem(1,_T("Ч��"));

	CRect rect;
	m_objMainTab.GetClientRect(&rect);
	ClientToScreen(&rect);
	rect.left -= 2;
	rect.top -= 4;

	m_objMagicWindow.Create(IDD_DIALOG_MAGIC,&m_objMainTab);
	m_objEffectCommonWindow.Create(IDD_DIALOG_EFFECTCOMMON,&m_objMainTab);
	m_objMagicWindow.MoveWindow(rect);
	m_objEffectCommonWindow.MoveWindow(rect);

	m_objMagicWindow.ShowWindow(SW_SHOW);
	m_objEffectCommonWindow.ShowWindow(SW_HIDE);
}

void CSkillEditorDlg::OpenExcelDBs()
{
	HMODULE module = GetModuleHandle(0);   
	wchar_t pFileName[MAX_PATH];   
	GetModuleFileName(module, pFileName, MAX_PATH);   

	CString strPath(pFileName);   
	int nPos = strPath.ReverseFind( _T('\\') );   
	if( nPos < 0 )   
		strPath = _T("");   
	else   
		strPath = strPath.Left( nPos );

	SExcelConfig magic;
	magic.m_strExcelPath = strPath + _T("\\MagicTypeConfig.xls");
	magic.m_strExcelCName = _T("MagicType");
	magic.m_strKeyCName = _T("ID");
	magic.m_strDesCName = _T("Des");
	magic.m_vtLayerCName.push_back(_T("Layer1"));
	magic.m_vtLayerCName.push_back(_T("Layer2"));
	magic.m_nHeadRow = 2;
	magic.m_nDataRow = 5;
	m_pExcel->OpenWorkbook(magic);

	SExcelConfig buff;
	buff.m_strExcelPath = strPath + _T("\\AuraEffectTypeConfig.xls");
	buff.m_strExcelCName = _T("AuraEffectType");
	buff.m_strKeyCName = _T("ID");
	buff.m_strDesCName = _T("Des");
	buff.m_nHeadRow = 2;
	buff.m_nDataRow = 5;
	m_pExcel->OpenWorkbook(buff);
}

void CSkillEditorDlg::OnLoadFromDB(MapCNameToValueT& mapValues)
{
	CString strCurrentDB = m_pTree->GetCurrentDB();
	if(strCurrentDB == _T("MagicType"))
		m_objMagicWindow.OnLoadFromDB(mapValues);
	else if(strCurrentDB == _T("AuraEffectType"))
		m_objEffectCommonWindow.OnLoadFromDB(mapValues);
}

void CSkillEditorDlg::OnSaveToDB(MapCNameToValueT& mapValues)
{
	CString strCurrentDB = m_pTree->GetCurrentDB();
	if(strCurrentDB == _T("MagicType"))
		m_objMagicWindow.OnSaveToDB(mapValues);
	else if(strCurrentDB == _T("AuraEffectType"))
		m_objEffectCommonWindow.OnSaveToDB(mapValues);
}

int CSkillEditorDlg::MenuNew()
{
	if(ToolApp::MenuNew() != 0)
	{
		WarningMessageBox(_T("��ǰ�����½�"));
		return -1;
	}

	int nNewKey = GetUnusedKey();
	ACCHECK(nNewKey > 0);
	
	CString strNewKey;
	strNewKey.Format(_T("%d"),nNewKey);

	MapCNameToValueT mapDefault;
	mapDefault[_T("ID")] = strNewKey;
	mapDefault[_T("Des")] = _T("��������");
	mapDefault[_T("TestInt")] = _T("0");
	mapDefault[_T("TestFloat")] = _T("0");
	mapDefault[_T("TestBool")] = _T("false");
	mapDefault[_T("TestEnum")] = _T("0");
	mapDefault[_T("TestArray")] = _T("");

	InsertByKey(nNewKey,mapDefault);
	return 0;
}

CWnd* CSkillEditorDlg::GetCurrentKeyWindow()
{
	CString strCurrentDB = m_pTree->GetCurrentDB();
	if(strCurrentDB == _T("MagicType"))
		return NULL;
	else if(strCurrentDB == _T("AuraEffectType"))
		return m_objEffectCommonWindow.GetDlgItem(IDC_EFFECT_ID);
}

void CSkillEditorDlg::OnTcnSelchangeMainTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	switch(m_objMainTab.GetCurSel())
	{
	case 0:
		m_pTree->SetCurrentDB(_T("MagicType"));
		m_objMagicWindow.ShowWindow(SW_SHOW);
		m_objEffectCommonWindow.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTree->SetCurrentDB(_T("AuraEffectType"));
		m_objMagicWindow.ShowWindow(SW_HIDE);
		m_objEffectCommonWindow.ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}
