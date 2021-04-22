///////////////////////////////////////////////////////////////
// MyFinder.cpp�ļ�

#include <stdio.h>
#include "RapidFinder.h"

UINT FinderEntry(LPVOID lpParam)
{
	CRapidFinder* pFinder = (CRapidFinder*)lpParam;
	CDirectoryNode* pNode = NULL;	// ��m_listDir��ȡ���Ľڵ�
	BOOL bActive = TRUE;		// ָʾ��ǰ�̵߳�״̬

	// ѭ������m_listDir�б��е�Ŀ¼
	while(1) 
	{
		// ��m_listDir�б���ȡ��һ���µ�Ŀ¼
	 	::EnterCriticalSection(&pFinder->m_cs);
		if(pFinder->m_listDir.IsEmpty())
		{
			bActive = FALSE;
		}
		else
		{
			pNode = pFinder->m_listDir.GetHead();
			pFinder->m_listDir.Remove(pNode);
		}
		::LeaveCriticalSection(&pFinder->m_cs);

		// m_listDirΪ�յĻ�����ͼ��m_hDirEvent�¼��ϵȴ�
		if(!bActive)
		{
			// ׼������ȴ�״̬
			::EnterCriticalSection(&pFinder->m_cs);
			pFinder->m_nThreadCount--;
			if(pFinder->m_nThreadCount == 0)  // �鿴�Ƿ��Ѿ��������
			{
				::LeaveCriticalSection(&pFinder->m_cs);
				break;
			}
			::LeaveCriticalSection(&pFinder->m_cs);

			// ����ȴ�״̬
			ResetEvent(pFinder->m_hDirEvent);
			::WaitForSingleObject(pFinder->m_hDirEvent, INFINITE);

			// ��ɻ�̺߳������һ��ѭ��
			::EnterCriticalSection(&pFinder->m_cs);
			pFinder->m_nThreadCount++;
			::LeaveCriticalSection(&pFinder->m_cs);
			bActive = TRUE;
			continue;
		}
			
		
		// ��pNodeָ���Ŀ¼�в����ļ�

		WIN32_FIND_DATA fileData;
		HANDLE hFindFile;
		// ���ó�X:\XXXX\*.*�ĸ�ʽ
		if(pNode->szDir[strlen(pNode->szDir)-1] != '\\')
			strcat(pNode->szDir, "\\");
		strcat(pNode->szDir, "*.*");
		hFindFile = ::FindFirstFile(pNode->szDir, &fileData);
		if(hFindFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(fileData.cFileName[0] == '.')
					continue;
				if(fileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
				{
					// ����������Ŀ¼��ӵ�Ŀ¼�б���

					// Ϊ�µĽڵ������ڴ�ռ䣬����������Ŀ¼����
					CDirectoryNode* p = new CDirectoryNode;
					strncpy(p->szDir, pNode->szDir, strlen(pNode->szDir)-3);
					strcat(p->szDir, fileData.cFileName);
					// ��ӵ��б���
					::EnterCriticalSection(&pFinder->m_cs);
					pFinder->m_listDir.AddHead(p);
					::LeaveCriticalSection(&pFinder->m_cs);
					// ��ʹһ�����ǻ�̡߳���ɡ���̡߳�
					::SetEvent(pFinder->m_hDirEvent);	
				}
				else
				{
					// ������������ļ�
					if(pFinder->CheckFile(fileData.cFileName))
					{
						::EnterCriticalSection(&pFinder->m_cs);
						::InterlockedIncrement((long*)&pFinder->m_nResultCount);
						::LeaveCriticalSection(&pFinder->m_cs);
						printf("  %s \n", fileData.cFileName);
					}		
				}
			}while(::FindNextFile(hFindFile, &fileData));
		}

		// �˽ڵ㱣���Ŀ¼�Ѿ�������ϣ��ͷ��ڴ�ռ䣬�����´�ѭ��
		delete pNode;
		pNode = NULL;
	}

	// ��ʹһ�������̴߳�WaitForSingleObject�������أ����˳�ѭ��
	::SetEvent(pFinder->m_hDirEvent);

	// �жϴ��߳��Ƿ������һ������ѭ�����̣߳�����Ǿ�֪ͨ���߳�
	if(::WaitForSingleObject(pFinder->m_hDirEvent, 0) != WAIT_TIMEOUT)
	// �����ʱpFinder->m_hDirEvent����Ӧ���¼�����Ϊ����״̬��
	// WaitForSingleObject�����ķ���ֵ����WAIT_OBJECT_0
	{
		// ֪ͨ���߳����һ�������̼߳����˳����ļ��������
		::SetEvent(pFinder->m_hExitEvent);
	
	}
	return 0;
}

void main()
{
	CRapidFinder* pFinder = new CRapidFinder(64);
	CDirectoryNode* pNode = new CDirectoryNode;

	// ����ֻ��Ϊ�˲��Բ���������
	char szPath[] = "C:\\";
	char szFile[] = "stdafx";

	// ���ò�����Ϣ
	strcpy(pNode->szDir, szPath);
	pFinder->m_listDir.AddHead(pNode);
	strcpy(pFinder->m_szMatchName, szFile);

	// ���������̲߳��ȴ����ҽ���
	pFinder->m_nThreadCount = pFinder->m_nMaxThread;
	for(int i=0; i<pFinder->m_nMaxThread; i++)
	{
		AfxBeginThread(FinderEntry, pFinder);
	}
	::WaitForSingleObject(pFinder->m_hExitEvent, INFINITE);

	// ��ӡ�����
	printf(" ���ղ��ҵ����ļ��ĸ���Ϊ��%d \n", pFinder->m_nResultCount);

	delete pFinder;
}



