///////////////////////////////////////////////////////
// MAP_PP.CPP文件


#include "_afxcoll.h"

CMapPtrToPtr::CMapPtrToPtr(int nBlockSize)
{
	m_pHashTable = NULL;
	m_nHashTableSize = 17;	// 默认大小
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
	m_pFreeList = NULL;
	m_nCount = 0;
}

inline UINT CMapPtrToPtr::HashKey(void* key) const
{
	return ((UINT)(void*)(DWORD)key) >> 4;
}

void CMapPtrToPtr::InitHashTable(UINT nHashSize, BOOL bAllocNow)
{
	if(m_pHashTable != NULL)
	{
		// 释放计算表
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}

	if(bAllocNow)
	{
		// 为计算表申请空间
		m_pHashTable = new CAssoc*[nHashSize];
		memset(m_pHashTable, 0, sizeof(CAssoc*)*nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

CMapPtrToPtr::CAssoc* CMapPtrToPtr::GetAssocAt(void* key, UINT& nHash) const
{
	// 计算包含key的项在表中的位置
	nHash = HashKey(key)%m_nHashTableSize;
	if(m_pHashTable == NULL)
		return NULL;

	// 在以m_pHashTable[nHash]为头指针的链表中查找
	CAssoc* pAssoc;
	for(pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if(pAssoc->key == key)
			return pAssoc;
	}
	return NULL;
}

BOOL CMapPtrToPtr::Lookup(void* key, void*& rValue)
{
	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if(pAssoc == NULL)
		return FALSE;	// 没有在映射中

	rValue = pAssoc->value;
	return TRUE;
}

void*& CMapPtrToPtr::operator [] (void* key)
{
	UINT nHash;
	CAssoc* pAssoc;
	if((pAssoc = GetAssocAt(key, nHash)) == NULL)
	{
		if(m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);

		// 既然映射中没有用户指定的项，我们就添加一个新的关联
		pAssoc = NewAssoc();
		pAssoc->key = key;

		// 将新的关联放入计算表中（放在表头，不是表尾）
		pAssoc->pNext = m_pHashTable[nHash];
		m_pHashTable[nHash] = pAssoc;
	}
	return pAssoc->value; // 返回value的引用
}

BOOL CMapPtrToPtr::RemoveKey(void* key)
{
	if(m_pHashTable == NULL) 
		return FALSE;	// 表中什么也没有

	CAssoc** ppAssocPre;	// 记录要删除的项的地址的变量的地址（如果存在的话，我们会改变此地址处的值）
	ppAssocPre = &m_pHashTable[HashKey(key)%m_nHashTableSize];

	CAssoc* pAssoc;
	for(pAssoc = *ppAssocPre; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if(pAssoc->key == key)
		{
			// 移除pAssoc指向的项
			*ppAssocPre = pAssoc->pNext;  // 从表中移除
			FreeAssoc(pAssoc);	      // 释放内存
			return TRUE;
		}
		ppAssocPre = &pAssoc->pNext;
	}
	return FALSE; // 没有找到
}

CMapPtrToPtr::~CMapPtrToPtr()
{
	RemoveAll();
}

void CMapPtrToPtr::RemoveAll()
{
	if(m_pHashTable != NULL)
	{
		// 释放计算表
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}

	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

CMapPtrToPtr::CAssoc* CMapPtrToPtr::NewAssoc()
{
	// 预留的空间已经被使用完
	if(m_pFreeList == NULL)
	{
		// 向m_pBlocks指向的链中添加一个新的内存块。m_nBlockSize的值可以由用户指定
		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CAssoc));

		// 将新内存块中各CAssoc结构添加到m_pFreeList指向的链中（空闲链）
		CAssoc* pAssoc = (CAssoc*)newBlock->data();
		pAssoc += m_nBlockSize -1;	// 注意，此时pAssoc指向内存块中最后一个CAssoc结构	
		for(int i = m_nBlockSize -1; i >= 0; i--, pAssoc--)
		{
			// 将pAssoc做为链的首地址添加到空闲链中（还是以相反的顺序向链中添加元素）
			pAssoc->pNext = m_pFreeList; 
			m_pFreeList = pAssoc;
		}
	}

	// 从空闲链中取出一个元素pAssoc
	CAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext; 
	m_nCount++;				// 又多使用了一个CAssoc结构

	// 初始化新关联的值
	pAssoc->key = 0;
	pAssoc->value = 0;
	return pAssoc;
}

void CMapPtrToPtr::FreeAssoc(CAssoc* pAssoc)
{
	// 将要释放的关联做为链的首地址添加到空闲链中（以相反的顺序）
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;				// 释放了一个CAssoc结构

	// 如果全部的关联都没被使用，就释放所有的内存空间，包括CPlex::Create函数申请的内存块
	if(m_nCount == 0)
		RemoveAll();  // 此函数会释放所有内存空间，待会儿我们再讨论
}









