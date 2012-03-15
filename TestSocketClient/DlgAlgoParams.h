#pragma once

#include "resource.h"
#include "Order.h"

class CDlgAlgoParams : public CDialog
{
public:

	typedef Order::TagValueList TagValueList;
	typedef Order::TagValueListSPtr TagValueListSPtr;

public:

    CDlgAlgoParams(CString& algoStrategy, TagValueListSPtr& algoParams, CWnd* pParent=NULL);
	~CDlgAlgoParams();

private:

// Dialog Data
	//{{AFX_DATA(CDlgAlgoParams)
	enum { IDD = IDD_ALGO_PARAMS };
	CString		m_edtAlgoStrategy;
	CListCtrl	m_lstAlgoParams;
	CString		m_edtParam;
	CString		m_edtValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlgoParams)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:

	// Generated message map functions
	//{{AFX_MSG(CDlgAlgoParams)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddParam();
	afx_msg void OnBtnRemoveParam();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	CString& m_algoStrategy;
	TagValueListSPtr& m_algoParams;
};
