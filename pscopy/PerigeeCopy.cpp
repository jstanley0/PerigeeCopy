// PerigeeCopy.cpp: implementation of the CPerigeeCopy class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "PerigeeCopy.h"
#include "DialogFailedFiles.h"
#include "DlgConfirmDelete.h"
#include "DlgElevate.h"
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

CPerigeeCopy::CPerigeeCopy(CPerigeeCopyOptions &opts, CString dest_folder, CStringList &source_files, bool delete_when_done) :
	PerigeeJob(opts),
	m_delete_when_done(delete_when_done),
	mp_lock(NULL)
{
	m_kb_suffix.LoadString(IDS_KB_SUFFIX);
	m_kb_per_sec_suffix.LoadString(IDS_KB_PER_SEC_SUFFIX);
	m_minutes_eta_template.LoadString(IDS_MINUTES_ETA_TEMPLATE);
	m_seconds_eta_template.LoadString(IDS_SECONDS_ETA_TEMPLATE);
		
	AppendFileList(m_files, source_files, dest_folder);

	m_progress.Create(CDialogProgress::IDD);
	CString caption;
	caption.LoadString( m_delete_when_done ? IDS_MOVING_FILES : IDS_COPYING_FILES );
	m_progress.SetWindowText(caption);
}

CPerigeeCopy::~CPerigeeCopy()
{
}

