// GameClient.h : GameClient ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CGameClientApp:
// �� Ŭ������ ������ ���ؼ��� GameClient.cpp�� �����Ͻʽÿ�.
//

class CGameClientApp : public CWinApp
{
public:
	CGameClientApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGameClientApp theApp;