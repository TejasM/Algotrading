#if !defined(AFX_DLGSCANNER_H__203C4C23_0F90_11D6_B0ED_00B0D074179C__INCLUDED_)
#define AFX_DLGSCANNER_H__203C4C23_0F90_11D6_B0ED_00B0D074179C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgScanner.h : header file
//

#include "resource.h"

struct ScannerSubscription;
class EClient;

/////////////////////////////////////////////////////////////////////////////
// CDlgScanner dialog
//

class CDlgScanner : public CDialog
{
// Construction
public:

	CDlgScanner(ScannerSubscription* subscription, EClient* client, CWnd* pParent = NULL);

private:

	void OnRequestParameters();
	void OnSubscribe();
	void OnCancelSubscription();

// Dialog Data
	//{{AFX_DATA(CDlgScanner)
	enum { IDD = IDD_SCANNER };

    int m_id;

    CString m_instrument;
    CString m_locationCode;
    CString m_scanCode;
	int m_numberOfRows;

    double m_abovePrice;
    double m_belowPrice;
    int m_aboveVolume;
	int m_averageOptionVolumeAbove;
    double m_marketCapAbove;
    double m_marketCapBelow;
    CString m_moodyRatingAbove;
    CString m_moodyRatingBelow;
    CString m_spRatingAbove;
    CString m_spRatingBelow;
    CString m_maturityDateAbove;
    CString m_maturityDateBelow;
    double m_couponRateAbove;
    double m_couponRateBelow;
    int m_excludeConvertible;
	CString m_scannerSettingPairs;
	CString m_stockTypeFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgScanner)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgScanner)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	void updateScanSubscription();

private:

	EClient* m_client;
	ScannerSubscription* m_subscription;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSCANNER_H__203C4C23_0F90_11D6_B0ED_00B0D074179C__INCLUDED_)
