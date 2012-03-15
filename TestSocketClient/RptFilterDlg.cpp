// RptFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client2.h"
#include "RptFilterDlg.h"
#include "Execution.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRptFilterDlg dialog


CRptFilterDlg::CRptFilterDlg(ExecutionFilter* filter, CWnd* pParent /*=NULL*/)
	: CDialog(CRptFilterDlg::IDD, pParent)
	, m_reqId(0)
	, m_filter(filter)
{
	//{{AFX_DATA_INIT(CRptFilterDlg)
	//}}AFX_DATA_INIT

#define GET_PROP(Prop) \
	m_##Prop = m_filter->m_##Prop;

	GET_PROP(clientId);
	GET_PROP(acctCode);
	GET_PROP(time);
	GET_PROP(symbol);
	GET_PROP(secType);
	GET_PROP(exchange);
	GET_PROP(side);

#undef GET_PROP
}


void CRptFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRptFilterDlg)
	DDX_Text(pDX, IDC_EDT_REQID_FILTER, m_reqId);
	DDX_Text(pDX, IDC_EDT_CLIENTID_FILTER, m_clientId);
	DDX_Text(pDX, IDC_EDT_ACCT_FILTER, m_acctCode);
	DDX_Text(pDX, IDC_EDT_TIME_FILTER, m_time);
	DDX_Text(pDX, IDC_EDT_SYMBOL_FILTER, m_symbol);
	DDX_Text(pDX, IDC_EDT_SECTYPE_FILTER, m_secType);
	DDX_Text(pDX, IDC_EDT_EXCHANGE_FILTER, m_exchange);
	DDX_Text(pDX, IDC_EDT_ACTION_FILTER, m_side);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRptFilterDlg, CDialog)
	//{{AFX_MSG_MAP(CRptFilterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRptFilterDlg message handlers
void CRptFilterDlg::OnOK()
{
    UpdateData();

#define PUT_PROP(Prop) \
	m_filter->m_##Prop = m_##Prop;

	PUT_PROP(clientId);
	PUT_PROP(acctCode);
	PUT_PROP(time);
	PUT_PROP(symbol);
	PUT_PROP(secType);
	PUT_PROP(exchange);
	PUT_PROP(side);

#undef PUT_PROP

	CDialog::OnOK();
}


