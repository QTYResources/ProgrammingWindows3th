///////////////////////////////////////////////////
// FilterDriver文件

#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <Winsvc.h>

class CDriver
{

public:
// 构造函数和析构函数
	// 构造函数，pszDriverPath为驱动所在目录，pszLinkName为符号连接名字
	// 在类的构造函数中，将试图创建或打开服务，
	CDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName);
	// 析构函数。在这里，将停止服务，
	virtual ~CDriver();

// 属性
	// 此驱动是否可用
	virtual BOOL IsValid() { return (m_hSCM != NULL && m_hService != NULL); }

// 操作
	// 开启服务。也就是说驱动的DriverEntry函数将被调用
	virtual BOOL StartDriver();
	// 结束服务。即驱动程序的DriverUnload例程将被调用
	virtual BOOL StopDriver();
	
	// 打开设备，即取得到此驱动的一个句柄
	virtual BOOL OpenDevice();

	// 向设备发送控制代码
	virtual DWORD IoControl(DWORD nCode, PVOID pInBuffer, 
			DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount);
// 实现
protected:
	char m_szLinkName[56];	// 符号连接名称

	BOOL m_bStarted;	// 指定服务是否启动
	BOOL m_bCreateService;	// 指定是否创建了服务

	HANDLE m_hSCM;		// SCM数据库句柄
	HANDLE m_hService;	// 服务句柄
	HANDLE m_hDriver;	// 设备句柄
};


CDriver::CDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName)
{
	strncpy(m_szLinkName, pszLinkName, 55);
	m_bStarted = FALSE;
	m_bCreateService = FALSE;
	m_hSCM = m_hService = NULL;
	m_hDriver = INVALID_HANDLE_VALUE;

	// 打开SCM管理器
	m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(m_hSCM == NULL)
	{
		MessageBox(0, "打开服务控制管理器失败\n", 
				"可能是因为您不拥有Administrator权限\n", 0);
		return;
	}

	// 创建或打开服务
	m_hService = ::CreateService(m_hSCM, m_szLinkName, m_szLinkName, SERVICE_ALL_ACCESS, 
				SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, 
				pszDriverPath, NULL, 0, NULL, NULL, NULL);
	if(m_hService == NULL)
	{
		int nError = ::GetLastError();
		if(nError == ERROR_SERVICE_EXISTS || nError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			m_hService = ::OpenService(m_hSCM, m_szLinkName, SERVICE_ALL_ACCESS);
		}
	}
	else
	{
		m_bCreateService = TRUE;
	}
}

CDriver::~CDriver()
{
	if(m_hDriver != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hDriver);

	StopDriver();
	
	if(m_bCreateService)
		::DeleteService(m_hService);	

	if(m_hService != NULL)
		::CloseServiceHandle(m_hService);
	if(m_hSCM != NULL)
		::CloseServiceHandle(m_hSCM);
}

BOOL CDriver::StartDriver()
{
	if(m_bStarted)
		return TRUE;
	if(m_hService == NULL)
		return FALSE;

	if(!::StartService(m_hService, 0, NULL))
	{
		int nError = ::GetLastError();
		if(nError == ERROR_SERVICE_ALREADY_RUNNING)
			m_bStarted = TRUE;
		else
			::DeleteService(m_hService);
	}
	else
	{
		// 启动成功后，等待服务进入运行状态
		int nTry = 0;
		SERVICE_STATUS ss;
		::QueryServiceStatus(m_hService, &ss);
		while(ss.dwCurrentState == SERVICE_START_PENDING && nTry++ < 80)
		{
			::Sleep(50);
			::QueryServiceStatus(m_hService, &ss);
		}
		if(ss.dwCurrentState == SERVICE_RUNNING)
			m_bStarted = TRUE;
	}
	return m_bStarted;
}

BOOL CDriver::StopDriver()
{
	if(!m_bStarted)
		return TRUE;
	if(m_hService == NULL)
		return FALSE;

	SERVICE_STATUS ss;
	if(!::ControlService(m_hService, SERVICE_CONTROL_STOP, &ss))
	{
		if(::GetLastError() == ERROR_SERVICE_NOT_ACTIVE)
			m_bStarted = FALSE;
	}
	else
	{
		// 等待服务完全停止运行
		int nTry = 0;
		while(ss.dwCurrentState == SERVICE_STOP_PENDING && nTry++ < 80)
		{
			::Sleep(50);
			::QueryServiceStatus(m_hService, &ss);
		}
		if(ss.dwCurrentState == SERVICE_STOPPED)
			m_bStarted = FALSE;
	}
	return !m_bStarted;
}

BOOL CDriver::OpenDevice()
{
	if(m_hDriver != INVALID_HANDLE_VALUE)
		return TRUE;

	char sz[256] = "";
	wsprintf(sz, "\\\\.\\%s", m_szLinkName);
	// 打开到驱动程序所控制设备的句柄
	m_hDriver = ::CreateFile(sz,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	return (m_hDriver != INVALID_HANDLE_VALUE);
}

DWORD CDriver::IoControl(DWORD nCode, PVOID pInBuffer, 
		DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount)
{
	if(m_hDriver == INVALID_HANDLE_VALUE)
		return -1;
	// 向驱动程序发送控制代码
	DWORD nBytesReturn;
	BOOL bRet = ::DeviceIoControl(m_hDriver, nCode, 
		pInBuffer, nInCount, pOutBuffer, nOutCount, &nBytesReturn, NULL);

	if(bRet)
		return nBytesReturn;
	else
		return -1;
}

#endif // __DRIVER_H__