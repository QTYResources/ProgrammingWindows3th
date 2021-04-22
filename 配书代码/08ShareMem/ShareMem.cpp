///////////////////////////////////////////
// ShareMem.cpp�ļ�


#include <stdio.h>
#include <windows.h>

void main()
{
	char szName[] = "08ShareMem";	// �ڴ�ӳ����������
	char szData[] = "123456";	// �����ڴ��е�����
	LPVOID pBuffer;			// �����ڴ�ָ��

	// ������ͼ��һ���������ڴ�ӳ���ļ�����
	HANDLE hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szName);
	if(hMap != NULL)
	{
		// �򿪳ɹ���ӳ������һ����ͼ���õ�ָ�����ڴ��ָ�룬��ʾ�����������
		pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		printf(" ���������ڴ����ݣ���%s��\n", (char*)pBuffer);
	}
	else
	{
		// ��ʧ�ܣ�����֮
		hMap = ::CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			strlen(szData) + 1,
			szName);	
		
		// ӳ������һ����ͼ���õ�ָ�����ڴ��ָ�룬�������������
		pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
		strcpy((char*)pBuffer, szData);

		printf(" д�빲���ڴ����ݣ���%s��\n", (char*)pBuffer);
	}


	getchar();

	// ����ļ�ӳ�䣬�ر��ڴ�ӳ���ļ�������
	::UnmapViewOfFile(pBuffer);
	::CloseHandle(hMap);

	return;
}