///////////////////////////////////////////////////////////////
// DynCreate.cpp�ļ�

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
{ "CPerson", sizeof(CPerson), 0xffff, &CPerson::CreateObject/*��ӵ�����*/, (CRuntimeClass*)&CObject::classCObject, NULL };

void main()
{
	// ȡ��CPerson����CRuntimeClass�ṹ��¼����Ϣ
	// ��ʵ��Ӧ���У�����һ��Ӵ�����ȡ����һ��Ϣ��
	// �Ӷ��ڴ�����û�и�������������´�������Ķ���
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CPerson);

	// ȡ����pRuntimeClassָ�룬����֪��������־Ϳ��Դ�������
	CObject* pObject = pRuntimeClass->CreateObject();
	if(pObject != NULL && pObject->IsKindOf(RUNTIME_CLASS(CPerson)))
	{
		cout << "�����ɹ���\n";
		delete pObject;
	}
}

/*

 // �ȼ۴��룺

class CPerson : public CObject
{
	DECLARE_DYNCREATE(CPerson)
};
IMPLEMENT_DYNCREATE(CPerson, CObject)

void main() // main������Ĵ���û�б仯
{
	CRuntimeClass* pRuntimeClass = RUNTIME_CLASS(CPerson);
	CObject* pObject = pRuntimeClass->CreateObject();
	if(pObject != NULL && pObject->IsKindOf(RUNTIME_CLASS(CPerson)))
	{
		cout << " �����ɹ���\n";
		delete pObject;
	}
}



  */