///////////////////////////////////////////
// _AFXCOLL.H文件

#ifndef __AFXCOLL_H__
#define __AFXCOLL_H__

#include "_afxwin.h"
#include "_afxplex_.h"

class CMapPtrToPtr
{
protected:
	// 关联（Association）
	struct CAssoc
	{
		CAssoc* pNext;	// 指向下一个CAssoc结构
		void* key;	// 键对象
		void* value;	// 值对象
	};

public:
// 构造函数（Construction）
	CMapPtrToPtr(int nBlockSize = 10);

// 属性成员（Attributes）
	// 元素的个数
	int GetCount() const;
	BOOL IsEmpty() const;
	// 在映射中查找key所对应的rValue
	BOOL Lookup(void* key, void*& rValue);

// 操作（Operations）
	// 查找或者是添加key对应的value
	void*& operator[](void* key);
	// 添加一个新的（key, vaule）对
	void SetAt(void* key, void* newValue);

	// 移除一个存在的（key, ?）对
	BOOL RemoveKey(void* key);
	void RemoveAll();

	UINT GetHashTableSize() const;
	void InitHashTable(UINT nHashSize, BOOL bAllocNow = TRUE);
	UINT HashKey(void* key) const;

// 实现（Implementation）
protected:
	CAssoc** m_pHashTable;
	int m_nHashTableSize;

	struct CPlex* m_pBlocks;	// 保存用CPlex::Create函数申请的内存块的首地址
	int m_nBlockSize;		// 指定每个内存块可以容纳多少个CAssoc结构
	CAssoc* m_pFreeList;		// 预留空间中没有被使用的CAssoc结构组成的链中第一个关联的指针
	int m_nCount;			// 记录了程序一共使用了多少个CAssoc结构，即关联的个数

	// 为一个新的CAssoc结构提供空间，相当于使用new操作符
	CAssoc* NewAssoc();
	// 释放一个CAssoc结构占用的空间，相当于使用delete操作符
	void FreeAssoc(CAssoc* pAssoc);
	// 寻找键key所在的关联
	CAssoc* GetAssocAt(void* key, UINT& nHash) const;
public:
	~CMapPtrToPtr();
};

inline int CMapPtrToPtr::GetCount() const
{ return m_nCount; }
inline BOOL CMapPtrToPtr::IsEmpty() const
{ return m_nCount == 0; }
inline void CMapPtrToPtr::SetAt(void* key, void* newValue)
{ (*this)[key] = newValue; }
inline UINT CMapPtrToPtr::GetHashTableSize() const
{ return m_nHashTableSize; }	

#endif // __AFXCOLL_H__