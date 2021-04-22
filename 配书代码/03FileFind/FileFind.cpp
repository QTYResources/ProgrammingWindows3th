///////////////////////////////////////////////////////////////
// FileFind�ļ�


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
			// ����Ϊ��.����Ŀ¼����Ŀ¼������Ϊ��..����Ŀ¼������һ��Ŀ¼
			// �������ﲻ��Ҫ�����ӡ����Щ����
			if(findData.cFileName[0] == '.')
				continue;

			// �����Ŀ¼�Ļ��ʹ�ӡ������ע�⣬��Ӧ�ö���������&������
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
		//...	// �������ҵ����ļ�
		}
		while(::FindNextFile(hFindFile, lpFindData));
		::FindClose(hFindFile);
	}


	lpFileName = "C:\\Windows\\*.*"; // ��C:\WindowsĿ¼�в��������ļ�
	lpFileName = "C:\\Windows\\System32\\*.exe"; // ��C:\Windows\System32�в�������exe�ļ�
	lpFileName = "C:\\boot.ini"; // ��C:\Ŀ¼�в���boot.ini�ļ�



  */