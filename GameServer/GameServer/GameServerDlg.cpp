// GameServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "GameServer.h"
#include "GameServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 상수 정의
#define WAIT_TIME 2		// 게임 준비 시간

// 타이머
#define T_PRESTART 0
#define T_GAMESTART 1
#define T_MAIN 2
#define T_CLEAR 3
// 맵 크기
#define MAX_WIDTH 32
#define MAX_HEIGHT 11

#define MAX_SKILL 20	// 스킬 객체 갯수
#define MAX_MONSTER 20	// 몬스터 객체 갯수

#define MAX_STAGE 4

#define TIME_MONSTER_MOVE 10

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGameServerDlg 대화 상자




CGameServerDlg::CGameServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGameServerDlg::IDD, pParent)
	, m_Port(_T("21000"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isServerOn = FALSE;
	ConnectedClient = 0;
	ConnectedNumber = 0;

	InitMapInfo();
	for(int i=0;i<2;i++)				// 유저 객체 초기화
	{
		user[i].Init();
	}
	for(int i=0;i<MAX_SKILL;i++)		// 스킬객체 초기화
	{
		effect[i].Init();
	}
	for(int i=0;i<MAX_MONSTER;i++)
	{
		monster[i].Init();
	}
	t_monster_move=0;
	stage=1;
}

void CGameServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Log, m_Log);
	DDX_Text(pDX, IDC_Port, m_Port);
	DDV_MaxChars(pDX, m_Port, 5);
	DDX_Control(pDX, IDC_Open, OpenButton);
}

BEGIN_MESSAGE_MAP(CGameServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Open, &CGameServerDlg::OnBnClickedOpen)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGameServerDlg 메시지 처리기

BOOL CGameServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CGameServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CGameServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CGameServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/***********************************
			서버 기록 관련
***********************************/
void CGameServerDlg::AddMessage(LPCTSTR message)
{
	int len = m_Log.GetWindowTextLengthW();
	m_Log.SetSel(len,len);
	m_Log.ReplaceSel(message);
}
void CGameServerDlg::ClearMessage(void)
{
	m_Log.SetWindowTextW(_T(""));
}

void CGameServerDlg::OnBnClickedOpen()
{
	UpdateData();
	if(isServerOn)
	{
		m_ListenSocket.Close();
		OpenButton.SetWindowTextW(_T("서버열기"));
		isServerOn = FALSE;
	}
	else
	{
		UINT uPort = _ttoi(m_Port);
		if(m_ListenSocket.Create(uPort,SOCK_STREAM))
		{
			if(!m_ListenSocket.Listen())
			{
				AfxMessageBox(_T("ERROR : Listen()"));
			}
			OpenButton.SetWindowTextW(_T("서버닫기"));
			isServerOn = TRUE;
		}
		else
		{
			AfxMessageBox(_T("ERROR : Failed to create server socket!"));
		}
	}

}
int CGameServerDlg::GetConnectedClient(void)
{
	ConnectedClient = m_ListenSocket.m_ptrClientSocketList.GetCount();
	return ConnectedClient;
}

/********************************
	Message Decoder Function
********************************/


void CGameServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString msg;
	switch(nIDEvent)
	{
	case T_PRESTART:
		if(PreTimer>0)
			msg.Format(_T("@0%d초 후 게임이 시작됩니다.\n"),PreTimer);
		else
			msg.Format(_T("@0게임이 시작됩니다.\n"));
		m_ListenSocket.BroadCast(msg);

		PreTimer--;

		if(PreTimer<0)
		{
			KillTimer(T_PRESTART);
			GameStart();
		}
		break;
	case T_GAMESTART:
		msg.Format(_T("#4"));
		m_ListenSocket.BroadCast(msg);
		KillTimer(T_GAMESTART);
		SetTimer(T_MAIN,100,NULL);
		SetStage(stage);
		break;
	case T_MAIN:
		SendGamedataMessage();
		break;
	case T_CLEAR:
		SetStage(stage);
		SendStageSet();
		SetTimer(T_MAIN,100,NULL);
		KillTimer(T_CLEAR);
		stage++;
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CGameServerDlg::PreStart(void)
{
	SetTimer(T_PRESTART,1000,NULL);
	PreTimer = WAIT_TIME;
}

void CGameServerDlg::GameStart(void)
{
	SetTimer(T_GAMESTART,500,NULL);
}

void CGameServerDlg::SendGamedataMessage(void)
{
	// 정보 갱신
	UpdateMapInfo();
	// 유저 정보 갱신
	UpdateUserData();
	// 유저 정보 발송
	CString msg;
	// x y max_hp max_mp hp | mp act def dir gold | max_exp exp sp skl1 skl2 | skl3 level status [18]
	msg.Format(_T("$01 %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d&2 %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d&"),

		user[0].GetPoint().x,
		user[0].GetPoint().y,
		user[0].GetMAXHP(),
		user[0].GetMAXMP(),
		user[0].GetHP(),
		user[0].GetMP(),
		user[0].GetAct(),
		user[0].GetDef(),
		user[0].GetDirection(),
		user[0].GetGold(),
		user[0].GetMAXExp(),
		user[0].GetExp(),
		user[0].GetSkillPoint(),
		user[0].GetSkill1(),
		user[0].GetSkill2(),
		user[0].GetSkill3(),
		user[0].GetLevel(),
		user[0].GetStatusFlag(),

		user[1].GetPoint().x,
		user[1].GetPoint().y,
		user[1].GetMAXHP(),
		user[1].GetMAXMP(),
		user[1].GetHP(),
		user[1].GetMP(),
		user[1].GetAct(),
		user[1].GetDef(),
		user[1].GetDirection(),
		user[1].GetGold(),
		user[1].GetMAXExp(),
		user[1].GetExp(),
		user[1].GetSkillPoint(),
		user[1].GetSkill1(),
		user[1].GetSkill2(),
		user[1].GetSkill3(),
		user[1].GetLevel(),
		user[1].GetStatusFlag()
		);

	msg+=SendSkillData();
	msg+=SendMonsterData();
	m_ListenSocket.BroadCast(msg);
}
void CGameServerDlg::UpdateUserData(void)
{
	int count=0;
	for(int i=0;i<ConnectedClient;i++)
	{
		if(user[i].GetStatusFlag()==S_DIE)
		{
			count++;
		}
		if(user[i].GetHP()<=0)			// 죽음 처리
		{
			user[i].SetStatusFlag(S_DIE);
			user[i].SetHP(0);
		}
		if(user[i].GetExp()>=user[i].GetMAXExp())		// 레벨업 처리
		{
			user[i].LevelUp();
			AddSkillData(CPoint(user[i].GetPoint().x-1,user[i].GetPoint().y-2),499,10);
		}
	}
	if(count==ConnectedClient)
	{
		SendLoseMessage();
	}
}
int CGameServerDlg::SearchUser(CPoint point)
{
	int index=-1;
	for(int i=0;i<ConnectedClient;i++)
	{
		if(user[i].GetStatusFlag()==S_DIE)
			continue;
		if(user[i].GetPoint().x == point.x &&
			user[i].GetPoint().y == point.y && user[i].GetStatusFlag()!=S_DIE)
		{
			index=i;
			break;
		}
	}
	return index;
}


/********************************
		    맵 관련
*********************************/
void CGameServerDlg::InitMapInfo(void)
{
	// 맵 정보 초기화
	for(int i=0;i<MAX_WIDTH;i++)
		for(int j=0;j<MAX_HEIGHT;j++)
			map[i][j] = 0;
	for(int i=0;i<6;i++)
	{
		map[i][0] = 1;
		map[i][10] = 1;
	}
	for(int i=MAX_WIDTH-6;i<MAX_WIDTH;i++)
	{
		map[i][0] = 1;
		map[i][10] = 1;
	}
	map[5][1]=1,map[26][1]=1,map[5][2]=1,map[26][2]=1,map[5][3]=1,map[26][3]=1;
	map[5][7]=1,map[26][7]=1,map[5][8]=1,map[26][8]=1,map[5][9]=1,map[26][9]=1;
}

void CGameServerDlg::UpdateMapInfo(void)
{
	InitMapInfo();
	for(int i=0;i<ConnectedClient;i++)	// 유저 장애물 처리
		map[user[i].GetPoint().x][user[i].GetPoint().y]=TRUE;

	for(int i=0;i<MAX_MONSTER;i++)		// 몬스터 장애물 처리
	{
		if(monster[i].GetStatusFlag()!=S_DIE)
			map[monster[i].GetPoint().x][monster[i].GetPoint().y]=TRUE;
	}
	// 디버깅
	CString de;
	de = _T("******************Map Preview******************\r\n");
	for(int i=0;i<11;i++)
	{
		for(int j=0;j<32;j++)
		{
			CString tmp;
			tmp.Format(_T("%d"),map[j][i]);
			de+=tmp;
		}
		de+=_T("\r\n");
	}
	ClearMessage();
	AddMessage(de);
}
/********************************
		스킬/이펙트 관련
*********************************/
CString CGameServerDlg::SendSkillData(void)
{
	UpdateSkillData();
	CString msg;
	// maxframe resource x y frame use [6]
	msg=_T("/");
	for(int i=0;i<MAX_SKILL;i++)	// 스킬 객체 갯수
	{
		CString tmp = _T("");
		tmp.Format(_T(" %d %d %d %d %d %d&"),
			effect[i].GetMaxFrame(),
			effect[i].GetResourceID(),
			effect[i].GetPoint().x,
			effect[i].GetPoint().y,
			effect[i].GetFrame(),
			effect[i].GetUse());
		msg+=tmp;
	}
	return msg;
}

void CGameServerDlg::UpdateSkillData(void)
{
	for(int i=0;i<MAX_SKILL;i++)
	{
		if(effect[i].GetUse()==TRUE)
		{
			effect[i].SetFrame(effect[i].GetFrame()+1);
			if(effect[i].GetFrame()>effect[i].GetMaxFrame())
			{
				effect[i].SetFrame(0);
				effect[i].SetUse(FALSE);
			}
		}
	}
}

void CGameServerDlg::AddSkillData(CPoint point, UINT Resource, int MaxFrame)
{
	for(int i=0;i<MAX_SKILL;i++)
	{
		if(effect[i].GetUse()==FALSE)
		{
			effect[i].SetInfo(MaxFrame,Resource,point);
			effect[i].SetUse(TRUE);
			return;
		}
	}
}
/********************************
			몬스터 관련
*********************************/
void CGameServerDlg::SetMonsterData(int info[])
{
	int count=0;
	for(int i=0;i<MAX_MONSTER;i++)
		monster[i].Init();
	for(int i=0;i<5;i++)		// 몬스터 다섯종류에 대한 값만 받음 info[index]=value index=종류 value=숫자
	{
		for(int j=0;j<info[i];j++)
		{
			monster[count].SetMonster(i);
			monster[count].SetPoint(CPoint(20+count%10,5+count/10));	// 몹 배치
			monster[count].SetPrevPoint(monster[count].GetPoint());		// 이전 좌표 초기화
			if(count<20)
				count++;
		}
	}
}

void CGameServerDlg::UpdateMonsterData(void)
{
	t_monster_move++;
	int count=0;
	for(int i=0;i<MAX_MONSTER;i++)
	{
		if(monster[i].GetStatusFlag()==S_DIE)
			continue;
		if(monster[i].GetStatusFlag()==S_ATTACK || monster[i].GetStatusFlag()==S_MOVE)	// 프레임 증가
			monster[i].SetFRAME(monster[i].GetFRAME()+1);

		if(monster[i].GetFRAME()>3)	// 프레임 초기화
		{
			monster[i].SetFRAME(0);
			monster[i].SetStatusFlag(S_NONE);
		}
		if(monster[i].GetHP()<=0 && monster[i].GetStatusFlag()!=S_DIE)	// 죽음 처리
		{
			monster[i].SetStatusFlag(S_DIE);
			monster[i].SetPoint(CPoint(-1,-1));
			user[0].DefeatMonster(monster[i].GetGold(),monster[i].GetExp());
			user[1].DefeatMonster(monster[i].GetGold(),monster[i].GetExp());
		}
		count++;
	}
	if(count==0)
	{
		if(stage<=MAX_STAGE)
			SendClearMessage();
		else
			SendVictoryMessage();
	}
	if(t_monster_move == TIME_MONSTER_MOVE)
	{
		MonsterMoveAlgorithm();
		t_monster_move=0;
	}
}

CString CGameServerDlg::SendMonsterData(void)
{
	UpdateMonsterData();
	CString msg;
	msg=_T("/");
	// x y job max_hp max_mp hp mp act def dir status frame [12]
	for(int i=0;i<MAX_MONSTER;i++)	// 스킬 객체 갯수
	{
		CString tmp = _T("");
		tmp.Format(_T(" %d %d %d %d %d %d %d %d %d %d %d %d&"),
			monster[i].GetPoint().x,
			monster[i].GetPoint().y,
			monster[i].GetJob(),
			monster[i].GetMAXHP(),
			monster[i].GetMAXMP(),
			monster[i].GetHP(),
			monster[i].GetMP(),
			monster[i].GetAct(),
			monster[i].GetDef(),
			monster[i].GetDirection(),
			monster[i].GetStatusFlag(),
			monster[i].GetFRAME()
			);
		msg+=tmp;
	}
	return msg;
	return CString();
}

void CGameServerDlg::MonsterMoveAlgorithm(void)
{
	// 공격 처리 넣음
	for(int i=0;i<MAX_MONSTER;i++)
	{
		if(monster[i].GetStatusFlag()==S_NONE)
		{
			CPoint temp_length=CPoint(100,100);
			CPoint mon_p = monster[i].GetPoint();
			// 우선순위와의 거리 검사
			for(int j=0;j<ConnectedNumber;j++)
			{
				if(user[j].GetStatusFlag()==S_DIE)
					continue;
				CPoint temp=monster[i].GetLength(user[j].GetPoint());	// 몬스터 - 대상
				if(abs(temp_length.x) >= abs(temp.x) 
					|| abs(temp_length.y) >= abs(temp.y))	// 거리 비교후 값 변경
					temp_length=temp;
			}
			if(temp_length.x==100)
				break;
			// 공격 처리
			if(temp_length.y==0)
			{
				if(temp_length.x==1)
				{
					monster[i].SetDirection(WEST);
					int index = SearchUser(CPoint(mon_p.x-1,mon_p.y));
					if(index!=-1)
					{
						user[index].Hit(monster[i].GetAct());
						AddSkillData(user[i].GetPoint(),469,9);
						monster[i].SetStatusFlag(S_ATTACK);
						monster[i].SetFRAME(0);
					}
					continue;
				}
				else if(temp_length.x==-1)
				{
					monster[i].SetDirection(EAST);
					int index = SearchUser(CPoint(mon_p.x+1,mon_p.y));
					if(index!=-1)
					{
						user[index].Hit(monster[i].GetAct());
						AddSkillData(user[i].GetPoint(),469,9);
						monster[i].SetStatusFlag(S_ATTACK);
						monster[i].SetFRAME(0);
					}
					continue;
				}
			}
			else if(temp_length.x==0)
			{
				if(temp_length.y==1)
				{
					monster[i].SetDirection(NORTH);
					int index = SearchUser(CPoint(mon_p.x,mon_p.y-1));
					if(index!=-1)
					{
						user[index].Hit(monster[i].GetAct());
						AddSkillData(user[i].GetPoint(),469,9);
						monster[i].SetStatusFlag(S_ATTACK);
						monster[i].SetFRAME(0);
					}
					continue;
				}
				else if(temp_length.y==-1)
				{
					monster[i].SetDirection(SOUTH);
					int index = SearchUser(CPoint(mon_p.x,mon_p.y+1));
					if(index!=-1)
					{
						user[index].Hit(monster[i].GetAct());
						AddSkillData(user[i].GetPoint(),469,9);
						monster[i].SetStatusFlag(S_ATTACK);
						monster[i].SetFRAME(0);
					}
					continue;
				}
			}
			if(abs(temp_length.x) <= abs(temp_length.y) || temp_length.y==0)		// EAST/WEST 에 대한 검사를 우선으로
			{
				if(mon_p.x < MAX_WIDTH-1 && temp_length.x<0 && map[mon_p.x+1][mon_p.y]==FALSE 
					&& monster[i].GetPrevPoint().x!=mon_p.x+1)
				{
					map[mon_p.x][mon_p.y]=FALSE;
					map[mon_p.x+1][mon_p.y]=TRUE;
					monster[i].Move(EAST);
					continue;
				}
				else if(mon_p.x > 0 && map[mon_p.x-1][mon_p.y]==FALSE
					&& monster[i].GetPrevPoint().x!=mon_p.x-11)
				{
					map[mon_p.x][mon_p.y]=FALSE;
					map[mon_p.x-1][mon_p.y]=TRUE;
					monster[i].Move(WEST);
					continue;
				}
				else if(mon_p.y < MAX_HEIGHT && map[mon_p.x][mon_p.y+1]==FALSE
					&& monster[i].GetPrevPoint().y!=mon_p.y+1)
				{
					map[mon_p.x][mon_p.y]=FALSE;
					map[mon_p.x][mon_p.y+1]=TRUE;
					monster[i].Move(SOUTH);
					continue;
				}
				else if(mon_p.y >0 && map[mon_p.x][mon_p.y-1]==FALSE
					&& monster[i].GetPrevPoint().y!=mon_p.y-1)
				{
					map[mon_p.x][mon_p.y]=FALSE;
					map[mon_p.x][mon_p.y-1]=TRUE;
					monster[i].Move(NORTH);
					continue;
				}
			}
			else if(abs(temp_length.y) <= abs(temp_length.x) || temp_length.x==0)	// NORTH/SOUTH 에 대한 검사
			{
				if(mon_p.y < MAX_HEIGHT && temp_length.y<0 && map[mon_p.x][mon_p.y+1]==FALSE
					&& monster[i].GetPrevPoint().y!=mon_p.y+1)
				{
					map[mon_p.x][mon_p.y]=FALSE;
					map[mon_p.x][mon_p.y+1]=TRUE;
					monster[i].Move(SOUTH);
					continue;
				}
				else if( mon_p.y > 0 && map[mon_p.x][mon_p.y-1]==FALSE
					&& monster[i].GetPrevPoint().y!=mon_p.y-1)
				{
					map[mon_p.x][mon_p.y]=FALSE;
					map[mon_p.x][mon_p.y-1]=TRUE;
					monster[i].Move(NORTH);
					continue;
				}
				else if(mon_p.x < MAX_WIDTH-1 && map[mon_p.x+1][mon_p.y]==FALSE
					&& monster[i].GetPrevPoint().x!=mon_p.x+1)
				{
					map[mon_p.x][mon_p.y]=FALSE;
					map[mon_p.x+1][mon_p.y]=TRUE;
					monster[i].Move(EAST);
					continue;
				}
				else if(mon_p.x > 0 && map[mon_p.x-1][mon_p.y]==FALSE
					&& monster[i].GetPrevPoint().x!=mon_p.x-1)
				{
					map[mon_p.x][mon_p.y]=FALSE;
					map[mon_p.x-1][mon_p.y]=TRUE;
					monster[i].Move(WEST);
					continue;
				}
			}
		}
		// 모든 조건 없을시 이전 포인터 변경
		monster[i].SetPrevPoint(monster[i].GetPoint());
	}
}


int CGameServerDlg::SearchMonster(CPoint point)
{
	int index=-1;
	for(int i=0;i<MAX_MONSTER;i++)
	{
		if(monster[i].GetPoint().x == point.x &&
			monster[i].GetPoint().y == point.y && monster[i].GetStatusFlag()!=S_DIE)
		{
			index=i;
			break;
		}
	}
	return index;
}


void CGameServerDlg::SendLoseMessage(void)
{
	CString msg=_T("@20");
	m_ListenSocket.BroadCast(msg);
	KillTimer(T_MAIN);
}
void CGameServerDlg::SendVictoryMessage(void)
{
	CString msg=_T("@21");
	m_ListenSocket.BroadCast(msg);
	KillTimer(T_MAIN);
}
void CGameServerDlg::SendClearMessage(void)
{
	CString msg=_T("@22");
	m_ListenSocket.BroadCast(msg);
	KillTimer(T_MAIN);
	SetTimer(T_CLEAR,3000,NULL);
}
void CGameServerDlg::SendStageSet(void)
{
	CString msg=_T("@29");
	m_ListenSocket.BroadCast(msg);
	KillTimer(T_MAIN);
}
void CGameServerDlg::SetStage(int stage)
{
	int stage1[5]={10,0,0,0,0};
	int stage2[5]={8,2,1,0,0};
	int stage3[5]={2,6,2,2,0};
	int stage4[5]={0,0,0,10,10};

	for(int i=0;i<ConnectedClient;i++)
	{
		user[i].SetPoint(CPoint(i,2));
		user[i].SetMP(user[i].GetMAXMP());
		switch(stage)
		{
		case 1:
			SetMonsterData(stage1);		// 스테이지 1은 두번 실행됨
			break;
		case 2:
			SetMonsterData(stage2);
			break;
		case 3:
			SetMonsterData(stage3);
			break;
		case 4:
			SetMonsterData(stage4);
			break;
		}
		
	}
}


