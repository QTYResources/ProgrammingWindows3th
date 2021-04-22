///////////////////////////////////////////////////////////////
// RapidFinder.cpp�ļ�


#include "RapidFinder.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// m_nMaxThread��Ա��һ��const���͵ı���������ʹ�ó�Ա��ʼ���б�����ʼ������ֵ
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

	// ���ַ���string��strSearch�е��ַ�ȫ��ת���ɴ�д
	_strupr(string);
	_strupr(strSearch);

	// �ҳ��ַ���strSearch���ַ���string�е�һ�γ��ֵ�λ��
	// ���string�в�����strSearch��strstr��������NULL
	if(strstr(string, strSearch) != NULL)
		return TRUE;

	return FALSE;
}
