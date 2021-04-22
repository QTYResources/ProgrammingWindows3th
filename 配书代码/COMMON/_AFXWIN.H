///////////////////////////////////////
// _AFXWIN.H文件

#ifndef __AFXWIN_H__
#define __AFXWIN_H__

#include <windows.h>

#include "_afx.h"
#include "_afxstat_.h"
#include "_afxmsg_.h"
#include "_afxcoll.h"


////////////////////////////////////////////////////////////

// type modifier for message handlers
#ifndef afx_msg
#define afx_msg  // 内部占位符
#endif


////////////////////////////////////////////////////
// 处理消息映射的代码
class CCmdTarget;
typedef void (CCmdTarget::*AFX_PMSG)(void);

// 一个映射表项
struct AFX_MSGMAP_ENTRY
{
	UINT nMessage;	// 窗口消息
	UINT nCode;	// 控制代码或WM_NOTIFY通知码
	UINT nID;	// 控件ID，如果为窗口消息其值为0
	UINT nLastID;	// 一定范围的命令的最后一个命令或控件ID，用于支持组消息映射
	UINT nSig;	// 指定了消息处理函数的类型
	AFX_PMSG pfn;   // 消息处理函数
};

// 消息映射表
struct AFX_MSGMAP
{
	const AFX_MSGMAP* pBaseMap;		// 其基类的消息映射表的地址
	const AFX_MSGMAP_ENTRY* pEntries;	// 消息映射项的指针
};

// 宏的定义
#define DECLARE_MESSAGE_MAP() \
private: \
	static const AFX_MSGMAP_ENTRY _messageEntries[]; \
protected: \
	static const AFX_MSGMAP messageMap; \
	virtual const AFX_MSGMAP* GetMessageMap() const; \

#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
	const AFX_MSGMAP* theClass::GetMessageMap() const \
		{ return &theClass::messageMap; } \
	const AFX_MSGMAP theClass::messageMap = \
	{ &baseClass::messageMap, &theClass::_messageEntries[0] }; \
	const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = \
	{ \

#define END_MESSAGE_MAP() \
		{0, 0, 0, 0, 0, (AFX_PMSG)0} \
	}; \

////////////////////////////////////////////////////
// CCmdTarget类（命令目标）

class CCmdTarget : public CObject
{
	DECLARE_DYNCREATE(CCmdTarget);
public:
	CCmdTarget();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////
// CWnd 类

class CWnd : public CCmdTarget
{
	DECLARE_DYNCREATE(CWnd)
public:	
	CWnd();
	virtual ~CWnd();

	HWND m_hWnd;
	operator HWND() const { return m_hWnd; }
	HWND GetSafeHwnd() { return this == NULL ? NULL : m_hWnd; }

	// 窗口句柄映射
	static CWnd* FromHandle(HWND hWnd);
	static CWnd* FromHandlePermanent(HWND hWnd);
	BOOL Attach(HWND hWndNew);
	HWND Detach();	


	// 为创建各种子窗口设置
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		LPVOID lpParam = NULL);

	// 最终创建窗口的代码
	BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		int x, int y, int nWidth, int nHeight,
		HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam = NULL);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();
	virtual void PreSubclassWindow();

	static const MSG* GetCurrentMessage();

