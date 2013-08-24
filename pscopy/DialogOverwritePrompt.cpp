// DialogOverwritePrompt.cpp : implementation file
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include <shlwapi.h>
#include "stdafx.h"
#include "resource.h"
#include "DialogOverwritePrompt.h"
#include "PerigeeJob.h"
#include "PerigeeCopyOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogOverwritePrompt dialog


CDialogOverwritePrompt::CDialogOverwritePrompt(LPCTSTR source, LPCTSTR dest, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOverwritePrompt::IDD, pParent),
	  m_overwrite( CPerigeeCopyOptions::overwrite_never ),
	  m_source(source),
	  m_dest(dest),
	  m_hicon1(NULL),
	  m_hicon2(NULL)
{
	//{{AFX_DATA_INIT(CDialogOverwritePrompt)
	m_fApplyToAll = CPerigeeCopyOptions::GetApplyToAll();
	m_csDate1 = _T("");
	m_csDate2 = _T("");
	m_csSize1 = _T("");
	m_csSize2 = _T("");
	//}}AFX_DATA_INIT
}


void CDialogOverwritePrompt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOverwritePrompt)
	DDX_Control(pDX, IDC_ICON2, m_Icon2);
	DDX_Control(pDX, IDC_TITLE_LINE, m_lblTitleLine);
	DDX_Control(pDX, IDC_ICON1, m_Icon1);
	DDX_Check(pDX, IDC_CHECK_APPLY_TO_ALL, m_fApplyToAll);
	DDX_Text(pDX, IDC_DATE1, m_csDate1);
	DDX_Text(pDX, IDC_DATE2, m_csDate2);
	DDX_Text(pDX, IDC_SIZE1, m_csSize1);
	DDX_Text(pDX, IDC_SIZE2, m_csSize2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOverwritePrompt, CDialog)
	//{{AFX_MSG_MAP(CDialogOverwritePrompt)
	ON_BN_CLICKED(IDC_BUTTON_YES, OnButtonYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, OnButtonNo)
	ON_BN_CLICKED(IDC_NEWER, OnNewer)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_MAKE_UNIQUE_NAME, &CDialogOverwritePrompt::OnBnClickedButtonMakeUniqueName)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOverwritePrompt message handlers

void CDialogOverwritePrompt::OnButtonYes() 
{
	m_overwrite = CPerigeeCopyOptions::overwrite_always;
	OnOK();
}

void CDialogOverwritePrompt::OnButtonNo() 
{
	m_overwrite = CPerigeeCopyOptions::overwrite_never;
	OnOK();
}

void CDialogOverwritePrompt::OnNewer() 
{
	m_overwrite = CPerigeeCopyOptions::overwrite_if_newer;
	OnOK();
}

void CDialogOverwritePrompt::OnBnClickedButtonMakeUniqueName()
{
	m_overwrite = CPerigeeCopyOptions::make_unique_name;
	OnOK();
}

BOOL CDialogOverwritePrompt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// name
	CString title;
	m_lblTitleLine.GetWindowText(title);
	title += PathFindFileName(m_source);
	CString fullstop;
	fullstop.LoadString(IDS_FULLSTOP);
	title += fullstop;
	m_lblTitleLine.SetWindowText(title);

	// dates
	FILETIME date;
	if ( PerigeeJob::PsGetFileTime(m_dest, &date) ) {
		m_csDate1 = PerigeeJob::FormatFileTime(date);
	}
	if ( PerigeeJob::PsGetFileTime(m_source, &date) ) {
		m_csDate2 = PerigeeJob::FormatFileTime(date);
	}

	// sizes
	__int64 size;
	if ( PerigeeJob::PsGetFileSize(m_dest, size) ) {
		m_csSize1 = PerigeeJob::FormatFileSize(size);
	}
	if ( PerigeeJob::PsGetFileSize(m_source, size) ) {
		m_csSize2 = PerigeeJob::FormatFileSize(size);
	}

	// icons
	SHFILEINFO fi;
	if ( SHGetFileInfo(m_dest, 0, &fi, sizeof(fi), SHGFI_ICON | SHGFI_LARGEICON) )
	{
		m_hicon1 = fi.hIcon;
		m_Icon1.SetIcon(m_hicon1);
	}
	if ( SHGetFileInfo(m_source, 0, &fi, sizeof(fi), SHGFI_ICON | SHGFI_LARGEICON) )
	{
		m_hicon2 = fi.hIcon;
		m_Icon2.SetIcon(m_hicon2);
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogOverwritePrompt::OnOK()
{
	CDialog::OnOK();
	CPerigeeCopyOptions::SetApplyToAll(m_fApplyToAll);
}

void CDialogOverwritePrompt::PostNcDestroy() 
{
	DestroyIcon(m_hicon1);
	DestroyIcon(m_hicon2);
	
	CDialog::PostNcDestroy();
}


