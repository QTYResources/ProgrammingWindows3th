///////////////////////////////////////////////////////
// APPINIT.CPP文件

#include "_afxwin.h"

BOOL AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	ASSERT(hPrevInstance == NULL);

	// 设置实例句柄
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	pModuleState->m_hCurrentInstanceHandle = hInstance;
	pModuleState->m_hCurrentResourceHandle = hInstance;

	// 为这个应用程序填写初始化状态
	CWinApp* pApp = AfxGetApp();
	if(pApp != NULL)
	{
		pApp->m_hInstance = hInstance;
		pApp->m_hPrevInstance = hInstance;
		pApp->m_lpCmdLine = lpCmdLine;
		pApp->m_nCmdShow = nCmdShow;
	}

	return TRUE;
}

/*

void CWinApp::SetCurrentHandles()
{
	ASSERT(this == AfxGetApp());

	// Set resource handles
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	pModuleState->m_hCurrentInstanceHandle = m_hInstance;
	pModuleState->m_hCurrentResourceHandle = m_hInstance;

	// Get path
	TCHAR szBuff[MAX_PATH];
	VERIFY(::GetModuleFileName(m_hInstance, szBuff, MAX_PATH));

	LPTSTR lpszExt = strrchr(szBuff, '.');
	ASSERT(lpszExt != NULL);
	ASSERT(*lpszExt == '.');
	*lpszExt = '\0'; // No suffix

	TCHAR szExtName[MAX_PATH];
//	TCHAR szTitle[256];
	VERIFY(AfxGetFileName(szBuff, szExtName, MAX_PATH) == 0);
	if(m_pszExeName == NULL)
	{
		m_pszExeName = strdup(szExtName);
	}

	if(m_pszAppName == NULL)
	{
		m_pszAppName = strdup(szExtName);
	}

	pModuleState->m_lpszCurrentAppName = m_pszAppName;

}

UINT AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax)
{
	LPSTR lpszTemp = (LPSTR)lpszPathName;

	for(LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz++)
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)(lpsz + 1);
	}

	if(lpszTitle == NULL)
		return strlen(lpszTemp) + 1;

	strncpy(lpszTitle, lpszTemp, nMax);
	return 0;
}
*/