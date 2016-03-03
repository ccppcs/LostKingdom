// ClientSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameServer.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "GameServerDlg.h"
// CClientSocket

#define MAXCLIENT 2		// �ִ� ������

//����
#define EAST 0  // ����
#define WEST 1  // ����
#define SOUTH 2  // ����
#define NORTH 3  // ����

CClientSocket::CClientSocket() 
{
}

CClientSocket::~CClientSocket()
{
}


// CClientSocket ��� �Լ�

void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}

void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}

void CClientSocket::OnReceive(int nErrorCode)
{
	CString strTmp = _T(""),strIPAddress = _T("");
	UINT uPortNumber = 0;

	TCHAR szBuffer[2048];
	::ZeroMemory(szBuffer,sizeof(szBuffer));

	GetPeerName(strIPAddress,uPortNumber);

	if(Receive(szBuffer,sizeof(szBuffer))>0)
	{
		CGameServerDlg* pMain = (CGameServerDlg*)AfxGetMainWnd();

		// �޼��� �ص�
		if(MessageDecoder(szBuffer,lstrlen(szBuffer))==0)	// client
		{
			strTmp.Format(_T("[%s:%d] : %s\r\n"),strIPAddress,uPortNumber,szBuffer);
			pMain->AddMessage(strTmp);
		}
		else												// broadcast (chatting)
		{
			CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
			pServerSocket->BroadCast(szBuffer);
			pMain->AddMessage(szBuffer);
		}
	}
		

	CSocket::OnReceive(nErrorCode);
}

