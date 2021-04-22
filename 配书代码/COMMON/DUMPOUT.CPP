///////////////////////////////////////////////////////
// DUMPOUT.CPP文件

#include <windows.h>
#include <stdio.h> // _vsnprintf function

#ifdef _DEBUG	// 只有在调试模式下才编译此文件

void __cdecl AfxTrace(LPCTSTR lpszFormat, ...)
{
	// 格式化我们得到的数据
	va_list args;
	va_start(args, lpszFormat);

	char szBuffer[512];
	_vsnprintf(szBuffer, 512, lpszFormat, args);

	// 输出到调试器
	::OutputDebugString(szBuffer);

	va_end(args);
}

#endif // _DEBUG