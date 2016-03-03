// WaitRoomDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameClient.h"
#include "WaitRoomDlg.h"

#include "MainFrm.h"
#include "GameClientDoc.h"
#include "GameClientView.h"

#define NOSELECTED 0
#define WARRIOR 1
#define ARCHER 2

// WaitRoomDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(WaitRoomDlg, CDialog)

WaitRoomDlg::WaitRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialog(WaitRoomDlg::IDD, pParent)
	, m_chat(_T(""))
{
	m_ClassSelect=NOSELECTED;
	isReady = FALSE;
	m_OtherSelected=NOSELECTED;
	
}

WaitRoomDlg::~WaitRoomDlg()
{
}

void WaitRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WAIT_CHATTING, m_chatting);
	DDX_Text(pDX, IDC_CHAT, m_chat);
	DDV_MaxChars(pDX, m_chat, 30);
	DDX_Control(pDX, IDC_READY, b_ready);
	DDX_Control(pDX, IDC_SELECT_WARRIOR, b_warrior);
	DDX_Control(pDX, IDC_SELECT_ARCHER, b_archer);
	DDX_Control(pDX, IDC_SELECT, m_selected);
}


BEGIN_MESSAGE_MAP(WaitRoomDlg, CDialog)
	ON_BN_CLICKED(IDC_SELECT_WARRIOR, &WaitRoomDlg::OnBnClickedSelectWarrior)
	ON_BN_CLICKED(IDC_SELECT_ARCHER, &WaitRoomDlg::OnBnClickedSelectArcher)
	ON_BN_CLICKED(IDC_READY, &WaitRoomDlg::OnBnClickedReady)
	ON_BN_CLICKED(IDCANCEL, &WaitRoomDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SEND, &WaitRoomDlg::OnBnClickedSend)
END_MESSAGE_MAP()


// WaitRoomDlg �޽��� ó�����Դϴ�.

void WaitRoomDlg::OnBnClickedSelectWarrior()
{
	m_ClassSelect = WARRIOR;
	SelectedClass();
}

void WaitRoomDlg::OnBnClickedSelectArcher()
{
	m_ClassSelect = ARCHER;
	SelectedClass();
}

void WaitRoomDlg::OnBnClickedReady()
{
	isReady=TRUE;
	if(isReady)
	{
		b_ready.SetWindowTextW(_T("�Ϸ� �����"));
		b_ready.EnableWindow(FALSE);

		b_warrior.EnableWindow(FALSE);
		b_archer.EnableWindow(FALSE);
	}
	UpdateData();
	CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->Ready(m_ClassSelect);
}

void WaitRoomDlg::OnBnClickedCancel()
{	
	//���ȭ�鿡�� ��������� �ʱ�ȭ ó��
	CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();

	pView->Ready(NOSELECTED);

	OnCancel();
	PostQuitMessage(0);
}

void WaitRoomDlg::OnBnClickedSend()
{
	UpdateData();

	CString msg;
	CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	msg.Format(_T("@0[%s] : %s\r\n"),pView->ID,m_chat);
	pView->m_socket.Send(msg,lstrlen(msg)*2);

	m_chat = _T("");
	UpdateData(FALSE);
}

void WaitRoomDlg::AddMessage(LPCTSTR message)
{
	int len = m_chatting.GetWindowTextLengthW();
	m_chatting.SetSel(len,len);
	m_chatting.ReplaceSel(message);
}

void WaitRoomDlg::SelectedClass(void)
{
	switch(m_ClassSelect)
	{
	case WARRIOR:
		b_warrior.SetWindowText(_T("������"));
		b_archer.SetWindowText(_T("����"));

		b_warrior.EnableWindow(FALSE);
		b_archer.EnableWindow(TRUE);

		b_ready.EnableWindow(TRUE);
		break;
	case ARCHER:
		b_warrior.SetWindowText(_T("����"));
		b_archer.SetWindowText(_T("������"));

		b_warrior.EnableWindow(TRUE);
		b_archer.EnableWindow(FALSE);

		b_ready.EnableWindow(TRUE);
		break;
	}
	UpdateData();
}


void WaitRoomDlg::UpdateSeleted(void)
{
	switch(m_OtherSelected)
	{
	case NOSELECTED:
		m_selected.SetWindowTextW(_T(" "));
		break;
	case WARRIOR:
		m_selected.SetWindowTextW(_T("����"));
		break;
	case ARCHER:
		m_selected.SetWindowTextW(_T("�ü�"));
		break;
	}
	UpdateData();
}

void WaitRoomDlg::DlgClose(void)
{
	CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->user[pView->ConnectedNumber-1].SetJob(m_ClassSelect-1);		// �� ���� ����
	pView->user[pView->ConnectedNumber%2].SetJob(m_OtherSelected-1);	// ���� ���� ����
	OnOK();
}
