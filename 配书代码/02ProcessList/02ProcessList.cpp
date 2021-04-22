///////////////////////////////////////////////////////////////
// 02ProcessList.cpp文件


#include "stdafx.h"
#include <windows.h>
#include <tlhelp32.h> // 声明快照函数的头文件

int main(int argc, char* argv[])
{
	PROCESSENTRY32 pe32;
	// 在使用这个结构之前，先设置它的大小
	pe32.dwSize = sizeof(pe32); 
	
	// 给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf(" CreateToolhelp32Snapshot调用失败！ \n");
		return -1;
	}
	
	// 遍历进程快照，轮流显示每个进程的信息
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while(bMore)
	{
		printf(" 进程名称：%s \n", pe32.szExeFile);
		printf(" 进程ID号：%u \n\n", pe32.th32ProcessID);

		bMore = ::Process32Next(hProcessSnap, &pe32);
	}

	// 不要忘记清除掉snapshot对象
	::CloseHandle(hProcessSnap);
	return 0;
}

