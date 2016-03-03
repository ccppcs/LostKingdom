// LoginDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameClient.h"
#include "LoginDlg.h"


#include "MainFrm.h"
#include "GameClientDoc.h"
#include "GameClientView.h"

// LoginDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(LoginDlg, CDialog)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LoginDlg::IDD, pParent)
	, m_ID(_T(""))
{
}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ID, m_ID);
	DDV_MaxChars(pDX, m_ID, 8);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &LoginDlg::OnBnClickedOk)
//	ON_WM_CREATE()
//ON_WM_ACTIVATE()
ON_BN_CLICKED(IDCANCEL, &LoginDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// LoginDlg �޽��� ó�����Դϴ�.

void LoginDlg::OnBnClickedOk()
{
	UpdateData();
	if(m_ID != _T(""))
	{
		CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	
		pView->Login(m_ID);
	}
	else
	{
		AfxMessageBox(_T("����Ͻ� ���̵� �Է��ϼ���"));
	}
}

BOOL LoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void LoginDlg::OnBnClickedCancel()
{
	PostQuitMessage(0);
	OnCancel();
}

void LoginDlg::LoginOk(void)
{
	CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->isLogin=TRUE;
	OnOK();
}
