////////////////////////////////////////////////
// RemThreadInjector.cpp�ļ�

#include "RemThreadInjector.h"
#include <tlhelp32.h>

CRemThreadInjector::CRemThreadInjector(LPCTSTR pszDllName)
{
	strncpy(m_szDllName, pszDllName, MAX_PATH);
	EnableDebugPrivilege(TRUE);
}

CRemThreadInjector::~CRemThreadInjector()
{
	EnableDebugPrivilege(FALSE);
}

BOOL CRemThreadInjector::EnableDebugPrivilege(BOOL bEnable) 
{
	// ������������Ȩ���Ա����ϵͳ����
	BOOL bOk = FALSE; 
	HANDLE hToken;
	
	// ��һ�����̵ķ�������
	if(::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) 
	{
		// ȡ����Ȩ����Ϊ��SetDebugPrivilege����LUID
		LUID uID;
		::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &uID);

		// ������Ȩ����
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = uID;
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (::GetLastError() == ERROR_SUCCESS);

		// �رշ������ƾ��
		::CloseHandle(hToken);
	}
	return bOk;
}

BOOL CRemThreadInjector::InjectModuleInto(DWORD dwProcessId)
{
	if(::GetCurrentProcessId() == dwProcessId)
		return FALSE;

	// ���Ȳ鿴Ŀ������Ƿ���������ģ��
	BOOL bFound = FALSE;
	MODULEENTRY32 me32 = { 0 };
	HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	me32.dwSize = sizeof(MODULEENTRY32);
	if(::Module32First(hModuleSnap, &me32))
	{
		do
		{
			if(lstrcmpiA(me32.szExePath, m_szDllName) == 0)
			{
				bFound = TRUE;
				break;
			}
		}
		while(::Module32Next(hModuleSnap, &me32));
	}
	::CloseHandle(hModuleSnap);

	// ����ܹ��ҵ����Ͳ��ظ������ˣ���Ϊ�ظ�����û���ã�Windowsֻ��ʹ�ü�����1������ʲôҲ������
	if(bFound)
		return FALSE;


	// ��ͼ��Ŀ�����
	HANDLE hProcess = ::OpenProcess(
		PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, FALSE, dwProcessId);
	if(hProcess == NULL)
		return FALSE;


	// ��Ŀ�����������ռ䣬����ַ���pszDllName����ΪԶ���̵߳Ĳ���
	int cbSize = (strlen(m_szDllName) + 1);
	LPVOID lpRemoteDllName = ::VirtualAllocEx(hProcess, NULL, cbSize, MEM_COMMIT, PAGE_READWRITE);
	::WriteProcessMemory(hProcess, lpRemoteDllName, m_szDllName, cbSize, NULL);

	// ȡ��LoadLibraryA�����ĵ�ַ�����ǽ�������ΪԶ���̺߳�������
	HMODULE hModule=::GetModuleHandle("kernel32.dll");
	LPTHREAD_START_ROUTINE pfnStartRoutine = 
			(LPTHREAD_START_ROUTINE)::GetProcAddress(hModule, "LoadLibraryA");
	

	// ����Զ���߳�
	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, pfnStartRoutine, lpRemoteDllName, 0, NULL);
	if(hRemoteThread == NULL)
	{
		::CloseHandle(hProcess);
		return FALSE;
	}
	// �ȴ�Ŀ���߳����н�������LoadLibraryA��������
	::WaitForSingleObject(hRemoteThread, INFINITE);
	
	::CloseHandle(hRemoteThread);
	::CloseHandle(hProcess);
	return TRUE;
}

BOOL CRemThreadInjector::EjectModuleFrom(DWORD dwProcessId)
{
	if(::GetCurrentProcessId() == dwProcessId)
		return FALSE;

	// ���Ȳ鿴Ŀ������Ƿ���������ģ��
	BOOL bFound = FALSE;
	MODULEENTRY32 me32 = { 0 };
	HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	me32.dwSize = sizeof(MODULEENTRY32);
	if(::Module32First(hModuleSnap, &me32))
	{
		do
		{
			if(lstrcmpiA(me32.szExePath, m_szDllName) == 0)
			{
				bFound = TRUE;
				break;
			}
		}
		while(::Module32Next(hModuleSnap, &me32));
	}
	::CloseHandle(hModuleSnap);

	// ����Ҳ����ͷ��س�����
	if(!bFound)
		return FALSE;

	// ��ͼ��Ŀ�����
	HANDLE hProcess = ::OpenProcess(
		PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, FALSE, dwProcessId);
	if(hProcess == NULL)
		return FALSE;


	// ȡ��LoadLibraryA�����ĵ�ַ�����ǽ�������ΪԶ���̺߳�������
	HMODULE hModule=::GetModuleHandle("kernel32.dll");
	LPTHREAD_START_ROUTINE pfnStartRoutine = 
			(LPTHREAD_START_ROUTINE)::GetProcAddress(hModule, "FreeLibrary");
	

	// ����Զ���߳�
	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, pfnStartRoutine, me32.hModule, 0, NULL);
	if(hRemoteThread == NULL)
	{
		::CloseHandle(hProcess);
		return FALSE;
	}
	// �ȴ�Ŀ���߳����н�������FreeLibrary��������
	::WaitForSingleObject(hRemoteThread, INFINITE);
	
	::CloseHandle(hRemoteThread);
	::CloseHandle(hProcess);
	return TRUE;
}