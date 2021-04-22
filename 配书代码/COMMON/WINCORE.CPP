////////////////////////////////////////////////
// WINCORE.CPP文件

#include "_afxwin.h"
#include "winhand_.h"
#include "_afximpl.h"


// 框架程序注册窗口类时使用的类名
const TCHAR _afxWnd[] = AFX_WND;
const TCHAR _afxWndFrameOrView[] = AFX_WNDFRAMEORVIEW;



CWnd::CWnd()
{
	m_hWnd = NULL;
	m_pfnSuper = NULL;
}

CWnd::~CWnd()
{
	if(m_hWnd != NULL)
	{
		::DestroyWindow(m_hWnd);
	}
}

/////////////////////////////////////////////////
// 初始化消息映射表
BEGIN_MESSAGE_MAP(CWnd, CCmdTarget)
ON_WM_NCDESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////
// 句柄映射

CHandleMap* afxMapHWND(BOOL bCreate = FALSE)
{
	AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
	if(pState->m_pmapHWND == NULL && bCreate)
	{
		pState->m_pmapHWND = new CHandleMap();
	}
	return pState->m_pmapHWND;
}

CWnd* CWnd::FromHandle(HWND hWnd)
{
	CHandleMap* pMap = afxMapHWND(TRUE); // 如果不存在则创建一个CHandleMap对象
	ASSERT(pMap != NULL);
	return (CWnd*)pMap->FromHandle(hWnd);
}

CWnd* CWnd::FromHandlePermanent(HWND hWnd)
{
	CHandleMap* pMap = afxMapHWND();
	CWnd* pWnd = NULL;
	if(pMap != NULL)
	{
		// 仅仅在永久映射（非临时映射）中查找——不创建任何新的CWnd对象
		pWnd = (CWnd*)pMap->LookupPermanent(hWnd);
	}
	return pWnd;
}

BOOL CWnd::Attach(HWND hWndNew)
{
	ASSERT(m_hWnd == NULL);	 // 仅仅附加一次
	ASSERT(FromHandlePermanent(hWndNew) == NULL); // 必须没有在永久映射中

	if(hWndNew == NULL)
		return FALSE;

	CHandleMap* pMap = afxMapHWND(TRUE);	// 如果不存在则创建一个CHandleMap对象
	ASSERT(pMap != NULL);

	pMap->SetPermanent(m_hWnd = hWndNew, this); // 添加一对映射
	return TRUE;
}

HWND CWnd::Detach()
{
	HWND hWnd = m_hWnd;
	if(hWnd != NULL)
	{
		CHandleMap* pMap = afxMapHWND(); // 如果不存在不去创建
		if(pMap != NULL)
			pMap->RemoveHandle(hWnd);
		m_hWnd = NULL;
	}
	return hWnd;
}


///////////////////////////////////////////////
// 分发消息

LRESULT __stdcall AfxWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd == hWnd);
	return AfxCallWndProc(pWnd, hWnd, nMsg, wParam, lParam);
}

LRESULT AfxCallWndProc(CWnd* pWnd, HWND hWnd, UINT nMsg, 
				WPARAM wParam = 0, LPARAM lParam = 0)
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	
	// 因为可能会发生嵌套调用，所以要首先保存旧的消息，在函数返回时恢复
	MSG oldState = pThreadState->m_lastSendMsg;

	// 更新本线程中变量m_lastSendMsg的值
	pThreadState->m_lastSendMsg.hwnd = hWnd;
	pThreadState->m_lastSendMsg.message = nMsg;
	pThreadState->m_lastSendMsg.wParam = wParam;
	pThreadState->m_lastSendMsg.lParam = lParam;

	// 处理接受到的消息

	// 将消息交给CWnd对象
	LRESULT lResult;
	lResult = pWnd->WindowProc(nMsg, wParam, lParam);

	// 消息处理完毕，在返回处理结果以前恢复m_lastSendMsg的值
	pThreadState->m_lastSendMsg = oldState;
	return lResult;
}

LRESULT CWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult;
	if(!OnWndMsg(message, wParam, lParam, &lResult))
		lResult = DefWindowProc(message, wParam, lParam);
	return lResult;
}

////////////////////////////////////////////////////////
// 注册窗口类

