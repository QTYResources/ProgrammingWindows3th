///////////////////////////////////////////
// IPPackLib.hÎÄ¼þ


#ifndef __IPPACKLIB_H__
#define __IPPACKLIB_H__

struct CMessageData
{
	DWORD dwThreadId;
	SOCKET socket;
	int nDataLength;
	// TCHAR[nDataLength]

	TCHAR* data()           // TCHAR* to managed data
	{ return (TCHAR*)(this+1); }
};

enum HOOK_MESSAGE
{
	HM_RECEIVE = WM_USER + 100,
	HM_SEND
};


#endif // __IPPACKLIB_H__
