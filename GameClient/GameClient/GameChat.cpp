// GameChat.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GameClient.h"
#include "GameChat.h"

#include "MainFrm.h"
#include "GameClientDoc.h"
#include "GameClientView.h"

// CGameChat

IMPLEMENT_DYNAMIC(CGameChat, CEdit)

CGameChat::CGameChat()
{

}

CGameChat::~CGameChat()
{
}


BEGIN_MESSAGE_MAP(CGameChat, CEdit)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CGameChat 메시지 처리기입니다.



void CGameChat::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_RETURN)
	{
		CString str;
		this->GetWindowTextW(str);
		this->SetWindowTextW(_T(""));

		CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
		CString msg;
		msg.Format(_T("@1[%s] : %s"),pView->ID,str);
		pView->m_socket.Send(msg,lstrlen(msg)*2);

		this->ShowWindow(SW_HIDE);
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
