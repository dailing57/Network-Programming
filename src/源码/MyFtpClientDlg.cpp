
// MyFtpClientDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyFtpClient.h"
#include "MyFtpClientDlg.h"
#include "afxdialogex.h"

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


// CMyFtpClientDlg 对话框



CMyFtpClientDlg::CMyFtpClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYFTPCLIENT_DIALOG, pParent)
	, m_ip(_T(""))
	, m_user(_T(""))
	, m_pwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyFtpClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ip);
	DDX_Text(pDX, IDC_EDIT2, m_user);
	DDX_Text(pDX, IDC_EDIT3, m_pwd);
	DDX_Control(pDX, IDC_LIST1, m_fileList);
	DDX_Control(pDX, IDC_BUTTON3, m_download);
	DDX_Control(pDX, IDC_BUTTON4, m_upload);
	DDX_Control(pDX, IDC_BUTTON5, m_back);
}

BEGIN_MESSAGE_MAP(CMyFtpClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CMyFtpClientDlg::FtpQuery)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyFtpClientDlg::FtpDownload)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyFtpClientDlg::FtpUpload)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyFtpClientDlg::FtpBack)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMyFtpClientDlg::OnSelchangeListFile)
	ON_LBN_DBLCLK(IDC_LIST1, &CMyFtpClientDlg::OnDblclkList1)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_32772, &CMyFtpClientDlg::DeleteFtpFile)

	ON_COMMAND(ID_32773, &CMyFtpClientDlg::OnUpload)
	ON_COMMAND(ID_32774, &CMyFtpClientDlg::OnDownload)
	ON_COMMAND(ID_32775, &CMyFtpClientDlg::OnNewFolder)
	ON_COMMAND(ID_32776, &CMyFtpClientDlg::OnRename)
END_MESSAGE_MAP()


// CMyFtpClientDlg 消息处理程序

BOOL CMyFtpClientDlg::OnInitDialog()
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

	m_ip = "127.0.0.1";
	m_user = "root";
	m_pwd = "123";

	m_download.EnableWindow(FALSE);
	m_upload.EnableWindow(FALSE);
	m_back.EnableWindow(FALSE);
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyFtpClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyFtpClientDlg::OnPaint()
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
HCURSOR CMyFtpClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyFtpClientDlg::FtpQuery()
{
	UpdateData(TRUE);
	SERVER_INFO* sp = new SERVER_INFO;
	sp->pList = &m_fileList;
	sp->ip = m_ip;
	sp->user = m_user;
	sp->pwd = m_pwd;
	while (m_fileList.GetCount())
		m_fileList.DeleteString(0);
	historyPath.clear();
	AfxBeginThread(Query, sp);
	m_back.EnableWindow(TRUE);
	m_upload.EnableWindow(TRUE);
}


void CMyFtpClientDlg::FtpDownload()
{
	UpdateData(TRUE);
	SERVER_INFO* sp = new SERVER_INFO;
	sp->pList = &m_fileList;
	sp->ip = m_ip;
	sp->user = m_user;
	sp->pwd = m_pwd;
	CString  sourceName;
	int  nSel = sp->pList->GetCurSel();//返回当前选中的item
	sp->pList->GetText(nSel, sourceName);//把nSel的文本值给sourceName
	for (auto x : historyPath) {
		sp->path += x;
	}
	if (sourceName.GetAt(0) == '[') {
		CString tmp;
		for (int i = 1; i < sourceName.GetLength() - 1; i++)
			tmp += sourceName[i];
		sp->path += "/" + tmp;
		AfxBeginThread(DownloadFolder, sp);
	}
	else {
		sp->path += "/" + sourceName;
		AfxBeginThread(DownloadFile, sp);
	}
}


void CMyFtpClientDlg::FtpUpload()
{
	UpdateData(TRUE);
	SERVER_INFO* sp = new SERVER_INFO;
	sp->pList = &m_fileList;
	sp->ip = m_ip;
	sp->user = m_user;
	sp->pwd = m_pwd;
	for (auto x : historyPath) {
		sp->path += x;
	}
	AfxBeginThread(UploadFile, sp);
}