BOOL AfxRegisterClass(WNDCLASS* lpWndClass)
{
	WNDCLASS wndclass;
	if (GetClassInfo(lpWndClass->hInstance, lpWndClass->lpszClassName,
		&wndclass))
	{
		// 已经注册了该类
		return TRUE;
	}
	if (!::RegisterClass(lpWndClass))
	{
		TRACE("Can't register window class named %s\n", lpWndClass->lpszClassName);
		return FALSE;
	}
	return TRUE;
}

LPCTSTR AfxRegisterWndClass(UINT nClassStyle, HCURSOR hCursor,
					HBRUSH hbrBackground, HICON hIcon)
{
	// 使用线程局部存储中的缓冲区存放临时类名
	LPTSTR lpszName = AfxGetThreadState()->m_szTempClassName;

	HINSTANCE hInst = AfxGetModuleState()->m_hCurrentInstanceHandle;
	if(hCursor == NULL && hbrBackground == NULL && hIcon == NULL)
		wsprintf(lpszName, "Afx:%d:%d", (int)hInst, nClassStyle);
	else
		wsprintf(lpszName, "Afx:%d:%d:%d:%d", (int)hInst, nClassStyle, 
					(int)hCursor, (int)hbrBackground, (int)hIcon);

	WNDCLASS wc = { 0 };
	if(::GetClassInfo(hInst, lpszName, &wc))
	{
		ASSERT(wc.style == nClassStyle);
		return lpszName;
	}
	
	wc.hInstance = hInst;
	wc.style = nClassStyle;
	wc.hCursor = hCursor;
	wc.hbrBackground = hbrBackground;
	wc.hIcon = hIcon;
	wc.lpszClassName = lpszName;
	wc.lpfnWndProc = ::DefWindowProc;

	if(!AfxRegisterClass(&wc))
	{
		TRACE("Can't register window class named %s\n", lpszName);						
		return NULL;
	}
	return lpszName;
}

BOOL AfxEndDeferRegisterClass(LONG fToRegister)
{
	WNDCLASS wndclass;
	memset(&wndclass, 0, sizeof(wndclass));
	wndclass.lpfnWndProc = ::DefWindowProc;
	wndclass.hInstance = AfxGetModuleState()->m_hCurrentInstanceHandle;
	wndclass.hCursor = ::LoadCursor(NULL, IDC_ARROW);

	BOOL bResult = FALSE;
	if(fToRegister & AFX_WND_REG)
	{
		// 子窗口——没有背景刷子，没有图标，最安全的风格
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wndclass.lpszClassName = _afxWnd;
		bResult = AfxRegisterClass(&wndclass);
	}
	else if(fToRegister & AFX_WNDFRAMEORVIEW_REG)
	{
		// 框架或视图窗口——普通的颜色
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndclass.lpszClassName = _afxWndFrameOrView;
		bResult = AfxRegisterClass(&wndclass);
	}

	return bResult;
}

////////////////////////////////////////////
// 挂钩消息

WNDPROC AfxGetAfxWndProc()
{
	return &AfxWndProc;
}

LRESULT __stdcall _AfxCbtFilterHook(int code, WPARAM wParam, LPARAM lParam)
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	if(code != HCBT_CREATEWND)
	{
		// 只对HCBT_CREATEWND通知事件感兴趣
		return ::CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code, wParam, lParam);
	}

	// 得到正在初始化的窗口的窗口句柄和CWnd对象的指针
	HWND hWnd = (HWND)wParam;
	CWnd* pWndInit = pThreadState->m_pWndInit;

	// 将hWnd关联到pWndInit指向的CWnd对象中，并设置窗口的窗口函数的地址
	if(pWndInit != NULL)
	{
		// hWnd不应该在永久句柄映射中
		ASSERT(CWnd::FromHandlePermanent(hWnd) == NULL);

		// 附加窗口句柄
		pWndInit->Attach(hWnd);
	
		// 允许其它子类化窗口的事件首先发生
		pWndInit->PreSubclassWindow();

		// 下面要改变窗口函数的地址

		// 要在pOldWndProc指向的变量中保存原来的窗口函数
		WNDPROC* pOldWndProc = pWndInit->GetSuperWndProcAddr();
		ASSERT(pOldWndProc != NULL);

		// 子类化此窗口（改变窗口函数的地址）
		WNDPROC afxWndProc = AfxGetAfxWndProc();
		WNDPROC oldWndProc = (WNDPROC)::SetWindowLong(hWnd, 
							GWL_WNDPROC, (DWORD)afxWndProc);
		ASSERT(oldWndProc != NULL);
		if(oldWndProc != afxWndProc) // 如果确实改变了
			*pOldWndProc = oldWndProc;

		pThreadState->m_pWndInit = NULL;
	}
	return ::CallNextHookEx(pThreadState->m_hHookOldCbtFilter, code, wParam, lParam);
}

