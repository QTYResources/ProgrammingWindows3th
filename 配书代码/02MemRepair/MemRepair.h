

#ifndef __MEMFINDER_H__
#define __MEMFINDER_H__

#include <windows.h>

class CMemFinder
{
public:
	CMemFinder(DWORD dwProcessId);
	virtual ~CMemFinder();

// 属性
public:
	BOOL IsFirst() const { return m_bFirst; }
	BOOL IsValid() const { return m_hProcess != NULL; }
	int GetListCount() const { return m_nListCnt; }
	DWORD operator [](int nIndex) { return m_arList[nIndex]; }

// 操作
	virtual BOOL FindFirst(DWORD dwValue);
	virtual BOOL FindNext(DWORD dwValue);
	virtual BOOL WriteMemory(DWORD dwAddr, DWORD dwValue);

// 实现
protected:
	virtual BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);

	DWORD m_arList[1024];	// 地址列表
	int m_nListCnt;		// 有效地址的个数
	HANDLE m_hProcess;	// 目标进程句柄
	BOOL m_bFirst;		// 是不是第一次搜索
};


CMemFinder::CMemFinder(DWORD dwProcessId)
{
	m_nListCnt = 0;
	m_bFirst = TRUE;
	m_hProcess = ::OpenProcess(PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_VM_OPERATION, FALSE, dwProcessId);
}

CMemFinder::~CMemFinder()
{
	if(m_hProcess != NULL)
		::CloseHandle(m_hProcess);
}

BOOL CMemFinder::FindFirst(DWORD dwValue)
{
	const DWORD dwOneGB = 1024*1024*1024;	// 1GB
	const DWORD dwOnePage = 4*1024;		// 4KB

	if(m_hProcess == NULL)
		return FALSE;
	
	// 查看操作系统类型，以决定开始地址
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(vi) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		dwBase = 4*1024*1024;		// Windows 98系列，4MB	
	else
		dwBase = 640*1024;		// Windows NT系列，64KB

	// 在开始地址到2GB的地址空间进行查找
	for(; dwBase < 2*dwOneGB; dwBase += dwOnePage)
	{
		// 比较1页大小的内存
		CompareAPage(dwBase, dwValue);
	}

	m_bFirst = FALSE;

	return TRUE;
}

BOOL CMemFinder::CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	// 读取1页内存
	BYTE arBytes[4096];
	if(!::ReadProcessMemory(m_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL))
		return FALSE;	// 此页不可读

	// 在这1页内存中查找
	DWORD* pdw;
	for(int i=0; i<(int)4*1024-3; i++)
	{
		pdw = (DWORD*)&arBytes[i];
		if(pdw[0] == dwValue)	// 等于要查找的值？
		{
			if(m_nListCnt >= 1024)
				return FALSE;
			// 添加到全局变量中
			m_arList[m_nListCnt++] = dwBaseAddr + i;
		}
	}

	return TRUE;
}

BOOL CMemFinder::FindNext(DWORD dwValue)
{
	// 保存m_arList数组中有效地址的个数，初始化新的m_nListCnt值
	int nOrgCnt = m_nListCnt;
	m_nListCnt = 0;	

	// 在m_arList数组记录的地址处查找
	BOOL bRet = FALSE;	// 假设失败	
	DWORD dwReadValue;
	for(int i=0; i<nOrgCnt; i++)
	{
		if(::ReadProcessMemory(m_hProcess, (LPVOID)m_arList[i], &dwReadValue, sizeof(DWORD), NULL))
		{
			if(dwReadValue == dwValue)
			{
				m_arList[m_nListCnt++] = m_arList[i];
				bRet = TRUE;
			}
		}
	}
	
	return bRet;
}

BOOL CMemFinder::WriteMemory(DWORD dwAddr, DWORD dwValue)
{
	return ::WriteProcessMemory(m_hProcess, (LPVOID)dwAddr, &dwValue, sizeof(DWORD), NULL);
}


#endif // __MEMFINDER_H__