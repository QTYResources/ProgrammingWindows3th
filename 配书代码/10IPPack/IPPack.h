///////////////////////////////////////////
// IPPack.h�ļ�

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
	// ��ʼ������
	void InitData();
	BOOL m_bOpen;	// �û��Ƿ��Ŀ�����
	BOOL m_bPause;	// �Ƿ���ͣ�ػ�Ŀ������е�����

protected:
	// �������
	void UIControl();
	CStatusBarCtrl m_bar;	// ״̬���ؼ�
	CListCtrl m_listData;	// �б���ͼ�ؼ�

protected:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	// �û�����Ŀ�갴ť
	afx_msg void OnTarget();
	// �û������رհ�ť
	afx_msg void OnClose();
	// �û�������ͣ��ť
	afx_msg void OnPause();
	// �û����������ť
	afx_msg void OnClear();
	// �û����������ǰ��ѡ��
	afx_msg void OnTopMost();

	// �û������б���ͼ�ؼ�
	afx_msg void OnClickListData(NMHDR* pNMHDR, LRESULT* pResult);
	// �б���ͼ�ؼ��е�һ���ɾ��
	afx_msg void OnDeleteItemList(NMHDR* pNMHDR, LRESULT* pResult);


	// ����DLL���͹�����HM_RECEIVE��HM_SEND��Ϣ
	afx_msg long OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg long OnSend(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};