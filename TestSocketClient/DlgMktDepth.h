#if !defined(AFX_DLGMKTDEPTH_H__866438BE_CB41_4A95_A863_067AED70391A__INCLUDED_)
#define AFX_DLGMKTDEPTH_H__866438BE_CB41_4A95_A863_067AED70391A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "resource.h"
// DlgMktDepth.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgMktDepth dialog

class CDlgMktDepth : public CDialog
{
// Construction
public:
	const static int MKT_DEPTH_DATA_RESET;
	CDlgMktDepth(CWnd* pParent = NULL);   // standard constructor
	void updateMktDepth( int id, int position, CString marketMaker, int operation, 
        int side, double price, int size);
	void clear();
     
// Dialog Data
	//{{AFX_DATA(CDlgMktDepth)
	enum { IDD = IDD_MKT_DEPTH };
	CListCtrl	m_listBid;
	CListCtrl	m_listAsk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMktDepth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
 
	// Generated message map functions
	//{{AFX_MSG(CDlgMktDepth)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    void addEntry(CListCtrl* pList, int position, CString marketMaker, double price, int size);
    void deleteEntry(CListCtrl* pList, int position);
    void updateEntry(CListCtrl* pList, int position, CString marketMaker, double price, int size);
    void updateList(CListCtrl* pList, int position);

private:

	bool m_active;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMKTDEPTH_H__866438BE_CB41_4A95_A863_067AED70391A__INCLUDED_)
