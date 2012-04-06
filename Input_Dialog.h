#pragma once

#include "TestSocketClient/resource.h"
// Input_Dialog dialog

class Input_Dialog : public CDialog
{ 
	DECLARE_DYNAMIC(Input_Dialog)

public:
	Input_Dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~Input_Dialog();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };
	CString	m_actioncode;
	CString	m_filepath;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
};
