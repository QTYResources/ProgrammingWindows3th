//////////////////////////////////////////////////////////
// ULHook.h

#ifndef __ULHOOK_H__
#define __ULHOOK_H__

#include <windows.h>

class CULHook
{
public:
	CULHook(LPSTR pszModName, LPSTR pszFuncName, PROC pfnHook);
	~CULHook();
	// ȡ���ҹ�
	void Unhook();
	// ���¹ҹ�
	void Rehook();
protected:
	PROC m_pfnOrig;                 // Ŀ��API�����ĵ�ַ
	BYTE m_btNewBytes[8];		// �¹�����8���ֽ�
	BYTE m_btOldBytes[8];		// ԭ��8���ֽ�
	HMODULE m_hModule;
};

#endif // __ULHOOK_H__

