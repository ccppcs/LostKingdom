// GameClientView.h : CGameClientView 클래스의 인터페이스
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
protected: // serialization에서만 만들어집니다.
	CGameClientView();
	DECLARE_DYNCREATE(CGameClientView)

// 특성입니다.
public:
	CGameClientDoc* GetDocument() const;

// 작업입니다.
public:
	/********* 범용 변수 *********/
	CString ID;					// 닉네임
	BOOL GameState;				// 게임 상태
	int ConnectedNumber;		// 접속한 순서

	int isPlayedBGMusic;		// 음악 나오는지
	/********* 대화 상자 *********/
	BOOL isConnect;				// 접속상태
	BOOL isLogin;				// 로그인 상태
	BOOL usedLoginForm;			// 로그인창 중복생성 방지
	BOOL isGameStart;			// 게임 실행 상태

	CConnectSocket m_socket;	// 소켓
	LoginDlg Login_dlg;			// 로그인 대화상자
	WaitRoomDlg WaitRoom_dlg;	// 대기화면 대화상자
	
	/********* 채팅 관련 *********/
	CGameChat m_Chat;			// CEdit
	int Count_Message;
	CString Message_Queue[5];	// 체팅메세지 큐 5개
	int Index_Queue;

	/******** 객체 *******/
	fighter user[2];
	Skill effect[20];
	Monster monster[20];
	/******* 개인 ********/
	BOOL F_Command;				// 명령 내렸는지
	BOOL F_BGMove;				// 맵 이동 상태인지
	int BG_x;					// 맵 좌표
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CGameClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // GameClientView.cpp의 디버그 버전
inline CGameClientDoc* CGameClientView::GetDocument() const
   { return reinterpret_cast<CGameClientDoc*>(m_pDocument); }
#endif

