// DialogDeleteProgress.cpp : implementation file
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.


#include "stdafx.h"
#include "resource.h"
#include "DialogDeleteProgress.h"
#include "PerigeeJob.h"
#include <strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDeleteProgress dialog


CDialogDeleteProgress::CDialogDeleteProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDeleteProgress::IDD, pParent),
	  m_CancelRequest(false)
{
	//{{AFX_DATA_INIT(CDialogDeleteProgress)
	m_csFilesDel = _T("");
	m_csFilesTot = _T("");
	m_csFoldersDel = _T("");
	m_csFoldersTot = _T("");
	//}}AFX_DATA_INIT
}


void CDialogDeleteProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDeleteProgress)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Text(pDX, IDC_LBL_FILE_DEL, m_csFilesDel);
	DDX_Text(pDX, IDC_LBL_FILE_TOTAL, m_csFilesTot);
	DDX_Text(pDX, IDC_LBL_FOLDER_DEL, m_csFoldersDel);
	DDX_Text(pDX, IDC_LBL_FOLDER_TOTAL, m_csFoldersTot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDeleteProgress, CDialog)
	//{{AFX_MSG_MAP(CDialogDeleteProgress)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(PerigeeJob::ms_TaskbarBtnCreated, &CDialogDeleteProgress::OnTaskbarBtnCreated)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDeleteProgress message handlers

void CDialogDeleteProgress::OnCancel() 
{
	m_CancelRequest = true;	
}

void CDialogDeleteProgress::SetCurrentFile(LPCTSTR file)
{
	if ( file )
		PathSetDlgItemPath(GetSafeHwnd(), IDC_LBL_DELETING, file);
	else
		SetDlgItemText(IDC_LBL_DELETING, _T(""));
}

BOOL CDialogDeleteProgress::OnInitDialog() 
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

LRESULT CDialogDeleteProgress::OnTaskbarBtnCreated(WPARAM wParam, LPARAM lParam)
{
	PerigeeJob::Instance()->OnTaskbarBtnCreated();
	return 0;
}
