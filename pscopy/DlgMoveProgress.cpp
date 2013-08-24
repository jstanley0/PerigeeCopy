// DlgMoveProgress.cpp : implementation file
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "DlgMoveProgress.h"
#include "PerigeeJob.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgMoveProgress dialog


DlgMoveProgress::DlgMoveProgress(CWnd* pParent /*=NULL*/)
	: CDialog(DlgMoveProgress::IDD, pParent),
	  m_CancelRequest(false)
{
	//{{AFX_DATA_INIT(DlgMoveProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgMoveProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgMoveProgress)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgMoveProgress, CDialog)
	//{{AFX_MSG_MAP(DlgMoveProgress)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(PerigeeJob::ms_TaskbarBtnCreated, &DlgMoveProgress::OnTaskbarBtnCreated)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgMoveProgress message handlers

void DlgMoveProgress::OnCancel() 
{
	m_CancelRequest = true;
}

void DlgMoveProgress::SetSourceDest(LPCTSTR source, LPCTSTR dest)
{
	if ( source ) 
		PathSetDlgItemPath(GetSafeHwnd(), IDC_LBL_SOURCE, source);
	else
		SetDlgItemText(IDC_LBL_SOURCE, _T(""));

	if ( dest )
		PathSetDlgItemPath(GetSafeHwnd(), IDC_LBL_DEST, dest);
	else
		SetDlgItemText(IDC_LBL_DEST, _T(""));
}

BOOL DlgMoveProgress::OnInitDialog() 
{
	PerigeeJob::Instance()->TaskbarProgressBarRedTape(GetSafeHwnd());
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Activate this window:  If we were launched with Ctrl or Shift down,
	// this doesn't happen automatically.
	SetForegroundWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT DlgMoveProgress::OnTaskbarBtnCreated(WPARAM wParam, LPARAM lParam)
{
	PerigeeJob::Instance()->OnTaskbarBtnCreated();
	return 0;
}
