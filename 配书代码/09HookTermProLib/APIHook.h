/////////////////////////////////////////////////////////
// APIHook.h�ļ�


#ifndef __APIHOOK_H__
#define __APIHOOK_H__

#include <windows.h>

class CAPIHook  
{
public:
	CAPIHook(LPSTR pszModName, 
		LPSTR pszFuncName, PROC pfnHook, BOOL bExcludeAPIHookMod = TRUE);
	virtual ~CAPIHook();
	operator PROC() { return m_pfnOrig; }

// ʵ��
private:
	LPSTR m_pszModName;		// ����ҪHOOK������ģ�������
	LPSTR m_pszFuncName;		// ҪHOOK�ĺ���������
	PROC m_pfnOrig;			// ԭAPI������ַ
	PROC m_pfnHook;			// HOOK�����ĵ�ַ
	BOOL m_bExcludeAPIHookMod;	// �Ƿ�HOOK API��ģ���ų�����

private:
	static void ReplaceIATEntryInAllMods(LPSTR pszExportMod, PROC pfnCurrent, 
				PROC pfnNew, BOOL bExcludeAPIHookMod);
	static void ReplaceIATEntryInOneMod(LPSTR pszExportMod, 
				PROC pfnCurrent, PROC pfnNew, HMODULE hModCaller);


// ����Ĵ��������������ģ�鶯̬����DLL������
private:
	// ������ָ�����������е�CAPIHook��������һ��
	static CAPIHook *sm_pHeader;
	CAPIHook *m_pNext;

private:
	// ��һ���µ�DLL������ʱ�����ô˺���
	static void WINAPI HookNewlyLoadedModule(HMODULE hModule, DWORD dwFlags);

	// �������ٵ�ǰ���̼����µ�DLL
	static HMODULE WINAPI LoadLibraryA(PCSTR  pszModulePath);
	static HMODULE WINAPI LoadLibraryW(PCWSTR pszModulePath);
	static HMODULE WINAPI LoadLibraryExA(PCSTR  pszModulePath, HANDLE hFile, DWORD dwFlags);
	static HMODULE WINAPI LoadLibraryExW(PCWSTR pszModulePath, HANDLE hFile, DWORD dwFlags);
	
	// ���������HOOK��API�������򷵻��û��Զ��庯���ĵ�ַ
	static FARPROC WINAPI GetProcAddress(HMODULE hModule, PCSTR pszProcName);
private:
	// �Զ�����Щ�������йҹ�
	static CAPIHook sm_LoadLibraryA;
	static CAPIHook sm_LoadLibraryW;
	static CAPIHook sm_LoadLibraryExA;
	static CAPIHook sm_LoadLibraryExW;
	static CAPIHook sm_GetProcAddress;
};

#endif // __APIHOOK_H__