void AfxHookWindowCreate(CWnd* pWnd)
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	if(pThreadState->m_pWndInit == pWnd)
		return;
	if(pThreadState->m_hHookOldCbtFilter == NULL)
		pThreadState->m_hHookOldCbtFilter = ::SetWindowsHookEx(WH_CBT, 
					_AfxCbtFilterHook, NULL, ::GetCurrentThreadId());

	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd == NULL); // 仅挂钩一次

	ASSERT(pThreadState->m_pWndInit == NULL);
	pThreadState->m_pWndInit = pWnd;
}

BOOL AfxUnhookWindowCreate()
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	if(pThreadState->m_hHookOldCbtFilter != NULL)
	{
		::UnhookWindowsHookEx(pThreadState->m_hHookOldCbtFilter);
		pThreadState->m_hHookOldCbtFilter = NULL;
	}

	if(pThreadState->m_pWndInit != NULL)
	{
		pThreadState->m_pWndInit = NULL;
		return FALSE;	// 钩子没有被成功的安装
	}
	return TRUE;
}

WNDPROC* CWnd::GetSuperWndProcAddr()
{
	return &m_pfnSuper;
}

LRESULT CWnd::Default()
{
	// 以最近接受到的一个消息为参数调用DefWindowProc函数
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	return DefWindowProc(pThreadState->m_lastSendMsg.message, 
			pThreadState->m_lastSendMsg.wParam, pThreadState->m_lastSendMsg.lParam);
}

LRESULT CWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_pfnSuper != NULL)
		return ::CallWindowProc(m_pfnSuper, m_hWnd, message, wParam, lParam);

	WNDPROC pfnWndProc;
	if((pfnWndProc = *GetSuperWndProcAddr()) == NULL) 
		return ::DefWindowProc(m_hWnd, message, wParam, lParam);
	else
		return ::CallWindowProc(pfnWndProc, m_hWnd, message, wParam, lParam); 	
}

////////////////////////////////////////////////////////
// 创建窗口

BOOL CWnd::Create(LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName, DWORD dwStyle,
	const RECT& rect,
	CWnd* pParentWnd, UINT nID,
	LPVOID lpParam)
{
	// 只允许创建非弹出式的子窗口
	ASSERT(pParentWnd != NULL);
	ASSERT((dwStyle & WS_POPUP) == 0);

	return CreateEx(0, lpszClassName, lpszWindowName,
		dwStyle | WS_CHILD,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(), (HMENU)nID, (LPVOID)lpParam);
}

BOOL CWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName,
	LPCTSTR lpszWindowName, DWORD dwStyle,
	int x, int y, int nWidth, int nHeight,
	HWND hWndParent, HMENU nIDorHMenu, LPVOID lpParam)
{
	CREATESTRUCT cs;
	cs.dwExStyle = dwExStyle;
	cs.lpszClass = lpszClassName;
	cs.lpszName = lpszWindowName;
	cs.style = dwStyle;
	cs.x = x;
	cs.y = y;
	cs.cx = nWidth;
	cs.cy = nHeight;
	cs.hwndParent = hWndParent;
	cs.hMenu = nIDorHMenu;
	cs.hInstance = AfxGetModuleState()->m_hCurrentInstanceHandle;
	cs.lpCreateParams = lpParam;

	// 调用虚函数PreCreateWindow，执行注册窗口类的代码
	if(!PreCreateWindow(cs))
	{
		// 调用虚函数PostNcDestroy，通知用户窗口没有被创建
		PostNcDestroy();
		return FALSE;
	}

	// 创建窗口
	AfxHookWindowCreate(this);
	HWND hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass,
			cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
			cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);
	if(!AfxUnhookWindowCreate())
		PostNcDestroy();        // CreateWindowEx调用失败，通知用户


	if(hWnd == NULL)
		return FALSE;
	ASSERT(hWnd == m_hWnd); // 至此，新窗口的句柄应该已经附加到当前CWnd对象
	return TRUE;
}

