/////////////////////////////////////////////
// APISpyLib.cpp�ļ�

#include <windows.h>
#include "APIHook.h"
#include "APISpyLib.h"

void HookProc();

// �����ڴ����ݣ��Ա��ʼ�������CAPIHook����
CMyShareMem g_shareData(FALSE);

// HOOK������ָ����API����
CAPIHook g_orgFun(g_shareData.GetData()->szModName, 
		       g_shareData.GetData()->szFuncName, (PROC)HookProc);

void NotifyCaller()
{
	CMyShareMem mem(FALSE);
	::SendMessage(mem.GetData()->hWndCaller, HM_SPYACALL, 0, 0);
}

// ʹ������������ҪHOOK��API
__declspec(naked)void HookProc()
{
	// ֪ͨ������
	NotifyCaller();

	// ��ת��ԭ���ĺ���
	DWORD dwOrgAddr;
	dwOrgAddr = (DWORD)PROC(g_orgFun);
	__asm
	{
		mov eax, dwOrgAddr;
		jmp eax;
	}
	// ��Զ���в�������
}
