#pragma once


// CFolder 对话框

class CFolder : public CDialogEx
{
	DECLARE_DYNAMIC(CFolder)

public:
	CFolder(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFolder();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString folderName;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
