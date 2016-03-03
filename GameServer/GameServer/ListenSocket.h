#pragma once

// CListenSocket ��� ����Դϴ�.

class CListenSocket : public CAsyncSocket
{
public:
	CListenSocket();
	virtual ~CListenSocket();

	CPtrList m_ptrClientSocketList;
	virtual void OnAccept(int nErrorCode);
	void CloseClientSocket(CSocket* pClient);
	void BroadCast(TCHAR* pszMessage);
	void BroadCast(CString pszMessage);
	void NotifyOtherClient(CSocket* pClient,LPCTSTR pszMessage);
};


