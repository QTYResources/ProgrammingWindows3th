#include <stdio.h>
#include <Windows.h>	// 包含 MessageBox 函数声明的头文件

int main(int argc, char* argv[])
{
	// 调用 API 函数 MessageBox
	int nSelect = MessageBox(NULL, L"Hell, Windows XP", L"Greetings", MB_OKCANCEL);
	if (nSelect == IDOK)
	{
		printf("用户选择了“确定”按钮\n");
	}
	else {
		printf("用户选择了“取消”按钮\n");
	}
	return 0;
}