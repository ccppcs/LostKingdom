// GameServerDlg.h : 헤더 파일
//
#include "ListenSocket.h"
#include "fighter.h"
#include "Skill.h"
#include "Monster.h"

#pragma once#include "afxwin.h"

// CGameServerDlg 대화 상자
class CGameServerDlg : public CDialog
{
// 생성입니다.
public:
	CGameServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	CListenSocket m_ListenSocket;
	BOOL isServerOn;

	int ConnectedNumber;					// 접속자 순서카운팅
	int ConnectedClient;					// 접속된 숫자 카운팅

	int PreTimer;							//PreStart()에서 쓸 타이머 카운트

	fighter user[2];						// 주인공 객체
	int map[32][11];						// 맵 정보
	Skill effect[20];						// 스킬/이펙트 객체
	Monster monster[20];					// 몬스터 객체

	int t_monster_move;						// 몬스터 이동 지연
	int stage;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_GAMESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void AddMessage(LPCTSTR message);
	CEdit m_Log;
	CString m_Port;
	afx_msg void OnBnClickedOpen();
	CButton OpenButton;
	int GetConnectedClient(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void PreStart(void);
	void GameStart(void);

	
	void SendGamedataMessage(void);
	void InitMapInfo(void);
	void UpdateMapInfo(void);
	CString SendSkillData(void);
	void UpdateSkillData(void);
	void AddSkillData(CPoint point, UINT Resource, int MaxFrame);
	void SetMonsterData(int info[]);
	void UpdateMonsterData(void);
	CString SendMonsterData(void);
	void MonsterMoveAlgorithm(void);
	void ClearMessage(void);
	int SearchMonster(CPoint point);
	int SearchUser(CPoint point);
	void UpdateUserData(void);
	void SendLoseMessage(void);
	void SendVictoryMessage(void);
	void SetStage(int stage);
	void SendClearMessage(void);
	void SendStageSet(void);
};
