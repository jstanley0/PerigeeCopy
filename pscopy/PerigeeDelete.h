// PerigeeDelete.h: interface for the PerigeeDelete class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_PERIGEEDELETE_H__C5080E94_6D5F_411E_99E5_81E7DEBBA76A__INCLUDED_)
#define AFX_PERIGEEDELETE_H__C5080E94_6D5F_411E_99E5_81E7DEBBA76A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PerigeeJob.h"
#include "DialogDeleteProgress.h"

class PerigeeDelete : public PerigeeJob  
{
public:
	PerigeeDelete(CPerigeeCopyOptions &opts, CStringList &source_files);
	virtual ~PerigeeDelete();

	bool Run();

private:
	CDialogDeleteProgress m_progress;

	bool InitIt();
	bool DoIt();
	void DoFileDelete(CString source, DWORD attrs, bool recurse);
	void DoFolderDelete(CString source_folder, bool recurse);

	void DoInventory();
	void InventoryItem(LPCTSTR name);
	void InventoryDirectory(LPCTSTR name, DWORD attrs);
	int m_file_count, m_dir_count;
	int m_files_deleted, m_dirs_deleted;

	file_list m_files;
	bool      m_recycle;

	void UpdateProgress();
	void PrePrompt(bool error_state);
	void PostPrompt();
	CWnd *GetMainWindow() { return &m_progress; }
};

#endif // !defined(AFX_PERIGEEDELETE_H__C5080E94_6D5F_411E_99E5_81E7DEBBA76A__INCLUDED_)
