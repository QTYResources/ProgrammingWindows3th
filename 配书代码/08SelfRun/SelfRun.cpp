///////////////////////////////////////////
// SelfRun.cpp�ļ�

#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])  // 09 SelfRun������
{
	// �������Ӽ����ơ��͵��Ӽ��ľ��
	HKEY hRoot = HKEY_LOCAL_MACHINE;
        char *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;

	// ��ָ���Ӽ�
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;	// ��������ڲ�����
	LONG lRet = ::RegCreateKeyEx(hRoot, szSubKey, 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if(lRet != ERROR_SUCCESS)
		return -1;

	// �õ���ǰִ���ļ����ļ���������·����
        char szModule[MAX_PATH] ;
        ::GetModuleFileName (NULL, szModule, MAX_PATH);
	// ����һ���µļ�ֵ�����ü�ֵ����Ϊ�ļ���
	lRet = ::RegSetValueEx(hKey, "SelfRunDemo", 0, REG_SZ, (BYTE*)szModule, strlen(szModule));
	if(lRet == ERROR_SUCCESS)
	{
		printf(" �Զ��������óɹ���\n");
	}

	// �ر��Ӽ����
	::RegCloseKey(hKey);

	getchar();
	return 0;
}