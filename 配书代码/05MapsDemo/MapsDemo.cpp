///////////////////////////////////////////////////////////////
// MapsDemo.cpp�ļ�



#include <iostream.h>
#include "../common/_afxcoll.h"  // ������CPlex�ṹ

void main()	// English-Chinese �ֵ�
{
	CMapPtrToPtr map;
	char szDay[][16] =
	{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

	// ��ӳ���������
	map[szDay[0]]		= "������";	// ������Ҫ������operator [ ]����
	map[szDay[1]]		= "����һ";
	map[szDay[2]]		= "���ڶ�";
	map[szDay[3]]		= "������";
	map[szDay[4]]		= "������";
	map[szDay[5]]		= "������";
	map[szDay[6]]		= "������";

	// ��ѯ
	cout << szDay[4] << " : " << (char*)map[szDay[4]] << "\n";
}