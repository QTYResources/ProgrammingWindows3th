
// 09DllDemo.h文件

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MY09DLLDEMO_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MY09DLLDEMO_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.


#ifdef MY09DLLDEMO_EXPORTS
#define MY09DLLDEMO_API __declspec(dllexport)
#else
#define MY09DLLDEMO_API __declspec(dllimport)
#endif

// 声明要导出的函数
MY09DLLDEMO_API void ExportFunc(LPCTSTR pszContent);