BOOL CWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if(cs.lpszClass == NULL)
	{
		// 默认情况下，创建的是子窗口
		VERIFY(AfxEndDeferRegisterClass(AFX_WND_REG));
		ASSERT(cs.style & WS_CHILD);
		cs.lpszClass = _afxWnd;
	}
	return TRUE;
}

void CWnd::PostNcDestroy()
{
	// 默认情况下什么也不做
}

void CWnd::PreSubclassWindow()
{
	// 默认情况下什么也不做
}





const MSG* CWnd::GetCurrentMessage()
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	pThreadState->m_lastSendMsg.time = ::GetMessageTime();
	DWORD dw = ::GetMessagePos();
	pThreadState->m_lastSendMsg.pt.x = LOWORD(dw);
	pThreadState->m_lastSendMsg.pt.y = HIWORD(dw);
	return &pThreadState->m_lastSendMsg;
}






////////////////////////////////////////////////////////////
// 分发消息

BOOL CWnd::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LRESULT lResult = 0;

	// 将命令消息和通知消息交给指定的函数处理
	if(message == WM_COMMAND)
	{
		if(OnCommand(wParam, lParam))
		{
			lResult = 1;
			goto LReturnTrue;
		}
		return FALSE;
	}
	if(message == WM_NOTIFY)
	{
		NMHDR* pHeader = (NMHDR*)lParam;
		if(pHeader->hwndFrom != NULL && OnNotify(wParam, lParam, &lResult))
			goto LReturnTrue;
		return FALSE;
	}

	// 在各类的消息映射表中查找合适的消息处理函数，找到的话就调用它
	const AFX_MSGMAP* pMessageMap;
	const AFX_MSGMAP_ENTRY* lpEntry;
	for(pMessageMap = GetMessageMap(); pMessageMap != NULL; pMessageMap = pMessageMap->pBaseMap)
	{
		ASSERT(pMessageMap != pMessageMap->pBaseMap);
		if((lpEntry = AfxFindMessageEntry(pMessageMap->pEntries, message, 0, 0)) != NULL)
				goto LDispatch;
	}
	return FALSE;

LDispatch:
	union MessageMapFunctions mmf;
	mmf.pfn = lpEntry->pfn;
	switch(lpEntry->nSig)
	{
	default:
		return FALSE;
	case AfxSig_vw:
		(this->*mmf.pfn_vw)(wParam);
		break;
	case AfxSig_vv:
		(this->*mmf.pfn_vv)();
		break;
	case AfxSig_is:
		(this->*mmf.pfn_is)((LPTSTR)lParam);
		break;	
	}

LReturnTrue:
	if(pResult != NULL)
		*pResult = lResult;
	return TRUE;
}

BOOL CWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{ 
	return FALSE;
}

BOOL CWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{ 
	return FALSE; 
}


////////////////////////////////////////////////////
// 默认的消息处理

int CWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return Default();
}

void CWnd::OnPaint()
{
	Default();
}

void CWnd::OnClose()
{
	Default();
}

void CWnd::OnDestroy()
{
	Default();
}

void CWnd::OnNcDestroy()
{
	CWinThread* pThread = AfxGetThread();
	if(pThread != NULL)     
	{
		if(pThread->m_pMainWnd == this)
		{
			if(pThread == AfxGetApp())   // 应用程序的退出
			{
				::PostQuitMessage(0); 
			}
			pThread->m_pMainWnd = NULL;
		}
	}

	Default();
	Detach();
	// 给子类做清理工作的一个机会
	PostNcDestroy();
}

void CWnd::OnTimer(UINT nIDEvent)
{
	Default();
}



const AFX_MSGMAP_ENTRY* AfxFindMessageEntry(const AFX_MSGMAP_ENTRY* lpEntry, 
						UINT nMsg, UINT nCode, UINT nID)
{
	while(lpEntry->nSig != AfxSig_end)
	{
		if(lpEntry->nMessage == nMsg && lpEntry->nCode == nCode &&
				(nID >= lpEntry->nID && nID <= lpEntry->nLastID))
			return lpEntry;
		lpEntry++;
	}
	return NULL;
}


IMPLEMENT_DYNCREATE(CWnd, CCmdTarget)
