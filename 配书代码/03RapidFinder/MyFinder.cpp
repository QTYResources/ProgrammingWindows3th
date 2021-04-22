///////////////////////////////////////////////////////////////
// MyFinder.cpp文件

#include <stdio.h>
#include "RapidFinder.h"

UINT FinderEntry(LPVOID lpParam)
{
	CRapidFinder* pFinder = (CRapidFinder*)lpParam;
	CDirectoryNode* pNode = NULL;	// 从m_listDir中取出的节点
	BOOL bActive = TRUE;		// 指示当前线程的状态

	// 循环处理m_listDir列表中的目录
	while(1) 
	{
		// 从m_listDir列表中取出一个新的目录
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

		// m_listDir为空的话就试图在m_hDirEvent事件上等待
		if(!bActive)
		{
			// 准备进入等待状态
			::EnterCriticalSection(&pFinder->m_cs);
			pFinder->m_nThreadCount--;
			if(pFinder->m_nThreadCount == 0)  // 查看是否已经查找完毕
			{
				::LeaveCriticalSection(&pFinder->m_cs);
				break;
			}
			::LeaveCriticalSection(&pFinder->m_cs);

			// 进入等待状态
			ResetEvent(pFinder->m_hDirEvent);
			::WaitForSingleObject(pFinder->m_hDirEvent, INFINITE);

			// 变成活动线程后进入下一次循环
			::EnterCriticalSection(&pFinder->m_cs);
			pFinder->m_nThreadCount++;
			::LeaveCriticalSection(&pFinder->m_cs);
			bActive = TRUE;
			continue;
		}
			
		
		// 在pNode指向的目录中查找文件

		WIN32_FIND_DATA fileData;
		HANDLE hFindFile;
		// 设置成X:\XXXX\*.*的格式
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
					// 将搜索到的目录添加到目录列表中

					// 为新的节点申请内存空间，设置完整的目录名称
					CDirectoryNode* p = new CDirectoryNode;
					strncpy(p->szDir, pNode->szDir, strlen(pNode->szDir)-3);
					strcat(p->szDir, fileData.cFileName);
					// 添加到列表中
					::EnterCriticalSection(&pFinder->m_cs);
					pFinder->m_listDir.AddHead(p);
					::LeaveCriticalSection(&pFinder->m_cs);
					// 促使一个“非活动线程”变成“活动线程”
					::SetEvent(pFinder->m_hDirEvent);	
				}
				else
				{
					// 检查搜索到的文件
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

		// 此节点保存的目录已经搜索完毕，释放内存空间，进入下次循环
		delete pNode;
		pNode = NULL;
	}

	// 促使一个搜索线程从WaitForSingleObject函数返回，并退出循环
	::SetEvent(pFinder->m_hDirEvent);

	// 判断此线程是否是最后一个结束循环的线程，如果是就通知主线程
	if(::WaitForSingleObject(pFinder->m_hDirEvent, 0) != WAIT_TIMEOUT)
	// 如果此时pFinder->m_hDirEvent所对应的事件对象为受信状态，
	// WaitForSingleObject函数的返回值将是WAIT_OBJECT_0
	{
		// 通知主线程最后一个搜索线程即将退出，文件搜索完毕
		::SetEvent(pFinder->m_hExitEvent);
	
	}
	return 0;
}

void main()
{
	CRapidFinder* pFinder = new CRapidFinder(64);
	CDirectoryNode* pNode = new CDirectoryNode;

	// 我们只是为了测试才这样做，
	char szPath[] = "C:\\";
	char szFile[] = "stdafx";

	// 设置参数信息
	strcpy(pNode->szDir, szPath);
	pFinder->m_listDir.AddHead(pNode);
	strcpy(pFinder->m_szMatchName, szFile);

	// 创建辅助线程并等待查找结束
	pFinder->m_nThreadCount = pFinder->m_nMaxThread;
	for(int i=0; i<pFinder->m_nMaxThread; i++)
	{
		AfxBeginThread(FinderEntry, pFinder);
	}
	::WaitForSingleObject(pFinder->m_hExitEvent, INFINITE);

	// 打印出结果
	printf(" 最终查找到的文件的个数为：%d \n", pFinder->m_nResultCount);

	delete pFinder;
}



