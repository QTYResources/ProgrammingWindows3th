///////////////////////////////////////////////////////////////
// FileFind文件


#include <stdio.h>
#include <windows.h>


int main(int argc, char* argv[])
{
	char szFileName[] = "D:\\Program Files\\*.*";
	WIN32_FIND_DATA findData;
	HANDLE hFindFile;

	hFindFile = ::FindFirstFile(szFileName, &findData);
	if(hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			// 名称为“.”的目录代表本目录，名称为“..”的目录代表上一层目录
			// 我们这里不想要程序打印出这些符号
			if(findData.cFileName[0] == '.')
				continue;

			// 如果是目录的话就打印出来。注意，你应该对它们做“&”操作
			if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				printf(" %s \n", findData.cFileName);

		}while(::FindNextFile(hFindFile, &findData));
		::FindClose(hFindFile);
	}

	return 0;
}

/*

	hFindFile = ::FindFirstFile(lpFileName, lpFindData);
	if(hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
		//...	// 处理本次找到的文件
		}
		while(::FindNextFile(hFindFile, lpFindData));
		::FindClose(hFindFile);
	}


	lpFileName = "C:\\Windows\\*.*"; // 在C:\Windows目录中查找所有文件
	lpFileName = "C:\\Windows\\System32\\*.exe"; // 在C:\Windows\System32中查找所有exe文件
	lpFileName = "C:\\boot.ini"; // 在C:\目录中查找boot.ini文件



  */