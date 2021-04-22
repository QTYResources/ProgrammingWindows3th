/////////////////////////////////////////////
// ShareMemory.h文件


#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__

class CShareMemory
{
public:
// 构造函数和析构函数
	CShareMemory(const char * pszMapName, int nFileSize = 0, BOOL bServer = FALSE);
	~CShareMemory();
// 属性
	LPVOID GetBuffer() const { return  m_pBuffer; }
// 实现
private:
	HANDLE	m_hFileMap;
	LPVOID	m_pBuffer;
};

inline CShareMemory::CShareMemory(const char * pszMapName, 
			int nFileSize, BOOL bServer) : m_hFileMap(NULL), m_pBuffer(NULL)
{
	if(bServer)
	{
		// 创建一个内存映射文件对象
		m_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, 
			NULL, PAGE_READWRITE, 0, nFileSize, pszMapName);
	}
	else
	{
		// 打开一个内存映射文件对象
		m_hFileMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, pszMapName);
	}
	
	// 映射它到内存，取得共享内存的首地址
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
	// 取消文件的映射，关闭文件映射对象句柄
	UnmapViewOfFile(m_pBuffer);
	CloseHandle(m_hFileMap);
}

#endif // __SHAREMEMORY_H__

/*


发送数据端：
	char szPathName[MAX_PATH] = "";
	::GetModuleFileName(NULL, szPathName, MAX_PATH - 1);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, "HOOK_WAIT_EVENT");
	CShareMemory sm("APP_PATH_NAME", 1024, TRUE);
	LPVOID lpBuffer = sm.GetBuffer();
	strcpy((char*)lpBuffer, szPathName);
	::PostThreadMessage(g_dwThread, UM_START, (WPARAM)hWnd, (LPARAM)0);
	WaitForSingleObject(hEvent, 1000);
 
接收数据端
	CShareMemory sm("APP_PATH_NAME");
	LPVOID lpBuffer = sm.GetBuffer();
	XTRACE("APP_PATH_NAME %s\n", lpBuffer);
	strcpy(_szAppModuleName, (char*)lpBuffer);
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, "HOOK_WAIT_EVENT");
	SetEvent(hEvent);
 
*/