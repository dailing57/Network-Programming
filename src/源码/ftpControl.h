#pragma once
#include "afxinet.h"
#include "MyFtpClientDlg.h"
#include "CFolder.h"
#include <queue>
UINT Query(LPVOID  pParam);
UINT DownloadFile(LPVOID  pParam);
UINT UploadFile(LPVOID  pParam);
UINT MyDeleteFile(LPVOID  pParam);
UINT CreateFolder(LPVOID  pParam);
UINT DownloadFolder(LPVOID  pParam);
UINT FtpRename(LPVOID  pParam);
BOOL MyDownload(CString  ip, CString  user, CString  pwd, CString  sourceName, CString  destName);
BOOL MyUpload(CString strFtpSite, CString strName, CString strPwd, CString strSourceName, CString strDestName);
BOOL MyCreateFolder(CString folderName, CString user, CString pwd, CString ip);

