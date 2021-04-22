///////////////////////////////////////////////////////////////
// TypeIdentify.cpp文件

#include <iostream.h>
#include "../common/_afx.h"

class CPerson : public CObject
{
public:
	virtual CRuntimeClass* GetRuntimeClass() const
		{ return (CRuntimeClass*)&classCPerson; }

	static const CRuntimeClass classCPerson;
};
const CRuntimeClass CPerson::classCPerson = 
	{ "CPerson", sizeof(CPerson), 0xffff, NULL, (CRuntimeClass*)&CObject::classCObject, NULL };

void main()
{
	CObject* pMyObject = new CPerson;

	// 判断对象pMyObject是否属于CPerson类或者此类的派生类
	if(pMyObject->IsKindOf(RUNTIME_CLASS(CPerson)))	
		// RUNTIME_CLASS(CPerson)宏被展开后相当于((CRuntimeClass*)&CPerson::classCPerson)
	{
		CPerson* pMyPerson = (CPerson*)pMyObject;
		
		cout << " a CPerson Object! \n";
		delete pMyPerson;
	}
	else
	{
		delete pMyObject;
	}

}

/*
	// 打印出类的信息
	CRuntimeClass* pClass = pMyObject->GetRuntimeClass();
	cout << pClass->m_lpszClassName << "\n";   // 打印出"CPerson"
	cout << pClass->m_nObjectSize << "\n";	   // 打印出"4"

*/