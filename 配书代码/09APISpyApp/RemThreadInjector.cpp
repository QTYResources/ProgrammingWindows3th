////////////////////////////////////////////////
// RemThreadInjector.cpp文件

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
	// 附给本进程特权，以便访问系统进程
	BOOL bOk = FALSE; 
	HANDLE hToken;
	
	// 打开一个进程的访问令牌
	if(::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) 
	{
		// 取得特权名称为“SetDebugPrivilege”的LUID
		LUID uID;
		::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &uID);

		// 调整特权级别
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = uID;
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (::GetLastError() == ERROR_SUCCESS);

		// 关闭访问令牌句柄
		::CloseHandle(hToken);
	}
	return bOk;
}

BOOL CRemThreadInjector::InjectModuleInto(DWORD dwProcessId)
{
	if(::GetCurrentProcessId() == dwProcessId)
		return FALSE;

	// 首先查看目标进程是否加载了这个模块
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

	// 如果能够找到，就不重复加载了（因为重复加载没有用，Windows只将使用计数加1，其它什么也不做）
	if(bFound)
		return FALSE;


	// 试图打开目标进程
	HANDLE hProcess = ::OpenProcess(
		PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, FALSE, dwProcessId);
	if(hProcess == NULL)
		return FALSE;


	// 在目标进程中申请空间，存放字符串pszDllName，作为远程线程的参数
	int cbSize = (strlen(m_szDllName) + 1);
	LPVOID lpRemoteDllName = ::VirtualAllocEx(hProcess, NULL, cbSize, MEM_COMMIT, PAGE_READWRITE);
	::WriteProcessMemory(hProcess, lpRemoteDllName, m_szDllName, cbSize, NULL);

	// 取得LoadLibraryA函数的地址，我们将以它作为远程线程函数启动
	HMODULE hModule=::GetModuleHandle("kernel32.dll");
	LPTHREAD_START_ROUTINE pfnStartRoutine = 
			(LPTHREAD_START_ROUTINE)::GetProcAddress(hModule, "LoadLibraryA");
	

	// 启动远程线程
	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, pfnStartRoutine, lpRemoteDllName, 0, NULL);
	if(hRemoteThread == NULL)
	{
		::CloseHandle(hProcess);
		return FALSE;
	}
	// 等待目标线程运行结束，即LoadLibraryA函数返回
	::WaitForSingleObject(hRemoteThread, INFINITE);
	
	::CloseHandle(hRemoteThread);
	::CloseHandle(hProcess);
	return TRUE;
}

BOOL CRemThreadInjector::EjectModuleFrom(DWORD dwProcessId)
{
	if(::GetCurrentProcessId() == dwProcessId)
		return FALSE;

	// 首先查看目标进程是否加载了这个模块
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

	// 如果找不到就返回出错处理
	if(!bFound)
		return FALSE;

	// 试图打开目标进程
	HANDLE hProcess = ::OpenProcess(
		PROCESS_VM_WRITE|PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION, FALSE, dwProcessId);
	if(hProcess == NULL)
		return FALSE;


	// 取得LoadLibraryA函数的地址，我们将以它作为远程线程函数启动
	HMODULE hModule=::GetModuleHandle("kernel32.dll");
	LPTHREAD_START_ROUTINE pfnStartRoutine = 
			(LPTHREAD_START_ROUTINE)::GetProcAddress(hModule, "FreeLibrary");
	

	// 启动远程线程
	HANDLE hRemoteThread = ::CreateRemoteThread(hProcess, NULL, 0, pfnStartRoutine, me32.hModule, 0, NULL);
	if(hRemoteThread == NULL)
	{
		::CloseHandle(hProcess);
		return FALSE;
	}
	// 等待目标线程运行结束，即FreeLibrary函数返回
	::WaitForSingleObject(hRemoteThread, INFINITE);
	
	::CloseHandle(hRemoteThread);
	::CloseHandle(hProcess);
	return TRUE;
}