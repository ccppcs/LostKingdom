// LoginDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GameClient.h"
#include "LoginDlg.h"


#include "MainFrm.h"
#include "GameClientDoc.h"
#include "GameClientView.h"

// LoginDlg 대화 상자입니다.

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


// LoginDlg 메시지 처리기입니다.

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
		AfxMessageBox(_T("사용하실 아이디를 입력하세요"));
	}
}

BOOL LoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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
