///////////////////////////////////////////////////
// FilterDriver�ļ�

#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <Winsvc.h>

class CDriver
{

public:
// ���캯������������
	// ���캯����pszDriverPathΪ��������Ŀ¼��pszLinkNameΪ������������
	// ����Ĺ��캯���У�����ͼ������򿪷���
	CDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName);
	// �����������������ֹͣ����
	virtual ~CDriver();

// ����
	// �������Ƿ����
	virtual BOOL IsValid() { return (m_hSCM != NULL && m_hService != NULL); }

// ����
	// ��������Ҳ����˵������DriverEntry������������
	virtual BOOL StartDriver();
	// �������񡣼����������DriverUnload���̽�������
	virtual BOOL StopDriver();
	
	// ���豸����ȡ�õ���������һ�����
	virtual BOOL OpenDevice();

	// ���豸���Ϳ��ƴ���
	virtual DWORD IoControl(DWORD nCode, PVOID pInBuffer, 
			DWORD nInCount, PVOID pOutBuffer, DWORD nOutCount);
// ʵ��
protected:
	char m_szLinkName[56];	// ������������

	BOOL m_bStarted;	// ָ�������Ƿ�����
	BOOL m_bCreateService;	// ָ���Ƿ񴴽��˷���

	HANDLE m_hSCM;		// SCM���ݿ���
	HANDLE m_hService;	// ������
	HANDLE m_hDriver;	// �豸���
};


CDriver::CDriver(LPCTSTR pszDriverPath, LPCTSTR pszLinkName)
{
	strncpy(m_szLinkName, pszLinkName, 55);
	m_bStarted = FALSE;
	m_bCreateService = FALSE;
	m_hSCM = m_hService = NULL;
	m_hDriver = INVALID_HANDLE_VALUE;

	// ��SCM������
	m_hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(m_hSCM == NULL)
	{
		MessageBox(0, "�򿪷�����ƹ�����ʧ��\n", 
				"��������Ϊ����ӵ��AdministratorȨ��\n", 0);
		return;
	}

	// ������򿪷���
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
		// �����ɹ��󣬵ȴ������������״̬
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
		// �ȴ�������ȫֹͣ����
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
	// �򿪵����������������豸�ľ��
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
	// �����������Ϳ��ƴ���
	DWORD nBytesReturn;
	BOOL bRet = ::DeviceIoControl(m_hDriver, nCode, 
		pInBuffer, nInCount, pOutBuffer, nOutCount, &nBytesReturn, NULL);

	if(bRet)
		return nBytesReturn;
	else
		return -1;
}

#endif // __DRIVER_H__