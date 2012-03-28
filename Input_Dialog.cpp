// Input_Dialog.cpp : implementation file
//

#include "stdafx.h"
#include "Input_Dialog.h"
#include "afxdialogex.h"


// Input_Dialog dialog

IMPLEMENT_DYNAMIC(Input_Dialog, CDialog)

Input_Dialog::Input_Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(Input_Dialog::IDD, pParent)
{

}

Input_Dialog::~Input_Dialog()
{
}

void Input_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_actioncode);
	DDX_Text(pDX, IDC_EDIT2, m_filepath);
}


BEGIN_MESSAGE_MAP(Input_Dialog, CDialog)
	ON_BN_CLICKED(IDOK, &Input_Dialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &Input_Dialog::OnEnChangeEdit1)
END_MESSAGE_MAP()


// Input_Dialog message handlers


void Input_Dialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	/*CString message;
	message.FormatMessage(_T("The inputs are: Code String: %d, Path: %d"), m_actioncode.GetString(), m_filepath.GetString());
	MessageBox(message);*/
	CDialog::OnOK();
}


void Input_Dialog::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	
}
