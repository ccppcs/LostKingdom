// ConnectSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GameClient.h"
#include "ConnectSocket.h"

#include "MainFrm.h"
#include "GameClientDoc.h"
#include "GameClientView.h"

// CConnectSocket

CConnectSocket::CConnectSocket()
{
}

CConnectSocket::~CConnectSocket()
{
}


// CConnectSocket 멤버 함수

void CConnectSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("ERROR : Disconnected from server!"));
	::PostQuitMessage(0);
}

void CConnectSocket::OnReceive(int nErrorCode)
{
	TCHAR szBuffer[4096];
	::ZeroMemory(szBuffer,sizeof(szBuffer));

	if(Receive(szBuffer,sizeof(szBuffer))>0)
	{
		CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();		// Get ViewClass Pointer
		
		// 메세지 해독
		MessageDecoder(szBuffer,lstrlen(szBuffer));
	}

	CSocket::OnReceive(nErrorCode);
}

void CConnectSocket::MessageDecoder(LPCTSTR Message,int length)		// 클라이언트 메세지 해독기
{
	CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();		// Get ViewClass Pointer
	CString space = _T(" ");
	CString and = _T("&");
	/******************************

	----#----  Personal -----------
		#0 로그인 실패
		#1 로그인 성공
		#2 대기화면 선택알림
		#3 접속 순서 받기
		#4 게임 시작 신호
	----@---- Broadcast -----------
		@0 대기화면 채팅
		@1 게임화면 채팅
		@2 게임 승리/패배
	----$---- GameMessage ---------
		$
	*******************************/

	if(Message[0]=='#')
	{
		if(Message[1]=='0')
		{
			AfxMessageBox(_T("로그인 실패"));
		}// end Message[1]=='0'
		else if(Message[1]=='1')
		{
			pView->isLogin=TRUE;
			AfxMessageBox(_T("로그인 성공\n환영합니다"));
			pView->Login_dlg.LoginOk();		// 로그인폼 닫기

		}// end Message[1]=='1'

		else if(Message[1]=='2')
		{
			CString tmp = _T("");
			tmp+=Message[2];

			pView->WaitRoom_dlg.m_OtherSelected= _ttoi(tmp);
			pView->WaitRoom_dlg.UpdateSeleted();			// 선택 상황 최신화

		}// end Message[1]=='2'

		else if(Message[1]=='3')
		{
			pView->ConnectedNumber = _ttoi(&Message[2]);	// 접속한 순서 정보 저장

		}// end Message[1]=='3'

		else if(Message[1]=='4')
		{
			pView->isGameStart = TRUE;			// 게임 시작 상태로변경
			pView->WaitRoom_dlg.DlgClose();
			::SetWindowPos(AfxGetMainWnd()->m_hWnd, HWND_TOPMOST, 50, 50, 1250, 890, SWP_FRAMECHANGED);	// 1200 x 840 로 크기변경
		}// end Message[1]=='4'
	}// end Message[0]=='#'
	else if(Message[0]=='@')
	{
		if(Message[1]=='0')		// 대기화면 채팅
		{
			if(pView->isGameStart != TRUE)
			{
				CString msg=Message;
				pView->WaitRoom_dlg.AddMessage(msg.Mid(2,msg.GetLength()));
				pView->WaitRoom_dlg.UpdateSeleted();
			}
		}// end Message[1]=='0'
		if(Message[1]=='1')		// 대기화면 채팅
		{
			if(pView->isGameStart)
			{
				CString msg=Message;
				msg = msg.Mid(2,msg.GetLength());
				pView->Message_Queue[(pView->Index_Queue++) % 5] = msg;		// 채팅 메세지 5개 까지 저장
				pView->Count_Message++;										// 채팅 메세지 카운트 증가
				pView->RePaint();
			}
		}// end Message[1]=='0'
		if(Message[1]=='2')		// 스테이지 변경
		{
			CString tmp=_T("");
			tmp+=Message[2];
			pView->GameState=_ttoi(tmp);
			pView->BG_x=0;
			pView->RePaint();
		}// end Message[1]=='0'
	}// end Message[0]=='@'

	else if(Message[0]=='$')
	{
		if(Message[1]=='0')
		{
			CString tmp[19]={0};			// 받는 %d 정보 +1
			int index=0;
			int count_object=0;
			BOOL flag_object=0;				// 0 유저 1 스킬 2 몬스터
			for(int i=2;i<length;i++)
			{ 	
					if(Message[i]=='/')
					{
						flag_object++;
						index=0;
						count_object=0;
					}
					if(flag_object==0)		// 유저 객체
					{
						if(Message[i]==space)
						{
								index++;
						}
						else if(Message[i]==and)
						{
							// 데이터입력
							pView->user[count_object].SetPoint(CPoint(_ttoi(tmp[1]),_ttoi(tmp[2])));
							pView->user[count_object].SetMAXHP(_ttoi(tmp[3]));
							pView->user[count_object].SetMAXMP(_ttoi(tmp[4]));
							pView->user[count_object].SetHP(_ttoi(tmp[5]));
							pView->user[count_object].SetMP(_ttoi(tmp[6]));
							pView->user[count_object].SetAct(_ttoi(tmp[7]));
							pView->user[count_object].SetDef(_ttoi(tmp[8]));
							pView->user[count_object].SetDirection(_ttoi(tmp[9])%10);		// 임시방편 버그 막기
							pView->user[count_object].SetGold(_ttoi(tmp[10]));
							pView->user[count_object].SetMAXExp(_ttoi(tmp[11]));
							pView->user[count_object].SetExp(_ttoi(tmp[12]));
							pView->user[count_object].SetSkillPoint(_ttoi(tmp[13]));
							pView->user[count_object].SetSkill1(_ttoi(tmp[14]));
							pView->user[count_object].SetSkill2(_ttoi(tmp[15]));
							pView->user[count_object].SetSkill3(_ttoi(tmp[16]));
							pView->user[count_object].SetLevel(_ttoi(tmp[17]));
							pView->user[count_object].SetStatusFlag(_ttoi(tmp[18]));
							index=0;
							for(int z=0;z<19;z++)	// 받는 %d 정보 +1
								tmp[z]=_T("");
							count_object++;
						}
						else
						{
							tmp[index]+=Message[i];
						}
					}
					else if(flag_object==1)		// 스킬 객체
					{
						if(Message[i]==space)
						{
								index++;
						}
						else if(Message[i]==and)
						{
							// 데이터입력
							pView->effect[count_object].SetInfo(_ttoi(tmp[1]),_ttoi(tmp[2]),CPoint(_ttoi(tmp[3]),_ttoi(tmp[4])));
							pView->effect[count_object].SetFrame(_ttoi(tmp[5]));
							pView->effect[count_object].SetUse(_ttoi(tmp[6]));
							index=0;
							for(int z=0;z<7;z++)	// 받는 %d 정보 +1
								tmp[z]=_T("");
								count_object++;
						}
						else
						{
							tmp[index]+=Message[i];
						}
					}
					else if(flag_object==2)		// 몬스터 객체
					{
						if(Message[i]==space)
						{
								index++;
						}
						else if(Message[i]==and)
						{
							// 데이터입력
							pView->monster[count_object].SetPoint(CPoint(_ttoi(tmp[1]),_ttoi(tmp[2])));
							pView->monster[count_object].SetJob(_ttoi(tmp[3]));
							pView->monster[count_object].SetMAXHP(_ttoi(tmp[4]));
							pView->monster[count_object].SetMAXMP(_ttoi(tmp[5]));
							pView->monster[count_object].SetHP(_ttoi(tmp[6]));
							pView->monster[count_object].SetMP(_ttoi(tmp[7]));
							pView->monster[count_object].SetAct(_ttoi(tmp[8]));
							pView->monster[count_object].SetDef(_ttoi(tmp[9]));
							pView->monster[count_object].SetDirection(_ttoi(tmp[10]));
							pView->monster[count_object].SetStatusFlag(_ttoi(tmp[11]));
							pView->monster[count_object].SetFRAME(_ttoi(tmp[12]));
							index=0;
							for(int z=0;z<13;z++)	// 받는 %d 정보 +1
								tmp[z]=_T("");
								count_object++;
						}
						else
						{
							tmp[index]+=Message[i];
						}
					}
					
				
			}
			pView->F_Command = FALSE;
		}// end Message[1]=='1'
		pView->RePaint();
	}// end Message[0]=='$'
}
