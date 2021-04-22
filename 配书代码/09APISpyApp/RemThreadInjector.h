////////////////////////////////////////////////
// RemThreadInjector.h文件


#include <windows.h>

class CRemThreadInjector
{
public:
	CRemThreadInjector(LPCTSTR pszDllName);
	~CRemThreadInjector();

	// 注入DLL到指定的进程空间
	BOOL InjectModuleInto(DWORD dwProcessId);

	// 从指定的进程空间卸载DLL
	BOOL EjectModuleFrom(DWORD dwProcessId);

protected:
	char m_szDllName[MAX_PATH];

	// 调整特权级别
	static BOOL EnableDebugPrivilege(BOOL bEnable);
};






