// GameClientView.cpp : CGameClientView Ŭ������ ����
//

#include "stdafx.h"
#include "GameClient.h"

#include "GameClientDoc.h"
#include "GameClientView.h"
#include "ServerConnectDlg.h"
#include <atlimage.h>

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

#define M_START _T("res/sound/after.mp3")					//�������
#define M_NIGHT _T("res/sound/night1.mp3")					//������
#define M_GAMEOVER _T("res/sound/gameover.mp3")				//���ӿ���
#define M_VICTORY _T("res/sound/clear.mp3")					//�¸�
#define M_USEPOTION _T("res/sound/e2.mp3")					//���ǻ��
#define M_USESKILLa1 _T("res/sound/s3.mp3")					//�ü�1~����3 ��ų//
#define M_USESKILLa2 _T("res/sound/s5.mp3")
#define M_USESKILLa3 _T("res/sound/s8.mp3")
#define M_USESKILLb1 _T("res/sound/s2.mp3")
#define M_USESKILLb2 _T("res/sound/s6.mp3")
#define M_USESKILLb3 _T("res/sound/s1.mp3")							//�̱���
#define M_ATK _T("res/sound/acc.mp3")							//����
#define M_HIT _T("res/sound/e1.mp3")							//������
#define M_LEVELUP _T("res/sound/e3.mp3")							//������

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//����
#define EAST 0  // ����
#define WEST 1  // ����
#define SOUTH 2  // ����
#define NORTH 3  // ����

#define SIZE_WIDTH 70
#define SIZE_HEIGHT 60

#define BGMOVE_WIDTH 9

#define MAX_WIDTH 32
#define MAX_HEIGHT 11

#define MAX_SKILL 20	// ��ų ��ü ����
#define MAX_MONSTER 20	// ���� ��ü ����

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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
//	ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_CREATE()
ON_WM_KEYDOWN()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGameClientView ����/�Ҹ�

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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}


// CGameClientView �׸���

void CGameClientView::OnDraw(CDC* /*pDC*/)
{
	CGameClientDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}


// CGameClientView �μ�

BOOL CGameClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CGameClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CGameClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CGameClientView ����

#ifdef _DEBUG
void CGameClientView::AssertValid() const
{
	CView::AssertValid();
}

void CGameClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameClientDoc* CGameClientView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameClientDoc)));
	return (CGameClientDoc*)m_pDocument;
}
#endif //_DEBUG

