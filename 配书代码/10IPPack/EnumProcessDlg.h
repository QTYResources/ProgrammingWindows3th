//////////////////////////////////////////
// EnumProcessDlg.h�ļ�

#include <afxwin.h>
#include <afxcmn.h>

class CEnumProcessDlg : public CDialog
{
public:
	CEnumProcessDlg(CWnd* pParentWnd);

	DWORD m_dwProcessId;	// �û�ѡ����̵Ľ���ID��
	DWORD m_dwThreadId;	// �û�ѡ����̵��߳�ID��

protected:
	// �б���ͼ�Ӵ���
	CListCtrl m_listPro;
	// ���½����б�
	void UpdateProcess();

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};