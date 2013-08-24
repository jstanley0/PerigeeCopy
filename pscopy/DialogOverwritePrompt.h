// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_DIALOGOVERWRITEPROMPT_H__006F9221_FDA4_484F_A0E2_C28BE16E061F__INCLUDED_)
#define AFX_DIALOGOVERWRITEPROMPT_H__006F9221_FDA4_484F_A0E2_C28BE16E061F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogOverwritePrompt.h : header file
//

#include "PerigeeCopyOptions.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogOverwritePrompt dialog

class CDialogOverwritePrompt : public CDialog
{
// Construction
public:
	CDialogOverwritePrompt(LPCTSTR source, LPCTSTR dest, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogOverwritePrompt)
	enum { IDD = IDD_OVERWRITE_PROMPT };
	CStatic	m_Icon2;
	CStatic	m_lblTitleLine;
	CStatic	m_Icon1;
	BOOL	m_fApplyToAll;
	CString	m_csDate1;
	CString	m_csDate2;
	CString	m_csSize1;
	CString	m_csSize2;
	//}}AFX_DATA

	CPerigeeCopyOptions::overwrite_spec GetOverwriteSpec()
	{
		return m_overwrite;
	}

	bool ApplyToAll()
	{
		return !!m_fApplyToAll;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogOverwritePrompt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	LPCTSTR m_source, m_dest;
	HICON m_hicon1, m_hicon2;
	CPerigeeCopyOptions::overwrite_spec m_overwrite;

	void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CDialogOverwritePrompt)
	afx_msg void OnButtonYes();
	afx_msg void OnButtonNo();
	afx_msg void OnNewer();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonMakeUniqueName();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGOVERWRITEPROMPT_H__006F9221_FDA4_484F_A0E2_C28BE16E061F__INCLUDED_)