protected:

	// 默认消息处理函数的地址
	WNDPROC m_pfnSuper;
	virtual WNDPROC* GetSuperWndProcAddr();

	// 对消息进行默认处理
	LRESULT Default();	
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	

	// 处理Windows消息
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);


	// 消息处理函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	// WM_CREATE消息
	afx_msg void OnPaint();			// WM_PAINT消息
	afx_msg void OnClose();			// WM_CLOSE消息
	afx_msg void OnDestroy();		// WM_DESTROY消息
	afx_msg void OnNcDestroy();		// WM_NCDESTROY消息
	afx_msg void OnTimer(UINT nIDEvent);	// WM_TIMER消息
	

	// 分发消息的实现
	friend LRESULT AfxCallWndProc(CWnd*, HWND, UINT, WPARAM, LPARAM);
	// 挂钩消息的实现
	friend LRESULT __stdcall _AfxCbtFilterHook(int, WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

typedef void (CWnd::*AFX_PMSGW)(void); 
		// like 'AFX_PMSG' but for CWnd derived classes only

// 注册你自己的窗口类的辅助函数
LPCTSTR AfxRegisterWndClass(UINT nClassStyle,
	HCURSOR hCursor = 0, HBRUSH hbrBackground = 0, HICON hIcon = 0);

BOOL AfxRegisterClass(WNDCLASS* lpWndClass);
BOOL AfxEndDeferRegisterClass(LONG fToRegister);

WNDPROC AfxGetAfxWndProc();

void AfxHookWindowCreate(CWnd* pWnd);
BOOL AfxUnhookWindowCreate();

const AFX_MSGMAP_ENTRY* AfxFindMessageEntry(const AFX_MSGMAP_ENTRY* lpEntry, 
						UINT nMsg, UINT nCode, UINT nID);

////////////////////////////////////////////////////
// CWinThread 类
extern CThreadSlotData* _afxThreadData;

typedef UINT (__cdecl *AFX_THREADPROC)(LPVOID);

class CWinThread : public CCmdTarget
{
	DECLARE_DYNCREATE(CWinThread)
public:
// 线程对象的初始化（Constructors）
	CWinThread();
	BOOL CreateThread(DWORD dwCreateFlags = 0, UINT nStackSize = 0,
		LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);


// 保存和设置线程对象特有的属性（Attributes）
	CWnd* m_pMainWnd;	// 主窗口
	BOOL m_bAutoDelete;	// 指示在线程结束后，是否要销毁此对象

	HANDLE m_hThread;
	DWORD m_nThreadID;	
	operator HANDLE() const
		{ return this == NULL ? NULL : m_hThread; }
	
	int GetThreadPriority()
		{ return ::GetThreadPriority(m_hThread); }
	BOOL SetThreadPriority(int nPriority)
		{ return ::SetThreadPriority(m_hThread, nPriority); }

// 对线程的操作（Operations）
	DWORD SuspendThread()
		{ return ::SuspendThread(m_hThread); }
	DWORD ResumeThread()
		{ return ::ResumeThread(m_hThread); }

// 允许重载的函数（Overridables）
	// 执行线程实例初始化
	virtual BOOL InitInstance();

	// 开始处理消息
	virtual int Run();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PumpMessage();
	virtual BOOL OnIdle(LONG lCount);
	virtual BOOL IsIdleMessage(MSG* pMsg);

	// 线程终止时执行清除
	virtual int ExitInstance();
	
// 具体实现(Implementation)
public:
	virtual ~CWinThread();
	virtual void Delete();
	void CommonConstruct();

	MSG m_msgCur;
public:
	CWinThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam);

	LPVOID m_pThreadParams;		// 用户传递给新线程的参数
	AFX_THREADPROC m_pfnThreadProc; // 线程函数的地址
};

// 提供给用户使用的全局函数
CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam,
	int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
	DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
CWinThread* AfxGetThread();
void AfxEndThread(UINT nExitCode, BOOL bDelete = TRUE);


///////////////////////////////////////////////////////////////////////////////
// CWinApp类
class CWinApp : public CWinThread
{
	DECLARE_DYNCREATE(CWinApp)
public:
	CWinApp();
	virtual ~CWinApp();

// 属性
	// WinMain函数的四个参数
	HINSTANCE m_hInstance;
	HINSTANCE m_hPrevInstance;
	LPTSTR m_lpCmdLine;
	int m_nCmdShow;

// 帮助操作，通常在InitInstance函数中进行
public:
	HCURSOR LoadCursor(UINT nIDResource) const;
	HICON LoadIcon(UINT nIDResource) const;

// 虚函数
public:
	virtual BOOL InitApplication();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
};

__inline HCURSOR CWinApp::LoadCursor(UINT nIDResource) const
	{ return::LoadCursor(AfxGetModuleState()->m_hCurrentResourceHandle, (LPCTSTR)nIDResource); }
__inline HICON CWinApp::LoadIcon(UINT nIDResource) const
	{ return::LoadIcon(AfxGetModuleState()->m_hCurrentResourceHandle, (LPCTSTR)nIDResource); }

CWinApp* AfxGetApp();

__inline CWinApp* AfxGetApp()
	{ return AfxGetModuleState()->m_pCurrentWinApp; }

BOOL AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				LPTSTR lpCmdLine, int nCmdShow);

#endif // __AFXWIN_H__


/*



////////////////////////////////////////////////////
// CWinThread 类
extern CThreadSlotData* _afxThreadData;

typedef UINT (__cdecl *AFX_THREADPROC)(LPVOID);

class CWinThread
{
public:
// 线程对象的初始化（Constructors）
	CWinThread();
	BOOL CreateThread(DWORD dwCreateFlags = 0, UINT nStackSize = 0,
		LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);


// 保存和设置线程对象特有的属性（Attributes）
	BOOL m_bAutoDelete;	// 指示在线程结束后，是否要销毁此对象

	HANDLE m_hThread;
	DWORD m_nThreadID;	
	operator HANDLE() const
		{ return this == NULL ? NULL : m_hThread; }
	
	int GetThreadPriority()
		{ return ::GetThreadPriority(m_hThread); }
	BOOL SetThreadPriority(int nPriority)
		{ return ::SetThreadPriority(m_hThread, nPriority); }

// 对线程的操作（Operations）
	DWORD SuspendThread()
		{ return ::SuspendThread(m_hThread); }
	DWORD ResumeThread()
		{ return ::ResumeThread(m_hThread); }

// 具体实现(Implementation)
public:
	virtual ~CWinThread();
	virtual void Delete();
	void CommonConstruct();
public:
	CWinThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam);

	LPVOID m_pThreadParams;		// 用户传递给新线程的参数
	AFX_THREADPROC m_pfnThreadProc; // 线程函数的地址
};

// 提供给用户使用的全局函数
CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam,
	int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
	DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
CWinThread* AfxGetThread();
void AfxEndThread(UINT nExitCode, BOOL bDelete = TRUE);





  */