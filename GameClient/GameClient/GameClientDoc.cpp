// GameClientDoc.cpp : CGameClientDoc Ŭ������ ����
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


// CGameClientDoc ����/�Ҹ�

CGameClientDoc::CGameClientDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CGameClientDoc::~CGameClientDoc()
{
}

BOOL CGameClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CGameClientDoc serialization

void CGameClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CGameClientDoc ����

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


// CGameClientDoc ���
