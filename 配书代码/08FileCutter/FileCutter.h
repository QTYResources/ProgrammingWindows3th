////////////////////////////////////////////////////
// FileCutter.h文件


#ifndef __FILECUTTER_H_
#define __FILECUTTER_H_

#include <afxwin.h>

// CFileCutter类发给主窗口的通知消息
#define WM_CUTTERSTART WM_USER + 100	// wParam == nTatolCount
#define WM_CUTTERSTOP WM_USER + 101	// wParam == nExitCode, lParam == nCompletedCount
#define WM_CUTTERSTATUS WM_USER + 102	//			lParam == nCompletedCount

class CFileCutter
{
public:
// 工作退出代码
	enum ExitCode{ 
		exitSuccess,	// 成功完成任务
		exitUserForce,  // 用户终止
		exitSourceErr,  // 源文件出错
		exitDestErr	// 目标文件出错
		};

// 构造函数
	CFileCutter(HWND hWndNotify);

// 属性
	BOOL IsRunning() const { return m_bRunning; }

// 操作
	BOOL StartSplit(LPCTSTR lpszDestDir, LPCTSTR lpszSourceFile, UINT uFileSize);
	BOOL StartMerge(LPCTSTR lpszDestFile, LPCTSTR lpszSourceDir);

	BOOL SuspendCutter();
	BOOL ResumeCutter();
	void StopCutter();
	
// 具体实现
public:
	~CFileCutter();

protected:
	// 重置参数信息和状态标志
	void Reset();
	// 进行真正的分割操作
	void DoSplit();
	// 进行真正的合并操作
	void DoMerge();
	// 工作线程
	UINT friend _CutterEntry(LPVOID lpParam);

	// 参数信息
	CString m_strSource;
	CString m_strDest;
	UINT m_uFileSize;
	BOOL m_bSplit;
	// 状态标志
	BOOL m_bContinue;	//  是否继续工作
	BOOL m_bRunning;	//  是否处于工作状态

	// 同步以上两组数据
	CRITICAL_SECTION m_cs;	// Data gard

private:
	// 对象的生命周期全局有效的数据
	HWND m_hWndNotify;	// 接受消息通知事件的窗口句柄
	HANDLE m_hWorkEvent;	// 通知开始工作的事件对象句柄
	CWinThread* m_pThread;	// 工作线程
	BOOL m_bSuspend;	// 暂停标志
	BOOL m_bExitThread;	// 退出标志
};

#endif // __FILECUTTER_H_