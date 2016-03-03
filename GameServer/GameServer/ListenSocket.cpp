// ListenSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GameServer.h"
#include "ListenSocket.h"
#include "ClientSocket.h"

// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket ��� �Լ�

void CListenSocket::OnAccept(int nErrorCode)
{
	CClientSocket* pClient = new CClientSocket;
	if(Accept(*pClient))
	{
		pClient->SetListenSocket(this);
		m_ptrClientSocketList.AddTail(pClient);
	}
	else
	{
		delete pClient;
		AfxMessageBox(_T("ERROR : �� Ŭ���̾�Ʈ�� ���ӿ� ����"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pClient)
{
	POSITION pos;
	pos = m_ptrClientSocketList.Find(pClient);
	if(pos != NULL)
	{
		if(pClient != NULL)
		{
			pClient->ShutDown();
			pClient->Close();
		}

		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
}

void CListenSocket::BroadCast(TCHAR* pszMessage)
{
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while(pos!=NULL)
	{
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if(pClient != NULL)
		{
			pClient->Send(pszMessage,lstrlen(pszMessage) * 2);
		}
	}
}
void CListenSocket::BroadCast(CString pszMessage)
{
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while(pos!=NULL)
	{
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if(pClient != NULL)
		{
			pClient->Send(pszMessage,lstrlen(pszMessage) * 2);
		}
	}
}
void CListenSocket::NotifyOtherClient(CSocket* pClient,LPCTSTR pszMessage)
{
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pOtherClient = NULL;

	while(pos!=NULL)
	{
		pOtherClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if(pOtherClient != NULL && pOtherClient != pClient)
		{
			pOtherClient->Send(pszMessage,lstrlen(pszMessage) * 2);
		}
	}
}
