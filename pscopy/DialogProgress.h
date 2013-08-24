// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_DIALOGPROGRESS_H__10E97351_F610_4C4D_929B_92310AA07C5D__INCLUDED_)
#define AFX_DIALOGPROGRESS_H__10E97351_F610_4C4D_929B_92310AA07C5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogProgress.h : header file
//

#include <afxcmn.h>
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogProgress dialog

class CDialogProgress : public CDialog
{
// Construction
public:
	CDialogProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogProgress)
	enum { IDD = IDD_DIALOG_PROGRESS };
	CProgressCtrl	m_progressJob;
	CProgressCtrl	m_progressFile;
	CString	m_csFileCnt;
	CString	m_csFileNum;
	CString	m_csKbPerSecFile;
	CString	m_csKbPerSecJob;
	CString	m_csKbCopiedFile;
	CString	m_csKbCopiedJob;
	CString	m_csKbTotalFile;
	CString	m_csKbTotalJob;
	CString m_csETAFile;
	CString m_csETAJob;
	//}}AFX_DATA

	bool CancelRequest;
	bool OverrideWaitRequest;
	void SetSourceDest(LPCTSTR source, LPCTSTR dest);

	void SetWaitMode(bool enable);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HWND m_hParent;
	CStatic m_iconInfo;

	// Generated message map functions
	//{{AFX_MSG(CDialogProgress)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartNow();
	afx_msg LRESULT OnTaskbarBtnCreated(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPROGRESS_H__10E97351_F610_4C4D_929B_92310AA07C5D__INCLUDED_)
