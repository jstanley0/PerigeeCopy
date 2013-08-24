// PerigeeDelete.cpp: implementation of the PerigeeDelete class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "PerigeeDelete.h"
#include "DialogFailedFiles.h"
#include "DlgConfirmDelete.h"
#include "VistaHelper.h"
#include <strsafe.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PerigeeDelete::PerigeeDelete(CPerigeeCopyOptions &opts, CStringList &source_files) :
	PerigeeJob(opts),
	m_recycle(opts.recycle)
{
	AppendFileList(m_files, source_files);
	m_progress.Create(CDialogDeleteProgress::IDD);
}

PerigeeDelete::~PerigeeDelete()
{

}

bool PerigeeDelete::Run()
{
	// If we're launched with the Shift key down, delete rather than recycle.
	if ( 0x8000 & GetAsyncKeyState(VK_SHIFT) )
		m_recycle = false;

	if ( m_options.prompt_for_confirmation_when_deleting || m_options.debug_prompt )
	{
		DlgConfirmDelete dcd(m_files);
		if ( m_recycle )
		{
			CString caption, prompt;
			caption.LoadString(IDS_CONFIRM_RECYCLE);
			prompt.LoadString(IDS_CONFIRM_RECYCLE_PROMPT);
			dcd.ChangeCaption(caption, prompt);
		}
		INT_PTR res = dcd.DoModal();
		if ( IDCANCEL == res )
		{
			return false;
		}
	}

	if (m_options.skip_inventory && VistaHelper::IsXPOrNewer())
		m_progress.m_Progress.ModifyStyle(0, PBS_MARQUEE);

	m_progress.ShowWindow(SW_SHOW);
	PostPrompt();

	Reset();
	if (!InitIt())
		return false;

	for(;;)
	{
		Reset();
		if ( DoIt() )
		{
			return true;
		}

		if ( m_cancel )
			return false;

		// show skipped files dialog
		PrePrompt(false);
		CDialogFailedFiles dff(NULL, m_error_files);
		INT_PTR res = dff.DoModal();
		PostPrompt();
		if ( IDOK == res )
		{
			// move failed files back to source list and try again
			m_files.clear();
			for(file_list::iterator it = m_error_files.begin(); 
				it != m_error_files.end(); ++it)
			{
				if ( it->error_type < warn_first )
				{
					// don't re-enumerate files in a directory for retry;
					// they will already be in the list
					it->recurse = false;	
					m_files.push_back(*it);
				}
			}
		}
		else
		{
			return false;
		}
	}
}

bool PerigeeDelete::InitIt()
{
	m_file_count = 0;
	m_dir_count = 0;
	m_files_deleted = 0;
	m_dirs_deleted = 0;

	// if applicable, do inventory
	if ( !m_options.skip_inventory )
	{
		CString prompt;
		prompt.LoadString( IDS_PREPARING_TO_DELETE_FILES );
		m_progress.SetCurrentFile(prompt);

		DoInventory();
		
		UpdateProgress();
		if ( m_cancel ) return false;
	}
	return true;
}

bool PerigeeDelete::DoIt()
{
	// begin delete job
	for(file_list::iterator it = m_files.begin(); it != m_files.end(); ++it)
	{
		DoFileDelete(it->source, GetFileAttributes(it->source), it->recurse);
		if ( m_cancel ) return false;
	}

	return m_error_files.empty();
}

void PerigeeDelete::DoFileDelete(CString source, DWORD attrs, bool recurse)
{
	if (attrs == INVALID_FILE_ATTRIBUTES)
		return;

	m_progress.SetCurrentFile(source);

	if ( m_options.overwrite_read_only )
	{
		PrepareOverwrite(source);
	}

	bool is_directory = (0 != (attrs & FILE_ATTRIBUTE_DIRECTORY));
	bool reparse_point = (0 != (attrs & FILE_ATTRIBUTE_REPARSE_POINT));

	if ( is_directory )
	{
		DoFolderDelete(source, recurse && !reparse_point);
	}
	else
	{
		m_progress.SetCurrentFile(source);

		if ( m_recycle )
		{
			while( !PsRecycle(source) )
			{
				file_info fi;
				fi.source = source;
				fi.error_code = GetLastError();
				fi.error_type = err_delete;
				if ( !HandleError(fi) )
				{
					break;
				}
			}
		}
		else
		{
			while( !DeleteFile(source) )
			{
				file_info fi;
				fi.source = source;
				fi.error_code = GetLastError();
				fi.error_type = err_delete;
				if ( !HandleError(fi) )
				{
					break;
				}
			}
		}

		m_files_deleted++;
		UpdateProgress();		
	}
}

