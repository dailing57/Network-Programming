#pragma once
#include "pch.h"
#include "ftpControl.h"
//#include "CFolder.h"

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
	BOOL isContinue=0;
	
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
		}
		else {
			CString folderName;
			for (int i = 1; i < fileName.GetLength()-1; i++) {
				folderName += fileName[i];
			}
			pConnection->RemoveDirectory(sourceName + "/" + folderName);
		}
	}
	if (pConnection != NULL) {
		AfxMessageBox("删除成功！", MB_OK | MB_ICONINFORMATION);
		Query(pParam);
	}
	return 0;
}

BOOL MyCreateFolder(CString folderName,CString user,CString pwd,CString ip){
	CInternetSession* pInternetSession;
	CFtpConnection* pFtpConnection;
	pInternetSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try{
		pFtpConnection = pInternetSession->GetFtpConnection(ip,user,pwd);
		if (pFtpConnection)
		{
			BOOL ret = pFtpConnection->CreateDirectory(folderName);//不能创建多级目录
			if (!ret)
			{
				AfxMessageBox("创建FTP文件夹失败");
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
			AfxMessageBox("FTP服务器连接失败");
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

UINT CreateFolder(LPVOID  pParam) {
	if (pParam == NULL)  AfxEndThread(NULL);
	CFolder folder;
	folder.DoModal();
	CString folderName = folder.folderName;
	SERVER_INFO* sp = (SERVER_INFO*)pParam;
	CListBox* pList = sp->pList;
	CString  ip = sp->ip;
	CString  user = sp->user;
	CString  pwd = sp->pwd;
	folderName = sp->path + "/" + folderName;
	MyCreateFolder(folderName, user, pwd, ip);
	Query(pParam);
	return 0;
}

void MyDownloadFolder(CFtpConnection* pConnection,CString path,CString destPath) {
	pConnection->SetCurrentDirectoryA(path);
	CFtpFileFind finder(pConnection);
	BOOL isContinue = finder.FindFile(path);
	queue<CString> q;
	while (isContinue) {							
		isContinue = finder.FindNextFileA();
		CString fileName = finder.GetFileName();
		if (finder.IsDirectory()) {
			q.push(fileName);
			CreateDirectory(destPath + "/" + fileName, NULL);
		}
		else {
			pConnection->GetFile(path + "/" + fileName, destPath + "/" + fileName);
		}
	}
	finder.Close();
	while (!q.empty()) {
		CString curDir = q.front();
		q.pop();
		MyDownloadFolder(pConnection, path + "/" + curDir, destPath + "/" + curDir);
	}
}

CString getFolderName() {
	char szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择目录：";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	if (lp && SHGetPathFromIDList(lp, szPath))
		str.Format("%s", szPath);
	else
		AfxMessageBox("无效的目录");
	return str;
}

UINT DownloadFolder(LPVOID  pParam) {
	if (pParam == NULL)  AfxEndThread(NULL);
	SERVER_INFO* sp = (SERVER_INFO*)pParam;
	CListBox* pList = sp->pList;
	CString  ip = sp->ip;
	CString  user = sp->user;
	CString  pwd = sp->pwd;
	CString  path = sp->path;
	CInternetSession* pSession = NULL;
	CFtpConnection* pConnection = NULL;
	CFtpFileFind* pFileFind = NULL;
	CString fileName;
	BOOL isContinue=0;
	int  nSel = pList->GetCurSel();//返回当前选中的item
	CString dir;
	CString curDir;
	pList->GetText(nSel, dir);
	for (int i = 1; i < dir.GetLength() - 1; i++)
		curDir += dir[i];
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
		CString destPath = getFolderName() + "/" + curDir;
		CreateDirectory(destPath , NULL);
		MyDownloadFolder(pConnection,path, destPath);
		pFileFind = NULL;
		delete pFileFind;								// 删除文件查询对象
		if (pConnection != NULL) {
			pConnection->Close();
			delete pConnection;							// 删除FTP连接对象
		}
	}
	delete pSession; 								// 删除Internet 会话对象 
	AfxMessageBox("文件夹下载成功");
	return 0;
}

UINT FtpRename(LPVOID  pParam) {
	if (pParam == NULL)
		AfxEndThread(NULL);
	SERVER_INFO* sp;
	sp = (SERVER_INFO*)pParam;
	CListBox* pList;
	CString ip;
	CString user;
	CString pwd;
	CString dicName;
	CString path;
	pList = sp->pList;
	ip = sp->ip;
	user = sp->user;
	pwd = sp->pwd;
	path = sp->path;
	CInternetSession* pSession = NULL;//会话
	CFtpConnection* pConnection = NULL;//连接
	CFtpFileFind* pFileFind = NULL;//查询
	CString fileName;
	BOOL isContinue = 0;
	int  nSel = pList->GetCurSel();
	CString cur, curName;
	pList->GetText(nSel, cur);
	if (cur.GetAt(0) == '[')
		for (int i = 1; i < cur.GetLength() - 1; i++)
			curName += cur[i];
	else
		curName = cur;
	pSession = new CInternetSession(AfxGetAppName(), 1, PRE_CONFIG_INTERNET_ACCESS);
	try {
		pConnection = pSession->GetFtpConnection(ip, user, pwd);
	}
	catch (CInternetException* e) {
		AfxMessageBox("连接失败!\n", MB_OK | MB_ICONSTOP);
		e->Delete();
		pConnection = NULL;
	}

	if (pConnection != NULL) {
		CFolder newName;
		newName.DoModal();
		pConnection->Rename(path+"/"+curName, path + "/" + newName.folderName);
	}
	if (pConnection != NULL) {
		pConnection->Close();
		delete pConnection;							// 删除FTP连接对象
	}
	delete pSession; 								// 删除Internet 会话对象 
	Query(pParam);
	return 0;
}



