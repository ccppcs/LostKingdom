// GameClientView.cpp : CGameClientView 클래스의 구현
//

#include "stdafx.h"
#include "GameClient.h"

#include "GameClientDoc.h"
#include "GameClientView.h"
#include "ServerConnectDlg.h"
#include <atlimage.h>

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

#define M_START _T("res/sound/after.mp3")					//낮배경음
#define M_NIGHT _T("res/sound/night1.mp3")					//밤배경음
#define M_GAMEOVER _T("res/sound/gameover.mp3")				//게임오버
#define M_VICTORY _T("res/sound/clear.mp3")					//승리
#define M_USEPOTION _T("res/sound/e2.mp3")					//포션사용
#define M_USESKILLa1 _T("res/sound/s3.mp3")					//궁수1~전사3 스킬//
#define M_USESKILLa2 _T("res/sound/s5.mp3")
#define M_USESKILLa3 _T("res/sound/s8.mp3")
#define M_USESKILLb1 _T("res/sound/s2.mp3")
#define M_USESKILLb2 _T("res/sound/s6.mp3")
#define M_USESKILLb3 _T("res/sound/s1.mp3")							//이까지
#define M_ATK _T("res/sound/acc.mp3")							//공격
#define M_HIT _T("res/sound/e1.mp3")							//맞을때
#define M_LEVELUP _T("res/sound/e3.mp3")							//레벨업

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//방향
#define EAST 0  // 동쪽
#define WEST 1  // 서쪽
#define SOUTH 2  // 남쪽
#define NORTH 3  // 북쪽

#define SIZE_WIDTH 70
#define SIZE_HEIGHT 60

#define BGMOVE_WIDTH 9

#define MAX_WIDTH 32
#define MAX_HEIGHT 11

#define MAX_SKILL 20	// 스킬 객체 갯수
#define MAX_MONSTER 20	// 몬스터 객체 갯수

#define M_SLIME 0
#define M_SKELETON 1
#define M_GOLEM 2
#define M_VAMPIRE 3
#define M_KNIGHT 4

#define G_LOSE 0
#define G_VICTORY 1
#define G_GAME 9
#define G_CLEAR 2

// CGameClientView

IMPLEMENT_DYNCREATE(CGameClientView, CView)

BEGIN_MESSAGE_MAP(CGameClientView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_CREATE()
ON_WM_KEYDOWN()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGameClientView 생성/소멸

CGameClientView::CGameClientView()
{
	isConnect=FALSE;
	isLogin=FALSE;
	usedLoginForm=FALSE;
	isGameStart = FALSE;
	
	Count_Message = 0;
	Index_Queue = 0;

	F_Command = FALSE;
	F_BGMove = FALSE;
	BG_x=0;
	GameState=G_GAME;
	isPlayedBGMusic=FALSE;
}

CGameClientView::~CGameClientView()
{
}

BOOL CGameClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}


// CGameClientView 그리기

void CGameClientView::OnDraw(CDC* /*pDC*/)
{
	CGameClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


// CGameClientView 인쇄

BOOL CGameClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CGameClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CGameClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CGameClientView 진단

#ifdef _DEBUG
void CGameClientView::AssertValid() const
{
	CView::AssertValid();
}

void CGameClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameClientDoc* CGameClientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameClientDoc)));
	return (CGameClientDoc*)m_pDocument;
}
#endif //_DEBUG

/********************************************
		CGameClientView 메시지 처리기
*********************************************/
/********************************************
		OnPaint() 메시지 처리기
*********************************************/
void CGameClientView::OnPaint()
{
	CBufferDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.

	/****************************************
				대화상자 출력
	*****************************************/
	if(!usedLoginForm)
		OpenLoginForm();
	if(isLogin&&isConnect&&!isGameStart)
		OpenWaitRoomDlg();
	/****************************************
				배경음 출력
	*****************************************/
	else if(!isPlayedBGMusic)
	{
		PlayMusic(M_START);
		isPlayedBGMusic=TRUE;
	}

	/***************************************
				그리기 작업
	****************************************/
	DrawBG(&dc);
	DrawInterface(&dc);

	DrawFighter(&dc);
	DrawEffect(&dc);
	DrawMonster(&dc);

	DrawWinLoseClear(&dc);
}	
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Chat.Create(WS_BORDER
		,CRect(42,810,595,835),this,123);		// 채팅 입력창
	return 0;
}

