#pragma once


// LoginDlg ��ȭ �����Դϴ�.

class LoginDlg : public CDialog
{
	DECLARE_DYNAMIC(LoginDlg)

public:
	LoginDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~LoginDlg();
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LOGINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_ID;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	void LoginOk(void);
};
