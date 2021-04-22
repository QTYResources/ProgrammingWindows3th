///////////////////////////////////////////
// _AFX.H文件


#ifndef __AFX_H__
#define __AFX_H__

#include <windows.h>


///////////////////////////////////////////////////
// 调试支持
#define AfxDebugBreak()    _asm { int 3 }
#ifdef _DEBUG

void __cdecl AfxTrace(LPCTSTR lpszFormat, ...);
#define TRACE              ::AfxTrace
#define ASSERT(f) \
	if (!(f)) \
		AfxDebugBreak();
#define VERIFY(f)          ASSERT(f)

#else // _DEBUG

#define ASSERT(f)          ((void)0)   
#define VERIFY(f)          ((void)(f))
inline void __cdecl AfxTrace(LPCTSTR, ...) { }
#define TRACE              (void)0

#endif // _DEBUG

/////////////////////////////////////////////////////
// 运行期类信息
class CObject;
struct CRuntimeClass
{
// 属性（Attributes）
	LPCSTR m_lpszClassName;	// 类的名称
	int m_nObjectSize;	// 类的大小
	UINT m_wSchema;		// 类的版本号
	CObject* (__stdcall* m_pfnCreateObject)();	// 创建类的函数的指针
	CRuntimeClass* m_pBaseClass;			// 其基类中CRuntimeClass结构的地址

// 操作（operations）
	CObject* CreateObject();
	BOOL IsDerivedFrom(const CRuntimeClass* pBaseClass) const;

// 内部实现（Implementation）
	CRuntimeClass* m_pNextClass;  // 将所有CRuntimeClass对象用简单链表连在一起
};

//////////////////////////////////////////////////////
// CObject 类
class CObject
{
public:
	virtual CRuntimeClass* GetRuntimeClass() const;
	virtual ~CObject();

// 属性（Attibutes）
public:
	BOOL IsKindOf(const CRuntimeClass* pClass) const;

// 实现（implementation）
public:
	static const CRuntimeClass classCObject; //   标识类的静态成员
};
inline CObject::~CObject() { }

// 宏定义

// RUNTIME_CLASS宏用来取得class_name类中CRuntimeClass结构的地址
#define RUNTIME_CLASS(class_name) ((CRuntimeClass*)&class_name::class##class_name)

// 支持动态类型识别的宏

#define DECLARE_DYNAMIC(class_name) \
public: \
	static const CRuntimeClass class##class_name; \
	virtual CRuntimeClass* GetRuntimeClass() const; 

#define IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
	const CRuntimeClass class_name::class##class_name = { \
		#class_name, sizeof(class class_name), wSchema, pfnNew, \
			RUNTIME_CLASS(base_class_name), NULL }; \
	CRuntimeClass* class_name::GetRuntimeClass() const \
		{ return RUNTIME_CLASS(class_name); } \

#define IMPLEMENT_DYNAMIC(class_name, base_class_name) \
	IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL)

// 支持动态创建的宏

#define DECLARE_DYNCREATE(class_name) \
	DECLARE_DYNAMIC(class_name) \
	static CObject* __stdcall CreateObject();

#define IMPLEMENT_DYNCREATE(class_name, base_class_name) \
	CObject* __stdcall class_name::CreateObject() \
		{ return new class_name; } \
	IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, \
		class_name::CreateObject)


#endif // __AFX_H__