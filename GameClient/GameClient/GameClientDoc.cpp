// GameClientDoc.cpp : CGameClientDoc 클래스의 구현
//

#include "stdafx.h"
#include "GameClient.h"

#include "GameClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameClientDoc

IMPLEMENT_DYNCREATE(CGameClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameClientDoc, CDocument)
END_MESSAGE_MAP()


// CGameClientDoc 생성/소멸

CGameClientDoc::CGameClientDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CGameClientDoc::~CGameClientDoc()
{
}

BOOL CGameClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGameClientDoc serialization

void CGameClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CGameClientDoc 진단

#ifdef _DEBUG
void CGameClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameClientDoc 명령
