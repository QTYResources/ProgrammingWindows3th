/////////////////////////////////////////////
// APISpyLib.cpp文件

#include <windows.h>
#include "APIHook.h"
#include "APISpyLib.h"

void HookProc();

// 共享内存数据，以便初始化下面的CAPIHook对象
CMyShareMem g_shareData(FALSE);

// HOOK主程序指定的API函数
CAPIHook g_orgFun(g_shareData.GetData()->szModName, 
		       g_shareData.GetData()->szFuncName, (PROC)HookProc);

void NotifyCaller()
{
	CMyShareMem mem(FALSE);
	::SendMessage(mem.GetData()->hWndCaller, HM_SPYACALL, 0, 0);
}

// 使用这个函数替代要HOOK的API
__declspec(naked)void HookProc()
{
	// 通知主程序
	NotifyCaller();

	// 跳转到原来的函数
	DWORD dwOrgAddr;
	dwOrgAddr = (DWORD)PROC(g_orgFun);
	__asm
	{
		mov eax, dwOrgAddr;
		jmp eax;
	}
	// 永远运行不到这里
}
