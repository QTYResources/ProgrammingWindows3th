///////////////////////////////////////////////////////////////
// RapidFinder.h�ļ�

#if !defined(AFX_RAPIDFINDER_H__CB51489C_D083_4902_8FA1_F01DA07EF176__INCLUDED_)
#define AFX_RAPIDFINDER_H__CB51489C_D083_4902_8FA1_F01DA07EF176__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common/_afxwin.h"

struct CDirectoryNode : public CNoTrackObject
{
	CDirectoryNode* pNext;	// CTypedSimpleList��ģ��Ҫʹ�ô˳�Ա
	char szDir[MAX_PATH];	// Ҫ���ҵ�Ŀ¼
};

class CRapidFinder  
{
public:
	CRapidFinder(int nMaxThread);
	virtual ~CRapidFinder();
	BOOL CheckFile(LPCTSTR lpszFileName);

	int m_nResultCount;				// �����Ŀ
	int m_nThreadCount;				// ��߳���Ŀ
	CTypedSimpleList<CDirectoryNode*> m_listDir;	// Ŀ¼�б�
	CRITICAL_SECTION m_cs;				// �ؼ������

	const int m_nMaxThread;				// ����߳���Ŀ
	char m_szMatchName[MAX_PATH];			// Ҫ�������ļ�
	HANDLE m_hDirEvent;				// ��m_listDir������µ�Ŀ¼����λ�����ţ�
	HANDLE m_hExitEvent;				// �������߳̽�Ҫ�˳�ʱ��λ�����ţ�
};

#endif // !defined(AFX_RAPIDFINDER_H__CB51489C_D083_4902_8FA1_F01DA07EF176__INCLUDED_)
