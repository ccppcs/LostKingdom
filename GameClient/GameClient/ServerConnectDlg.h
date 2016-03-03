#pragma once
#include "afxcmn.h"


// ServerConnectDlg 대화 상자입니다.

class ServerConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(ServerConnectDlg)

public:
	ServerConnectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~ServerConnectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SERVERCONNECTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IPAddress;
	CString m_Port;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

};
