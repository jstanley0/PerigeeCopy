// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_DIALOGFAILEDFILES_H__C71AD0FA_61A8_4F49_975A_EB9380731E12__INCLUDED_)
#define AFX_DIALOGFAILEDFILES_H__C71AD0FA_61A8_4F49_975A_EB9380731E12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogFailedFiles.h : header file
//

#include "PerigeeJob.h"
#include "SizeHelper.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogFailedFiles dialog

class CDialogFailedFiles : public CDialog
{
// Construction
public:
	CDialogFailedFiles(CWnd* pParent, const PerigeeJob::file_list &errors);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogFailedFiles)
	enum { IDD = IDD_FAILED_FILES };
	CButton	m_btnTryAgain;
	CString	m_log;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogFailedFiles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	const PerigeeJob::file_list &m_errors;
	SizeHelper m_sizer;

	// Generated message map functions
	//{{AFX_MSG(CDialogFailedFiles)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGFAILEDFILES_H__C71AD0FA_61A8_4F49_975A_EB9380731E12__INCLUDED_)
