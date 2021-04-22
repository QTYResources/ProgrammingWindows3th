/////////////////////////////////////////////
// ShareMemory.h�ļ�


#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__

class CShareMemory
{
public:
// ���캯������������
	CShareMemory(const char * pszMapName, int nFileSize = 0, BOOL bServer = FALSE);
	~CShareMemory();
// ����
	LPVOID GetBuffer() const { return  m_pBuffer; }
// ʵ��
private:
	HANDLE	m_hFileMap;
	LPVOID	m_pBuffer;
};

inline CShareMemory::CShareMemory(const char * pszMapName, 
			int nFileSize, BOOL bServer) : m_hFileMap(NULL), m_pBuffer(NULL)
{
	if(bServer)
	{
		// ����һ���ڴ�ӳ���ļ�����
		m_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, 
			NULL, PAGE_READWRITE, 0, nFileSize, pszMapName);
	}
	else
	{
		// ��һ���ڴ�ӳ���ļ�����
		m_hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, pszMapName);
	}
	
	// ӳ�������ڴ棬ȡ�ù����ڴ���׵�ַ
	m_pBuffer = (LPBYTE)MapViewOfFile(
		m_hFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0
		);
}

inline CShareMemory::~CShareMemory()
{
	// ȡ���ļ���ӳ�䣬�ر��ļ�ӳ�������
	UnmapViewOfFile(m_pBuffer);
	CloseHandle(m_hFileMap);
}

#endif // __SHAREMEMORY_H__

/*


�������ݶˣ�
	char szPathName[MAX_PATH] = "";
	::GetModuleFileName(NULL, szPathName, MAX_PATH - 1);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, "HOOK_WAIT_EVENT");
	CShareMemory sm("APP_PATH_NAME", 1024, TRUE);
	LPVOID lpBuffer = sm.GetBuffer();
	strcpy((char*)lpBuffer, szPathName);
	::PostThreadMessage(g_dwThread, UM_START, (WPARAM)hWnd, (LPARAM)0);
	WaitForSingleObject(hEvent, 1000);
 
�������ݶ�
	CShareMemory sm("APP_PATH_NAME");
	LPVOID lpBuffer = sm.GetBuffer();
	XTRACE("APP_PATH_NAME %s\n", lpBuffer);
	strcpy(_szAppModuleName, (char*)lpBuffer);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, "HOOK_WAIT_EVENT");
	SetEvent(hEvent);
 
*/