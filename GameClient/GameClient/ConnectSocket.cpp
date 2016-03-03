// ConnectSocket.cpp : ���� �����Դϴ�.
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


// CConnectSocket ��� �Լ�

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
		
		// �޼��� �ص�
		MessageDecoder(szBuffer,lstrlen(szBuffer));
	}

	CSocket::OnReceive(nErrorCode);
}

void CConnectSocket::MessageDecoder(LPCTSTR Message,int length)		// Ŭ���̾�Ʈ �޼��� �ص���
{
	CGameClientView* pView = (CGameClientView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();		// Get ViewClass Pointer
	CString space = _T(" ");
	CString and = _T("&");
	/******************************

	----#----  Personal -----------
		#0 �α��� ����
		#1 �α��� ����
		#2 ���ȭ�� ���þ˸�
		#3 ���� ���� �ޱ�
		#4 ���� ���� ��ȣ
	----@---- Broadcast -----------
		@0 ���ȭ�� ä��
		@1 ����ȭ�� ä��
		@2 ���� �¸�/�й�
	----$---- GameMessage ---------
		$
	*******************************/

	if(Message[0]=='#')
	{
		if(Message[1]=='0')
		{
			AfxMessageBox(_T("�α��� ����"));
		}// end Message[1]=='0'
		else if(Message[1]=='1')
		{
			pView->isLogin=TRUE;
			AfxMessageBox(_T("�α��� ����\nȯ���մϴ�"));
			pView->Login_dlg.LoginOk();		// �α����� �ݱ�

		}// end Message[1]=='1'

		else if(Message[1]=='2')
		{
			CString tmp = _T("");
			tmp+=Message[2];

			pView->WaitRoom_dlg.m_OtherSelected= _ttoi(tmp);
			pView->WaitRoom_dlg.UpdateSeleted();			// ���� ��Ȳ �ֽ�ȭ

		}// end Message[1]=='2'

		else if(Message[1]=='3')
		{
			pView->ConnectedNumber = _ttoi(&Message[2]);	// ������ ���� ���� ����

		}// end Message[1]=='3'

		else if(Message[1]=='4')
		{
			pView->isGameStart = TRUE;			// ���� ���� ���·κ���
			pView->WaitRoom_dlg.DlgClose();
			::SetWindowPos(AfxGetMainWnd()->m_hWnd, HWND_TOPMOST, 50, 50, 1250, 890, SWP_FRAMECHANGED);	// 1200 x 840 �� ũ�⺯��
		}// end Message[1]=='4'
	}// end Message[0]=='#'
	else if(Message[0]=='@')
	{
		if(Message[1]=='0')		// ���ȭ�� ä��
		{
			if(pView->isGameStart != TRUE)
			{
				CString msg=Message;
				pView->WaitRoom_dlg.AddMessage(msg.Mid(2,msg.GetLength()));
				pView->WaitRoom_dlg.UpdateSeleted();
			}
		}// end Message[1]=='0'
		if(Message[1]=='1')		// ���ȭ�� ä��
		{
			if(pView->isGameStart)
			{
				CString msg=Message;
				msg = msg.Mid(2,msg.GetLength());
				pView->Message_Queue[(pView->Index_Queue++) % 5] = msg;		// ä�� �޼��� 5�� ���� ����
				pView->Count_Message++;										// ä�� �޼��� ī��Ʈ ����
				pView->RePaint();
			}
		}// end Message[1]=='0'
		if(Message[1]=='2')		// �������� ����
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
			CString tmp[19]={0};			// �޴� %d ���� +1
			int index=0;
			int count_object=0;
			BOOL flag_object=0;				// 0 ���� 1 ��ų 2 ����
			for(int i=2;i<length;i++)
			{ 	
					if(Message[i]=='/')
					{
						flag_object++;
						index=0;
						count_object=0;
					}
					if(flag_object==0)		// ���� ��ü
					{
						if(Message[i]==space)
						{
								index++;
						}
						else if(Message[i]==and)
						{
							// �������Է�
							pView->user[count_object].SetPoint(CPoint(_ttoi(tmp[1]),_ttoi(tmp[2])));
							pView->user[count_object].SetMAXHP(_ttoi(tmp[3]));
							pView->user[count_object].SetMAXMP(_ttoi(tmp[4]));
							pView->user[count_object].SetHP(_ttoi(tmp[5]));
							pView->user[count_object].SetMP(_ttoi(tmp[6]));
							pView->user[count_object].SetAct(_ttoi(tmp[7]));
							pView->user[count_object].SetDef(_ttoi(tmp[8]));
							pView->user[count_object].SetDirection(_ttoi(tmp[9])%10);		// �ӽù��� ���� ����
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
							for(int z=0;z<19;z++)	// �޴� %d ���� +1
								tmp[z]=_T("");
							count_object++;
						}
						else
						{
							tmp[index]+=Message[i];
						}
					}
					else if(flag_object==1)		// ��ų ��ü
					{
						if(Message[i]==space)
						{
								index++;
						}
						else if(Message[i]==and)
						{
							// �������Է�
							pView->effect[count_object].SetInfo(_ttoi(tmp[1]),_ttoi(tmp[2]),CPoint(_ttoi(tmp[3]),_ttoi(tmp[4])));
							pView->effect[count_object].SetFrame(_ttoi(tmp[5]));
							pView->effect[count_object].SetUse(_ttoi(tmp[6]));
							index=0;
							for(int z=0;z<7;z++)	// �޴� %d ���� +1
								tmp[z]=_T("");
								count_object++;
						}
						else
						{
							tmp[index]+=Message[i];
						}
					}
					else if(flag_object==2)		// ���� ��ü
					{
						if(Message[i]==space)
						{
								index++;
						}
						else if(Message[i]==and)
						{
							// �������Է�
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
							for(int z=0;z<13;z++)	// �޴� %d ���� +1
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
