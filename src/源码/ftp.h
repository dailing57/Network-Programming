#pragma once

#include "afxinet.h"
#include "MyFtpClientDlg.h"

UINT Query(LPVOID  pParam) {
	if(pParam == NULL)
		AfxEndThread(NULL);
	SERVER_INFO* sp;	
	sp = (SERVER_INFO*)pParam;
	CListBox* pList;
	CString ip;
	CString user;
	CString pwd;
	CString dicName;
	pList = sp->pList;
	ip = sp->ip;
	user = sp->user;
	pwd = sp->pwd;
	if (sp->path == "")
		dicName = "/";
	else
		dicName = sp->path;
	CInternetSession* pSession = NULL;//会话
	CFtpConnection* pConnection = NULL;//连接
	CFtpFileFind* pFileFind = NULL;//查询
	CString fileName;
	BOOL isContinue;
	
	while (pList->GetCount()) {
		pList->DeleteString(0);
	}

	//Internet会话
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);

	try{  
		// 连接服务器
		pConnection = pSession->GetFtpConnection(ip, user, pwd);
	}
	catch (CInternetException* e) {
		// 连接失败
		AfxMessageBox("连接失败!\n", MB_OK | MB_ICONSTOP);
		e->Delete(); 						
		pConnection = NULL;
	}

	if (pConnection != NULL){
		m_upload.EnableWindow(TRUE);
		m_back.EnableWindow(TRUE);
		
		pFileFind = new CFtpFileFind(pConnection);		// 创建CFtpFileFind对象，向构造函数传递CFtpConnection对象的指针
		BOOL isContinue = pFileFind->FindFile(dicName);	// 查找服务器上当前目录的任意文件
		if (!isContinue)								// 没有文件就结束查找
		{
			pFileFind->Close();
			pFileFind = NULL;
		}

		while (isContinue) {							// 找到了第一个文件，继续找其他的文件			
			isContinue = pFileFind->FindNextFile();		// 查找下一个文件	
			CString fileName = pFileFind->GetFileName();// 获得找到的文件的文件名													
			if (pFileFind->IsDirectory())				// 如果找到的对象是个目录，就将目录名放在括弧中
				fileName = "[" + fileName + "]";		// 将找到的文件或目录名显示在列表框中			
			pList->AddString(fileName);
		}
		
		if (pFileFind != NULL) {
			pFileFind->Close();						// 结束查询
			pFileFind = NULL;
		}
	}
	delete pFileFind;								// 删除文件查询对象
	if (pConnection != NULL){
		pConnection->Close();
		delete pConnection;							// 删除FTP连接对象
	}
	delete pSession; 								// 删除Internet 会话对象 
	return 0;
}

