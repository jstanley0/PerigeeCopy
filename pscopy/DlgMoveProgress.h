// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_DLGMOVEPROGRESS_H__4D4B8AB7_F7C9_48BE_97DF_11DA0D1375E7__INCLUDED_)
#define AFX_DLGMOVEPROGRESS_H__4D4B8AB7_F7C9_48BE_97DF_11DA0D1375E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMoveProgress.h : header file
//

#include <afxcmn.h>

/////////////////////////////////////////////////////////////////////////////
// DlgMoveProgress dialog

class DlgMoveProgress : public CDialog
{
// Construction
public:
	DlgMoveProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgMoveProgress)
	enum { IDD = IDD_DIALOG_MOVE_PROGRESS };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA

	bool m_CancelRequest;
	void SetSourceDest(LPCTSTR src, LPCTSTR dest);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgMoveProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg LRESULT OnTaskbarBtnCreated(WPARAM wParam, LPARAM lParam);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgMoveProgress)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMOVEPROGRESS_H__4D4B8AB7_F7C9_48BE_97DF_11DA0D1375E7__INCLUDED_)
