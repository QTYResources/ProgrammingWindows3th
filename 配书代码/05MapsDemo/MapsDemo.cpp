///////////////////////////////////////////////////////////////
// MapsDemo.cpp文件



#include <iostream.h>
#include "../common/_afxcoll.h"  // 定义了CPlex结构

void main()	// English-Chinese 字典
{
	CMapPtrToPtr map;
	char szDay[][16] =
	{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

	// 向映射中添加项
	map[szDay[0]]		= "星期日";	// 这里主要调用了operator [ ]函数
	map[szDay[1]]		= "星期一";
	map[szDay[2]]		= "星期二";
	map[szDay[3]]		= "星期三";
	map[szDay[4]]		= "星期四";
	map[szDay[5]]		= "星期五";
	map[szDay[6]]		= "星期六";

	// 查询
	cout << szDay[4] << " : " << (char*)map[szDay[4]] << "\n";
}