// CFolder.cpp: 实现文件
//

#include "pch.h"
#include "MyFtpClient.h"
#include "CFolder.h"
#include "afxdialogex.h"


// CFolder 对话框

IMPLEMENT_DYNAMIC(CFolder, CDialogEx)

CFolder::CFolder(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, folderName(_T(""))
{

}

CFolder::~CFolder()
{
}

void CFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, folderName);
}


BEGIN_MESSAGE_MAP(CFolder, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFolder::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFolder::OnBnClickedCancel)
END_MESSAGE_MAP()


// CFolder 消息处理程序


void CFolder::OnBnClickedOk()
{
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


void CFolder::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