/********************************************
		CGameClientView �޽��� ó����
*********************************************/
/********************************************
		OnPaint() �޽��� ó����
*********************************************/
void CGameClientView::OnPaint()
{
	CBufferDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CView::OnPaint()��(��) ȣ������ ���ʽÿ�.

	/****************************************
				��ȭ���� ���
	*****************************************/
	if(!usedLoginForm)
		OpenLoginForm();
	if(isLogin&&isConnect&&!isGameStart)
		OpenWaitRoomDlg();
	/****************************************
				����� ���
	*****************************************/
	else if(!isPlayedBGMusic)
	{
		PlayMusic(M_START);
		isPlayedBGMusic=TRUE;
	}

	/***************************************
				�׸��� �۾�
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
		,CRect(42,810,595,835),this,123);		// ä�� �Է�â
	return 0;
}

void CGameClientView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == VK_RETURN)
	{
		m_Chat.ShowWindow(SW_SHOW);
		m_Chat.SetFocus();
	}

	// ȭ�� ���ΰ�ħ
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
				����Ű
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
			if(user[ConnectedNumber-1].GetPoint().x==BGMOVE_WIDTH)		// ���� ����
				F_BGMove=FALSE;
			break;
		case VK_RIGHT:
			if(user[ConnectedNumber-1].GetPoint().x < MAX_WIDTH-2)
				SendMoveMessage(EAST);
			if(user[ConnectedNumber-1].GetPoint().x==MAX_WIDTH-BGMOVE_WIDTH-1)	// ���� ����
				F_BGMove=FALSE;
			break;
		/*************************
				����/��ų
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
				�׿ܴ���Ű
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
			// ����׿� �Ŀ�����
			DebugPower();
			break;

		}
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CGameClientView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	//return CView::OnEraseBkgnd(pDC);
	return true;
}

/**************************************
			�α��� ó��
***************************************/
void CGameClientView::OpenLoginForm(void)
{
	// �������� ��ȭ����
	ServerConnectDlg s_dlg;
	s_dlg.DoModal();

	// �α��� ��ȭ����
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
			���ȭ�� ó��
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
			�׸��� �Լ�
***************************************/
///////////////////////////////////////
//				����				//
//////////////////////////////////////

void CGameClientView::RePaint(void)
{
	RedrawWindow();
}

///////////////////////////////////////
//			���� ���̽�				//
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
	// ��ų ������ �׸���
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

	font.CreatePointFont(140,_T("������"));
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
//			���� ȭ�� 				//
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

		if(user[i].GetStatusFlag()==S_MOVE || user[i].GetStatusFlag()==S_NONE)	// �̵� ���� ���ҽ�
		{
			if(user[i].GetJob()==1)
			{
				switch(user[i].GetDirection())		// �ü� ���ҽ�
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
				switch(user[i].GetDirection())		// ���� ���ҽ�
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
		else if(user[i].GetStatusFlag()==S_ATTACK)					// ���� �� ���ҽ�
		{
			if(user[i].GetJob()==1)
			{
				switch(user[i].GetDirection())		// �ü� ���ҽ�
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
				switch(user[i].GetDirection())		// ���� ���ҽ�
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
			user[i].SetFRAME(0);		// ������ ������ 0���� ����
		}
		else if(user[i].GetStatusFlag()==S_MOVE || user[i].GetStatusFlag()==S_ATTACK)
		{
			user[i].SetFRAME(user[i].GetFRAME()+1);
		}
		if(user[i].GetFRAME()>=0)		// ��� ��ǥ �κ�
		{
			bmp.LoadBitmapW(uID+user[i].GetFRAME());
			bmp.GetBitmap(&bmpInfo);
			pOldBmp = MemDC.SelectObject(&bmp);

			int temp_x=(user[i].GetPoint().x - BG_x)*SIZE_WIDTH;			// ����/���� ��
			int temp_y=temp_y=user[i].GetPoint().y*SIZE_HEIGHT;
		
			if(user[i].GetStatusFlag()==S_MOVE)
			{
				switch(user[i].GetDirection())		// ���⿡ ���� �ִϸ��̼� ����� ����
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
			if(user[ConnectedNumber-1].GetFRAME()>=3)		// �ڽ� �϶���
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
					(user[ConnectedNumber-1].GetPoint().x<=MAX_WIDTH-BGMOVE_WIDTH-1) )			// �� �̵� ����
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
void CGameClientView::DrawEffect(CBufferDC* dc)		// ����Ʈ �׸���
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
		if(monster[i].GetStatusFlag()==S_MOVE || monster[i].GetStatusFlag()==S_NONE || monster[i].GetStatusFlag()==S_ATTACK)	// ���� �̵� ���� ���ҽ�
		{
			if(monster[i].GetJob()==M_SLIME)		
			{
				switch(monster[i].GetDirection())		// ������ ���ҽ�
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
				switch(monster[i].GetDirection())		// �ذ� ���ҽ�
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
				switch(monster[i].GetDirection())		// �� ���ҽ�
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
				switch(monster[i].GetDirection())		// ���� ���ҽ�
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
				switch(monster[i].GetDirection())		// ����� ���ҽ�
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
		if(monster[i].GetFRAME()>=0)		// ��� ��ǥ �κ�
		{
			bmp.LoadBitmapW(uID+monster[i].GetFRAME());
			bmp.GetBitmap(&bmpInfo);
			MemDC.SelectObject(&bmp);

			int temp_x=(monster[i].GetPoint().x - BG_x)*SIZE_WIDTH;			// ����/���� ��
			int temp_y=temp_y=monster[i].GetPoint().y*SIZE_HEIGHT;
		
			if(monster[i].GetStatusFlag()==S_MOVE)
			{
				switch(monster[i].GetDirection())		// ���⿡ ���� �ִϸ��̼� ����� ����
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
/**************����**********************/
void CGameClientView::UseSkill1()
{
	if(user[ConnectedNumber-1].GetMP()<100)
		return;
	else
	{
		if(user[ConnectedNumber-1].GetJob()==0)	// �����϶���
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
			else						//�ü�
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
		if(user[ConnectedNumber-1].GetJob()==0)	// �����϶���
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
		if(user[ConnectedNumber-1].GetJob()==0)	// �����϶���
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

	font.CreatePointFont(900,_T("������"));
	pOldFont = dc->SelectObject(&font);
	CString msg;
	
	
	
	if(GameState==G_LOSE)
	{
		msg=_T("�� ��");
		PlayMusic(M_GAMEOVER);
		StopMusic(M_START);
	}
	else if(GameState==G_VICTORY)
	{
		msg=_T("�� ��");
		PlayMusic(M_VICTORY);
		StopMusic(M_START);
	}
	else if(GameState==G_CLEAR)
	{
		msg=_T("�� ��");
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
			    �� �� �� ��
******************************************/
void CGameClientView::PlayMusic(LPCWSTR Music_Path)
{
	wchar_t szCommand[256], szReturnStr[256];            
	wsprintf(szCommand, _T("open %s alias mydev_%s"),Music_Path,Music_Path);		//mydev ����,����
	mciSendString(szCommand, szReturnStr, sizeof(szReturnStr), NULL);
	wsprintf(szCommand, _T("play mydev_%s from 0"),Music_Path);
	mciSendString(szCommand, szReturnStr, sizeof(szReturnStr), NULL);	// ����
}

void CGameClientView::PauseMusic(LPCWSTR Music_Path)
{
	wchar_t szCommand[256];
	wsprintf(szCommand, _T("pause mydev_%s"),Music_Path);
	mciSendString(szCommand, NULL, NULL, NULL);  // �Ͻ�����
}

void CGameClientView::ResumeMusic(LPCWSTR Music_Path)
{
	wchar_t szCommand[256];
	wsprintf(szCommand, _T("resume mydev_%s"),Music_Path);
	mciSendString(szCommand, NULL, NULL, NULL);  // �����
}

void CGameClientView::StopMusic(LPCWSTR Music_Path)
{
	wchar_t szCommand[256];
	wsprintf(szCommand, _T("stop mydev_%s"),Music_Path);
	mciSendString(szCommand, NULL, NULL, NULL);  // ����
}
