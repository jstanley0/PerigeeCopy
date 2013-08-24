// DialogProgress.cpp : implementation file
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "DialogProgress.h"
#include "PerigeeJob.h"

#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogProgress dialog

CDialogProgress::CDialogProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogProgress)
	m_csFileCnt = _T("");
	m_csFileNum = _T("");
	m_csKbPerSecFile = _T("");
	m_csKbPerSecJob = _T("");
	m_csKbCopiedFile = _T("");
	m_csKbCopiedJob = _T("");
	m_csKbTotalFile = _T("");
	m_csKbTotalJob = _T("");
	m_csETAFile = _T("");
	m_csETAJob = _T("");
	//}}AFX_DATA_INIT
	CancelRequest = false;
	OverrideWaitRequest = false;
}


void CDialogProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogProgress)
	DDX_Control(pDX, IDC_PROGRESS_JOB, m_progressJob);
	DDX_Control(pDX, IDC_PROGRESS_FILE, m_progressFile);
	DDX_Text(pDX, IDC_LBL_FILECNT, m_csFileCnt);
	DDX_Text(pDX, IDC_LBL_FILENUM, m_csFileNum);
	DDX_Text(pDX, IDC_LBL_KB_PER_SEC_FILE, m_csKbPerSecFile);
	DDX_Text(pDX, IDC_LBL_KB_PER_SEC_JOB, m_csKbPerSecJob);
	DDX_Text(pDX, IDC_LBL_KBCOPIED_JOB, m_csKbCopiedJob);
	DDX_Text(pDX, IDC_LBL_KBTOTAL_FILE, m_csKbTotalFile);
	DDX_Text(pDX, IDC_LBL_KBTOTAL_JOB, m_csKbTotalJob);
	DDX_Text(pDX, IDC_LBL_KBCOPIED_FILE, m_csKbCopiedFile);
	DDX_Text(pDX, IDC_LBL_ETA_FILE, m_csETAFile);
	DDX_Text(pDX, IDC_LBL_ETA_JOB, m_csETAJob);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_INFO_ICON, m_iconInfo);
}


BEGIN_MESSAGE_MAP(CDialogProgress, CDialog)
	//{{AFX_MSG_MAP(CDialogProgress)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START_NOW, &CDialogProgress::OnBnClickedButtonStartNow)
	ON_REGISTERED_MESSAGE(PerigeeJob::ms_TaskbarBtnCreated, &CDialogProgress::OnTaskbarBtnCreated)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogProgress message handlers

void CDialogProgress::OnCancel() 
{
	CancelRequest = true;	
}

BOOL CDialogProgress::OnInitDialog() 
{
	PerigeeJob::Instance()->TaskbarProgressBarRedTape(GetSafeHwnd());
	CDialog::OnInitDialog();
	
	// fake modality 
	//m_hParent = GetParent()->GetSafeHwnd();
	//::EnableWindow(m_hParent, FALSE);
	//EnableWindow(TRUE);

	m_iconInfo.SetIcon(::LoadIcon(NULL, IDI_INFORMATION));

	m_progressFile.SetRange32(0, 1000);
	m_progressJob.SetRange32(0, 1000);

	// Activate this window:  If we were launched with Ctrl or Shift down,
	// this doesn't happen automatically.
	SetForegroundWindow();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogProgress::OnClose() 
{
	// re-enable parent window
	//::EnableWindow(m_hParent, TRUE);	
	CDialog::OnClose();
}

void CDialogProgress::PostNcDestroy() 
{
	// re-enable parent window, in case OnClose() didn't happen
	//::EnableWindow(m_hParent, TRUE);
	CDialog::PostNcDestroy();
}

void CDialogProgress::SetSourceDest(LPCTSTR source, LPCTSTR dest)
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

void CDialogProgress::SetWaitMode(bool enable)
{
	UINT show = enable ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_INFO_ICON)->ShowWindow(show);
	GetDlgItem(IDC_LABEL_WAIT_PREVIOUS)->ShowWindow(show);
	GetDlgItem(IDC_BUTTON_START_NOW)->ShowWindow(show);
}

void CDialogProgress::OnBnClickedButtonStartNow()
{
	OverrideWaitRequest = true;
}

LRESULT CDialogProgress::OnTaskbarBtnCreated(WPARAM wParam, LPARAM lParam)
{
	PerigeeJob::Instance()->OnTaskbarBtnCreated();
	return 0;
}
