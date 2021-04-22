//////////////////////////////////////////
// EnumProcessDlg.h文件

#include <afxwin.h>
#include <afxcmn.h>

class CEnumProcessDlg : public CDialog
{
public:
	CEnumProcessDlg(CWnd* pParentWnd);

	DWORD m_dwProcessId;	// 用户选择进程的进程ID号
	DWORD m_dwThreadId;	// 用户选择进程的线程ID号

protected:
	// 列表视图子窗口
	CListCtrl m_listPro;
	// 更新进程列表
	void UpdateProcess();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};