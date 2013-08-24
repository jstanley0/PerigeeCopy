// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_DLGERROR_H__F3613508_BB84_49E0_A85A_2734778E9C07__INCLUDED_)
#define AFX_DLGERROR_H__F3613508_BB84_49E0_A85A_2734778E9C07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgError.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgError dialog

class DlgError : public CDialog
{
// Construction
public:
	DlgError(CString prompt, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgError)
	enum { IDD = IDD_ERROR };
	CStatic	m_Icon;
	//}}AFX_DATA

	enum action { actCancel, actTryAgain, actSkipFile, actSkipAll };
	action GetAction() { return m_act; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgError)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	action m_act;
	CString m_prompt;

	// Generated message map functions
	//{{AFX_MSG(DlgError)
	afx_msg void OnTryagain();
	afx_msg void OnSkipfile();
	afx_msg void OnSkipAll();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGERROR_H__F3613508_BB84_49E0_A85A_2734778E9C07__INCLUDED_)
