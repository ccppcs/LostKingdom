#pragma once


// CGameChat

class CGameChat : public CEdit
{
	DECLARE_DYNAMIC(CGameChat)

public:
	CGameChat();
	virtual ~CGameChat();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