void PerigeeDelete::DoFolderDelete(CString source_folder, bool recurse)
{
	if ( m_recycle )
	{
		// for namby-pamby beginners who need exact change to get on a PCI bus
		while ( !PsRecycle(source_folder) )
		{
			file_info fi;
			fi.source = source_folder;
			fi.error_code = GetLastError();
			fi.error_type = err_remove_dir;
			if ( !HandleError(fi) )
			{
				break;
			}
		}
	}
	else
	{
		if ( recurse )
		{
			CString search_path = PsAppendPath(source_folder, _T("*"));
			WIN32_FIND_DATA fd;
				
			HANDLE h = INVALID_HANDLE_VALUE;
			do
			{
				h = FindFirstFile(search_path, &fd);
				if ( h == INVALID_HANDLE_VALUE )
				{
					DWORD err = GetLastError();
					if (err == ERROR_FILE_NOT_FOUND)
						return;	// nothing to delete
					
					file_info fi;
					fi.source = source_folder;
					fi.dest   = CString();
					fi.error_code = err;
					fi.error_type = err_read_dir;
					if (!HandleError(fi))
						return;
				}
			} while (h == INVALID_HANDLE_VALUE);

			do
			{
				if ( PsIsSelfOrParentDirEntry(&fd) )
					continue;

				CString source = PsAppendPath(source_folder, fd.cFileName);
				
				DoFileDelete( source, fd.dwFileAttributes, recurse );
			}
			while( !m_cancel && FindNextFile(h, &fd) );
			FindClose(h);
		}

		while ( !RemoveDirectory(source_folder) )
		{
			file_info fi;
			fi.source = source_folder;
			fi.error_code = GetLastError();
			fi.error_type = err_remove_dir;

			// no sense retrying dir-not-empty
			if ( fi.error_code == ERROR_DIR_NOT_EMPTY )
			{
				m_error_files.push_back(fi);
				break;
			}
			else if ( !HandleError(fi) )
			{
				break;
			}
		}
	}

	m_dirs_deleted++;
	UpdateProgress();
}

void PerigeeDelete::DoInventory()
{
	for(file_list::iterator it = m_files.begin(); it != m_files.end(); ++it)
	{
		InventoryItem(it->source);
	}
}

void PerigeeDelete::InventoryItem(LPCTSTR name)
{
	DWORD attrs = GetFileAttributes(name);
	if ( INVALID_FILE_ATTRIBUTES == attrs )
		return;

	if (EnableElevatePrompt() &&
		!VistaHelper::CheckAccess(name, DELETE, m_token))
	{
		CString message;
		message.Format(attrs & FILE_ATTRIBUTE_DIRECTORY ?
			IDS_ELEVATION_REQUIRED_TO_DELETE_FOLDER :
		    IDS_ELEVATION_REQUIRED_TO_DELETE_FILE
			, name);
		DoElevatePrompt(message);
		if (m_cancel)
			return;
	}

	if ( attrs & FILE_ATTRIBUTE_DIRECTORY )
	{
		if ( m_recycle )
		{
			m_dir_count++;	// recycling a directory is not recursive
		}
		else
		{
			InventoryDirectory(name, attrs);
		}
	}
	else
	{
		m_file_count++;
		if ( 0 == (m_file_count & 0x3f) )
		{
			UpdateProgress();
			if ( m_cancel ) return;
		}
	}
}

void PerigeeDelete::InventoryDirectory(LPCTSTR name, DWORD attrs)
{
	m_dir_count++;

	if (attrs & FILE_ATTRIBUTE_REPARSE_POINT)
		return;		// don't traverse reparse points when deleting

	CString search_path = PsAppendPath(name, _T("*"));
	WIN32_FIND_DATA fd;
		
	HANDLE h = FindFirstFile(search_path, &fd);
	if ( h == INVALID_HANDLE_VALUE )
	{
		DWORD err = GetLastError();
		if (ERROR_ACCESS_DENIED == err && EnableElevatePrompt())
		{
			CString message;
			message.Format(IDS_ELEVATION_REQUIRED_TO_DELETE_FOLDER, name);
			DoElevatePrompt(message);
		}
		return;
	}

	do
	{
		if ( PsIsSelfOrParentDirEntry(&fd) )
			continue;

		CString child_name = PsAppendPath(name, fd.cFileName);

		if (EnableElevatePrompt() &&
			!VistaHelper::CheckAccess(child_name, DELETE, m_token))
		{
			CString message;
			message.Format(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ?
				IDS_ELEVATION_REQUIRED_TO_DELETE_FOLDER :
				IDS_ELEVATION_REQUIRED_TO_DELETE_FILE
				, child_name);
			DoElevatePrompt(message);
			if (m_cancel)
				break;
		}

		if ( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
			InventoryDirectory(child_name, fd.dwFileAttributes);
		}
		else
		{
			m_file_count++;		
			if ( 0 == (m_file_count & 0x3f) )
			{
				UpdateProgress();
			}
		}
	}
	while( !m_cancel && FindNextFile(h, &fd) );
	FindClose(h);
}

void PerigeeDelete::UpdateProgress()
{
	CString num_fmt(_T("%d "));
	
	m_progress.m_csFilesDel.Format(num_fmt, m_files_deleted);
	m_progress.m_csFoldersDel.Format(num_fmt, m_dirs_deleted);

	if ( m_file_count || m_dir_count )
	{
		m_progress.m_csFilesTot.Format(num_fmt, m_file_count);
		m_progress.m_csFoldersTot.Format(num_fmt, m_dir_count);

		m_progress.m_Progress.SetRange32(0, m_file_count + m_dir_count);
		m_progress.m_Progress.SetPos(m_files_deleted + m_dirs_deleted);
	}

	m_progress.UpdateData(FALSE);
	UpdateTitleBar(m_progress, IDS_DELETING_FILES, m_files_deleted + m_dirs_deleted, m_file_count + m_dir_count);
	ProcessMessages();
	m_cancel = m_cancel || m_progress.m_CancelRequest;
}

void PerigeeDelete::PrePrompt(bool error_state)
{
	PerigeeJob::PrePrompt(error_state);
	PauseProgressBar(m_progress.m_Progress, m_options.skip_inventory, error_state);
}

void PerigeeDelete::PostPrompt()
{
	PerigeeJob::PostPrompt();
	UnpauseProgressBar(m_progress.m_Progress, m_options.skip_inventory);
}