/////////////////////////////////////////////////////
// FileCutter.cpp�ļ�

#include "FileCutter.h"


// �ڲ������߳�
UINT _CutterEntry(LPVOID lpParam)
{
	// �õ�CFileCutter�����ָ��
	CFileCutter* pCutter = (CFileCutter*)lpParam;

	// ѭ�������û��Ĺ�������
	while(::WaitForSingleObject(pCutter->m_hWorkEvent, INFINITE) == WAIT_OBJECT_0 && 
									!pCutter->m_bExitThread)
	{
		// ����״̬��־��˵�����ڹ���
		::EnterCriticalSection(&pCutter->m_cs);
		pCutter->m_bRunning = TRUE;
		::LeaveCriticalSection(&pCutter->m_cs);

		// ��ʼ�����Ĺ���
		if(pCutter->m_bSplit)
			pCutter->DoSplit();
		else
			pCutter->DoMerge();

		// ׼�������µĹ�������
		pCutter->Reset();
	}

	return 0;
}

//-----------------------�ӿڳ�Ա------------------------//

CFileCutter::CFileCutter(HWND hWndNotify) 
{
	// ��ʼ��ȫ����Ч����

	m_hWndNotify = hWndNotify;
	m_bExitThread = FALSE;
	m_bSuspend = FALSE;
	// �����ȴ��¼�����
	m_hWorkEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	// ���������߳�
	m_pThread = AfxBeginThread(_CutterEntry, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	m_pThread->m_bAutoDelete = FALSE;
	m_pThread->ResumeThread();

	// ��ʼ�������ڼ���Ч����

	// �����ؼ������
	::InitializeCriticalSection(&m_cs);	
	Reset(); 
}

void CFileCutter::Reset()
{
	::EnterCriticalSection(&m_cs);
	
	// ���ò�����Ϣ
	m_strSource.Empty();
	m_strDest.Empty();
	m_uFileSize = 0;
	m_bSplit = TRUE;

	// ����״̬��־
	m_bContinue = TRUE;
	m_bRunning = FALSE;
	
	::LeaveCriticalSection(&m_cs);
}

CFileCutter::~CFileCutter()
{
	// ���ý�����־
	m_bExitThread = TRUE;

	// ����ǿ���˳���־
	::EnterCriticalSection(&m_cs);
	m_bContinue = FALSE;
	::LeaveCriticalSection(&m_cs);

	// ��ֹ�߳���m_hWorkEvent�¼��ϵȴ�
	::SetEvent(m_hWorkEvent);

	// ȷ�������߳̽���
	::WaitForSingleObject(m_pThread->m_hThread, INFINITE);
	
	// �ͷ�������Դ
	::CloseHandle(m_hWorkEvent);
	::DeleteCriticalSection(&m_cs); 
	delete m_pThread;
}

BOOL CFileCutter::StartSplit(LPCTSTR lpszDestDir, LPCTSTR lpszSourceFile, UINT uFileSize)
{
	if(m_bRunning)
		return FALSE;

	// �������
	::EnterCriticalSection(&m_cs);
	m_strSource = lpszSourceFile; 
	m_strDest = lpszDestDir;
	m_uFileSize = uFileSize;
	m_bSplit = TRUE;
	::LeaveCriticalSection(&m_cs);

	// ֪ͨ�߳̿�ʼ����
	::SetEvent(m_hWorkEvent);
	return TRUE;
}

BOOL CFileCutter::StartMerge(LPCTSTR lpszDestFile, LPCTSTR lpszSourceDir)
{
	if(m_bRunning)
		return FALSE;

	// �������
	::EnterCriticalSection(&m_cs);
	m_strSource = lpszSourceDir; 
	m_strDest = lpszDestFile;
	m_bSplit = FALSE;
	::LeaveCriticalSection(&m_cs);

	// ֪ͨ�߳̿�ʼ����
	::SetEvent(m_hWorkEvent);
	return TRUE;
}

BOOL CFileCutter::SuspendCutter()
{
	if(!m_bRunning)
		return FALSE;

	// ��ͣ�����߳�
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

	// ���ѹ����߳�
	if(m_bSuspend)
	{
		m_pThread->ResumeThread();
		m_bSuspend = FALSE;
	}
	return TRUE;
}

void CFileCutter::StopCutter()
{
	// ����ǿ���˳���־
	::EnterCriticalSection(&m_cs);
	m_bContinue = FALSE;
	::LeaveCriticalSection(&m_cs);

	// ��ֹ�̴߳�����ͣ״̬
	ResumeCutter();
}

//-------------------------ʵ�ִ���-------------------------//


void CFileCutter::DoSplit()
{
	int nCompleted = 0;
	CString strSourceFile = m_strSource;
	CString strDestDir = m_strDest;
	CFile sourceFile, destFile;
	
	// ��Դ�ļ�
	BOOL bOK = sourceFile.Open(strSourceFile,
		CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary);
	if(!bOK)
	{
		// ֪ͨ�û���Դ�ļ�����
		::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSourceErr, nCompleted);
		return;
	}

	// ȷ��Ŀ��Ŀ¼���ڣ���㴴�����ǣ�
	int nPos = -1;
	while((nPos = strDestDir.Find('\\', nPos+1)) != -1)
	{
		::CreateDirectory(strDestDir.Left(nPos), NULL);
	} 
	::CreateDirectory(strDestDir, NULL);
	if(strDestDir.Right(1) != '\\')
		strDestDir += '\\';

	// ֪ͨ�û�����ʼ�ָ��ļ�
	int nTotalFiles = sourceFile.GetLength()/m_uFileSize + 1;
	::PostMessage(m_hWndNotify, WM_CUTTERSTART, nTotalFiles, TRUE);
	
	// ��ʼȥ��Դ�ļ���������д��Ŀ���ļ�
	const int c_page = 4*1024;
	char buff[c_page];
	DWORD dwRead;

	CString sDestName;
	int nPreCount = 1;
	UINT uWriteBytes;
	do
	{
		// ����һ��Ŀ���ļ�
		sDestName.Format("%d__", nPreCount);
		sDestName += sourceFile.GetFileName();
		if(!destFile.Open(strDestDir + sDestName, CFile::modeWrite|CFile::modeCreate))
		{
			::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitDestErr, nCompleted);
			sourceFile.Close();
			return;
		}

		// ��Ŀ���ļ�д���ݣ�ֱ����С�����û���Ҫ�󣬻���Դ�ļ�����
		uWriteBytes = 0;
		do
		{
			// �����ж��Ƿ�Ҫ����ִֹ��
			if(!m_bContinue)
			{
				destFile.Close();
				sourceFile.Close();
				if(!m_bExitThread)
					::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitUserForce, nCompleted);
				return;
			}

			// ���������Ķ�д����
			dwRead = sourceFile.Read(buff, c_page);
			destFile.Write(buff, dwRead);
			uWriteBytes += dwRead;
		}while(dwRead > 0 && uWriteBytes < m_uFileSize);

		// �ر����Ŀ���ļ�
		destFile.Close();

		// ֪ͨ�û�����ǰ��״̬��Ϣ
		nCompleted = nPreCount++;
		::PostMessage(m_hWndNotify, WM_CUTTERSTATUS, 0, nCompleted);
	}while(dwRead > 0);

	// �ر�Դ�ļ�
	sourceFile.Close();

	// ֪ͨ�û����������
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
	
	// ȡ��ԴĿ¼�д��ϲ����ļ����ļ����ƺ�����
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
		// ֪ͨ�û���Դ�ļ�����
		::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSourceErr, nCompleted);
		return;
	}

	// ȡ���ļ�����
	strFileName = strFileName.Mid(strFileName.Find("__") + 2);

	// ȷ��Ŀ��Ŀ¼���ڣ���㴴�����ǣ�
	int nPos = 0;
	while((nPos = strDestFile.Find('\\', nPos+1)) != -1)
	{
		::CreateDirectory(strDestFile.Left(nPos + 1), NULL);
	}
	::CreateDirectory(strDestFile, NULL);
	
	// ����Ŀ���ļ�
	CFile sourceFile, destFile;
	strDestFile += strFileName;
	if(!destFile.Open(strDestFile, CFile::modeRead|CFile::modeWrite|CFile::modeCreate))
	{
		::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitDestErr, nCompleted);
		return;
	}

	// ֪ͨ�û�����ʼ�ָ��ļ�
	::PostMessage(m_hWndNotify, WM_CUTTERSTART, nTotalFiles, nCompleted);	

	// ��ʼȥ��Դ�ļ���������д��Ŀ���ļ�
	const int c_page = 4*1024;
	char buff[c_page];
	int nPreCount = 1;
	CString sSourceName;
	DWORD dwRead;
	do
	{
		// ��һ��Դ�ļ�
		sSourceName.Format("%d__", nPreCount);
		sSourceName += strFileName;
		if(!sourceFile.Open(strSourceDir + sSourceName, CFile::modeRead|CFile::shareDenyWrite))
		{
			break;
		}

		// �����Դ�ļ��е�����ȫ��д��Ŀ���ļ�
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

		// ֪ͨ�û�����ǰ��״̬��Ϣ
		nCompleted = nPreCount++;
		::PostMessage(m_hWndNotify, WM_CUTTERSTATUS, 0, nCompleted);
	}
	while(TRUE);

	// ֪ͨ�û����������
	::PostMessage(m_hWndNotify, WM_CUTTERSTOP, exitSuccess, nCompleted);
}