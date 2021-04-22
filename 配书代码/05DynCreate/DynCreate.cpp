///////////////////////////////////////////////////////////////
// DynCreate.cpp文件

#include <iostream.h>
#include "../common/_afx.h"

class CPerson : public CObject
{
public:
	virtual CRuntimeClass* GetRuntimeClass() const
		{ return (CRuntimeClass*)&classCPerson; }

	static const CRuntimeClass classCPerson;

	static CObject* __stdcall CreateObject()
		{ return new CPerson; }
};
const CRuntimeClass CPerson::classCPerson = 
{ "CPerson", sizeof(CPerson), 0xffff, &CPerson::CreateObject/*添加到这里*/, (CRuntimeClass*)&CObject::classCObject, NULL };

void main()
{
	// 取得CPerson类中CRuntimeClass结构记录的信息
	// 在实际应用中，我们一般从磁盘上取得这一信息，
	// 从而在代码中没有给出类名的情况下创建该类的对象
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CPerson);

	// 取得了pRuntimeClass指针，不用知道类的名字就可以创建该类
	CObject* pObject = pRuntimeClass->CreateObject();
	if(pObject != NULL && pObject->IsKindOf(RUNTIME_CLASS(CPerson)))
	{
		cout << "创建成功！\n";
		delete pObject;
	}
}

/*

 // 等价代码：

class CPerson : public CObject
{
	DECLARE_DYNCREATE(CPerson)
};
IMPLEMENT_DYNCREATE(CPerson, CObject)

void main() // main函数里的代码没有变化
{
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CPerson);
	CObject* pObject = pRuntimeClass->CreateObject();
	if(pObject != NULL && pObject->IsKindOf(RUNTIME_CLASS(CPerson)))
	{
		cout << " 创建成功！\n";
		delete pObject;
	}
}



  */