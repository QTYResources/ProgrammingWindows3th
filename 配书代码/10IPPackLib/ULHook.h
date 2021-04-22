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
	// 取消挂钩
	void Unhook();
	// 重新挂钩
	void Rehook();
protected:
	PROC m_pfnOrig;                 // 目标API函数的地址
	BYTE m_btNewBytes[8];		// 新构建的8个字节
	BYTE m_btOldBytes[8];		// 原来8个字节
	HMODULE m_hModule;
};

#endif // __ULHOOK_H__

