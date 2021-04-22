///////////////////////////////////////////
// IPPack.h文件

#include <afxwin.h>	
#include <afxcmn.h>
#include <winsock2.h> 

class CMyApp : public CWinApp
{
public:
	BOOL InitInstance();
};

class CMainDialog : public CDialog
{
public:
	CMainDialog(CWnd* pParentWnd = NULL);

protected:
	// 初始化数据
	void InitData();
	BOOL m_bOpen;	// 用户是否打开目标进程
	BOOL m_bPause;	// 是否暂停截获目标进程中的数据

protected:
	// 界面控制
	void UIControl();
	CStatusBarCtrl m_bar;	// 状态栏控件
	CListCtrl m_listData;	// 列表试图控件

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	// 用户单击目标按钮
	afx_msg void OnTarget();
	// 用户单击关闭按钮
	afx_msg void OnClose();
	// 用户单击暂停按钮
	afx_msg void OnPause();
	// 用户单击清除按钮
	afx_msg void OnClear();
	// 用户点击总在最前复选框
	afx_msg void OnTopMost();

	// 用户单击列表视图控件
	afx_msg void OnClickListData(NMHDR* pNMHDR, LRESULT* pResult);
	// 列表视图控件中的一个项被删除
	afx_msg void OnDeleteItemList(NMHDR* pNMHDR, LRESULT* pResult);


	// 处理DLL发送过来的HM_RECEIVE和HM_SEND消息
	afx_msg long OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSend(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};