
// MFCSampleDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCSample.h"
#include "MFCSampleDlg.h"
#include "afxdialogex.h"

#include "include/cef_app.h"
#include "SampleApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCSampleDlg 对话框



CMFCSampleDlg::CMFCSampleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSAMPLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCSampleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMFCSampleDlg 消息处理程序

BOOL CMFCSampleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CefRefPtr<SampleHandler> handler(new SampleHandler());
	m_simpleHandler = handler;

	CefRefPtr<SampleApp> app(new SampleApp());


	CefSettings settings;
	CefSettingsTraits::init(&settings);
	settings.multi_threaded_message_loop = true;
	settings.remote_debugging_port = 8088;//如果不定义，则不能运行调试工具
	settings.single_process = false;

	CefMainArgs mainArgs;

	CefRefPtr<CefApp> cefApp;
	cefApp = app;

	CefInitialize(mainArgs, settings, cefApp, NULL);

	RECT rect;
	GetClientRect(&rect);
	RECT rectnew = rect;
	rectnew.top = rect.top + 50;
	rectnew.bottom = rect.bottom;
	rectnew.left = rect.left;
	rectnew.right = rect.right;

	CefWindowInfo winInfo;
	winInfo.SetAsChild(GetSafeHwnd(), rectnew);

	
	std::wstring domain = L"baidu.com";


	domain = L"https://" + domain;

	CefBrowserSettings browserSettings;
	CefBrowserHost::CreateBrowser(winInfo, m_simpleHandler, domain.c_str(), browserSettings, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCSampleDlg::OnPaint()
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
HCURSOR CMFCSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCSampleDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CefShutdown();
	CDialogEx::OnClose();
}


void CMFCSampleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	RECT rect;
	GetClientRect(&rect);

	if (m_simpleHandler.get())
	{
		CefRefPtr<CefBrowser> browser = m_simpleHandler->GetBrowser();
		if (browser)
		{
			CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
			::MoveWindow(hwnd, 0, 50, rect.right - rect.left, rect.bottom - 50, true);
		}
	}
}
