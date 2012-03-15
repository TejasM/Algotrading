#pragma once

#include "resource.h"
#include "Order.h"

class CDlgSmartComboRoutingParams : public CDialog
{
public:

	typedef Order::TagValueList TagValueList;
	typedef Order::TagValueListSPtr TagValueListSPtr;

public:

	CDlgSmartComboRoutingParams(TagValueListSPtr& smartComboRoutingParams, CWnd* pParent=NULL);
	~CDlgSmartComboRoutingParams();

private:

// Dialog Data
	//{{AFX_DATA(CDlgSmartComboRoutingParams)
	enum { IDD = IDD_SMART_COMBO_ROUTING_PARAMS };
	CListCtrl	m_lstSmartComboRoutingParams;
	CString		m_edtParam;
	CString		m_edtValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSmartComboRoutingParams)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:

	// Generated message map functions
	//{{AFX_MSG(CDlgSmartComboRoutingParams)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddParam();
	afx_msg void OnBtnRemoveParam();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	TagValueListSPtr& m_smartComboRoutingParams;
};
