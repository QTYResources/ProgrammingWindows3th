///////////////////////////////////////////////////////////////
// TypeIdentify.cpp�ļ�

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

	// �ж϶���pMyObject�Ƿ�����CPerson����ߴ����������
	if(pMyObject->IsKindOf(RUNTIME_CLASS(CPerson)))	
		// RUNTIME_CLASS(CPerson)�걻չ�����൱��((CRuntimeClass*)&CPerson::classCPerson)
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
	// ��ӡ�������Ϣ
	CRuntimeClass* pClass = pMyObject->GetRuntimeClass();
	cout << pClass->m_lpszClassName << "\n";   // ��ӡ��"CPerson"
	cout << pClass->m_nObjectSize << "\n";	   // ��ӡ��"4"

*/