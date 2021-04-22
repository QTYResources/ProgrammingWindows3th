

#ifndef __MEMFINDER_H__
#define __MEMFINDER_H__

#include <windows.h>

class CMemFinder
{
public:
	CMemFinder(DWORD dwProcessId);
	virtual ~CMemFinder();

// ����
public:
	BOOL IsFirst() const { return m_bFirst; }
	BOOL IsValid() const { return m_hProcess != NULL; }
	int GetListCount() const { return m_nListCnt; }
	DWORD operator [](int nIndex) { return m_arList[nIndex]; }

// ����
	virtual BOOL FindFirst(DWORD dwValue);
	virtual BOOL FindNext(DWORD dwValue);
	virtual BOOL WriteMemory(DWORD dwAddr, DWORD dwValue);

// ʵ��
protected:
	virtual BOOL CompareAPage(DWORD dwBaseAddr, DWORD dwValue);

	DWORD m_arList[1024];	// ��ַ�б�
	int m_nListCnt;		// ��Ч��ַ�ĸ���
	HANDLE m_hProcess;	// Ŀ����̾��
	BOOL m_bFirst;		// �ǲ��ǵ�һ������
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
	
	// �鿴����ϵͳ���ͣ��Ծ�����ʼ��ַ
	DWORD dwBase;
	OSVERSIONINFO vi = { sizeof(vi) };
	::GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		dwBase = 4*1024*1024;		// Windows 98ϵ�У�4MB	
	else
		dwBase = 640*1024;		// Windows NTϵ�У�64KB

	// �ڿ�ʼ��ַ��2GB�ĵ�ַ�ռ���в���
	for(; dwBase < 2*dwOneGB; dwBase += dwOnePage)
	{
		// �Ƚ�1ҳ��С���ڴ�
		CompareAPage(dwBase, dwValue);
	}

	m_bFirst = FALSE;

	return TRUE;
}

BOOL CMemFinder::CompareAPage(DWORD dwBaseAddr, DWORD dwValue)
{
	// ��ȡ1ҳ�ڴ�
	BYTE arBytes[4096];
	if(!::ReadProcessMemory(m_hProcess, (LPVOID)dwBaseAddr, arBytes, 4096, NULL))
		return FALSE;	// ��ҳ���ɶ�

	// ����1ҳ�ڴ��в���
	DWORD* pdw;
	for(int i=0; i<(int)4*1024-3; i++)
	{
		pdw = (DWORD*)&arBytes[i];
		if(pdw[0] == dwValue)	// ����Ҫ���ҵ�ֵ��
		{
			if(m_nListCnt >= 1024)
				return FALSE;
			// ��ӵ�ȫ�ֱ�����
			m_arList[m_nListCnt++] = dwBaseAddr + i;
		}
	}

	return TRUE;
}

BOOL CMemFinder::FindNext(DWORD dwValue)
{
	// ����m_arList��������Ч��ַ�ĸ�������ʼ���µ�m_nListCntֵ
	int nOrgCnt = m_nListCnt;
	m_nListCnt = 0;	

	// ��m_arList�����¼�ĵ�ַ������
	BOOL bRet = FALSE;	// ����ʧ��	
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