// pscopy.h : main header file for the PSCOPY application
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_PSCOPY_H__D2C915A4_51D3_4FC3_8CAE_73E40E130B6E__INCLUDED_)
#define AFX_PSCOPY_H__D2C915A4_51D3_4FC3_8CAE_73E40E130B6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPscopyApp:
// See pscopy.cpp for the implementation of this class
//

class CPscopyApp : public CWinApp
{
public:
	CPscopyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPscopyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPscopyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSCOPY_H__D2C915A4_51D3_4FC3_8CAE_73E40E130B6E__INCLUDED_)
