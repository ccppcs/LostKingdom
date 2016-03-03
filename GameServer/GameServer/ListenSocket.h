#pragma once

// CListenSocket 명령 대상입니다.

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


