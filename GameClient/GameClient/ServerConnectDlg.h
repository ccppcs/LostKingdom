#pragma once
#include "afxcmn.h"


// ServerConnectDlg ��ȭ �����Դϴ�.

class ServerConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(ServerConnectDlg)

public:
	ServerConnectDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~ServerConnectDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SERVERCONNECTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IPAddress;
	CString m_Port;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

};
