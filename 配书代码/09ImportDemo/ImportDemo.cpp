//////////////////////////////////////////////////
// ImportDemo.cpp�ļ�


#include <windows.h>

// ��������ԭ��
typedef void (*PFNEXPORTFUNC)(LPCTSTR);

int main(int argc, char* argv[])
{
        // ����DLL��
        HMODULE hModule = ::LoadLibrary("..\\09DllDemo\\Debug\\09DllDemo.dll");
        if(hModule != NULL)
	{ 
		// ȡ��ExportFunc�����ĵ�ַ
		PFNEXPORTFUNC mExportFunc = (PFNEXPORTFUNC)::GetProcAddress(hModule, "ExportFunc");
		if(mExportFunc != NULL)
		{
			mExportFunc("��Һã�");
		}

		// ж��DLL��
		::FreeLibrary (hModule);	
	}
	
        return 0;
}





/*

#include <windows.h>
#include "09DllDemo.h"

// ָ��Ҫ���ӵ�09DllDemo.lib��
#pragma comment(lib, "09DllDemo")

void main()
{
	// ����ñ��غ���һ������09DllDemo.dll��ĵ�������
	ExportFunc("��Һã�");
}

*/