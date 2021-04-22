//////////////////////////////////
// APISpyLib.h�ļ�

#ifndef __APISPYLIB_H__
#define __APISPYLIB_H__

#include "ShareMemory.h"

// �����ڴ�����
#define SHAREMEM "APISpyLib"

// ���͸��������֪ͨ��Ϣ
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
		if(m_pData == NULL) // û�����ù����ڴ棿
			::ExitProcess(-1);
	}
	~CMyShareMem() { delete m_pMem; }
	// ȡ�ù����ڴ��е�����
	CAPISpyData* GetData() { return m_pData; }
private:
	CAPISpyData* m_pData;
	CShareMemory* m_pMem;
};

#endif // __APISPYLIB_H__