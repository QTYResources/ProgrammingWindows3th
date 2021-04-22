///////////////////////////////////////
// ValidPE.cpp�ļ�


#include <afxdlgs.h>

#include "ValidPE.h"

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	// ����ѡɫ�ļ��Ի���
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() != IDOK)
		return FALSE;


	// �򿪼����ļ�
	HANDLE hFile = ::CreateFile(dlg.GetFileName(), GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "��Ч�ļ���", "ValidPE", MB_OK);
	}

	// ����PE�ļ��е�DOSͷ��NTͷ
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS32 ntHeader;	
	
	// ��֤����
	BOOL bValid = FALSE;
	DWORD dwRead;
	// ��ȡDOSͷ
	::ReadFile(hFile, &dosHeader, sizeof(dosHeader), &dwRead, NULL);
	if(dwRead == sizeof(dosHeader))
	{
		if(dosHeader.e_magic == IMAGE_DOS_SIGNATURE) // �ǲ�����Ч��DOSͷ��
		{
			// ��λNTͷ
			if(::SetFilePointer(hFile, dosHeader.e_lfanew, NULL, FILE_BEGIN) != -1)
			{
				// ��ȡNTͷ
				::ReadFile(hFile, &ntHeader, sizeof(ntHeader), &dwRead, NULL);
				if(dwRead == sizeof(ntHeader))
				{
					if(ntHeader.Signature == IMAGE_NT_SIGNATURE)	// �ǲ�����Ч��NTͷ
						bValid = TRUE;
				}
			}
		}
	}

	// ��ʾ���
	if(bValid)
		MessageBox(NULL, "��һ��PE��ʽ���ļ���", "ValidPE", MB_OK);
	else
		MessageBox(NULL, "����һ��PE��ʽ���ļ���", "ValidPE", MB_OK);

	::CloseHandle(hFile);
	return FALSE;
}
