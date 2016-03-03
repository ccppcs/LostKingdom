// GameServerDlg.h : ��� ����
//
#include "ListenSocket.h"
#include "fighter.h"
#include "Skill.h"
#include "Monster.h"

#pragma once#include "afxwin.h"

// CGameServerDlg ��ȭ ����
class CGameServerDlg : public CDialog
{
// �����Դϴ�.
public:
	CGameServerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	CListenSocket m_ListenSocket;
	BOOL isServerOn;

	int ConnectedNumber;					// ������ ����ī����
	int ConnectedClient;					// ���ӵ� ���� ī����

	int PreTimer;							//PreStart()���� �� Ÿ�̸� ī��Ʈ

	fighter user[2];						// ���ΰ� ��ü
	int map[32][11];						// �� ����
	Skill effect[20];						// ��ų/����Ʈ ��ü
	Monster monster[20];					// ���� ��ü

	int t_monster_move;						// ���� �̵� ����
	int stage;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GAMESERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
