///////////////////////////////////////
// ValidPE.cpp文件


#include <afxdlgs.h>

#include "ValidPE.h"

CMyApp theApp;

BOOL CMyApp::InitInstance()
{
	// 弹出选色文件对话框
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() != IDOK)
		return FALSE;


	// 打开检查的文件
	HANDLE hFile = ::CreateFile(dlg.GetFileName(), GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "无效文件！", "ValidPE", MB_OK);
	}

	// 定义PE文件中的DOS头和NT头
	IMAGE_DOS_HEADER dosHeader;
	IMAGE_NT_HEADERS32 ntHeader;	
	
	// 验证过程
	BOOL bValid = FALSE;
	DWORD dwRead;
	// 读取DOS头
	::ReadFile(hFile, &dosHeader, sizeof(dosHeader), &dwRead, NULL);
	if(dwRead == sizeof(dosHeader))
	{
		if(dosHeader.e_magic == IMAGE_DOS_SIGNATURE) // 是不是有效的DOS头？
		{
			// 定位NT头
			if(::SetFilePointer(hFile, dosHeader.e_lfanew, NULL, FILE_BEGIN) != -1)
			{
				// 读取NT头
				::ReadFile(hFile, &ntHeader, sizeof(ntHeader), &dwRead, NULL);
				if(dwRead == sizeof(ntHeader))
				{
					if(ntHeader.Signature == IMAGE_NT_SIGNATURE)	// 是不是有效的NT头
						bValid = TRUE;
				}
			}
		}
	}

	// 显示结果
	if(bValid)
		MessageBox(NULL, "是一个PE格式的文件！", "ValidPE", MB_OK);
	else
		MessageBox(NULL, "不是一个PE格式的文件！", "ValidPE", MB_OK);

	::CloseHandle(hFile);
	return FALSE;
}
