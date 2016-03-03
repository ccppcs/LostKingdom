#pragma once
#include "afxwin.h"


// WaitRoomDlg ��ȭ �����Դϴ�.

class WaitRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(WaitRoomDlg)

public:
	WaitRoomDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~WaitRoomDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_WAITROOMDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
