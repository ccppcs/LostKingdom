// GameClientDoc.h : CGameClientDoc Ŭ������ �������̽�
//


#pragma once


class CGameClientDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CGameClientDoc();
	DECLARE_DYNCREATE(CGameClientDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CGameClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


