///////////////////////////////////////////
// SelfRun.cpp文件

#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])  // 09 SelfRun工程下
{
	// 根键、子键名称、和到子键的句柄
	HKEY hRoot = HKEY_LOCAL_MACHINE;
        char *szSubKey = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;

	// 打开指定子键
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;	// 如果不存在不创建
	LONG lRet = ::RegCreateKeyEx(hRoot, szSubKey, 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if(lRet != ERROR_SUCCESS)
		return -1;

	// 得到当前执行文件的文件名（包含路径）
        char szModule[MAX_PATH] ;
        ::GetModuleFileName (NULL, szModule, MAX_PATH);
	// 创建一个新的键值，设置键值数据为文件名
	lRet = ::RegSetValueEx(hKey, "SelfRunDemo", 0, REG_SZ, (BYTE*)szModule, strlen(szModule));
	if(lRet == ERROR_SUCCESS)
	{
		printf(" 自动启动设置成功！\n");
	}

	// 关闭子键句柄
	::RegCloseKey(hKey);

	getchar();
	return 0;
}