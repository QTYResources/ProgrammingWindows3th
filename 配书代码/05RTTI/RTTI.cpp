///////////////////////////////////////////////////////////////
// RIIT.cpp�ļ�


#include <iostream.h>

class CBoy
{
public:
	// ...	// ������Ա
public:
	const int* GetRuntimeClass() { return &classCBoy; }
	static const int classCBoy;	// classCBoy��Ա���ڴ��ַ��CBoy���Ψһ��ʶ
};
const int CBoy::classCBoy = 1;		// ����ʼ��һ��ֵ������

class CGirl
{
public:
	// ...	// ������Ա
public:
	const int* GetRuntimeClass() { return &classCGirl; }
	static const int classCGirl;	// classCGirl��Ա���ڴ��ַ��CGirl���Ψһ��ʶ
};
const int CGirl::classCGirl = 1;

void main()
{
	CBoy student;
	if(student.GetRuntimeClass() == &CGirl::classCGirl) // �þ�̬��Ա�ĵ�ַ���student�����Ƿ�����CGirl��
		cout << " a girl \n";
	else if(student.GetRuntimeClass() == &CBoy::classCBoy)
		cout << " a boy \n";
	else
		cout << " unknown \n";
}
