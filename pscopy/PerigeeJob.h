// PerigeeJob.h: interface for the PerigeeJob class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_PERIGEEJOB_H__D814CB0A_377F_4CED_BEEC_6B8C459E9987__INCLUDED_)
#define AFX_PERIGEEJOB_H__D814CB0A_377F_4CED_BEEC_6B8C459E9987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PerigeeCopyOptions.h"
#include <vector>

class PerigeeJob  
{
public:
	static PerigeeJob *Instance() { return ms_instance; }

	enum operation { op_none, op_copy, op_move, op_delete, op_configure };

	enum error_type_e {
		err_none,
		err_copy,
		err_move,
		err_delete,
		err_create_dir,
		err_remove_dir,
		err_read_dir,
		warn_first,
		warn_remove_file,
		warn_remove_dir,
		warn_move_dest_is_subfolder_of_source,
		warn_copy_dest_is_subfolder_of_source,
	};

	struct file_info {
		file_info() : error_code(NO_ERROR), error_type(err_none), recurse(true) {}
		CString source;
		CString dest;
		DWORD error_code;
		error_type_e error_type;
		bool recurse;
	};

	typedef std::vector<file_info> file_list;
	
	static PerigeeJob *Create(CString dest_folder, CStringList &source_files, operation op, CPerigeeCopyOptions &opts);

	explicit PerigeeJob(CPerigeeCopyOptions &opts);
	virtual ~PerigeeJob();

	virtual bool Run() = 0;

	bool ShouldRetryElevated() { return m_retry_elevated; }

	// helpers for kludgy API
	static bool PsIsSelfOrParentDirEntry(WIN32_FIND_DATA *fd);

	static bool PsGetFileSize(LPCTSTR name, __int64 &size);
	static bool PsGetFileSize(HANDLE h, __int64 &size);

	static bool PsGetFileTime(LPCTSTR file, LPFILETIME pft);

	static CString PsAppendPath(LPCTSTR left, LPCTSTR right);

	static bool FileIsNewer(LPCTSTR file1, LPCTSTR file2);	

	static CString FormatNumber(double num);
	static CString FormatFileTime(FILETIME &ft);

	static CString FormatFileSize(__int64 sz);

	// Format a title bar
	void UpdateTitleBar(CWnd &wnd, UINT action_str, __int64 count, __int64 total, bool bytes = false);

	// Format the error in readable localized text
	static CString FormatError(const file_info &fi);

	// Deletes the file/folder to recycle bin
	static bool PsRecycle(CString path);

	// Generates a unique filename for a file copied in its own directory
	static CString PsMakeUniqueCopyName(CString path);

	// test whether the first directory is a subdirectory of the second
	static bool DestIsSubfolderOfSource(const CString &dest, const CString &source);

	virtual CWnd *GetMainWindow() { return NULL; }
	
	static void PauseProgressBar(CWnd &ctl, bool indeterminate, bool error_state);
	static void UnpauseProgressBar(CWnd &ctl, bool indeterminate);

	static const UINT ms_TaskbarBtnCreated;
	void OnTaskbarBtnCreated();
	BOOL TaskbarProgressBarRedTape(HWND hwnd);

protected:
	// call base class also in your override
	virtual void Reset();

	// update the display
	virtual void UpdateProgress();

	// called before and after modal prompts
	// so we can put the progress bars into paused/error state
	// also used by PerigeeCopy jobs to release the single-instance mutex
	// and let other jobs continue to run
	virtual void PrePrompt(bool error_state);
	virtual void PostPrompt();
	
	void PauseProgressBar(CWnd *ctl, bool indeterminate, bool error_state);
	void UnpauseProgressBar(CWnd *ctl, bool indeterminate);

	// use the same dest for each source
	void AppendFileList(file_list &fl, CStringList &source, CString dest = CString());

	enum TargetState {
		targetDoesNotExist,
		targetCanBeOverwritten,
		targetCannotBeOverwritten
	};
	// Tests whether we can overwrite a file, taking into account user preferences.
	// Prompts the user if necessary.  Can rename destination file.
	TargetState CheckTarget(CString &dest, LPCTSTR source);

	// Helpful test for deciding whether to do an optimized move
	static bool TheUserIsNotTryingToOverwriteAFolder(CString dest_folder, CStringList &source_files);

	// Return true if we should retry; otherwise add the error to m_error_files
	// and set m_cancel if we should bail out
	bool HandleError(const file_info &fi);

	// Clears read-only, system, and hidden bits so that file can be overwritten
	void PrepareOverwrite(LPCTSTR dest);

	// Removes read-only bit
	void RemoveReadOnlyBit(LPCTSTR dest);

	// Tests whether the archive bit it set.
	bool IsArchiveBitSet(LPCTSTR path);

	// Removes the archive bit.
	void RemoveArchiveBit(LPCTSTR path);

	// make the dialog respond
	static void ProcessMessages();

	// prompt for elevation
	bool EnableElevatePrompt();
	void DoElevatePrompt(CString message);

	CPerigeeCopyOptions &m_options;
	file_list m_error_files;
	bool m_cancel;
	bool m_retry_elevated;
	bool m_skip_elevation;
	HANDLE m_token;

private:
	static PerigeeJob *ms_instance;
	HWND m_progress_hwnd;
	CComPtr<ITaskbarList3> mp_TaskbarList;
	bool m_indeterminate;
};

#endif // !defined(AFX_PERIGEEJOB_H__D814CB0A_377F_4CED_BEEC_6B8C459E9987__INCLUDED_)
