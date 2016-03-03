// ServerConnectDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameClient.h"
#include "ServerConnectDlg.h"

#include "MainFrm.h"
#include "GameClientDoc.h"
#include "GameClientView.h"

// ServerConnectDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(ServerConnectDlg, CDialog)

ServerConnectDlg::ServerConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ServerConnectDlg::IDD, pParent)
	, m_Port(_T("21000"))
{
	
}

ServerConnectDlg::~ServerConnectDlg()
{
}

void ServerConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Text(pDX, IDC_Port, m_Port);
	DDV_MaxChars(pDX, m_Port, 5);
}


BEGIN_MESSAGE_MAP(ServerConnectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &ServerConnectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ServerConnectDlg �޽��� ó�����Դϴ�.

BOOL ServerConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_IPAddress.SetWindowTextW(_T("127.0.0.1"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void ServerConnectDlg::OnBnClickedOk()
{
	UpdateData();
	CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();		// Get ViewClass Pointer
	pView->m_socket.Create();

	CString m_ip;
	m_IPAddress.GetWindowTextW(m_ip);
	UINT uPort = _ttoi(m_Port);
	if(pView->m_socket.Connect(m_ip,uPort)==FALSE)
	{
		AfxMessageBox(_T("ERROR : Failed to connect server"));
		PostQuitMessage(0);
	}
	pView->isConnect = TRUE;
	OnOK();
}
