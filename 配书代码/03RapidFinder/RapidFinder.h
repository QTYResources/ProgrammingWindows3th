///////////////////////////////////////////////////////////////
// RapidFinder.h文件

#if !defined(AFX_RAPIDFINDER_H__CB51489C_D083_4902_8FA1_F01DA07EF176__INCLUDED_)
#define AFX_RAPIDFINDER_H__CB51489C_D083_4902_8FA1_F01DA07EF176__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common/_afxwin.h"

struct CDirectoryNode : public CNoTrackObject
{
	CDirectoryNode* pNext;	// CTypedSimpleList类模板要使用此成员
	char szDir[MAX_PATH];	// 要查找的目录
};

class CRapidFinder  
{
public:
	CRapidFinder(int nMaxThread);
	virtual ~CRapidFinder();
	BOOL CheckFile(LPCTSTR lpszFileName);

	int m_nResultCount;				// 结果数目
	int m_nThreadCount;				// 活动线程数目
	CTypedSimpleList<CDirectoryNode*> m_listDir;	// 目录列表
	CRITICAL_SECTION m_cs;				// 关键代码段

	const int m_nMaxThread;				// 最大线程数目
	char m_szMatchName[MAX_PATH];			// 要搜索的文件
	HANDLE m_hDirEvent;				// 向m_listDir中添加新的目录后置位（受信）
	HANDLE m_hExitEvent;				// 各搜索线程将要退出时置位（受信）
};

#endif // !defined(AFX_RAPIDFINDER_H__CB51489C_D083_4902_8FA1_F01DA07EF176__INCLUDED_)
