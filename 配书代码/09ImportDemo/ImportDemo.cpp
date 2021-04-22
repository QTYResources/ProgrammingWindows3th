//////////////////////////////////////////////////
// ImportDemo.cpp文件


#include <windows.h>

// 声明函数原形
typedef void (*PFNEXPORTFUNC)(LPCTSTR);

int main(int argc, char* argv[])
{
        // 加载DLL库
        HMODULE hModule = ::LoadLibrary("..\\09DllDemo\\Debug\\09DllDemo.dll");
        if(hModule != NULL)
	{ 
		// 取得ExportFunc函数的地址
		PFNEXPORTFUNC mExportFunc = (PFNEXPORTFUNC)::GetProcAddress(hModule, "ExportFunc");
		if(mExportFunc != NULL)
		{
			mExportFunc("大家好！");
		}

		// 卸载DLL库
		::FreeLibrary (hModule);	
	}
	
        return 0;
}





/*

#include <windows.h>
#include "09DllDemo.h"

// 指明要链接到09DllDemo.lib库
#pragma comment(lib, "09DllDemo")

void main()
{
	// 像调用本地函数一样调用09DllDemo.dll库的导出函数
	ExportFunc("大家好！");
}

*/