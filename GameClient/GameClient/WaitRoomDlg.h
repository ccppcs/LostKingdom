#pragma once
#include "afxwin.h"


// WaitRoomDlg 대화 상자입니다.

class WaitRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(WaitRoomDlg)

public:
	WaitRoomDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~WaitRoomDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WAITROOMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelectWarrior();
	afx_msg void OnBnClickedSelectArcher();
	afx_msg void OnBnClickedReady();
	afx_msg void OnBnClickedCancel();

	CEdit m_chatting;
	CString m_chat;
	int m_ClassSelect;
	BOOL isReady;

	int m_OtherSelected;

	afx_msg void OnBnClickedSend();
	void AddMessage(LPCTSTR message);
	void SelectedClass(void);
	CButton b_ready;
	CButton b_warrior;
	CButton b_archer;
	void UpdateSeleted(void);
	CStatic m_selected;
	void DlgClose(void);
};
