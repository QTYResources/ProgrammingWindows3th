///////////////////////////////////////////////////////
// AFXTLS.CPP文件



#include "_AFXTLS_.H"


//---------------------------------------

void CSimpleList::AddHead(void* p)
{
	*GetNextPtr(p) = m_pHead;
	m_pHead = p;
}

BOOL CSimpleList::Remove(void* p)
{
	if(p == NULL)	// 检查参数
		return FALSE;
	
	BOOL bResult = FALSE; // 假设移除失败
	if(p == m_pHead)
	{
	// 要移除头元素
		m_pHead = *GetNextPtr(p);
		bResult = TRUE;
	}
	else
	{
		// 试图在表中查找要移除的元素
		void* pTest = m_pHead;
		while(pTest != NULL && *GetNextPtr(pTest) != p)
			pTest = *GetNextPtr(pTest);

		// 如果找到，就将元素移除
		if(pTest != NULL)
		{
			*GetNextPtr(pTest) = *GetNextPtr(p);
			bResult = TRUE;
		}
	}
	return bResult;
}

//-------------------CThreadSlotData类----------------------//
BYTE __afxThreadData[sizeof(CThreadSlotData)];	// 为下面的_afxThreadData变量提供内存
CThreadSlotData* _afxThreadData; // 定义全局变量_afxThreadData来为全局变量分配空间

struct CSlotData
{
	DWORD dwFlags;	// 槽的使用标志（被分配/未被分配）
	HINSTANCE hInst;// 占用此槽的模块句柄
};

struct CThreadData : public CNoTrackObject
{
	CThreadData* pNext; // CSimpleList类要使用此成员
	int nCount;	    // 数组元素的个数
	LPVOID* pData;      // 数组的首地址
};

#define SLOT_USED 0x01		// CSlotData结构中dwFlags成员的值为0x01时表示该槽已被使用

CThreadSlotData::CThreadSlotData()
{
	m_list.Construct(offsetof(CThreadData, pNext)); // 初始化CTypedSimpleList对象

	m_nMax = 0;
	m_nAlloc = 0;
	m_nRover = 1;	// 我们假定Slot1还未被分配（第一个槽（Slot0）总是保留下来不被使用）
	m_pSlotData = NULL;

	m_tlsIndex = ::TlsAlloc();	// 使用系统的TLS申请一个索引
	::InitializeCriticalSection(&m_cs);	// 初始化关键段变量
}

int CThreadSlotData::AllocSlot()
{
	::EnterCriticalSection(&m_cs);	// 进入临界区（也叫关键段）
	int nAlloc = m_nAlloc;
	int nSlot = m_nRover;

	if(nSlot >= nAlloc || m_pSlotData[nSlot].dwFlags & SLOT_USED)
	{
		// 搜索m_pSlotData，查找空槽（SLOT）
		for(nSlot = 1; nSlot < nAlloc && m_pSlotData[nSlot].dwFlags & SLOT_USED; nSlot ++) ;

		// 如果不存在空槽，申请更多的空间
		if(nSlot >= nAlloc)
		{
			// 增加全局数组的大小，分配或再分配内存以创建新槽
			int nNewAlloc = nAlloc + 32;

			HGLOBAL hSlotData;
			if(m_pSlotData == NULL)	// 第一次使用
			{
				hSlotData = ::GlobalAlloc(GMEM_MOVEABLE, nNewAlloc*sizeof(CSlotData));
			}
			else
			{
				hSlotData = ::GlobalHandle(m_pSlotData);
				::GlobalUnlock(hSlotData);
				hSlotData = ::GlobalReAlloc(hSlotData, 
					nNewAlloc*sizeof(CSlotData), GMEM_MOVEABLE);
			}
			CSlotData* pSlotData = (CSlotData*)::GlobalLock(hSlotData);
	
			// 将新申请的空间初始化为0
			memset(pSlotData + m_nAlloc, 0, (nNewAlloc - nAlloc)*sizeof(CSlotData));
			m_nAlloc = nNewAlloc;
			m_pSlotData = pSlotData;
		}
	}

	// 调整m_nMax的值，以便为各线程的私有数据分配内存
	if(nSlot >= m_nMax)
		m_nMax = nSlot + 1;

	m_pSlotData[nSlot].dwFlags |= SLOT_USED;
	// 更新m_nRover的值(我们假设下一个槽未被使用)
	m_nRover = nSlot + 1;

	::LeaveCriticalSection(&m_cs);
	return nSlot; // 返回的槽号可以被FreeSlot, GetThreadValue, SetValue函数使用了
}

void CThreadSlotData::FreeSlot(int nSlot)
{
	::EnterCriticalSection(&m_cs);	

	// 删除所有线程中的数据
	CThreadData* pData = m_list;
	while(pData != NULL)
	{
		if(nSlot < pData->nCount)
		{
			delete (CNoTrackObject*)pData->pData[nSlot];
			pData->pData[nSlot] = NULL;
		}
		pData = pData->pNext;
	}

	// 将此槽号标识为未被使用
	m_pSlotData[nSlot].dwFlags &= ~SLOT_USED;
	::LeaveCriticalSection(&m_cs);
}

inline void* CThreadSlotData::GetThreadValue(int nSlot)
{
	CThreadData* pData = (CThreadData*)::TlsGetValue(m_tlsIndex);
	if(pData == NULL || nSlot >= pData->nCount)
		return NULL;
	return pData->pData[nSlot];
}

