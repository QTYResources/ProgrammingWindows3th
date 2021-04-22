//////////////////////////////////
// APISpyLib.h文件

#ifndef __APISPYLIB_H__
#define __APISPYLIB_H__

#include "ShareMemory.h"

// 共享内存名称
#define SHAREMEM "APISpyLib"

// 发送给主程序的通知消息
#define HM_SPYACALL WM_USER + 102

struct CAPISpyData
{
	char szModName[256];
	char szFuncName[256];
	HWND hWndCaller;
};

class CMyShareMem
{
public:
	CMyShareMem(BOOL bServer = FALSE)
	{
		m_pMem = new CShareMemory("APISpyLib", sizeof(CAPISpyData), bServer);
		m_pData = (CAPISpyData*)(m_pMem->GetBuffer());
		if(m_pData == NULL) // 没有设置共享内存？
			::ExitProcess(-1);
	}
	~CMyShareMem() { delete m_pMem; }
	// 取得共享内存中的数据
	CAPISpyData* GetData() { return m_pData; }
private:
	CAPISpyData* m_pData;
	CShareMemory* m_pMem;
};

#endif // __APISPYLIB_H__