bool CPerigeeCopy::Run()
{
	if ( m_options.debug_prompt )
	{
		DlgConfirmDelete dcd(m_files);
		CString csCaption;
		CString csTempl;
		CString csPrompt;

		if ( m_delete_when_done ) 
		{
			csCaption.LoadString(IDS_CONFIRM_MOVE);
			csTempl.LoadString(IDS_CONFIRM_MOVE_PROMPT_TEMPL);
			csPrompt.Format(csTempl, (LPCTSTR)m_files[0].dest);
		}
		else 
		{
			csCaption.LoadString(IDS_CONFIRM_COPY);
			csTempl.LoadString(IDS_CONFIRM_COPY_PROMPT_TEMPL);
			csPrompt.Format(csTempl, (LPCTSTR)m_files[0].dest);
		}
		dcd.ChangeCaption(csCaption, csPrompt);

		if ( IDCANCEL == dcd.DoModal() )
		{
			return false;
		}
	}
	
	if (m_options.skip_inventory && VistaHelper::IsXPOrNewer())
		m_progress.m_progressJob.ModifyStyle(0, PBS_MARQUEE);

	m_progress.ShowWindow(SW_SHOW);

	// On Windows Vista, verify the user has permission to create files
	// in the destination folder, and prompt for elevation otherwise
	if (!m_files.empty() && EnableElevatePrompt() && 
		!VistaHelper::CheckAccess(m_files[0].dest, GENERIC_WRITE, m_token))
	{
		CString message;
		message.Format(IDS_ELEVATION_REQUIRED_TO_CREATE_FILES_IN_FOLDER, m_files[0].dest);
		DoElevatePrompt(message);
		if (m_cancel)
			return false;
	}

	// If another job is running, pause.
	CMutex mtx(FALSE, _T("PerigeeCopySerMtx"));
	CSingleLock lock(&mtx, FALSE);
	scoped_clear sc(this);
	if ( m_options.wait_for_previous )
	{
		mp_lock = &lock;
	}
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

		bool retry = false;

		// if we're in "postpone" mode, enable prompting and retry immediately
		if (m_options.overwrite == CPerigeeCopyOptions::overwrite_postpone) {
			retry = true;
			m_options.overwrite = CPerigeeCopyOptions::overwrite_prompt;
		}
		
		// show skipped files dialog
		if (!retry) {
			PrePrompt(false);
			CDialogFailedFiles dff(NULL, m_error_files);
			retry = IDOK == dff.DoModal();
			PostPrompt();
		}

		if ( retry )
		{
			// move failed files back to source list and try again
			m_files.clear();
			for(file_list::iterator it = m_error_files.begin(); 
				it != m_error_files.end(); ++it)
			{
				if ( it->error_type < warn_first )
				{
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

bool CPerigeeCopy::InitIt()
{
	m_job_started = false;
	m_file_count = 0;
	m_dir_count = 0;
	m_byte_count = 0;
	m_files_copied = 0;
	m_dirs_created = 0;
	m_bytes_copied = 0;
	m_job_time = 0;
	m_file_start = 0;
	m_cur_file_size = 0;
	m_cur_file_copied = 0;

	// if applicable, do inventory
	if ( !m_options.skip_inventory )
	{
		CString prompt;
		prompt.LoadString( m_delete_when_done ? 
			IDS_PREPARING_TO_MOVE_FILES :
			IDS_PREPARING_TO_COPY_FILES );
		m_progress.SetSourceDest(prompt, NULL);

		DoInventory();
		
		UpdateProgress();
		if ( m_cancel ) return false;
	}

	return true;
}

bool CPerigeeCopy::DoIt()
{
	// begin copy job
	m_job_started = true;
	for(file_list::iterator it = m_files.begin(); it != m_files.end(); ++it)
	{
		__int64 size;
		PsGetFileSize(it->source, size);
		DWORD attrs = GetFileAttributes(it->source);
		if (attrs == INVALID_FILE_ATTRIBUTES)
			continue;
		bool is_dir = (0 != (attrs & FILE_ATTRIBUTE_DIRECTORY));
		if ( DoFileCopy(it->source, it->dest, size, attrs) )
		{
			// recycle the source file/folder only if everything succeeded
			if ( m_delete_when_done && m_options.recycle )
			{
				if ( !PsRecycle(it->source) )
				{
					file_info fi;
					fi.source = it->source;
					fi.error_code = GetLastError();
					fi.error_type = is_dir ? warn_remove_dir : warn_remove_file;
					m_error_files.push_back(fi);
				}
			}
		}
				
		if ( m_cancel ) return false;
	}

	return m_error_files.empty();
}

// adds up the total size of all files to copy and sets m_file_count and m_byte_count
void CPerigeeCopy::DoInventory()
{
	for(file_list::iterator it = m_files.begin(); it != m_files.end(); ++it)
	{
		InventoryItem(it->source);
	}
}

void CPerigeeCopy::InventoryItem(LPCTSTR name)
{
	DWORD attrs = GetFileAttributes(name);
	if ( INVALID_FILE_ATTRIBUTES == attrs )
		return;

	if ( attrs & FILE_ATTRIBUTE_DIRECTORY )
	{
		InventoryDirectory(name, attrs);
		if (m_cancel)
			return;
	}
	else
	{
		if (EnableElevatePrompt() && 
			!VistaHelper::CheckAccess(name, GENERIC_READ | (m_delete_when_done ? DELETE : 0), m_token))
		{
			CString message;
			message.Format(m_delete_when_done ?
				IDS_ELEVATION_REQUIRED_TO_MOVE_FILE :
			    IDS_ELEVATION_REQUIRED_TO_COPY_FILE
				, name);
			DoElevatePrompt(message);
			if (m_cancel)
				return;
		}

		m_file_count++;

		__int64 size;
		if ( PsGetFileSize(name, size) )
			m_byte_count += size;

		if ( 0 == (m_file_count & 0x3f) )
		{
			UpdateProgress();
			if ( m_cancel ) return;
		}
	}
}

void CPerigeeCopy::InventoryDirectory(LPCTSTR name, DWORD attrs)
{
	m_dir_count++;	// count the directory even if we can't get any files in it

	if (m_delete_when_done && (attrs & FILE_ATTRIBUTE_REPARSE_POINT))
		return;		// don't traverse reparse points when moving files

	CString search_path = PsAppendPath(name, _T("*"));
	WIN32_FIND_DATA fd;
		
	HANDLE h = FindFirstFile(search_path, &fd);
	if ( h == INVALID_HANDLE_VALUE )
	{
		DWORD err = GetLastError();
		if (ERROR_ACCESS_DENIED == err && EnableElevatePrompt() &&
			!(attrs & FILE_ATTRIBUTE_REPARSE_POINT))
		{
			CString message;
			message.Format(IDS_ELEVATION_REQUIRED_TO_READ_FOLDER, name);
			DoElevatePrompt(message);
		}
		return;
	}

	do
	{
		if ( PsIsSelfOrParentDirEntry(&fd) )
			continue;

		if ( (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 )
		{
			CString subdir_name = PsAppendPath(name, fd.cFileName);
			InventoryDirectory(subdir_name, fd.dwFileAttributes);
		}
		else
		{
			if (EnableElevatePrompt() &&
				!VistaHelper::CheckAccess(name, GENERIC_READ | (m_delete_when_done ? DELETE : 0), m_token))
			{
				CString message;
				message.Format(m_delete_when_done ?
					IDS_ELEVATION_REQUIRED_TO_MOVE_FILE :
					IDS_ELEVATION_REQUIRED_TO_COPY_FILE
					, name);
				DoElevatePrompt(message);
				if (m_cancel)
					break;
			}

			m_file_count++;		

			ULARGE_INTEGER uli;
			uli.LowPart = fd.nFileSizeLow;
			uli.HighPart = fd.nFileSizeHigh;
			m_byte_count += uli.QuadPart;

			if ( 0 == (m_file_count & 0x3f) )
			{
				UpdateProgress();
			}
		}
	}
	while( !m_cancel && FindNextFile(h, &fd) );
	FindClose(h);
}


void CPerigeeCopy::UpdateProgress()
{
	CString cs;	
	DWORD now = GetTickCount();
		
	if ( m_job_started )
	{
		// file #
		cs.Format( _T("%d "), m_files_copied + 1 );
		m_progress.m_csFileNum = cs;

		// KB copied (file)
		m_progress.m_csKbCopiedFile = FormatKb( m_cur_file_copied );

		// Total size (file)
		m_progress.m_csKbTotalFile = FormatKb( m_cur_file_size );

		// KB/sec (file)
		if ( m_file_start )
		{
			m_progress.m_csKbPerSecFile = FormatKbPerSec( m_cur_file_copied, now - m_file_start );
		}

		// progress bar (file)
		if ( m_cur_file_size )
		{
			m_progress.m_progressFile.SetPos( (int)((m_cur_file_copied * 1000) / m_cur_file_size) );
			m_progress.m_csETAFile = FormatETA(m_cur_file_copied, m_cur_file_size, now - m_file_start);
		}
		else
		{
			m_progress.m_progressFile.SetPos( 0 );
			m_progress.m_csETAFile.Empty();
		}
	}

	// file count
	if ( m_file_count )
	{
		cs.Format( _T("%d "), m_file_count);
		m_progress.m_csFileCnt = cs;
	}

	__int64 total_copied = m_bytes_copied + m_cur_file_copied;

	// KB copied (total)
	m_progress.m_csKbCopiedJob = FormatKb( total_copied );
	
	if ( m_byte_count )
	{
		// Total size (job)
		m_progress.m_csKbTotalJob = FormatKb( m_byte_count );
		
		// progress bar (job)
		m_progress.m_progressJob.SetPos( (int)((total_copied * 1000) / m_byte_count) );
	}

	// KB/sec (job)
	if ( m_job_started )
	{
		DWORD elapsed = m_job_time;
		if ( m_file_start ) elapsed += (now - m_file_start);
		m_progress.m_csKbPerSecJob = FormatKbPerSec( total_copied, elapsed );

		// ETA (job)
		if ( m_byte_count && m_file_count )
		{
			m_progress.m_csETAJob = FormatETA(total_copied, m_byte_count, elapsed);
		}
	}

	m_progress.UpdateData(FALSE);

	// Title bar
	UpdateTitleBar(
		m_progress,
		m_delete_when_done ? IDS_MOVING_FILES : IDS_COPYING_FILES, 
		total_copied, 
		m_byte_count,
		true
	);

	
	ProcessMessages();
	
	m_cancel = m_cancel || m_progress.CancelRequest;
}

CString CPerigeeCopy::FormatKb(__int64 bytes)
{
	CString retval = FormatNumber( (double)bytes / 1024.0 );
	return retval + m_kb_suffix;
}

CString CPerigeeCopy::FormatKbPerSec(__int64 bytes, DWORD ms)
{
	if ( ms == 0 )
		return CString();

	CString retval = FormatNumber( (double)bytes / (1.024 * ms) );
	return retval + m_kb_per_sec_suffix;
}

bool CPerigeeCopy::DoFileCopy(CString source, CString dest_folder, __int64 size, DWORD source_attrs)
{
	CString dest_name = PsAppendPath(dest_folder, PathFindFileName(source) );

	bool is_directory = (0 != (source_attrs & FILE_ATTRIBUTE_DIRECTORY));
	bool reparse_point = (0 != (source_attrs & FILE_ATTRIBUTE_REPARSE_POINT));

	if ( is_directory )
	{
		// check whether the destination is a subfolder of the source, and skip
		if ( DestIsSubfolderOfSource(dest_folder, source) )
		{
			file_info fi;
			fi.source = source;
			fi.dest = dest_folder;
			fi.error_code = ERROR_INVALID_PARAMETER;
			fi.error_type = warn_copy_dest_is_subfolder_of_source;
			m_error_files.push_back(fi);
			return false;
		}

		// test whether the source and destination folders are the same
		if ( 0 == dest_name.CompareNoCase(source) )
		{
			dest_name = PsMakeUniqueCopyName(dest_name);
		}
		
		// create the destination direcory if it doesn't already exist
		if ( !PathIsDirectory(dest_name) )
		{
			while ( !CreateDirectory(dest_name, NULL) )
			{
				file_info fi;
				fi.source = source;
				fi.dest   = dest_folder;
				fi.error_code = GetLastError();
				fi.error_type = err_create_dir;

				if ( !HandleError(fi) )
					return false;
			}
			m_dirs_created++;
		}

		// if the source is a reparse point and we're doing a move operation,
		// don't follow it - just leave an empty directory
		if (reparse_point && m_delete_when_done)
			return true;

		return DoFolderCopy(source, dest_name, source_attrs);
	}

	if ( m_options.only_copy_archive )
	{
		if ( !IsArchiveBitSet(source) ) {
			if (!m_options.skip_inventory) {
				// deduct this file from the total
				m_file_count--;
				m_byte_count -= size;
			}
			return false;	// no files were copied
		}
	}

	m_progress.SetSourceDest(source, dest_name);
	m_cur_file_size = size;
	UpdateProgress();	

	// Test whether the source and destination filenames are the same
	if ( 0 == dest_name.CompareNoCase(source) )
	{
		dest_name = PsMakeUniqueCopyName(dest_name);
	}
	else
	{
		if ( targetCannotBeOverwritten == CheckTarget(dest_name, source) )
		{
			if (m_options.overwrite == CPerigeeCopyOptions::overwrite_postpone) {
				// put in the error list for now
				file_info fi;
				fi.source = source;
				fi.dest = dest_folder;
				fi.error_code = ERROR_FILE_EXISTS;
				fi.error_type = err_copy;
				m_error_files.push_back(fi);
				return false;
			}
			else
			{
				if (!m_options.skip_inventory)
				{
					// deduct this file from the total 
					m_file_count--;
					m_byte_count -= size;
				}
				return false;	// no files copied
			}
		}

		if ( m_options.overwrite_read_only )
		{
			PrepareOverwrite(dest_name);
		}
	}

	bool result = true;

	while( !DoTheActualFileCopyIMeanItThisTime(source, dest_name) )
	{
		file_info fi;
		fi.source = source;
		fi.dest   = dest_folder;
		fi.error_code = GetLastError();
		fi.error_type = m_delete_when_done ? err_move : err_copy;
		if ( !HandleError(fi) )
		{
			result = false;
			break;
		}
	}

	m_file_start = 0;
	
	if ( result )
	{
		if ( m_delete_when_done )
		{
			if ( !m_options.recycle && !DeleteFile(source) )
			{
				file_info fi;
				fi.source = source;
				fi.error_code = GetLastError();
				fi.error_type = warn_remove_file;
				m_error_files.push_back(fi);
			}
		}
		else if ( m_options.clear_archive_after_copy )
		{
			RemoveArchiveBit(source);
		}

		if ( m_options.clear_read_only_after_copy )
		{
			RemoveReadOnlyBit(dest_name);
		}

		m_files_copied++;
		m_bytes_copied += m_cur_file_size;
		
		m_cur_file_size = 0;
		m_cur_file_copied = 0;
	}

	return result;
}

bool CPerigeeCopy::DoFolderCopy(CString source_folder, CString dest_folder, DWORD source_attrs)
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
			if ( err == ERROR_FILE_NOT_FOUND || 
				(err == ERROR_ACCESS_DENIED && (source_attrs & FILE_ATTRIBUTE_REPARSE_POINT)) )
					// if we can't follow a symlink on copy, don't worry about it
				return true;	// all 0 files were copied!  w00t!
			
			file_info fi;
			fi.source = source_folder;
			fi.dest   = dest_folder;
			fi.error_code = err;
			fi.error_type = err_read_dir;
			if (!HandleError(fi))
				return false;
		}
	} while (h == INVALID_HANDLE_VALUE);

	bool result = true;

	do
	{
		if ( PsIsSelfOrParentDirEntry(&fd) )
			continue;

		CString source = PsAppendPath(source_folder, fd.cFileName);
		
		LARGE_INTEGER li;
		li.LowPart  = fd.nFileSizeLow;
		li.HighPart = fd.nFileSizeHigh;
		
		bool copied = DoFileCopy( source, dest_folder, li.QuadPart, fd.dwFileAttributes);
		result = result && copied;
	}
	while( !m_cancel && FindNextFile(h, &fd) );
	FindClose(h);

	if ( !m_options.recycle && m_delete_when_done )
	{
		if ( !RemoveDirectory(source_folder) )
		{
			file_info fi;
			fi.source = source_folder;
			fi.error_code = GetLastError();
			fi.error_type = warn_remove_dir;
			m_error_files.push_back(fi);
		}
	}
	else if ( m_options.clear_archive_after_copy )
	{
		RemoveArchiveBit(source_folder);
	}

	return result;
}

bool CPerigeeCopy::DoTheActualFileCopyIMeanItThisTime(CString source, CString dest)
{
	m_cur_file_copied = 0;
	m_file_start = GetTickCount();
	
	BOOL cancel = FALSE;
	BOOL res = CopyFileEx(source, dest, CopyProgressRoutine, this, &cancel, 0);
	m_job_time += (GetTickCount() - m_file_start);

	return !!res;
}

/* static */ DWORD CALLBACK CPerigeeCopy::CopyProgressRoutine(
	LARGE_INTEGER TotalFileSize,
	LARGE_INTEGER TotalBytesTransferred,
	LARGE_INTEGER StreamSize,
	LARGE_INTEGER StreamBytesTransferred,
	DWORD dwStreamNumber,
	DWORD dwCallbackReason,
	HANDLE hSourceFile,
	HANDLE hDestinationFile,
	LPVOID pData
)
{
	CPerigeeCopy *self = (CPerigeeCopy *)pData;

	// maybe this will include alternate data stream size, which we
	// did not get during the inventory
	self->m_cur_file_size = TotalFileSize.QuadPart;

	self->m_cur_file_copied = TotalBytesTransferred.QuadPart;

	ASSERT( self->m_cur_file_copied <= self->m_cur_file_size );

	// cap update frequency to 20fps to speed things up a bit
	static DWORD last_update = 0;
	DWORD now = GetTickCount();
	if ( now - last_update > 50 )
	{
		self->UpdateProgress();
		last_update = now;
	}

	if ( self->m_cancel )
		return PROGRESS_CANCEL;
	else
		return PROGRESS_CONTINUE;
}

CString CPerigeeCopy::FormatETA(__int64 count, __int64 total, DWORD ms_elapsed)
{
	if ( count == 0 || ms_elapsed < 125 )
		return CString();	// don't draw any conclusions on less than 1/8 second's data

	// Here I'm padding the schedule a bit, trying to avoid the usual underestimation
	// of the remaining time...
	int multiplier = 600 + (400 * count) / total;
	
	__int64 bytes_per_sec = (count * multiplier) / ms_elapsed;
	__int64 bytes_remaining = total - count;
	int seconds_remaining = (bytes_remaining + bytes_per_sec - 1) / bytes_per_sec; // round up
	if ( seconds_remaining <= 0 )
		return CString();

	if ( seconds_remaining > 45 )
	{
		// report minutes remaining, rounding up
		int minutes = (seconds_remaining + 30) / 60;
		CString msg;
		msg.Format(m_minutes_eta_template, minutes);
		return msg;
	}
	else if ( seconds_remaining > 10 )
	{
		// report seconds remaining, rounding up to the nearest 5
		seconds_remaining = ((seconds_remaining + 4) / 5) * 5;
		CString msg;
		msg.Format(m_seconds_eta_template, seconds_remaining);
		return msg;
	}
	else
	{
		// report seconds remaining
		CString msg;
		msg.Format(m_seconds_eta_template, seconds_remaining);
		return msg;
	}
}

void CPerigeeCopy::PrePrompt(bool error_state)
{
	PerigeeJob::PrePrompt(error_state);
	PauseProgressBar(m_progress.m_progressFile, false, error_state);
	PauseProgressBar(m_progress.m_progressJob, m_options.skip_inventory, error_state);
	
	if ( mp_lock )
	{
		mp_lock->Unlock();
	}
}

void CPerigeeCopy::PostPrompt()
{
	if ( mp_lock )
	{
		if (mp_lock->Lock(0))
		{
			// we're good.
		}
		else
		{
			// somebody else is copying.  wait until they're done.
			CString oldCaption;
			m_progress.GetWindowText(oldCaption);
			CString newCaption;
			newCaption.LoadString(IDS_WAITING);
			m_progress.SetWindowText(newCaption);
			m_progress.SetWaitMode(true);

			while( !mp_lock->Lock(200) )
			{
				ProcessMessages();
				if (m_progress.OverrideWaitRequest)
					break;	// proceed w/o the mutex
			}

			m_progress.SetWaitMode(false);
			m_progress.SetWindowText(oldCaption);
		}
	}

	PerigeeJob::PostPrompt();
	UnpauseProgressBar(m_progress.m_progressFile, false);
	UnpauseProgressBar(m_progress.m_progressJob, m_options.skip_inventory);
}
