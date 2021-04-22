///////////////////////////////////////////////////////
// _AFXSTAT_.H文件

#ifndef __AFXSTAT_H__
#define __AFXSTAT_H__

#ifndef __AFXTLS_H__
	#include "_afxtls_.h"
#endif


// AFX_MODULE_THREAD_STATE (模块-线程状态)
class CWinThread;
class CHandleMap;
class AFX_MODULE_THREAD_STATE : public CNoTrackObject
{
public:
	// 指向当前线程对象(CWinThread对象)的指针
	CWinThread* m_pCurrentWinThread;

	// 窗口句柄映射
	CHandleMap* m_pmapHWND;
};

AFX_MODULE_THREAD_STATE* AfxGetModuleThreadState();


// AFX_MODULE_STATE (模块状态)
struct CRuntimeClass;
class CWinApp;
class AFX_MODULE_STATE : public CNoTrackObject
{
public:
	
	CTypedSimpleList<CRuntimeClass*> m_listClass;	// 记录模块中的类信息

	CWinApp* m_pCurrentWinApp;	    // 当前CWinApp对象的指针
	HINSTANCE m_hCurrentInstanceHandle; // 当前模块的实例句柄（也就是模块句柄）
	HINSTANCE m_hCurrentResourceHandle; // 包含资源的实例句柄

	// CThreadLocal<AFX_MODULE_THREAD_STATE> m_thread;
	THREAD_LOCAL(AFX_MODULE_THREAD_STATE, m_thread) // 特定于线程的状态数据
};

AFX_MODULE_STATE* AfxGetAppModuleState();
AFX_MODULE_STATE* AfxGetModuleState();

// _AFX_THREAD_STATE （线程状态）
class CWnd;
class _AFX_THREAD_STATE : public CNoTrackObject
{
public:
	AFX_MODULE_STATE* m_pModuleState;

	// 注册窗口类时使用（缓冲区）
	TCHAR m_szTempClassName[96]; 

	// 创建窗口时使用
	CWnd* m_pWndInit;		// 正在初始化的CWnd对象的指针
	HHOOK m_hHookOldCbtFilter;	// 钩子句柄

	MSG m_lastSendMsg;
};

EXTERN_THREAD_LOCAL(_AFX_THREAD_STATE, _afxThreadState);
_AFX_THREAD_STATE* AfxGetThreadState();

#endif // __AFXSTAT_H__



/*
3.3 中

#ifndef __AFXSTAT_H__
#define __AFXSTAT_H__

#ifndef __AFXTLS_H__
	#include "_afxtls_.h"
#endif

class CWinThread;

// AFX_MODULE_THREAD_STATE (模块-线程状态)
class AFX_MODULE_THREAD_STATE : public CNoTrackObject
{
public:
	// 指向当前线程对象(CWinThread对象)的指针
	CWinThread* m_pCurrentWinThread;
};

EXTERN_THREAD_LOCAL(AFX_MODULE_THREAD_STATE, _afxModuleThreadState)
AFX_MODULE_THREAD_STATE* AfxGetModuleThreadState();



#endif // __AFXSTAT_H__

 */