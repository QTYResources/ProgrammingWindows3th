///////////////////////////////////////////////////////////////
// DynClass.cpp.cppÎÄ¼þ

#include <iostream.h>
#include "../common/_afx.h"

class CPerson : public CObject
{
	DECLARE_DYNAMIC(CPerson)
};
IMPLEMENT_DYNAMIC(CPerson, CObject)

void main()
{
	CObject* pMyObject = new CPerson;
	if(pMyObject->IsKindOf(RUNTIME_CLASS(CPerson)))
	{
		CPerson* pMyPerson = (CPerson*) pMyObject ;
		cout << " a CPerson Object! \n";
		delete pMyPerson;
	}
	else
		delete pMyObject;
}