void CGameClientView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_RETURN)
	{
		m_Chat.ShowWindow(SW_SHOW);
		m_Chat.SetFocus();
	}

	// 화면 새로고침
	if(nChar == 'R' || nChar=='r' && GetKeyState(VK_CONTROL))		
	{
		if((user[ConnectedNumber-1].GetPoint().x>=BGMOVE_WIDTH) && 
			(user[ConnectedNumber-1].GetPoint().x<=MAX_WIDTH-BGMOVE_WIDTH-1) )
		{
			BG_x = user[ConnectedNumber-1].GetPoint().x-BGMOVE_WIDTH;
			F_BGMove=TRUE;
		}
		else if(user[ConnectedNumber-1].GetPoint().x>MAX_WIDTH-BGMOVE_WIDTH-1)
		{
			BG_x = BG_x=MAX_WIDTH-BGMOVE_WIDTH*2-1;
			F_BGMove=FALSE;
		}
		else
		{
			BG_x = 0;
			F_BGMove=FALSE;
		}
		RePaint();
	}

	if(!F_Command && user[ConnectedNumber-1].GetStatusFlag()!=S_DIE)
	{
		F_Command = TRUE;
		switch (nChar)
		{
		/*************************
				방향키
		*************************/	
		case VK_UP:
			if(user[ConnectedNumber-1].GetPoint().y > 0)
				SendMoveMessage(NORTH);
			break;
		case VK_DOWN:
			if(user[ConnectedNumber-1].GetPoint().y < MAX_HEIGHT-1)
				SendMoveMessage(SOUTH);
			break;
		case VK_LEFT:
			if(user[ConnectedNumber-1].GetPoint().x > 0)
			SendMoveMessage(WEST);
			if(user[ConnectedNumber-1].GetPoint().x==BGMOVE_WIDTH)		// 버그 방지
				F_BGMove=FALSE;
			break;
		case VK_RIGHT:
			if(user[ConnectedNumber-1].GetPoint().x < MAX_WIDTH-2)
				SendMoveMessage(EAST);
			if(user[ConnectedNumber-1].GetPoint().x==MAX_WIDTH-BGMOVE_WIDTH-1)	// 버그 방지
				F_BGMove=FALSE;
			break;
		/*************************
				공격/스킬
		*************************/	
		case VK_SPACE:
			SendAttackMessage(user[ConnectedNumber-1].GetDirection());
			PlayMusic(M_ATK);
			break;
		case 'q':
		case 'Q':
			UseSkill1();
			break;
		case 'w':
		case 'W':
			UseSkill2();
			break;
		case 'e':
		case 'E':
			UseSkill3();
			break;
		/*************************
				그외단축키
		*************************/	
		case '1' :
			UsePotion(0);
			break;
		case '2' :
			UsePotion(1);
			break;
		case '3' :
			StatusUp(0);
			break;
		case '4' :
			StatusUp(1);
			break;
		case '5' :
			StatusUp(2);
			break;
		case '6' :
			StatusUp(3);
			break;
		case '7' :
			SkillUp(0);
			break;
		case '8' :
			SkillUp(1);
			break;
		case '9' :
			SkillUp(2);
			break;
		case '0' :
			// 디버그용 파워업글
			DebugPower();
			break;

		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CGameClientView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	//return CView::OnEraseBkgnd(pDC);
	return true;
}

/**************************************
			로그인 처리
***************************************/
void CGameClientView::OpenLoginForm(void)
{
	// 서버접속 대화상자
	ServerConnectDlg s_dlg;
	s_dlg.DoModal();

	// 로그인 대화상자
	Login_dlg.DoModal();
	usedLoginForm = TRUE;

	ID = Login_dlg.m_ID;
}

void CGameClientView::Login(CString ID)
{
	CString msg;
	msg.Format(_T("$1%s"),ID);
	m_socket.Send(msg,msg.GetLength()*2);
}

/**************************************
			대기화면 처리
***************************************/
void CGameClientView::OpenWaitRoomDlg(void)
{
	WaitRoom_dlg.DoModal();
}
void CGameClientView::Ready(int class_select)
{
	CString msg;
	msg.Format(_T("$2%d"),class_select);
	m_socket.Send(msg,lstrlen(msg)*2);
}

/**************************************
			그리기 함수
***************************************/
///////////////////////////////////////
//				범용				//
//////////////////////////////////////

void CGameClientView::RePaint(void)
{
	RedrawWindow();
}

///////////////////////////////////////
//			인터 페이스				//
//////////////////////////////////////
void CGameClientView::DrawInterface(CBufferDC* dc)
{
	CDC MemDC;
	BITMAP bmpInfo;
	MemDC.CreateCompatibleDC(dc);
	CBitmap bmp;
	CBitmap* pOldBmp = NULL;
	bmp.LoadBitmapW(IDB_INTERFACE);
	bmp.GetBitmap(&bmpInfo);
	pOldBmp = MemDC.SelectObject(&bmp);

	dc->TransparentBlt(0,660,bmpInfo.bmWidth,bmpInfo.bmHeight,&MemDC,
							0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,
							RGB(192,0,192));
	DrawChattingBox(dc);
	DrawStatusBar(dc);
	DrawInfo(dc);
}
void CGameClientView::DrawChattingBox(CBufferDC* dc)
{
	DrawChattingMsg(dc);
	CString help=_T("Press Enter Key");
	dc->TextOut(45,815,help);
}

void CGameClientView::DrawChattingMsg(CBufferDC* dc)
{

	dc->SetTextColor(RGB(255,255,255));
	dc->SetBkMode(TRANSPARENT);

	for(int i=5;i>0;i--)
	{
		dc->TextOutW(25,650+i*25,
			Message_Queue[(Count_Message-1+i)%5]);
	}
}

void CGameClientView::DrawStatusBar(CBufferDC* dc)
{
	// 스킬 아이콘 그리기
	if(user[ConnectedNumber-1].GetJob()==1)
	{
		CImage S;
		HRESULT hResult = S.Load(_T("as1.jpg"));
		S.BitBlt(dc->m_hDC,650,680);
		CImage S1;
		HRESULT hResult1 = S1.Load(_T("as2.jpg"));
		S1.BitBlt(dc->m_hDC,650,728);
		CImage S2;
		HRESULT hResult2 = S2.Load(_T("as3.jpg"));
		S2.BitBlt(dc->m_hDC,650,776);
	}
	else
	{
		CImage S;
		HRESULT hResult = S.Load(_T("bs1.jpg"));
		S.BitBlt(dc->m_hDC,650,680);
		CImage S1;
		HRESULT hResult1 = S1.Load(_T("bs2.jpg"));
		S1.BitBlt(dc->m_hDC,650,728);
		CImage S2;
		HRESULT hResult2 = S2.Load(_T("bs3.jpg"));
		S2.BitBlt(dc->m_hDC,650,776);
	}

	CDC MemDC;
	BITMAP bmpInfo;
	MemDC.CreateCompatibleDC(dc);
	CBitmap bmp;
	CBitmap* pOldBmp = NULL;
	bmp.LoadBitmapW(IDB_PBAR);
	bmp.GetBitmap(&bmpInfo);
	pOldBmp = MemDC.SelectObject(&bmp);

	dc->TransparentBlt(770,670,
					bmpInfo.bmWidth,bmpInfo.bmHeight,&MemDC,
					0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,
					RGB(255,0,255));

}
void CGameClientView::DrawInfo(CBufferDC* dc)
{
	CFont font;
	CFont* pOldFont;

	font.CreatePointFont(140,_T("복숭아"));
	pOldFont = dc->SelectObject(&font);
	CString status;
	status.Format(_T("HP:%d/%d"),user[ConnectedNumber-1].GetHP(),user[ConnectedNumber-1].GetMAXHP());
	dc->TextOutW(760,729,status);
	status.Format(_T("LV:%d"),user[ConnectedNumber-1].GetLevel());
	dc->TextOutW(908,729,status);
	status.Format(_T("exp:%d/%d"),user[ConnectedNumber-1].GetExp(),user[ConnectedNumber-1].GetMAXExp());
	dc->TextOutW(986,729,status);
	status.Format(_T("Skill Point:%d"),user[ConnectedNumber-1].GetSkillPoint());
	dc->TextOutW(1117,729,status);
	status.Format(_T("MP:%d/%d"),user[ConnectedNumber-1].GetMP(),user[ConnectedNumber-1].GetMAXMP());
	dc->TextOutW(760,748,status);
	status.Format(_T("ATK:%d"),user[ConnectedNumber-1].GetAct());
	dc->TextOutW(908,748,status);
	status.Format(_T("DEF:%d"),user[ConnectedNumber-1].GetDef());
	dc->TextOutW(986,748,status);
	status.Format(_T("GOLD:%d"),user[ConnectedNumber-1].GetGold());
	dc->TextOutW(1076,748,status);
	status.Format(_T("%d      %d      %d"),user[ConnectedNumber-1].GetSkill1(),user[ConnectedNumber-1].GetSkill2(),user[ConnectedNumber-1].GetSkill3());
	dc->TextOutW(1070,765,status);
	dc->SelectObject(pOldFont);
	font.DeleteObject();

	CBrush brush(RGB(205,0,0));
	CBrush* pOldBrush=dc->SelectObject(&brush);	
	dc->Rectangle(800,673,793+(int)(user[ConnectedNumber-1].GetHpRate()*362),697);
	dc->SelectObject(pOldBrush);
	
	CBrush brush2(RGB(0,0,205));
	CBrush* pOldBrush2=dc->SelectObject(&brush2);	
	dc->Rectangle(800,701,793+(int)(user[ConnectedNumber-1].GetMpRate()*362),724);
	dc->SelectObject(pOldBrush2);
}
///////////////////////////////////////
//			게임 화면 				//
//////////////////////////////////////


void CGameClientView::DrawBG(CBufferDC* dc)
{
	
	CImage BG;
	HRESULT hResult = BG.Load(_T("bg.jpg"));

	CString de;

	if(F_BGMove && user[ConnectedNumber-1].GetStatusFlag()==S_MOVE)
	{
		switch(user[ConnectedNumber-1].GetDirection())
		{
		case EAST:
			BG.BitBlt(dc->m_hDC,(BG_x+1)*SIZE_WIDTH*(-1)+SIZE_WIDTH*(4-user[ConnectedNumber-1].GetFRAME())/4,0);
			break;
		case WEST:
			BG.BitBlt(dc->m_hDC,(BG_x-1)*SIZE_WIDTH*(-1)-SIZE_WIDTH*(4-user[ConnectedNumber-1].GetFRAME())/4,0);
			break;
		case NORTH:
		case SOUTH:
			BG.BitBlt(dc->m_hDC,BG_x*SIZE_WIDTH*(-1),0);
			break;
		}
	}
	else
		BG.BitBlt(dc->m_hDC,BG_x*SIZE_WIDTH*(-1),0);
	

}

void CGameClientView::DrawFighter(CBufferDC* dc)
{
	CDC MemDC;
	BITMAP bmpInfo;
	MemDC.CreateCompatibleDC(dc);
	CBitmap* pOldBmp = NULL;

	
	UINT uID=478;
	for(int i=0;i<2;i++)
	{
		CBitmap bmp;

		if(user[i].GetStatusFlag()==S_MOVE || user[i].GetStatusFlag()==S_NONE)	// 이동 정지 리소스
		{
			if(user[i].GetJob()==1)
			{
				switch(user[i].GetDirection())		// 궁수 리소스
				{
				case EAST:
					uID = 325;
					break;
				case WEST:
					uID = 321;
					break;
				case NORTH:
					uID = 329;
					break;
				case SOUTH:
					uID = 317;
					break;
				}
			}
			else
			{
				switch(user[i].GetDirection())		// 전사 리소스
				{
				case EAST:
					uID = 357;
					break;
				case WEST:
					uID = 353;
					break;
				case NORTH:
					uID = 361;
					break;
				case SOUTH:
					uID = 349;
					break;
				}
			}
		}
		else if(user[i].GetStatusFlag()==S_ATTACK)					// 공격 시 리소스
		{
			if(user[i].GetJob()==1)
			{
				switch(user[i].GetDirection())		// 궁수 리소스
				{
				case EAST:
					uID = 309;
					break;
				case WEST:
					uID = 305;
					break;
				case NORTH:
					uID = 313;
					break;
				case SOUTH:
					uID = 301;
					break;
				}
			}
			else
			{
				switch(user[i].GetDirection())		// 전사 리소스
				{
				case EAST:
					uID = 341;
					break;
				case WEST:
					uID = 337;
					break;
				case NORTH:
					uID = 345;
					break;
				case SOUTH:
					uID = 333;
					break;
				}
			}
		}
		if(user[i].GetStatusFlag()==S_NONE)
		{
			user[i].SetFRAME(0);		// 정지시 프레임 0으로 변경
		}
		else if(user[i].GetStatusFlag()==S_MOVE || user[i].GetStatusFlag()==S_ATTACK)
		{
			user[i].SetFRAME(user[i].GetFRAME()+1);
		}
		if(user[i].GetFRAME()>=0)		// 출력 좌표 부분
		{
			bmp.LoadBitmapW(uID+user[i].GetFRAME());
			bmp.GetBitmap(&bmpInfo);
			pOldBmp = MemDC.SelectObject(&bmp);

			int temp_x=(user[i].GetPoint().x - BG_x)*SIZE_WIDTH;			// 정지/공격 시
			int temp_y=temp_y=user[i].GetPoint().y*SIZE_HEIGHT;
		
			if(user[i].GetStatusFlag()==S_MOVE)
			{
				switch(user[i].GetDirection())		// 방향에 따른 애니메이션 출력을 위해
				{
				case EAST:
					if(!F_BGMove || i!=(ConnectedNumber-1))
						temp_x=(user[i].GetPoint().x - BG_x)*SIZE_WIDTH-SIZE_WIDTH*(4-user[i].GetFRAME())/4;
					else 
						temp_x=(user[i].GetPoint().x - BG_x-1)*SIZE_WIDTH;
					temp_y=user[i].GetPoint().y*SIZE_HEIGHT;
					break;
				case WEST:
					if(!F_BGMove || i!=(ConnectedNumber-1))
						temp_x=(user[i].GetPoint().x - BG_x)*SIZE_WIDTH+SIZE_WIDTH*(4-user[i].GetFRAME())/4;
					else
						temp_x=(user[i].GetPoint().x - BG_x+1)*SIZE_WIDTH;
					temp_y=user[i].GetPoint().y*SIZE_HEIGHT;
					break;
				case NORTH:
					temp_x=(user[i].GetPoint().x - BG_x)*SIZE_WIDTH;
					temp_y=user[i].GetPoint().y*SIZE_HEIGHT+SIZE_HEIGHT*(4-user[i].GetFRAME())/4;
					break;
				case SOUTH:
					temp_x=(user[i].GetPoint().x - BG_x)*SIZE_WIDTH;
					temp_y=user[i].GetPoint().y*SIZE_HEIGHT-SIZE_HEIGHT*(4-user[i].GetFRAME())/4;
					break;

				}
			}
			
			dc->TransparentBlt(temp_x,temp_y,
								bmpInfo.bmWidth,bmpInfo.bmHeight,&MemDC,
								0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,
								RGB(192,0,192));
		}
		bmp.DeleteObject();
		if(user[i].GetFRAME()>=3)
		{
			if(user[ConnectedNumber-1].GetFRAME()>=3)		// 자신 일때만
			{
				if(F_BGMove && user[ConnectedNumber-1].GetStatusFlag()==S_MOVE)
				{
					if(user[ConnectedNumber-1].GetDirection()==EAST)
						BG_x++;
					else if(user[ConnectedNumber-1].GetDirection()==WEST)
						BG_x--;
				}
				if(
					(user[ConnectedNumber-1].GetPoint().x>=BGMOVE_WIDTH) && 
					(user[ConnectedNumber-1].GetPoint().x<=MAX_WIDTH-BGMOVE_WIDTH-1) )			// 맵 이동 조건
					F_BGMove = TRUE;
				else
					F_BGMove = FALSE;
			}

			SendStatusMessage(S_NONE);
			user[i].SetStatusFlag(S_NONE);
			user[i].SetFRAME(0);
		}
	}
	MemDC.SelectObject(pOldBmp);
}
void CGameClientView::DrawEffect(CBufferDC* dc)		// 이펙트 그리기
{
	CDC MemDC;
	BITMAP bmpInfo;
	MemDC.CreateCompatibleDC(dc);
	CBitmap* pOldBmp = NULL;
	for(int i=0;i<MAX_SKILL;i++)
	{
		CBitmap bmp;
		if(effect[i].GetUse()==TRUE)
		{
			bmp.LoadBitmapW(effect[i].GetResourceID() + effect[i].GetFrame());
			bmp.GetBitmap(&bmpInfo);
			pOldBmp = MemDC.SelectObject(&bmp);

			
			dc->TransparentBlt((effect[i].GetPoint().x - BG_x)*SIZE_WIDTH, effect[i].GetPoint().y*SIZE_HEIGHT
									,bmpInfo.bmWidth,bmpInfo.bmHeight,&MemDC,
									0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,
									RGB(192,0,192));

		}
		bmp.DeleteObject();
	}
	MemDC.SelectObject(pOldBmp);
}
void CGameClientView::DrawMonster(CBufferDC* dc)
{
	CDC MemDC;
	BITMAP bmpInfo;
	MemDC.CreateCompatibleDC(dc);

	UINT uID=566;
	for(int i=0;i<MAX_MONSTER;i++)
	{
		CBitmap bmp;
		if(monster[i].GetStatusFlag()==S_DIE)
		{
			bmp.DeleteObject();
				continue;
		}
		if(monster[i].GetStatusFlag()==S_MOVE || monster[i].GetStatusFlag()==S_NONE || monster[i].GetStatusFlag()==S_ATTACK)	// 공격 이동 정지 리소스
		{
			if(monster[i].GetJob()==M_SLIME)		
			{
				switch(monster[i].GetDirection())		// 슬라임 리소스
				{
				case EAST:
					uID = 566;
					break;
				case WEST:
					uID = 562;
					break;
				case NORTH:
					uID = 570;
					break;
				case SOUTH:
					uID = 558;
					break;
				}
			}
			else if(monster[i].GetJob()==M_SKELETON)
			{
				switch(monster[i].GetDirection())		// 해골 리소스
				{
				case EAST:
					uID = 582;
					break;
				case WEST:
					uID = 578;
					break;
				case NORTH:
					uID = 586;
					break;
				case SOUTH:
					uID = 574;
					break;
				}
			}
			else if(monster[i].GetJob()==M_GOLEM)
			{
				switch(monster[i].GetDirection())		// 골렘 리소스
				{
				case EAST:
					uID = 518;
					break;
				case WEST:
					uID = 514;
					break;
				case NORTH:
					uID = 522;
					break;
				case SOUTH:
					uID = 510;
					break;
				}
			}
			else if(monster[i].GetJob()==M_VAMPIRE)
			{
				switch(monster[i].GetDirection())		// 뱀파 리소스
				{
				case EAST:
					uID = 550;
					break;
				case WEST:
					uID = 546;
					break;
				case NORTH:
					uID = 554;
					break;
				case SOUTH:
					uID = 542;
					break;
				}
			}
			else if(monster[i].GetJob()==M_KNIGHT)
			{
				switch(monster[i].GetDirection())		// 듀라한 리소스
				{
				case EAST:
					uID = 534;
					break;
				case WEST:
					uID = 530;
					break;
				case NORTH:
					uID = 538;
					break;
				case SOUTH:
					uID = 526;
					break;
				}
			}
		}
		if(monster[i].GetFRAME()>=0)		// 출력 좌표 부분
		{
			bmp.LoadBitmapW(uID+monster[i].GetFRAME());
			bmp.GetBitmap(&bmpInfo);
			MemDC.SelectObject(&bmp);

			int temp_x=(monster[i].GetPoint().x - BG_x)*SIZE_WIDTH;			// 정지/공격 시
			int temp_y=temp_y=monster[i].GetPoint().y*SIZE_HEIGHT;
		
			if(monster[i].GetStatusFlag()==S_MOVE)
			{
				switch(monster[i].GetDirection())		// 방향에 따른 애니메이션 출력을 위해
				{
				case EAST:
					if(!F_BGMove || i!=(ConnectedNumber-1))
						temp_x=(monster[i].GetPoint().x - BG_x)*SIZE_WIDTH-SIZE_WIDTH*(4-monster[i].GetFRAME())/4;
					else 
						temp_x=(monster[i].GetPoint().x - BG_x-1)*SIZE_WIDTH;
					temp_y=monster[i].GetPoint().y*SIZE_HEIGHT;
					break;
				case WEST:
					if(!F_BGMove || i!=(ConnectedNumber-1))
						temp_x=(monster[i].GetPoint().x - BG_x)*SIZE_WIDTH+SIZE_WIDTH*(4-monster[i].GetFRAME())/4;
					else
						temp_x=(monster[i].GetPoint().x - BG_x+1)*SIZE_WIDTH;
					temp_y=monster[i].GetPoint().y*SIZE_HEIGHT;
					break;
				case NORTH:
					temp_x=(monster[i].GetPoint().x - BG_x)*SIZE_WIDTH;
					temp_y=monster[i].GetPoint().y*SIZE_HEIGHT+SIZE_HEIGHT*(4-monster[i].GetFRAME())/4;
					break;
				case SOUTH:
					temp_x=(monster[i].GetPoint().x - BG_x)*SIZE_WIDTH;
					temp_y=monster[i].GetPoint().y*SIZE_HEIGHT-SIZE_HEIGHT*(4-monster[i].GetFRAME())/4;
					break;

				}
			}
			
			dc->TransparentBlt(temp_x,temp_y,
								bmpInfo.bmWidth,bmpInfo.bmHeight,&MemDC,
								0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,
								RGB(192,0,192));
		}
		bmp.DeleteObject();
	}
}
void CGameClientView::SendMoveMessage(int direction)
{
	CString msg;
	msg.Format(_T("#0%d%d"),ConnectedNumber,direction);
	m_socket.Send(msg,lstrlen(msg)*2);
}
void CGameClientView::SendStatusMessage(int status)
{
	CString msg;
	msg.Format(_T("#4%d%d"),ConnectedNumber,status);
	m_socket.Send(msg,lstrlen(msg)*2);
}

void CGameClientView::UsePotion(int select)
{
	if(user[ConnectedNumber-1].GetGold()<100)
	{
		return;
	}
	if(select==0)
	{
		if(user[ConnectedNumber-1].GetHP()==user[ConnectedNumber-1].GetMAXHP())
			return;
	}
	else
	{
		if(user[ConnectedNumber-1].GetMP()==user[ConnectedNumber-1].GetMAXMP())
			return;
	}
	CPoint tmp=user[ConnectedNumber-1].GetPoint();
	SendSkillMessage2(tmp,479,19,select);
	PlayMusic(M_USEPOTION);
}

void CGameClientView::StatusUp(int select)
{
	if(user[ConnectedNumber-1].GetGold()<300)
	{
		return;
	}
	CString msg;
	msg.Format(_T("#2%d%d"),ConnectedNumber,select);
	m_socket.Send(msg,lstrlen(msg)*2);
}

void CGameClientView::SkillUp(int select)
{
	if(user[ConnectedNumber-1].GetSkillPoint()<1)
	{
		return;
	}
	CString msg;
	msg.Format(_T("#3%d%d"),ConnectedNumber,select);
	m_socket.Send(msg,lstrlen(msg)*2);
}
void CGameClientView::SendAttackMessage(int direction)
{
	CString msg;
	msg.Format(_T("#5%d%d"),ConnectedNumber,direction);
	m_socket.Send(msg,lstrlen(msg)*2);
}


void CGameClientView::SendSkillMessage(CPoint point,UINT Resource, int MAXFrame, int select)
{
	CString msg;
	msg.Format(_T("#6%d%d%d %d %d %d"),ConnectedNumber,select,point.x,point.y,Resource,MAXFrame);
	m_socket.Send(msg,lstrlen(msg)*2);
}

void CGameClientView::SendSkillMessage2(CPoint point,UINT Resource, int MAXFrame, int select)
{
	CString msg;
	msg.Format(_T("#1%d%d%d %d %d %d"),ConnectedNumber,select,point.x,point.y,Resource,MAXFrame);
	m_socket.Send(msg,lstrlen(msg)*2);
}
/**************수정**********************/
void CGameClientView::UseSkill1()
{
	if(user[ConnectedNumber-1].GetMP()<100)
		return;
	else
	{
		if(user[ConnectedNumber-1].GetJob()==0)	// 전사일때만
			{
				CPoint tmp=user[ConnectedNumber-1].GetPoint();
				switch(user[ConnectedNumber-1].GetDirection())
				{
				case EAST:
					tmp.x++;tmp.y-=2;
					break;
				case WEST:
					tmp.x-=2;tmp.y-=2;
					break;
				case NORTH:
					tmp.y-=3;
					break;
				case SOUTH:
					tmp.y--;
					break;
				}
				SendSkillMessage(tmp,420,14,1);
				PlayMusic(M_USESKILLb1);
			}
			else						//궁수
			{
				CPoint tmp=user[ConnectedNumber-1].GetPoint();
				switch(user[ConnectedNumber-1].GetDirection())
				{
				case EAST:
					;tmp.y-=1;
					break;
				case WEST:
					tmp.x-=2;tmp.y-=1;
					break;
				case NORTH:
					tmp.y-=2;tmp.x-=1;
					break;
				case SOUTH:
					tmp.x-=1;
					break;
				}
				SendSkillMessage(tmp,365,19,1);
				PlayMusic(M_USESKILLa1);
			}
	}
}
void CGameClientView::UseSkill2()
{
	if(user[ConnectedNumber-1].GetMP()<150)
		return;
	else
	{
		if(user[ConnectedNumber-1].GetJob()==0)	// 전사일때만
			{
				CPoint tmp=user[ConnectedNumber-1].GetPoint();
				SendSkillMessage(tmp,435,18,2);
				PlayMusic(M_USESKILLb2);
			}
			else
			{
				CPoint tmp=user[ConnectedNumber-1].GetPoint();
				SendSkillMessage(tmp,385,9,2);
				PlayMusic(M_USESKILLa2);
			}
	}
}
void CGameClientView::UseSkill3()
{
	if(user[ConnectedNumber-1].GetMP()<200)
		return;
	else
	{
		if(user[ConnectedNumber-1].GetJob()==0)	// 전사일때만
			{
				CPoint tmp=user[ConnectedNumber-1].GetPoint();
				switch(user[ConnectedNumber-1].GetDirection())
				{
				case EAST:
					tmp.x++;tmp.y-=2;
					break;
				case WEST:
					tmp.x-=2;tmp.y-=2;
					break;
				case NORTH:
					tmp.y-=3;
					break;
				case SOUTH:
					tmp.y;
					break;
				}
				SendSkillMessage(tmp,454,14,3);
				PlayMusic(M_USESKILLb3);
			}
			else
			{
				CPoint tmp=user[ConnectedNumber-1].GetPoint();
				switch(user[ConnectedNumber-1].GetDirection())
				{
				case EAST:
					tmp.x+=1;
					break;
				case WEST:
					tmp.x-=1;
					break;
				case NORTH:
					tmp.y-=2;
					break;
				case SOUTH:
					tmp.y+=1;
					break;
				}
				SendSkillMessage(tmp,395,24,3);
				PlayMusic(M_USESKILLb3);
			}
	}
}

void CGameClientView::DrawWinLoseClear(CBufferDC* dc)
{
	CFont font;
	CFont* pOldFont;

	font.CreatePointFont(900,_T("복숭아"));
	pOldFont = dc->SelectObject(&font);
	CString msg;
	
	
	
	if(GameState==G_LOSE)
	{
		msg=_T("패 배");
		PlayMusic(M_GAMEOVER);
		StopMusic(M_START);
	}
	else if(GameState==G_VICTORY)
	{
		msg=_T("승 리");
		PlayMusic(M_VICTORY);
		StopMusic(M_START);
	}
	else if(GameState==G_CLEAR)
	{
		msg=_T("통 과");
		PlayMusic(M_NIGHT);
		PauseMusic(M_START);

	}
	else if(GameState==G_GAME)
	{
		msg=_T("");
		StopMusic(M_NIGHT);
		ResumeMusic(M_START);
	}
	dc->TextOutW(500,200,msg);
	dc->SelectObject(pOldFont);
	font.DeleteObject();

}

void CGameClientView::DebugPower(void)
{
	CString msg;
	msg.Format(_T("*%d"),ConnectedNumber-1);
	m_socket.Send(msg,lstrlen(msg)*2);
}
/******************************************
			    음 악 재 생
******************************************/
void CGameClientView::PlayMusic(LPCWSTR Music_Path)
{
	wchar_t szCommand[256], szReturnStr[256];            
	wsprintf(szCommand, _T("open %s alias mydev_%s"),Music_Path,Music_Path);		//mydev 생성,시작
	mciSendString(szCommand, szReturnStr, sizeof(szReturnStr), NULL);
	wsprintf(szCommand, _T("play mydev_%s from 0"),Music_Path);
	mciSendString(szCommand, szReturnStr, sizeof(szReturnStr), NULL);	// 시작
}

void CGameClientView::PauseMusic(LPCWSTR Music_Path)
{
	wchar_t szCommand[256];
	wsprintf(szCommand, _T("pause mydev_%s"),Music_Path);
	mciSendString(szCommand, NULL, NULL, NULL);  // 일시정지
}

void CGameClientView::ResumeMusic(LPCWSTR Music_Path)
{
	wchar_t szCommand[256];
	wsprintf(szCommand, _T("resume mydev_%s"),Music_Path);
	mciSendString(szCommand, NULL, NULL, NULL);  // 재시작
}

void CGameClientView::StopMusic(LPCWSTR Music_Path)
{
	wchar_t szCommand[256];
	wsprintf(szCommand, _T("stop mydev_%s"),Music_Path);
	mciSendString(szCommand, NULL, NULL, NULL);  // 정지
}
