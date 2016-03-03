// GameClientView.h : CGameClientView Ŭ������ �������̽�
//


#pragma once
#include "ConnectSocket.h"
#include "LoginDlg.h"
#include "WaitRoomDlg.h"
#include "GameChat.h"
#include "BufferDC.h"

#include "fighter.h"
#include "Skill.h"
#include "Monster.h"

class CGameClientView : public CView
{
protected: // serialization������ ��������ϴ�.
	CGameClientView();
	DECLARE_DYNCREATE(CGameClientView)

// Ư���Դϴ�.
public:
	CGameClientDoc* GetDocument() const;

// �۾��Դϴ�.
public:
	/********* ���� ���� *********/
	CString ID;					// �г���
	BOOL GameState;				// ���� ����
	int ConnectedNumber;		// ������ ����

	int isPlayedBGMusic;		// ���� ��������
	/********* ��ȭ ���� *********/
	BOOL isConnect;				// ���ӻ���
	BOOL isLogin;				// �α��� ����
	BOOL usedLoginForm;			// �α���â �ߺ����� ����
	BOOL isGameStart;			// ���� ���� ����

	CConnectSocket m_socket;	// ����
	LoginDlg Login_dlg;			// �α��� ��ȭ����
	WaitRoomDlg WaitRoom_dlg;	// ���ȭ�� ��ȭ����
	
	/********* ä�� ���� *********/
	CGameChat m_Chat;			// CEdit
	int Count_Message;
	CString Message_Queue[5];	// ü�ø޼��� ť 5��
	int Index_Queue;

	/******** ��ü *******/
	fighter user[2];
	Skill effect[20];
	Monster monster[20];
	/******* ���� ********/
	BOOL F_Command;				// ��� ���ȴ���
	BOOL F_BGMove;				// �� �̵� ��������
	int BG_x;					// �� ��ǥ
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CGameClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	void Login(CString ID);

	void OpenLoginForm(void);
	afx_msg void OnPaint();
	void OpenWaitRoomDlg(void);
	void Ready(int class_select);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void DrawChattingBox(CBufferDC* dc);
	void DrawChattingMsg(CBufferDC* dc);
	void RePaint(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void DrawBG(CBufferDC* dc);
	void DrawStatusBar(CBufferDC* dc);
	void DrawInterface(CBufferDC* dc);
	void DrawInfo(CBufferDC* dc);
	void DrawFighter(CBufferDC* dc);
	void SendMoveMessage(int direction);
	void SendStatusMessage(int status);
	void UsePotion(int select);
	void StatusUp(int select);
	void SkillUp(int select);
	void SendAttackMessage(int direction);
	void DrawEffect(CBufferDC* dc);
	void SendSkillMessage(CPoint point,UINT Resource, int MAXFrame, int select);
	void DrawMonster(CBufferDC* dc);

	void SendSkillMessage2(CPoint point,UINT Resource, int MAXFrame, int select);
	void UseSkill1(void);
	void UseSkill2(void);
	void UseSkill3(void);
	void DrawWinLoseClear(CBufferDC* dc);
	void DebugPower(void);

	void PlayMusic(LPCWSTR Music_Path);
	void PauseMusic(LPCWSTR Music_Path);
	void ResumeMusic(LPCWSTR Music_Path);
	void StopMusic(LPCWSTR Music_Path);
};

#ifndef _DEBUG  // GameClientView.cpp�� ����� ����
inline CGameClientDoc* CGameClientView::GetDocument() const
   { return reinterpret_cast<CGameClientDoc*>(m_pDocument); }
#endif

