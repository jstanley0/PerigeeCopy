// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_DIALOGDELETEPROGRESS_H__4EC39A7B_B8C5_4A95_BADB_6110E350CB11__INCLUDED_)
#define AFX_DIALOGDELETEPROGRESS_H__4EC39A7B_B8C5_4A95_BADB_6110E350CB11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDeleteProgress.h : header file
//

#include <afxcmn.h>

/////////////////////////////////////////////////////////////////////////////
// CDialogDeleteProgress dialog

class CDialogDeleteProgress : public CDialog
{
// Construction
public:
	CDialogDeleteProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDeleteProgress)
	enum { IDD = IDD_DIALOG_DELETE_PROGRESS };
	CProgressCtrl	m_Progress;
	CString	m_csFilesDel;
	CString	m_csFilesTot;
	CString	m_csFoldersDel;
	CString	m_csFoldersTot;
	//}}AFX_DATA

	void SetCurrentFile(LPCTSTR file);
	bool m_CancelRequest;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDeleteProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDeleteProgress)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg LRESULT OnTaskbarBtnCreated(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDELETEPROGRESS_H__4EC39A7B_B8C5_4A95_BADB_6110E350CB11__INCLUDED_)
