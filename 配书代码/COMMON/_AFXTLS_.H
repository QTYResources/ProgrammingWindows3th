///////////////////////////////////////////////////////
// _AFXTLS_.H文件



#ifndef __AFXTLS_H__  // _AFXTLS_.H 文件
#define __AFXTLS_H__

#include <windows.h>
#include <stddef.h>


class CNoTrackObject;

////////////////////////////////////////////////
// CSimpleList

class CSimpleList
{
public:
	CSimpleList(int nNextOffset = 0);
	void Construct(int nNextOffset);

// 提供给用户的接口函数（Operations），用于添加、删除和遍历节点
	BOOL IsEmpty() const;
	void AddHead(void* p);
	void RemoveAll();
	void* GetHead() const;
	void* GetNext(void* p) const;
	BOOL Remove(void* p);

// 为实现接口函数所需的成员（Implementation）
	void* m_pHead;		// 链表中第一个元素的地址
	size_t m_nNextOffset;	// 数据结构中pNext成员的偏移量
	void** GetNextPtr(void* p) const;
};

// 类的内联函数
inline CSimpleList::CSimpleList(int nNextOffset)
{ m_pHead = NULL; m_nNextOffset = nNextOffset; }
inline void CSimpleList::Construct(int nNextOffset)
{ m_nNextOffset = nNextOffset; }
inline BOOL CSimpleList::IsEmpty() const
{ return m_pHead == NULL; }
inline void CSimpleList::RemoveAll()
{ m_pHead = NULL; }
inline void* CSimpleList::GetHead() const
{ return m_pHead; }
inline void* CSimpleList::GetNext(void* preElement) const
{ return *GetNextPtr(preElement); }
inline void** CSimpleList::GetNextPtr(void* p) const
{ return (void**)((BYTE*)p + m_nNextOffset); }


template<class TYPE>
class CTypedSimpleList : public CSimpleList
{
public:
	CTypedSimpleList(int nNextOffset = 0) 
		: CSimpleList(nNextOffset) { }
	void AddHead(TYPE p) 
		{ CSimpleList::AddHead((void*)p); }
	TYPE GetHead()
		{ return (TYPE)CSimpleList::GetHead(); }
	TYPE GetNext(TYPE p)
		{ return (TYPE)CSimpleList::GetNext(p); }
	BOOL Remove(TYPE p)
		{ return CSimpleList::Remove(p); }
	operator TYPE()
		{ return (TYPE)CSimpleList::GetHead(); }
};


////////////////////////////////////////////////
// CNoTrackObject
class CNoTrackObject
{
public:
	void* operator new(size_t nSize);
	void operator delete(void*);
	virtual ~CNoTrackObject() { }
};

/////////////////////////////////////////////////
// CThreadSlotData - 管理我们自己的线程局部存储

// warning C4291: no matching operator delete found
#pragma warning(disable : 4291) 

struct CSlotData;
struct CThreadData;

class CThreadSlotData
{
public:
	CThreadSlotData();

// 提供给用户的接口函数（Operations）
	int AllocSlot();	
	void FreeSlot(int nSlot); 
	void* GetThreadValue(int nSlot); 
	void SetValue(int nSlot, void* pValue);
	void DeleteValues(HINSTANCE hInst, BOOL bAll = FALSE);

// 类的实现代码（Implementations）
	DWORD m_tlsIndex;	// 用来访问系统提供的线程局部存储

	int m_nAlloc;		//  m_pSlotData所指向数组的大小
	int m_nRover;		// 为了快速找到一个空闲的槽而设定的值
	int m_nMax;		// CThreadData结构中pData指向的数组的大小
	CSlotData* m_pSlotData;	// 标识每个槽状态的全局数组的首地址
	CTypedSimpleList<CThreadData*> m_list;	// CThreadData结构的列表
	CRITICAL_SECTION m_cs;

	void* operator new(size_t, void* p)
			{ return p; }
	void DeleteValues(CThreadData* pData, HINSTANCE hInst);
	~CThreadSlotData();
};


///////////////////////////////////////////////

class CThreadLocalObject
{
public:
// 属性成员(Attributes)，用于取得保存在线程局部的变量中的指针
	CNoTrackObject* GetData(CNoTrackObject* (*pfnCreateObject)());
	CNoTrackObject* GetDataNA();

// 具体实现(Implementation)
	DWORD m_nSlot;
	~CThreadLocalObject();
};


template<class TYPE>
class CThreadLocal : public CThreadLocalObject
{
// 属性成员（Attributes）
public:
	TYPE* GetData()
	{
		TYPE* pData = (TYPE*)CThreadLocalObject::GetData(&CreateObject);
		return pData;
	}
	TYPE* GetDataNA()
	{
		TYPE* pData = (TYPE*)CThreadLocalObject::GetDataNA();
		return pData;
	}
	operator TYPE*()
		{ return GetData(); }
	TYPE* operator->()
		{ return GetData(); }

// 具体实现（Implementation）
public:
	static CNoTrackObject* CreateObject()
		{ return new TYPE; }
};


#define THREAD_LOCAL(class_name, ident_name) \
	CThreadLocal<class_name> ident_name;
#define EXTERN_THREAD_LOCAL(class_name, ident_name) \
	extern THREAD_LOCAL(class_name, ident_name)


#endif // __AFXTLS_H__


/*


2.2.4 


#ifndef __AFXTLS_H__  // _AFXTLS_.H 文件
#define __AFXTLS_H__

#include <windows.h>

class CThreadLocalObject
{
public:
	CThreadLocalObject();

// 属性成员(Attributes)，用于取得保存在线程局部的变量中的指针
	LPVOID GetData(LPVOID (*pfnCreateObject)());
	LPVOID GetDataNA();

// 执行体(Implementation)，实现类接口所需的成员
	DWORD m_tlsIndex;
	~CThreadLocalObject();	
};

#endif // __AFXTLS_H__










 */