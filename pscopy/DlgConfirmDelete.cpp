// DlgConfirmDelete.cpp : implementation file
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "DlgConfirmDelete.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgConfirmDelete dialog


DlgConfirmDelete::DlgConfirmDelete(const PerigeeJob::file_list &fl, CWnd* pParent /*=NULL*/)
	: CDialog(DlgConfirmDelete::IDD, pParent),
	  m_fl(fl)
{
	//{{AFX_DATA_INIT(DlgConfirmDelete)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DlgConfirmDelete::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgConfirmDelete)
	DDX_Control(pDX, IDC_FILE_LIST, m_FileList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgConfirmDelete, CDialog)
	//{{AFX_MSG_MAP(DlgConfirmDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgConfirmDelete message handlers

BOOL DlgConfirmDelete::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// set new caption and prompt, if applicable
	if ( !m_caption.IsEmpty() )
		SetWindowText( m_caption );
	if ( !m_prompt.IsEmpty() )
		SetDlgItemText( IDC_PROMPT, m_prompt );

	CString filename;
	filename.LoadString(IDS_COL_FILENAME);
	m_FileList.InsertColumn(0, filename, LVCFMT_LEFT, 320);

	int smx = GetSystemMetrics(SM_CXSMICON);
	int smy = GetSystemMetrics(SM_CYSMICON);
	m_ImageList.Create(smx, smy, ILC_COLOR32, 16, 16); 
	m_FileList.SetImageList(&m_ImageList, LVSIL_SMALL);

	int nItem = 0;
	for( PerigeeJob::file_list::const_iterator it = m_fl.begin(); it != m_fl.end(); ++it )
	{
		int nImage = -1;

		SHFILEINFO fi;
		if ( SHGetFileInfo(it->source, 0, &fi, sizeof(fi), SHGFI_ICON | SHGFI_SMALLICON) )
		{
			nImage = m_ImageList.Add(fi.hIcon);
			DestroyIcon(fi.hIcon);
		}
				
		m_FileList.InsertItem(nItem++, it->source, nImage);
	}
		
	// Activate this window:  If we were launched with Ctrl or Shift down,
	// this doesn't happen automatically.
	SetForegroundWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
