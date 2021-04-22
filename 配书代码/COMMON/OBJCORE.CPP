///////////////////////////////////////////////////////
// OBJCORE.CPP文件


#include "_afx.h"



CObject* CRuntimeClass::CreateObject()
{
	if(m_pfnCreateObject == NULL)
		return NULL;
	return (*m_pfnCreateObject)(); // 调用创建类的函数
}

BOOL CRuntimeClass::IsDerivedFrom(const CRuntimeClass* pBaseClass) const
{
	const CRuntimeClass* pClassThis = this;
	while(pClassThis != NULL)
	{
		if(pClassThis == pBaseClass) // 判断标识类的CRuntimeClass的首地址是否相同
			return TRUE;
		pClassThis = pClassThis->m_pBaseClass;
	}
	return FALSE; // 查找到了继承结构的顶层，没有一个匹配
}

const struct CRuntimeClass CObject::classCObject = 
	{ "CObject"/*类名*/, sizeof(CObject)/*大小*/, 0xffff/*无版本号*/, 
					NULL/*不支持动态创建*/, NULL/*没有基类*/, NULL};

CRuntimeClass* CObject::GetRuntimeClass() const
{
	// 下面的语句展开后就是“return ((CRuntimeClass*)&(CObject::classCObject));”
	return RUNTIME_CLASS(CObject);
}

BOOL CObject::IsKindOf(const CRuntimeClass* pClass) const
{
	CRuntimeClass* pClassThis = GetRuntimeClass();
	return pClassThis->IsDerivedFrom(pClass);
}