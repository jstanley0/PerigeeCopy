// DialogFailedFiles.cpp : implementation file
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "DialogFailedFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogFailedFiles dialog


CDialogFailedFiles::CDialogFailedFiles(CWnd* pParent, const PerigeeJob::file_list &errors) :
	CDialog(CDialogFailedFiles::IDD, pParent),
	m_errors(errors)
{
	//{{AFX_DATA_INIT(CDialogFailedFiles)
	m_log = _T("");
	//}}AFX_DATA_INIT
}


void CDialogFailedFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogFailedFiles)
	DDX_Control(pDX, IDOK, m_btnTryAgain);
	DDX_Text(pDX, IDC_EDIT_ERRORS, m_log);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogFailedFiles, CDialog)
	//{{AFX_MSG_MAP(CDialogFailedFiles)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogFailedFiles message handlers

BOOL CDialogFailedFiles::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int error_count = 0;

	// set up resizability 
	HWND hdlg = GetSafeHwnd();
	m_sizer.SetDialog(hdlg);
	m_sizer.AddWidget(hdlg, IDC_EDIT_ERRORS, 1.0, 1.0, 0.0, 0.0);
	m_sizer.AddWidget(hdlg, IDOK, 0.0, 0.0, 0.5, 1.0);
	m_sizer.AddWidget(hdlg, IDCANCEL, 0.0, 0.0, 0.5, 1.0);

	// if no non-warnings exist, disable the "Try Again" button
	for(PerigeeJob::file_list::const_iterator it = m_errors.begin();
		it != m_errors.end(); ++it)
	{
		CString msg = PerigeeJob::FormatError(*it);

		m_log += (msg + _T("\r\n"));
			
		if ( it->error_type < PerigeeJob::warn_first )
			error_count++;
	}

	m_btnTryAgain.EnableWindow( error_count > 0 );

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogFailedFiles::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	m_sizer.OnSize();
}

void CDialogFailedFiles::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	m_sizer.OnGetMinMaxInfo(lpMMI);
}
