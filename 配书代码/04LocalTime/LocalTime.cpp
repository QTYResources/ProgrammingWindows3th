///////////////////////////////////////////////////////////////
// LocalTime.cpp�ļ�


#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
	SYSTEMTIME time;
	::GetLocalTime(&time);
	printf(" ��ǰʱ��Ϊ��%.2d:%.2d:%.2d \n", time.wHour, time.wMinute, time.wSecond);

	return 0;
}