void CThreadSlotData::SetValue(int nSlot, void* pValue)
{
	// 通过TLS索引得到我们为线程安排的私有存储空间
	CThreadData* pData = (CThreadData*)::TlsGetValue(m_tlsIndex);

	// 为线程私有数据申请内存空间
	if((pData == NULL || nSlot >= pData->nCount) && pValue != NULL)
	{
		// pData的值为空，表示该线程第一次访问线程私有数据
		if(pData == NULL)
		{
			pData = new CThreadData;
			pData->nCount = 0;
			pData->pData = NULL;

			// 将新申请的内存的地址添加到全局列表中
			::EnterCriticalSection(&m_cs);
			m_list.AddHead(pData);
			::LeaveCriticalSection(&m_cs);
		}

		// pData->pData指向真正的线程私有数据，下面的代码将私有数据占用的空间增长到m_nMax指定的大小
		if(pData->pData == NULL)
			pData->pData = (void**)::GlobalAlloc(LMEM_FIXED, m_nMax*sizeof(LPVOID));
		else
			pData->pData = (void**)::GlobalReAlloc(pData->pData, m_nMax*sizeof(LPVOID), LMEM_MOVEABLE);
		
		// 将新申请的内存初始话为0
		memset(pData->pData + pData->nCount, 0, 
			(m_nMax - pData->nCount) * sizeof(LPVOID));
		pData->nCount = m_nMax;
		::TlsSetValue(m_tlsIndex, pData);
	}

	// 设置线程私有数据的值
	pData->pData[nSlot] = pValue;
}

void CThreadSlotData::DeleteValues(HINSTANCE hInst, BOOL bAll)
{
	::EnterCriticalSection(&m_cs);
	if(!bAll)
	{
		// 仅仅删除当前线程的线程局部存储占用的空间
		CThreadData* pData = (CThreadData*)::TlsGetValue(m_tlsIndex);
		if(pData != NULL)
			DeleteValues(pData, hInst);
	}
	else
	{
		// 删除所有线程的线程局部存储占用的空间
		CThreadData* pData = m_list.GetHead();
		while(pData != NULL)
		{
			CThreadData* pNextData = pData->pNext;
			DeleteValues(pData, hInst);
			pData = pNextData;
		}
	}
	::LeaveCriticalSection(&m_cs);
}

void CThreadSlotData::DeleteValues(CThreadData* pData, HINSTANCE hInst)
{
	// 释放表中的每一个元素
	BOOL bDelete = TRUE;
	for(int i=1; i<pData->nCount; i++)
	{
		if(hInst == NULL || m_pSlotData[i].hInst == hInst)
		{
			// hInst匹配，删除数据
			delete (CNoTrackObject*)pData->pData[i];
			pData->pData[i] = NULL;
		}
		else
		{
			// 还有其它模块在使用，不要删除数据
			if(pData->pData[i] != NULL)
			bDelete = FALSE;
		}
	}

	if(bDelete)
	{
		// 从列表中移除
		::EnterCriticalSection(&m_cs);
		m_list.Remove(pData);
		::LeaveCriticalSection(&m_cs);
		::LocalFree(pData->pData);
		delete pData;

		// 清除TLS索引，防止重用
		::TlsSetValue(m_tlsIndex, NULL);
	}
}

CThreadSlotData::~CThreadSlotData()
{
	CThreadData *pData = m_list;
	while(pData != NULL)
	{
		CThreadData* pDataNext = pData->pNext;
		DeleteValues(pData, NULL);
		pData = pData->pNext;
	}

	if(m_tlsIndex != (DWORD)-1)
		::TlsFree(m_tlsIndex);

	if(m_pSlotData != NULL)
	{
		HGLOBAL hSlotData = ::GlobalHandle(m_pSlotData);
		::GlobalUnlock(hSlotData);
		::GlobalFree(m_pSlotData);
	}

	::DeleteCriticalSection(&m_cs);
}

//---------------------------------------

void* CNoTrackObject::operator new(size_t nSize)
{
	// 申请一块带有GMEM_FIXED和GMEM_ZEROINIT标志的内存
	void* p = ::GlobalAlloc(GPTR, nSize);
	return p;
}

void CNoTrackObject::operator delete(void* p)
{
	if(p != NULL)
		::GlobalFree(p);
}

//----------------------------CThreadLocalObject 类--------------------------------//

CNoTrackObject* CThreadLocalObject::GetData(CNoTrackObject* (*pfnCreateObject)())
{
	if(m_nSlot == 0)
	{
		if(_afxThreadData == NULL)
			_afxThreadData = new(__afxThreadData) CThreadSlotData;
		m_nSlot = _afxThreadData->AllocSlot();
	}
 
	CNoTrackObject* pValue = (CNoTrackObject*)_afxThreadData->GetThreadValue(m_nSlot);
	if(pValue == NULL)
	{
		// 创建一个数据项
		pValue = (*pfnCreateObject)();

		// 使用线程私有数据保存新创建的对象
		_afxThreadData->SetValue(m_nSlot, pValue);	
	}
	
	return pValue;
}

CNoTrackObject* CThreadLocalObject::GetDataNA()
{
	if(m_nSlot == 0 || _afxThreadData == 0)
		return NULL;
	return (CNoTrackObject*)_afxThreadData->GetThreadValue(m_nSlot);
}

CThreadLocalObject::~CThreadLocalObject()
{
	if(m_nSlot != 0 && _afxThreadData != NULL)
		_afxThreadData->FreeSlot(m_nSlot);
	m_nSlot = 0;
}

//------------------------------------------