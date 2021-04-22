////////////////////////////////////////////////////
// FileCutter.h�ļ�


#ifndef __FILECUTTER_H_
#define __FILECUTTER_H_

#include <afxwin.h>

// CFileCutter�෢�������ڵ�֪ͨ��Ϣ
#define WM_CUTTERSTART WM_USER + 100	// wParam == nTatolCount
#define WM_CUTTERSTOP WM_USER + 101	// wParam == nExitCode, lParam == nCompletedCount
#define WM_CUTTERSTATUS WM_USER + 102	//			lParam == nCompletedCount

class CFileCutter
{
public:
// �����˳�����
	enum ExitCode{ 
		exitSuccess,	// �ɹ��������
		exitUserForce,  // �û���ֹ
		exitSourceErr,  // Դ�ļ�����
		exitDestErr	// Ŀ���ļ�����
		};

// ���캯��
	CFileCutter(HWND hWndNotify);

// ����
	BOOL IsRunning() const { return m_bRunning; }

// ����
	BOOL StartSplit(LPCTSTR lpszDestDir, LPCTSTR lpszSourceFile, UINT uFileSize);
	BOOL StartMerge(LPCTSTR lpszDestFile, LPCTSTR lpszSourceDir);

	BOOL SuspendCutter();
	BOOL ResumeCutter();
	void StopCutter();
	
// ����ʵ��
public:
	~CFileCutter();

protected:
	// ���ò�����Ϣ��״̬��־
	void Reset();
	// ���������ķָ����
	void DoSplit();
	// ���������ĺϲ�����
	void DoMerge();
	// �����߳�
	UINT friend _CutterEntry(LPVOID lpParam);

	// ������Ϣ
	CString m_strSource;
	CString m_strDest;
	UINT m_uFileSize;
	BOOL m_bSplit;
	// ״̬��־
	BOOL m_bContinue;	//  �Ƿ��������
	BOOL m_bRunning;	//  �Ƿ��ڹ���״̬

	// ͬ��������������
	CRITICAL_SECTION m_cs;	// Data gard

private:
	// �������������ȫ����Ч������
	HWND m_hWndNotify;	// ������Ϣ֪ͨ�¼��Ĵ��ھ��
	HANDLE m_hWorkEvent;	// ֪ͨ��ʼ�������¼�������
	CWinThread* m_pThread;	// �����߳�
	BOOL m_bSuspend;	// ��ͣ��־
	BOOL m_bExitThread;	// �˳���־
};

#endif // __FILECUTTER_H_