
// MyFtpClientDlg.h: 头文件
//

#pragma once
#include <vector>
#include "Resource.h"
#include "ftpControl.h"

using namespace std;
typedef struct {
	CListBox* pList;
	CString  ip;
	CString  user;
	CString  pwd;
	CString  path;
} SERVER_INFO;


// CMyFtpClientDlg 对话框
class CMyFtpClientDlg : public CDialogEx
{
// 构造
public:
	CMyFtpClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFTPCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void FtpQuery();
	afx_msg void FtpDownload();
	afx_msg void FtpUpload();
	afx_msg void FtpBack();
	afx_msg void OnSelchangeListFile();
	afx_msg void OnDblclkList1();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void DeleteFtpFile();
	// 目标主机IP
	CString m_ip;
	// 用户名
	CString m_user;
	// 密码
	CString m_pwd;
	// 文件列表
	CListBox m_fileList;
	// 路径
	vector<CString> historyPath;
	// 下载按钮
	CButton m_download;
	// 上传按钮
	CButton m_upload;
	// 返回
	CButton m_back;
	afx_msg void OnUpload();
	afx_msg void OnDownload();
	afx_msg void OnNewFolder();
	afx_msg void OnRename();
};