int CClientSocket::MessageDecoder(LPCTSTR Message,int length)			// ���� �޼��� �ص���
{
	CGameServerDlg* pMain = (CGameServerDlg*)AfxGetMainWnd();
	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;

	const CString space = _T(" ");
	CString msg = _T("");
	/******************************

	----$----   Client   ----------
		$1 �α��� �޼���
		$2 ���ȭ�� �غ�Ϸ�
	----@---- Broadcast -----------
		@0 ä�� �޼���
	----#---- GameMessage ---------
		#0 �̵� ��û
		#1 ���� ��� ��û
		#2 ���� ���� ��û
		#3 ��ų�� ��û
		#4 ���� ���� ��û
		#5 ���� ��û
		#6 ��ų ��û
	*******************************/
	if(Message[0]=='$')
	{
		if(Message[1]=='1')
		{
			CString ID=_T("");
			for(int i=2;i<length;i++)
			{
				ID+=Message[i];
			}
			if(pMain->GetConnectedClient()<=MAXCLIENT)
			{
				msg = _T("#1");
			}
			else
			{
				msg = _T("#0");
				pMain->AddMessage(_T("�ִ� ������ �ʰ�"));
			}
			pMain->AddMessage(_T("�α��� ���� �õ�\r\n"));
		}// end Message[1]=='1'

		else if(Message[1]=='2')
		{
			if(Message[2]=='0')
			{
				pMain->ConnectedNumber--;
				return 0;
			}
			msg = _T("#2");
			msg += Message[2];

			// ���� �޾Ƽ� �Ҵ�
			CString temp=_T("");
			temp+=Message[2];
			int temp_job = _ttoi(temp);
			pMain->user[pMain->ConnectedNumber].InitData(temp_job,pMain->ConnectedNumber);

			pServerSocket->NotifyOtherClient(this,msg);			
			msg.Format(_T("#3%d"),++pMain->ConnectedNumber);
			this->Send(msg,lstrlen(msg)*2);

			if(pMain->ConnectedNumber>=MAXCLIENT)
				pMain->PreStart();
			return 0;
		}// end Message[1]=='2'
		this->Send(msg,sizeof(msg)*2);
		return 0;
	}// end Message[0]=='$'

	else if(Message[0]=='#')
	{
		if(Message[1]=='0')
		{
			CString Num = _T("");
			Num += Message[2];
			int num = _ttoi(Num)-1;
			Num =_T("");
			Num += Message[3];

			int dir = _ttoi(Num);
			// ��ֹ� �˻�
			int result = 0;
			switch(dir)
			{
			case EAST:
				if(pMain->map[pMain->user[num].GetPoint().x+1][pMain->user[num].GetPoint().y]==0)
					result=1;
				break;
			case WEST:
				if(pMain->map[pMain->user[num].GetPoint().x-1][pMain->user[num].GetPoint().y]==0)
					result=1;
				break;
			case SOUTH:
				if(pMain->map[pMain->user[num].GetPoint().x][pMain->user[num].GetPoint().y+1]==0)
					result=1;
				break;
			case NORTH:
				if(pMain->map[pMain->user[num].GetPoint().x][pMain->user[num].GetPoint().y-1]==0)
					result=1;
				break;
			}
			if(result)
				pMain->user[num].Move(dir);
			else
				pMain->user[num].SetDirection(dir);
		}
		if(Message[1]=='1')
		{
			CString tmp[4] = {0};
			int count=0;
			for(int i=4;i<length;i++)
			{
				if(Message[i]==space)
				{
					count++;
				}
				else
				{
					tmp[count]+=Message[i];
				}
			}
			pMain->AddSkillData(CPoint(_ttoi(tmp[0]),_ttoi(tmp[1])),_ttoi(tmp[2]),_ttoi(tmp[3]));

			CString Num = _T("");
			Num += Message[2];
			int num = _ttoi(Num)-1;
			Num =_T("");
			Num += Message[3];
			pMain->user[num].UsePotion((_ttoi(Num)));
		}
		if(Message[1]=='2')
		{
			CString Num = _T("");
			Num += Message[2];
			int num = _ttoi(Num)-1;
			Num =_T("");
			Num += Message[3];

			pMain->user[num].StatusUp(_ttoi(Num));
		}
		if(Message[1]=='3')
		{
			CString Num = _T("");
			Num += Message[2];
			int num = _ttoi(Num)-1;
			Num =_T("");
			Num += Message[3];

			pMain->user[num].SkillUp(_ttoi(Num));
		}
		if(Message[1]=='4')
		{
			CString Num = _T("");
			Num += Message[2];
			int num = _ttoi(Num)-1;
			Num =_T("");
			Num += Message[3];
			pMain->user[num].SetStatusFlag((_ttoi(Num)));
		}
		if(Message[1]=='5')
		{
			CString Num = _T("");
			Num += Message[2];
			int num = _ttoi(Num)-1;
			Num =_T("");
			Num += Message[3];
			pMain->user[num].Attack((_ttoi(Num)));
			// ���� ���� ó��
			CPoint temp = pMain->user[num].GetPoint();
			switch(pMain->user[num].GetDirection())
			{
			case EAST:
				temp.x+=1;
				break;
			case WEST:
				temp.x-=1;
				break;
			case NORTH:
				temp.y-=1;
				break;
			case SOUTH:
				temp.y+=1;
				break;
			}
			int index=pMain->SearchMonster(temp);
			if(index!=-1 && pMain->monster[index].GetStatusFlag()!=S_DIE)
			{
				pMain->monster[index].Hit(pMain->user[num].GetAct());
				pMain->AddSkillData(pMain->monster[index].GetPoint(),469,9);
			}
		}
		if(Message[1]=='6')
		{
			CString tmp[4] = {0};
			int count=0;
			for(int i=4;i<length;i++)
			{
				if(Message[i]==space)
				{
					count++;
				}
				else
				{
					tmp[count]+=Message[i];
				}
			}
			pMain->AddSkillData(CPoint(_ttoi(tmp[0]),_ttoi(tmp[1])),_ttoi(tmp[2]),_ttoi(tmp[3]));		
			CString Num = _T("");
			Num += Message[2];
			int num = _ttoi(Num)-1;
			Num =_T("");
			Num += Message[3];
			pMain->user[num].SetStatusFlag(S_ATTACK);
			pMain->user[num].UseSkill((_ttoi(Num)));
			
			// ���� ��ų ���� ó��
			CPoint temp = pMain->user[num].GetPoint();
			switch(pMain->user[num].GetDirection())
			{
			case EAST:
				temp.x+=1;
				break;
			case WEST:
				temp.x-=1;
				break;
			case NORTH:
				temp.y-=1;
				break;
			case SOUTH:
				temp.y+=1;
				break;
			}
			int index=pMain->SearchMonster(temp);
			if(index!=-1 && pMain->monster[index].GetStatusFlag()!=S_DIE)
			{
				pMain->monster[index].Hit(pMain->user[num].GetSkillDamage(_ttoi(Num)));
			}

		}
		return 0;
	}// end Message[0]=='#'
	// ����׿�
	else if(Message[0]=='*')
	{
		CString t=_T("");
		t+=Message[1];
		pMain->user[_ttoi(t)].SetAct(pMain->user[_ttoi(t)].GetAct()+1000);
		pMain->user[_ttoi(t)].SetDef(pMain->user[_ttoi(t)].GetDef()+1000);
	}
	return -1;

}
