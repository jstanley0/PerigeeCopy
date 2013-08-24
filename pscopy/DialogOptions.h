// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_DIALOGOPTIONS_H__903B155B_6D42_446C_BCE2_5976B3A9E4FA__INCLUDED_)
#define AFX_DIALOGOPTIONS_H__903B155B_6D42_446C_BCE2_5976B3A9E4FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogOptions.h : header file
//

#include "PerigeeCopyOptions.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogOptions dialog

class CDialogOptions : public CDialog
{
// Construction
public:
	CDialogOptions(CPerigeeCopyOptions &opts, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogOptions)
	enum { IDD = IDD_OPTIONS_DIALOG };
	BOOL	m_ArchiveOnly;
	BOOL	m_ClearArchiveBit;
	BOOL	m_OverwriteReadOnly;
	BOOL	m_SkipInventory;
	BOOL	m_ClearReadOnlyBit;
	int		m_OnError;
	int		m_Overwrite;
	BOOL	m_PromptConfirmDelete;
	BOOL	m_UsePerigeeCopyByDefault;
	BOOL	m_SkipErrorFiles;
	BOOL	m_Recycle;
	BOOL	m_DebugPrompt;
	BOOL	m_WaitForPrevious;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPerigeeCopyOptions &m_options;

	// Generated message map functions
	//{{AFX_MSG(CDialogOptions)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGOPTIONS_H__903B155B_6D42_446C_BCE2_5976B3A9E4FA__INCLUDED_)
