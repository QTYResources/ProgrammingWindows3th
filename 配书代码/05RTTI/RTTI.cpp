///////////////////////////////////////////////////////////////
// RIIT.cpp文件


#include <iostream.h>

class CBoy
{
public:
	// ...	// 其它成员
public:
	const int* GetRuntimeClass() { return &classCBoy; }
	static const int classCBoy;	// classCBoy成员的内存地址是CBoy类的唯一标识
};
const int CBoy::classCBoy = 1;		// 随便初始化一个值就行了

class CGirl
{
public:
	// ...	// 其它成员
public:
	const int* GetRuntimeClass() { return &classCGirl; }
	static const int classCGirl;	// classCGirl成员的内存地址是CGirl类的唯一标识
};
const int CGirl::classCGirl = 1;

void main()
{
	CBoy student;
	if(student.GetRuntimeClass() == &CGirl::classCGirl) // 用静态成员的地址辨别student对象是否属于CGirl类
		cout << " a girl \n";
	else if(student.GetRuntimeClass() == &CBoy::classCBoy)
		cout << " a boy \n";
	else
		cout << " unknown \n";
}
