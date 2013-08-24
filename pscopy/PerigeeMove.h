// PerigeeMove.h: interface for the PerigeeMove class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_PERIGEEMOVE_H__450861A8_51E9_406E_93A7_B5EB7530A04C__INCLUDED_)
#define AFX_PERIGEEMOVE_H__450861A8_51E9_406E_93A7_B5EB7530A04C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PerigeeJob.h"
#include "DlgMoveProgress.h"

class PerigeeMove : public PerigeeJob  
{
public:
	PerigeeMove(CPerigeeCopyOptions &opts, CString dest_folder, CStringList &source_files);
	virtual ~PerigeeMove();

	bool Run();

private:
	file_list m_files;
	DlgMoveProgress m_progress;

	bool InitIt();
	bool DoIt();

	void PrePrompt(bool error_state);
	void PostPrompt();
	CWnd *GetMainWindow() { return &m_progress; }
};

#endif // !defined(AFX_PERIGEEMOVE_H__450861A8_51E9_406E_93A7_B5EB7530A04C__INCLUDED_)