void CMyFtpClientDlg::FtpBack()
{
	UpdateData(TRUE);
	SERVER_INFO* sp = new SERVER_INFO;
	sp->pList = &m_fileList;
	sp->ip = m_ip;
	sp->user = m_user;
	sp->pwd = m_pwd;
	if (!historyPath.empty()) {
		historyPath.pop_back();
	}
	else {
		AfxMessageBox("已经到达根目录！", MB_OK | MB_ICONSTOP);
		return;
	}
	for (auto x : historyPath) {
		sp->path += x;
	}
	AfxBeginThread(Query, sp);
}


void CMyFtpClientDlg::OnSelchangeListFile()
{
	m_download.EnableWindow(TRUE);
}


void CMyFtpClientDlg::OnDblclkList1()
{
	UpdateData(TRUE);
	SERVER_INFO* sp = new SERVER_INFO;
	sp->pList = &m_fileList;
	sp->ip = m_ip;
	sp->user = m_user;
	sp->pwd = m_pwd;
	int  nSel = sp->pList->GetCurSel();//返回当前选中的item
	CString  sourceName;
	sp->pList->GetText(nSel, sourceName);//把nSel的文本值给sourceName
	for (auto x : historyPath) {
		sp->path += x;
	}
	if (sourceName.GetAt(0) != '[') {
		sp->path += "/" + sourceName;
		AfxBeginThread(DownloadFile, sp);
		return;
	}
	CString dicName;
	for (int i = 1; i < sourceName.GetLength()-1; i++) {
		dicName += sourceName.GetAt(i);
	}
	historyPath.push_back("/" + dicName);
	sp->path += "/" + dicName;
	AfxBeginThread(Query, sp);
}


void CMyFtpClientDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu menu;
	int deleteAttributes = MF_BYCOMMAND;
	int downloadAttributes = MF_BYCOMMAND;
	int renameAttributes = MF_BYCOMMAND;
	if (m_fileList.GetCurSel() == CB_ERR)
		deleteAttributes |= MF_GRAYED, downloadAttributes |= MF_GRAYED, renameAttributes |= MF_GRAYED;
	menu.LoadMenu(IDR_MENU1);
	CMenu* pMenu;
	pMenu = menu.GetSubMenu(0);
	pMenu->EnableMenuItem(ID_32772, deleteAttributes);
	pMenu->EnableMenuItem(ID_32774, downloadAttributes);
	pMenu->EnableMenuItem(ID_32776, renameAttributes);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	pMenu->Detach();
	menu.DestroyMenu();

}

void CMyFtpClientDlg::DeleteFtpFile() {
	UpdateData(TRUE);
	SERVER_INFO* sp = new SERVER_INFO;
	sp->pList = &m_fileList;
	sp->ip = m_ip;
	sp->user = m_user;
	sp->pwd = m_pwd;
	for (auto x : historyPath) {
		sp->path += x;
	}
	AfxBeginThread(MyDeleteFile, sp);
}

void CMyFtpClientDlg::OnUpload()
{
	FtpUpload();
}


void CMyFtpClientDlg::OnDownload()
{
	FtpDownload();
}


void CMyFtpClientDlg::OnNewFolder()
{
	UpdateData(TRUE);
	SERVER_INFO* sp = new SERVER_INFO;
	sp->pList = &m_fileList;
	sp->ip = m_ip;
	sp->user = m_user;
	sp->pwd = m_pwd;
	for (auto x : historyPath) {
		sp->path += x;
	}
	AfxBeginThread(CreateFolder, sp);
}


void CMyFtpClientDlg::OnRename()
{
	UpdateData(TRUE);
	SERVER_INFO* sp = new SERVER_INFO;
	sp->pList = &m_fileList;
	sp->ip = m_ip;
	sp->user = m_user;
	sp->pwd = m_pwd;
	for (auto x : historyPath) {
		sp->path += x;
	}
	AfxBeginThread(FtpRename, sp);
}
