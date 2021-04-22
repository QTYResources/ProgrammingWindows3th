/////////////////////////////////////////////////////
// FileCutter.cpp文件

#include "FileCutter.h"


// 内部工作线程
UINT _CutterEntry(LPVOID lpParam)
{
	// 得到CFileCutter对象的指针
	CFileCutter* pCutter = (CFileCutter*)lpParam;

	// 循环处理用户的工作请求
	while(::WaitForSingleObject(pCutter->m_hWorkEvent, INFINITE) == WAIT_OBJECT_0 && 
									!pCutter->m_bExitThread)
	{
		// 设置状态标志，说明正在工作
		::EnterCriticalSection(&pCutter->m_cs);
		pCutter->m_bRunning = TRUE;
		::LeaveCriticalSection(&pCutter->m_cs);

		// 开始真正的工作
		if(pCutter->m_bSplit)
			pCutter->DoSplit();
		else
			pCutter->DoMerge();

		// 准备接受新的工作任务
		pCutter->Reset();
	}

	return 0;
}

//-----------------------接口成员------------------------//

CFileCutter::CFileCutter(HWND hWndNotify) 
{
	// 初始化全局有效变量

	m_hWndNotify = hWndNotify;
	m_bExitThread = FALSE;
	m_bSuspend = FALSE;
	// 创建等待事件对象
	m_hWorkEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	// 创建工作线程
	m_pThread = AfxBeginThread(_CutterEntry, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_pThread->m_bAutoDelete = FALSE;
	m_pThread->ResumeThread();

	// 初始化工作期间有效变量

	// 创建关键代码段
	::InitializeCriticalSection(&m_cs);	
	Reset(); 
}

void CFileCutter::Reset()
{
	::EnterCriticalSection(&m_cs);
	
	// 重置参数信息
	m_strSource.Empty();
	m_strDest.Empty();
	m_uFileSize = 0;
	m_bSplit = TRUE;

	// 重置状态标志
	m_bContinue = TRUE;
	m_bRunning = FALSE;
	
	::LeaveCriticalSection(&m_cs);
}

CFileCutter::~CFileCutter()
{
	// 设置结束标志
	m_bExitThread = TRUE;

	// 设置强制退出标志
	::EnterCriticalSection(&m_cs);
	m_bContinue = FALSE;
	::LeaveCriticalSection(&m_cs);

	// 防止线程在m_hWorkEvent事件上等待
	::SetEvent(m_hWorkEvent);

	// 确保工作线程结束
	::WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	
	// 释放所有资源
	::CloseHandle(m_hWorkEvent);
	::DeleteCriticalSection(&m_cs); 
	delete m_pThread;
}

BOOL CFileCutter::StartSplit(LPCTSTR lpszDestDir, LPCTSTR lpszSourceFile, UINT uFileSize)
{
	if(m_bRunning)
		return FALSE;

	// 保存参数
	::EnterCriticalSection(&m_cs);
	m_strSource = lpszSourceFile; 
	m_strDest = lpszDestDir;
	m_uFileSize = uFileSize;
	m_bSplit = TRUE;
	::LeaveCriticalSection(&m_cs);

	// 通知线程开始工作
	::SetEvent(m_hWorkEvent);
	return TRUE;
}

BOOL CFileCutter::StartMerge(LPCTSTR lpszDestFile, LPCTSTR lpszSourceDir)
{
	if(m_bRunning)
		return FALSE;

	// 保存参数
	::EnterCriticalSection(&m_cs);
	m_strSource = lpszSourceDir; 
	m_strDest = lpszDestFile;
	m_bSplit = FALSE;
	::LeaveCriticalSection(&m_cs);

	// 通知线程开始工作
	::SetEvent(m_hWorkEvent);
	return TRUE;
}

BOOL CFileCutter::SuspendCutter()
{
	if(!m_bRunning)
		return FALSE;

	// 暂停工作线程
	if(!m_bSuspend)
	{
		m_pThread->SuspendThread();
		m_bSuspend = TRUE;
	}
	return TRUE;
}

BOOL CFileCutter::ResumeCutter()
{
	if(!m_bRunning)
		return FALSE;

	// 唤醒工作线程
	if(m_bSuspend)
	{
		m_pThread->ResumeThread();
		m_bSuspend = FALSE;
	}
	return TRUE;
}

void CFileCutter::StopCutter()
{
	// 设置强制退出标志
	::EnterCriticalSection(&m_cs);
	m_bContinue = FALSE;
	::LeaveCriticalSection(&m_cs);

	// 防止线程处于暂停状态
	ResumeCutter();
}

//-------------------------实现代码-------------------------//


void CFileCutter::DoSplit()
{
	int nCompleted = 0;
	CString strSourceFile = m_strSource;
	CString strDestDir = m_strDest;
	CFile sourceFile, destFile;
	
	// 打开源文件
	BOOL bOK = sourceFile.Open(strSourceFile,
		CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary);
	if(!bOK)
	{
		// 通知用户，源文件出错
		::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSourceErr, nCompleted);
		return;
	}

	// 确保目标目录存在（逐层创建它们）
	int nPos = -1;
	while((nPos = strDestDir.Find('\\', nPos+1)) != -1)
	{
		::CreateDirectory(strDestDir.Left(nPos), NULL);
	} 
	::CreateDirectory(strDestDir, NULL);
	if(strDestDir.Right(1) != '\\')
		strDestDir += '\\';

	// 通知用户，开始分割文件
	int nTotalFiles = sourceFile.GetLength()/m_uFileSize + 1;
	::PostMessage(m_hWndNotify, WM_CUTTERSTART, nTotalFiles, TRUE);
	
	// 开始去读源文件，将数据写入目标文件
	const int c_page = 4*1024;
	char buff[c_page];
	DWORD dwRead;

	CString sDestName;
	int nPreCount = 1;
	UINT uWriteBytes;
	do
	{
		// 创建一个目标文件
		sDestName.Format("%d__", nPreCount);
		sDestName += sourceFile.GetFileName();
		if(!destFile.Open(strDestDir + sDestName, CFile::modeWrite|CFile::modeCreate))
		{
			::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitDestErr, nCompleted);
			sourceFile.Close();
			return;
		}

		// 向目标文件写数据，直到大小符合用户的要求，或者源文件读完
		uWriteBytes = 0;
		do
		{
			// 首先判断是否要求终止执行
			if(!m_bContinue)
			{
				destFile.Close();
				sourceFile.Close();
				if(!m_bExitThread)
					::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitUserForce, nCompleted);
				return;
			}

			// 进行真正的读写操作
			dwRead = sourceFile.Read(buff, c_page);
			destFile.Write(buff, dwRead);
			uWriteBytes += dwRead;
		}while(dwRead > 0 && uWriteBytes < m_uFileSize);

		// 关闭这个目标文件
		destFile.Close();

		// 通知用户，当前的状态信息
		nCompleted = nPreCount++;
		::PostMessage(m_hWndNotify, WM_CUTTERSTATUS, 0, nCompleted);
	}while(dwRead > 0);

	// 关闭源文件
	sourceFile.Close();

	// 通知用户，工作完成
	::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSuccess, nCompleted);
}


