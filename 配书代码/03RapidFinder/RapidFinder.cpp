///////////////////////////////////////////////////////////////
// RapidFinder.cpp文件


#include "RapidFinder.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// m_nMaxThread成员是一个const类型的变量，必须使用成员初始化列表来初始化它的值
CRapidFinder::CRapidFinder(int nMaxThread) : m_nMaxThread(nMaxThread)
{
	m_nResultCount = 0;
	m_nThreadCount = 0;
	m_szMatchName[0] = '\0';

	m_listDir.Construct(offsetof(CDirectoryNode, pNext));
	m_hDirEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	::InitializeCriticalSection(&m_cs);
}

CRapidFinder::~CRapidFinder()
{
	::CloseHandle(m_hDirEvent);
	::CloseHandle(m_hExitEvent);
	::DeleteCriticalSection(&m_cs);
}

BOOL CRapidFinder::CheckFile(LPCTSTR lpszFileName)
{
	char string[MAX_PATH];
	char strSearch[MAX_PATH];
	strcpy(string, lpszFileName);
	strcpy(strSearch, m_szMatchName);

	// 将字符串string和strSearch中的字符全部转化成大写
	_strupr(string);
	_strupr(strSearch);

	// 找出字符串strSearch在字符串string中第一次出现的位置
	// 如果string中不包含strSearch，strstr函数返回NULL
	if(strstr(string, strSearch) != NULL)
		return TRUE;

	return FALSE;
}
