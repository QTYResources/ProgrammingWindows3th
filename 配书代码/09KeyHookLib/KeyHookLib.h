///////////////////////////////////////////
// KeyHookLib.h�ļ�

// ���庯�����κ꣬�������ñ�DLL���̵ĵ�������
#ifdef KEYHOOKLIB_EXPORTS
#define KEYHOOKLIB_API __declspec(dllexport)
#else
#define KEYHOOKLIB_API __declspec(dllimport)
#endif

// �Զ�����������ͨ�ŵ���Ϣ
#define HM_KEY WM_USER + 101

// ����Ҫ�����ĺ���
BOOL KEYHOOKLIB_API WINAPI SetKeyHook(BOOL bInstall, 
				      DWORD dwThreadId = 0, HWND hWndCaller = NULL);



