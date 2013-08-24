// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_DLGCONFIRMDELETE_H__AE5D7306_DB5E_48FD_A1A5_617333F28292__INCLUDED_)
#define AFX_DLGCONFIRMDELETE_H__AE5D7306_DB5E_48FD_A1A5_617333F28292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfirmDelete.h : header file
//

#include <afxcmn.h>
#include "PerigeeJob.h"

/////////////////////////////////////////////////////////////////////////////
// DlgConfirmDelete dialog

class DlgConfirmDelete : public CDialog
{
// Construction
public:
	DlgConfirmDelete(const PerigeeJob::file_list &fl, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgConfirmDelete)
	enum { IDD = IDD_CONFIRM_DELETE };
	CListCtrl	m_FileList;
	//}}AFX_DATA

	void ChangeCaption(CString new_caption, CString new_prompt) {
		m_caption = new_caption;
		m_prompt  = new_prompt;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgConfirmDelete)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_caption, m_prompt;

	const PerigeeJob::file_list &m_fl;
	CImageList m_ImageList;

	// Generated message map functions
	//{{AFX_MSG(DlgConfirmDelete)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIRMDELETE_H__AE5D7306_DB5E_48FD_A1A5_617333F28292__INCLUDED_)
