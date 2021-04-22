///////////////////////////////////////////
// ShareMem.cpp文件


#include <stdio.h>
#include <windows.h>

void main()
{
	char szName[] = "08ShareMem";	// 内存映射对象的名称
	char szData[] = "123456";	// 共享内存中的数据
	LPVOID pBuffer;			// 共享内存指针

	// 首先试图打开一个命名的内存映射文件对象
	HANDLE hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szName);
	if(hMap != NULL)
	{
		// 打开成功，映射对象的一个视图，得到指向共享内存的指针，显示出里面的数据
		pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		printf(" 读出共享内存数据：“%s”\n", (char*)pBuffer);
	}
	else
	{
		// 打开失败，创建之
		hMap = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			strlen(szData) + 1,
			szName);	
		
		// 映射对象的一个视图，得到指向共享内存的指针，设置里面的数据
		pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		strcpy((char*)pBuffer, szData);

		printf(" 写入共享内存数据：“%s”\n", (char*)pBuffer);
	}


	getchar();

	// 解除文件映射，关闭内存映射文件对象句柄
	::UnmapViewOfFile(pBuffer);
	::CloseHandle(hMap);

	return;
}