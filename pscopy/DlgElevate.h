#pragma once
#include "afxwin.h"


// DlgElevate dialog

class DlgElevate : public CDialog
{
	DECLARE_DYNAMIC(DlgElevate)

public:
	DlgElevate(CString message, CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgElevate();

// Dialog Data
	enum { IDD = IDD_ELEVATE };

	enum result
	{
		Elevate = IDOK,
		Ignore = IDIGNORE,
		Cancel = IDCANCEL
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CString m_message;
public:
	CStatic m_iconWarning;
	afx_msg void OnBnClickedIgnore();
	virtual BOOL OnInitDialog();
};