//下载文件调用的函数
BOOL MyDownload(CString  ip,CString  user,CString  pwd,CString  sourceName,CString  destName){

	CInternetSession* pSession;			//定义会话对象变量指针
	CFtpConnection* pConnection;		//定义连接对象变量指针

	pConnection = NULL;

	//创建Internet会话对象
	pSession = new CInternetSession(AfxGetAppName(), 1,PRE_CONFIG_INTERNET_ACCESS);

	try{
		//建立FTP连接
		pConnection = pSession->GetFtpConnection(ip,user, pwd);
	}
	catch (CInternetException* e) {
		//错误处理
		e->Delete();
		pConnection = NULL;
		return FALSE;
	}

	if (pConnection != NULL){
		//下载文件
		if (!pConnection->GetFile(sourceName, destName)){
			//下载文件错误
			pConnection->Close();
			delete pConnection;
			delete pSession;
			return FALSE;
		}
	}

	//清除对象
	if (pConnection != NULL){
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return TRUE;
}

UINT DownloadFile(LPVOID  pParam) {
	if (pParam == NULL)
		AfxEndThread(NULL);
	SERVER_INFO* sp;
	CListBox* pList;
	CString ip;
	CString user;
	CString pwd;
	sp = (SERVER_INFO*)pParam;
	pList = sp->pList;
	ip = sp->ip;
	user = sp->user;
	pwd = sp->pwd;
	CString  sourceName = sp->path;
	CInternetSession* pSession = NULL;//会话
	CFtpConnection* pConnection = NULL;//连接
	CFtpFileFind* pFileFind = NULL;//查询
	int  nSel = pList->GetCurSel();//返回当前选中的item
	CString fileName;
	pList->GetText(nSel, fileName);
	if (sourceName.GetAt(0) != '[') {
		CString destName;
		CFileDialog dlg(FALSE, "", fileName);	//文件对话框对象变量
		if (dlg.DoModal() == IDOK)     		//激活文件对话框
		{
			//获得下载文件在本地机上存储的路径和名称
			destName = dlg.GetPathName();
			//调用函数下载文件
			if (MyDownload(ip, user, pwd, sourceName, destName))
				AfxMessageBox("下载成功！", MB_OK | MB_ICONINFORMATION);
			else {
				AfxMessageBox("下载失败！", MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
		else {
			return FALSE;
		}
	}
	else {
		AfxMessageBox("不能下载目录!\n", MB_OK | MB_ICONSTOP);
		return FALSE;
	}
	return 0;
}

BOOL MyUpload(CString strFtpSite, CString strName, CString strPwd, CString strSourceName, CString strDestName){
	
	CInternetSession* pSession;
	CFtpConnection* pConnection;

	pConnection = NULL;

	//创建Internet会话
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);

	try{
		//建立FTP连接
		pConnection = pSession->GetFtpConnection(strFtpSite, strName, strPwd);
	}
	catch (CInternetException* e){
		//错误处理
		e->Delete();
		pConnection = NULL;
		return FALSE;
	}

	if (pConnection != NULL){
		//上传文件
		if (!pConnection->PutFile(_T(strSourceName), _T(strDestName))){
			//上传文件错误
			pConnection->Close();
			delete pConnection;
			delete pSession;
			return FALSE;
		}
	}

	//清除对象
	if (pConnection != NULL){
		pConnection->Close();
		delete pConnection;
	}
	delete pSession;
	return TRUE;
}

UINT UploadFile(LPVOID  pParam) {
	if (pParam == NULL)  AfxEndThread(NULL);
	//用来获取函数调用的参数的代码 
	SERVER_INFO* sp;
	CListBox* pList;
	CString  ip;
	CString  user;
	CString  pwd;
	sp = (SERVER_INFO*)pParam;
	pList = sp->pList;
	ip = sp->ip;
	user = sp->user;
	pwd = sp->pwd;
	CString sourceName;
	CString destPath = sp->path;
	CFileDialog dlg(TRUE, "", "*.*");       //定义文本对话框对象变量
	if (dlg.DoModal() == IDOK)
	{
		//获得待上传的本地机文件路径和文件名
		sourceName = dlg.GetPathName();
		CString destName = destPath + "/" +dlg.GetFileName();

		//调用Upload函数上传文件
		if (MyUpload(ip, user, pwd, sourceName, destName))
			AfxMessageBox("上传成功！", MB_OK | MB_ICONINFORMATION);
		else
			AfxMessageBox("上传失败！", MB_OK | MB_ICONSTOP);
		Query(pParam);
	}
	return 0;
}

UINT MyDeleteFile(LPVOID  pParam) {
	if (pParam == NULL)
		AfxEndThread(NULL);
	SERVER_INFO* sp;
	CListBox* pList;
	CString ip;
	CString user;
	CString pwd;
	sp = (SERVER_INFO*)pParam;
	pList = sp->pList;
	ip = sp->ip;
	user = sp->user;
	pwd = sp->pwd;
	CString  sourceName = sp->path;
	CInternetSession* pSession = NULL;//会话
	CFtpConnection* pConnection = NULL;//连接
	CFtpFileFind* pFileFind = NULL;//查询
	int  nSel = pList->GetCurSel();//返回当前选中的item
	CString fileName;
	pList->GetText(nSel, fileName);

	//Internet会话
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);

	try {
		// 连接服务器
		pConnection = pSession->GetFtpConnection(ip, user, pwd);
	}
	catch (CInternetException* e) {
		// 连接失败
		AfxMessageBox("连接失败!\n", MB_OK | MB_ICONSTOP);
		e->Delete();
		pConnection = NULL;
	}
	if (pConnection != NULL) {
		if (fileName.GetAt(0) != '[') {
			pConnection->Remove(sourceName + "/" + fileName);
			AfxMessageBox("删除成功！", MB_OK | MB_ICONINFORMATION);
			Query(pParam);
		}
		else {
			AfxMessageBox("不能删除目录!\n", MB_OK | MB_ICONSTOP);
			return 0;
		}
	}
	return 0;
}

BOOL putFileToFtp(CString strFilePathName, CString strFileFtpPathName)
{
    CInternetSession *pInternetSession;
    CFtpConnection *pFtpConnection;
    pInternetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
    try
    {
        pFtpConnection = pInternetSession->GetFtpConnection
            (_T("127.0.0.1"),//FTP服务器IP
            _T("user"),//用户名
            _T("pwd"),//密码
            21,//FTP服务器端口号
            TRUE);
        if (pFtpConnection)
        {
            BOOL bPut = pFtpConnection->PutFile(strFilePathName, strFileFtpPathName);
            if (!bPut)
            {
				AfxMessageBox("上传失败");
                return FALSE;
            }
            if (pFtpConnection)
            {
                pFtpConnection->Close();
                pFtpConnection = NULL;
            }
            if (pInternetSession)
            {
                pInternetSession->Close();
                pInternetSession = NULL;
            }
            return TRUE;
        }
        else
        {
			AfxMessageBox(_T("FTP服务器连接失败"));
            return FALSE;
        }
    }
    catch (CInternetException *pEx)
    {
        TCHAR szError[100];
        if (pEx->GetErrorMessage(szError, 100))
        {
            AfxMessageBox(szError);
        }
        pEx->Delete();
        pFtpConnection = NULL;
        if (pInternetSession)
        {
            pInternetSession->Close();
            pInternetSession = NULL;
        }
        return FALSE;
    }
}

BOOL CreateFolderToFtp(CString& strFtpPath){
	//要这一个传入参数好像没啥用，创建的文件夹固定为skonda//SN//image  skonda文件夹已存在
	CInternetSession* pInternetSession;
	CFtpConnection* pFtpConnection;
	pInternetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try{
		pFtpConnection = pInternetSession->GetFtpConnection
		(_T("127.0.0.1"),//FTP服务器IP
			_T("user"),//用户名
			_T("pwd"),//密码
			21,//FTP服务器端口号
			TRUE);
		if (pFtpConnection)
		{
			CString strFtp1Path, strFtp2Path;
			strFtp1Path = _T("/1111");
			strFtp2Path = strFtp1Path + _T("/2222");
			BOOL ret = pFtpConnection->CreateDirectory(strFtp1Path);//不能创建多级目录
			if (!ret)
			{
				AfxMessageBox("创建FTP文件夹失败");
				strFtpPath.Empty();
				return FALSE;
			}

			ret = pFtpConnection->CreateDirectory(strFtp2Path);//不能创建多级目录
			if (!ret)
			{
				AfxMessageBox("创建FTP文件夹失败");
				strFtpPath = strFtp1Path;
				return FALSE;
			}
			if (pFtpConnection)
			{
				pFtpConnection->Close();
				pFtpConnection = NULL;
			}
			if (pInternetSession)
			{
				pInternetSession->Close();
				pInternetSession = NULL;
			}
			strFtpPath = strFtp2Path;
			return TRUE;
		}
		else
		{
			AfxMessageBox("FTP服务器连接失败");
			strFtpPath.Empty();
			return FALSE;
		}
	}
	catch (CInternetException* pEx)
	{
		TCHAR szError[100];
		if (pEx->GetErrorMessage(szError, 100))
		{
			AfxMessageBox(szError);
		}
		pEx->Delete();
		pFtpConnection = NULL;
		if (pInternetSession)
		{
			pInternetSession->Close();
			pInternetSession = NULL;
		}
		return FALSE;
	}
}


