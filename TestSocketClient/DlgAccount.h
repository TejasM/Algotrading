#if !defined(AFX_DLGACCOUNT_H__6EA50722_2652_11D6_9B97_00B0D0B520CA__INCLUDED_)
#define AFX_DLGACCOUNT_H__6EA50722_2652_11D6_9B97_00B0D0B520CA__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgAccount.h : header file
//
#include "Contract.h"
#include "HScrollListBox.h" // '<INSTALL_DIR>\Shared' must be added to include path

class CHScrollListBox;
/////////////////////////////////////////////////////////////////////////////
// CDlgAccount dialog
class CDlgAccount : public CDialog
{
// Construction
public:
	CDlgAccount(CWnd* pParent = NULL);   // standard constructor

	void updateAccountValue(const CString &key, const CString &val,
		const CString &currency, const CString &accountName);
	void updatePortfolio( const Contract &constact, int position, double marketPrice,
        double marketValue, double averageCost, double unrealizedPNL, double realizedPNL,
		const CString &accountName);
	void updateAccountTime(const CString &timeStamp);

	void accountDownloadBegin( const CString &accountName);
	void accountDownloadEnd(const CString &accountName);

	// Dialog Data
	//{{AFX_DATA(CDlgAccount)
	enum { IDD = IDD_ACCOUNT };
	CEdit  	    m_updateTime;
	CHScrollListBox	m_portfolio;
	CHScrollListBox	m_keyValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAccount)
	protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAccount)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	void updateTitle();

private:

	CString m_accountName;
	bool    m_complete;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGACCOUNT_H__6EA50722_2652_11D6_9B97_00B0D0B520CA__INCLUDED_)
