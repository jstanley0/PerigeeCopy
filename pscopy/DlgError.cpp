// DlgError.cpp : implementation file
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "DlgError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgError dialog


DlgError::DlgError(CString prompt, CWnd* pParent /*=NULL*/)
	: CDialog(DlgError::IDD, pParent),
	  m_prompt(prompt),
	  m_act(actCancel)
{
	//{{AFX_DATA_INIT(DlgError)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgError::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgError)
	DDX_Control(pDX, IDC_ICON1, m_Icon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgError, CDialog)
	//{{AFX_MSG_MAP(DlgError)
	ON_BN_CLICKED(IDC_TRYAGAIN, OnTryagain)
	ON_BN_CLICKED(IDC_SKIPFILE, OnSkipfile)
	ON_BN_CLICKED(IDC_SKIPALL, OnSkipAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgError message handlers

void DlgError::OnTryagain() 
{
	m_act = actTryAgain;	
	OnOK();
}

void DlgError::OnSkipfile() 
{
	m_act = actSkipFile;
	OnOK();
}

void DlgError::OnSkipAll()
{
	m_act = actSkipAll;
	OnOK();
}

BOOL DlgError::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Icon.SetIcon( LoadIcon(NULL, MAKEINTRESOURCE(IDI_ERROR)) );
	
	SetDlgItemText(IDC_PROMPT, m_prompt);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