void CFileCutter::DoMerge()
{
	int nCompleted = 0;
	CString strSourceDir = m_strSource;
	CString strDestFile = m_strDest;
	if(strSourceDir.Right(1) != '\\')
		strSourceDir += '\\';
	if(strDestFile.Right(1) != '\\')
		strDestFile += '\\';
	
	// 取得源目录中待合并的文件的文件名称和数量
	CString strFileName;
	int nTotalFiles = 0;
	CFileFind find;
	BOOL bRet;
	if(find.FindFile(strSourceDir + "*.*"))
	{
		do
		{
			bRet = find.FindNextFile();
			if(find.IsDirectory() && find.IsDots())
				continue;
			if(find.GetFileName().Find("__", 0) != -1)
			{
				nTotalFiles++;
				strFileName = find.GetFileName();
			}
		}while(bRet);
	}
	find.Close();

	if(nTotalFiles == 0)
	{
		// 通知用户，源文件出错
		::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSourceErr, nCompleted);
		return;
	}

	// 取得文件名称
	strFileName = strFileName.Mid(strFileName.Find("__") + 2);

	// 确保目标目录存在（逐层创建它们）
	int nPos = 0;
	while((nPos = strDestFile.Find('\\', nPos+1)) != -1)
	{
		::CreateDirectory(strDestFile.Left(nPos + 1), NULL);
	}
	::CreateDirectory(strDestFile, NULL);
	
	// 创建目标文件
	CFile sourceFile, destFile;
	strDestFile += strFileName;
	if(!destFile.Open(strDestFile, CFile::modeRead|CFile::modeWrite|CFile::modeCreate))
	{
		::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitDestErr, nCompleted);
		return;
	}

	// 通知用户，开始分割文件
	::PostMessage(m_hWndNotify, WM_CUTTERSTART, nTotalFiles, nCompleted);	

	// 开始去读源文件，将数据写入目标文件
	const int c_page = 4*1024;
	char buff[c_page];
	int nPreCount = 1;
	CString sSourceName;
	DWORD dwRead;
	do
	{
		// 打开一个源文件
		sSourceName.Format("%d__", nPreCount);
		sSourceName += strFileName;
		if(!sourceFile.Open(strSourceDir + sSourceName, CFile::modeRead|CFile::shareDenyWrite))
		{
			break;
		}

		// 将这个源文件中的数据全部写入目标文件
		do
		{
			if(!m_bContinue)
			{
				sourceFile.Close();
				destFile.Close();
				if(!m_bExitThread)
					::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitUserForce, nCompleted);
				return;
			}
			dwRead = sourceFile.Read(buff, c_page);
			destFile.Write(buff, dwRead);
		}
		while(dwRead > 0);
		sourceFile.Close();

		// 通知用户，当前的状态信息
		nCompleted = nPreCount++;
		::PostMessage(m_hWndNotify, WM_CUTTERSTATUS, 0, nCompleted);
	}
	while(TRUE);

	// 通知用户，工作完成
	::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSuccess, nCompleted);
}