////////////////////////////////////////////////
// RemThreadInjector.h�ļ�


#include <windows.h>

class CRemThreadInjector
{
public:
	CRemThreadInjector(LPCTSTR pszDllName);
	~CRemThreadInjector();

	// ע��DLL��ָ���Ľ��̿ռ�
	BOOL InjectModuleInto(DWORD dwProcessId);

	// ��ָ���Ľ��̿ռ�ж��DLL
	BOOL EjectModuleFrom(DWORD dwProcessId);

protected:
	char m_szDllName[MAX_PATH];

	// ������Ȩ����
	static BOOL EnableDebugPrivilege(BOOL bEnable);
};






