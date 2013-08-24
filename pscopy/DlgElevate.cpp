// DlgElevate.cpp : implementation file
//

#include "stdafx.h"
#include "pscopy.h"
#include "DlgElevate.h"


// DlgElevate dialog

IMPLEMENT_DYNAMIC(DlgElevate, CDialog)

DlgElevate::DlgElevate(CString message, CWnd* pParent /*=NULL*/)
	: CDialog(DlgElevate::IDD, pParent),
	  m_message(message)
{
}

DlgElevate::~DlgElevate()
{
}

void DlgElevate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WARNING, m_iconWarning);
}


BEGIN_MESSAGE_MAP(DlgElevate, CDialog)
	ON_BN_CLICKED(IDIGNORE, &DlgElevate::OnBnClickedIgnore)
END_MESSAGE_MAP()


// DlgElevate message handlers

void DlgElevate::OnBnClickedIgnore()
{
	EndDialog(IDIGNORE);
}

BOOL DlgElevate::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Load the warning icon
	m_iconWarning.SetIcon(::LoadIcon(NULL, IDI_WARNING));

	// Put the shield on the Continue button
	::SendMessage(GetDlgItem(IDOK)->GetSafeHwnd(), 0x0000160C /* BCM_SETSHIELD */, 0, TRUE);

	// Set the correct message text, if provided
	if (!m_message.IsEmpty())
		SetDlgItemText(IDC_PROMPT, m_message);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
