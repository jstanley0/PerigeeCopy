// DialogOptions.cpp : implementation file
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "DialogOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogOptions dialog


CDialogOptions::CDialogOptions(CPerigeeCopyOptions &opts, CWnd* pParent /*=NULL*/) : 
	CDialog(CDialogOptions::IDD, pParent),
	m_options(opts)
	{
	//{{AFX_DATA_INIT(CDialogOptions)
	m_ArchiveOnly = FALSE;
	m_ClearArchiveBit = FALSE;
	m_OverwriteReadOnly = FALSE;
	m_SkipInventory = FALSE;
	m_ClearReadOnlyBit = FALSE;
	m_OnError = -1;
	m_PromptConfirmDelete = FALSE;
	m_UsePerigeeCopyByDefault = FALSE;
	m_SkipErrorFiles = FALSE;
	m_Recycle = FALSE;
	m_DebugPrompt = FALSE;
	m_WaitForPrevious = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOptions)
	DDX_Check(pDX, IDC_CHECK_ARCHIVE_ONLY, m_ArchiveOnly);
	DDX_Check(pDX, IDC_CHECK_CLEAR_ARCHIVE_BIT, m_ClearArchiveBit);
	DDX_Check(pDX, IDC_CHECK_OPT_OVERWRITE_READ_ONLY, m_OverwriteReadOnly);
	DDX_Check(pDX, IDC_CHECK_SKIP_INVENTORY, m_SkipInventory);
	DDX_Check(pDX, IDC_CHECK_CLEAR_READ_ONLY_BIT, m_ClearReadOnlyBit);
	DDX_Check(pDX, IDC_PROMPT_CONFIRM_DELETE, m_PromptConfirmDelete);
	DDX_Check(pDX, IDC_CHECK_USE_PERIGEECOPY_BY_DEFAULT, m_UsePerigeeCopyByDefault);
	DDX_Check(pDX, IDC_CHECK_SKIP_ERROR_FILES, m_SkipErrorFiles);
	DDX_Check(pDX, IDC_PROMPT_RECYCLE, m_Recycle);
	DDX_Check(pDX, IDC_PROMPT_DEBUG, m_DebugPrompt);
	DDX_Check(pDX, IDC_CHECK_WAIT_FOR_PREVIOUS_JOBS, m_WaitForPrevious);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogOptions, CDialog)
	//{{AFX_MSG_MAP(CDialogOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOptions message handlers

BOOL CDialogOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();

	switch(m_options.overwrite)
	{
	case CPerigeeCopyOptions::overwrite_always:
		CheckDlgButton(IDC_RADIO_OVERWRITE_ALWAYS, BST_CHECKED); break;
	case CPerigeeCopyOptions::overwrite_never:
		CheckDlgButton(IDC_RADIO_OVERWRITE_NEVER, BST_CHECKED); break;
	case CPerigeeCopyOptions::overwrite_if_newer:
		CheckDlgButton(IDC_RADIO_OVERWRITE_NEWER, BST_CHECKED); break;
	case CPerigeeCopyOptions::make_unique_name:
		CheckDlgButton(IDC_RADIO_OVERWRITE_MAKE_UNIQUE_NAME, BST_CHECKED); break;
	case CPerigeeCopyOptions::overwrite_prompt:
		CheckDlgButton(IDC_RADIO_OVERWRITE_PROMPT, BST_CHECKED); break;
	case CPerigeeCopyOptions::overwrite_postpone:
		CheckDlgButton(IDC_RADIO_OVERWRITE_POSTPONE, BST_CHECKED); break;
	}

	m_SkipErrorFiles = (int)m_options.skip_error_files;

	m_OverwriteReadOnly = m_options.overwrite_read_only;
	m_ClearReadOnlyBit = m_options.clear_read_only_after_copy;

	m_ArchiveOnly = m_options.only_copy_archive;
	m_ClearArchiveBit = m_options.clear_archive_after_copy;
	
	m_PromptConfirmDelete = m_options.prompt_for_confirmation_when_deleting;
	m_UsePerigeeCopyByDefault = m_options.use_perigee_copy_by_default_when_dragging;
	m_SkipInventory = m_options.skip_inventory;

	m_Recycle = m_options.recycle;

	m_WaitForPrevious = m_options.wait_for_previous;

	m_DebugPrompt = m_options.debug_prompt;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogOptions::OnOK() 
{
	UpdateData();

	if (IsDlgButtonChecked(IDC_RADIO_OVERWRITE_ALWAYS))
		m_options.overwrite = CPerigeeCopyOptions::overwrite_always;
	else if (IsDlgButtonChecked(IDC_RADIO_OVERWRITE_NEVER))
		m_options.overwrite = CPerigeeCopyOptions::overwrite_never;
	else if (IsDlgButtonChecked(IDC_RADIO_OVERWRITE_NEWER))
		m_options.overwrite = CPerigeeCopyOptions::overwrite_if_newer;
	else if (IsDlgButtonChecked(IDC_RADIO_OVERWRITE_MAKE_UNIQUE_NAME))
		m_options.overwrite = CPerigeeCopyOptions::make_unique_name;
	else if (IsDlgButtonChecked(IDC_RADIO_OVERWRITE_PROMPT))
		m_options.overwrite = CPerigeeCopyOptions::overwrite_prompt;
	else if (IsDlgButtonChecked(IDC_RADIO_OVERWRITE_POSTPONE))
		m_options.overwrite = CPerigeeCopyOptions::overwrite_postpone;

	m_options.skip_error_files = !!m_SkipErrorFiles;

	m_options.overwrite_read_only = !!m_OverwriteReadOnly;
	m_options.clear_read_only_after_copy = !!m_ClearReadOnlyBit;

	m_options.only_copy_archive = !!m_ArchiveOnly;
	m_options.clear_archive_after_copy = !!m_ClearArchiveBit;

	m_options.prompt_for_confirmation_when_deleting = !!m_PromptConfirmDelete;
	m_options.use_perigee_copy_by_default_when_dragging = !!m_UsePerigeeCopyByDefault;
	m_options.skip_inventory = !!m_SkipInventory;

	m_options.recycle = !!m_Recycle;

	m_options.wait_for_previous = !!m_WaitForPrevious;

	m_options.debug_prompt = !!m_DebugPrompt;

	m_options.Save();

	CDialog::OnOK();
}
