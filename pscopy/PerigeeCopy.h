// PerigeeCopy.h: interface for the CPerigeeCopy class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_PERIGEECOPY_H__6462BA17_0A0D_42EC_96C0_C4898C634723__INCLUDED_)
#define AFX_PERIGEECOPY_H__6462BA17_0A0D_42EC_96C0_C4898C634723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
#include <afxcoll.h>
#include "PerigeeJob.h"
#include "DialogProgress.h"

class CPerigeeCopy : public PerigeeJob
{
public:
	CPerigeeCopy(CPerigeeCopyOptions &opts, CString dest_folder, CStringList &source_files, bool delete_when_done);
	virtual ~CPerigeeCopy();

	bool Run();

private:
	CDialogProgress m_progress;

	bool InitIt();
	bool DoIt();
	bool DoFileCopy(CString source, CString dest_folder, __int64 size, DWORD source_attrs);
	bool DoFolderCopy(CString source_folder, CString dest_folder, DWORD source_attrs);

	bool DoTheActualFileCopyIMeanItThisTime(CString source, CString dest);

	void DoInventory();
	void InventoryItem(LPCTSTR name);
	void InventoryDirectory(LPCTSTR name, DWORD attrs);
	void PrePrompt(bool error_state);
	void PostPrompt();
	CWnd *GetMainWindow() { return &m_progress; }

	int m_file_count, m_dir_count;
	__int64 m_byte_count;
	__int64 m_cur_file_size;

	int m_files_copied, m_dirs_created;
	__int64 m_bytes_copied;
	__int64 m_cur_file_copied;

	file_list m_files;
	bool m_delete_when_done;

	bool m_job_started;

	DWORD m_job_time;
	DWORD m_file_start;

	// a pointer to a stack object, for use by PrePrompt/PostPrompt; this class
	// will ensure the pointer is nulled when the object goes out of scope
	CSingleLock *mp_lock;
	class scoped_clear
	{
		CPerigeeCopy *mp_copy;
	public:
		scoped_clear(CPerigeeCopy *copy) : mp_copy(copy) {}
		~scoped_clear() { mp_copy->mp_lock = NULL; }
	};
	friend class scoped_clear;

	void UpdateProgress();
	static DWORD CALLBACK CopyProgressRoutine(
		LARGE_INTEGER TotalFileSize,
		LARGE_INTEGER TotalBytesTransferred,
		LARGE_INTEGER StreamSize,
		LARGE_INTEGER StreamBytesTransferred,
		DWORD dwStreamNumber,
		DWORD dwCallbackReason,
		HANDLE hSourceFile,
		HANDLE hDestinationFile,
		LPVOID pData
	);
	
	CString FormatKb(__int64 bytes);
	CString FormatKbPerSec(__int64 bytes, DWORD ms);
	CString FormatETA(__int64 count, __int64 total, DWORD elapsed);

	CString m_kb_suffix;
	CString m_kb_per_sec_suffix;
	CString m_minutes_eta_template;
	CString m_seconds_eta_template;
};

#endif // !defined(AFX_PERIGEECOPY_H__6462BA17_0A0D_42EC_96C0_C4898C634723__INCLUDED_)
