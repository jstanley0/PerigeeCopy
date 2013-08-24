// PerigeeJob.cpp: implementation of the PerigeeJob class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "PerigeeJob.h"
#include "PerigeeCopy.h"
#include "PerigeeDelete.h"
#include "PerigeeMove.h"
#include "DialogOverwritePrompt.h"
#include "DlgError.h"
#include "DlgElevate.h"
#include "VistaHelper.h"

#include <strsafe.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const UINT PerigeeJob::ms_TaskbarBtnCreated = ::RegisterWindowMessage(_T("TaskbarButtonCreated"));
PerigeeJob *PerigeeJob::ms_instance = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PerigeeJob::PerigeeJob(CPerigeeCopyOptions &opts) :
	m_options(opts),
	m_cancel(false),
	m_retry_elevated(false),
	m_skip_elevation(false),
	m_token(INVALID_HANDLE_VALUE)
{
	ms_instance = this;
	if (VistaHelper::IsVistaOrNewer())
		m_token = VistaHelper::GetToken();
}

PerigeeJob::~PerigeeJob()
{
	if (m_token != INVALID_HANDLE_VALUE)
		CloseHandle(m_token);
}

/* static */ bool PerigeeJob::TheUserIsNotTryingToOverwriteAFolder(CString dest_folder, CStringList &source_files)
{
	POSITION pos = source_files.GetHeadPosition();
	while( pos )
	{
		CString dest_name = PsAppendPath(dest_folder, PathFindFileName(source_files.GetNext(pos)));
		if ( PathIsDirectory(dest_name) )
			return false;
	}
	return true;
}

/* static */ PerigeeJob *PerigeeJob::Create(CString dest_folder, CStringList &source_files, operation op, CPerigeeCopyOptions &opts)
{
	if ( source_files.GetCount() == 0 )
		return NULL;
	
	bool del_when_done = false;
	switch(op)
	{
	case op_move:
		if (PathIsSameRoot(dest_folder, source_files.GetHead()) &&
			TheUserIsNotTryingToOverwriteAFolder(dest_folder, source_files))
		{
			return new PerigeeMove(opts, dest_folder, source_files);
		}
		else
		{
			// simulate move with copy-then-delete
			del_when_done = true;
		}
		// fall through
	case op_copy:
		return new CPerigeeCopy(opts, dest_folder, source_files, del_when_done);

	case op_delete:
		return new PerigeeDelete(opts, source_files);
	}

	return NULL;
}

void PerigeeJob::Reset()
{
	m_error_files.clear();
}

void PerigeeJob::AppendFileList(file_list &fl, CStringList &source, CString dest)
{
	POSITION pos = source.GetHeadPosition();
	while(pos)
	{
		file_info fi;
		fi.source = source.GetNext(pos);
		fi.dest = dest;
		fl.push_back(fi);
	}
}

/* static */ bool PerigeeJob::PsIsSelfOrParentDirEntry(WIN32_FIND_DATA *fd)
{
	return (  (fd->cFileName[0] == _T('.')) &&
				( (fd->cFileName[1] == _T('\0')) ||
				  (fd->cFileName[1] == _T('.') && fd->cFileName[2] == _T('\0')) )  );
}

/* static */ CString PerigeeJob::PsAppendPath(LPCTSTR left, LPCTSTR right)
{
	TCHAR pat[MAX_PATH];
	StringCchCopyN(pat, MAX_PATH, left, MAX_PATH );
	PathAppend(pat, right);
	return pat;
}

/* static */ bool PerigeeJob::PsGetFileSize(HANDLE h, __int64 &size)
{
	ULARGE_INTEGER uli;
	SetLastError(NO_ERROR);
	uli.LowPart = GetFileSize(h, &uli.HighPart);
	if ( NO_ERROR == GetLastError() )
	{ 
		size = uli.QuadPart;
		return true;
	} 
	else
	{
		size = 0;
		return false;
	}
}

/* static */ bool PerigeeJob::PsGetFileSize(LPCTSTR name, __int64 &size)
{
		
	HANDLE h = CreateFile(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, 0, NULL);
	if ( h == INVALID_HANDLE_VALUE )
		return false;
 
	bool ret = PsGetFileSize(h, size);

	CloseHandle(h);
	return ret;
}

// returns the modification date of the file
/* static */ bool PerigeeJob::PsGetFileTime(LPCTSTR file, LPFILETIME pft)
{
	HANDLE h = CreateFile(file, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, 0, NULL);
	if ( h == INVALID_HANDLE_VALUE )
		return false;
	bool ret = !!GetFileTime(h, NULL, NULL, pft);
	CloseHandle(h);
	return ret;
}

// returns true if both files exist and file1 is newer than file2
/* static */ bool PerigeeJob::FileIsNewer(LPCTSTR file1, LPCTSTR file2)
{
	FILETIME ft1, ft2;
	if ( !PsGetFileTime(file1, &ft1) || !PsGetFileTime(file2, &ft2) )
		return false;

	return (CompareFileTime(&ft1, &ft2) > 0);
}

/* static */ CString PerigeeJob::FormatNumber(double num)
{
	TCHAR buf_out[64];
	CString cnum;
	cnum.Format(_T("%.2f"), num);
	if ( GetNumberFormat(LOCALE_USER_DEFAULT, 0, cnum, NULL, buf_out, 64) )
	{
		return CString(buf_out);
	}
	else return CString();
}

/* static */ CString PerigeeJob::FormatFileTime(FILETIME &ft)
{
	SYSTEMTIME st_local;
	SYSTEMTIME st;
	if ( !FileTimeToSystemTime(&ft, &st) )
		return CString();

	if ( !SystemTimeToTzSpecificLocalTime(NULL, &st, &st_local) )
		return CString();

	TCHAR buf[MAX_PATH];

	if ( !GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE, &st_local, NULL, buf, MAX_PATH) )
		return CString();

	CString date = buf;

	if ( GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st_local, NULL, buf, MAX_PATH) )
	{
		date += _T(" ");
		date += buf;
	}

	return date;
}

/* static */ void PerigeeJob::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }
}

PerigeeJob::TargetState PerigeeJob::CheckTarget(CString& dest, LPCTSTR source)
{
	DWORD dattr = GetFileAttributes(dest);
	if ( dattr == INVALID_FILE_ATTRIBUTES )
		return targetDoesNotExist;

	// if the source file disappeared, don't delete the destination
	DWORD sattr = GetFileAttributes(source);
	if ( sattr == INVALID_FILE_ATTRIBUTES )
		return targetCannotBeOverwritten;

	CPerigeeCopyOptions::overwrite_spec o = m_options.overwrite;

	for(;;)
	{
		switch(o)
		{
		case CPerigeeCopyOptions::overwrite_always:
			return targetCanBeOverwritten;
		case CPerigeeCopyOptions::overwrite_never:
		case CPerigeeCopyOptions::overwrite_postpone:
			return targetCannotBeOverwritten;
		case CPerigeeCopyOptions::overwrite_if_newer:
			return FileIsNewer(source, dest) ? 
				targetCanBeOverwritten : targetCannotBeOverwritten;
		case CPerigeeCopyOptions::make_unique_name:
			dest = PsMakeUniqueCopyName(dest);
			return targetDoesNotExist;
		}

		ASSERT( m_options.overwrite == CPerigeeCopyOptions::overwrite_prompt );

		UpdateProgress();	// make sure we're visually up to date before we freeze the job
		PrePrompt(false);
		CDialogOverwritePrompt op(source, dest);
		op.DoModal();
		PostPrompt();
		o = op.GetOverwriteSpec();
		if ( op.ApplyToAll() )
		{
			m_options.overwrite = o;
		}
	}

	// shouldn't get here
}

void PerigeeJob::PrepareOverwrite(LPCTSTR dest)
{
	DWORD attrs = GetFileAttributes(dest);
	if ( attrs == INVALID_FILE_ATTRIBUTES )
		return;

	DWORD clear_attrs = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY;
	if ( 0 != (attrs & clear_attrs) )
	{
		attrs &= ~clear_attrs;
		SetFileAttributes(dest, attrs);
	}
}

void PerigeeJob::RemoveReadOnlyBit(LPCTSTR dest)
{
	DWORD attrs = GetFileAttributes(dest);
	if ( attrs == INVALID_FILE_ATTRIBUTES )
		return;

	if ( 0 != (attrs & FILE_ATTRIBUTE_READONLY) )
	{
		attrs &= ~FILE_ATTRIBUTE_READONLY;
		SetFileAttributes(dest, attrs);
	}
}

bool PerigeeJob::IsArchiveBitSet(LPCTSTR path)
{
	DWORD attrs = GetFileAttributes(path);
	if ( attrs == INVALID_FILE_ATTRIBUTES )
		return false;

	return (0 != (attrs & FILE_ATTRIBUTE_ARCHIVE));
}

void PerigeeJob::RemoveArchiveBit(LPCTSTR path)
{
	DWORD attrs = GetFileAttributes(path);
	if ( attrs == INVALID_FILE_ATTRIBUTES )
		return;

	if ( 0 != (attrs & FILE_ATTRIBUTE_ARCHIVE) )
	{
		attrs &= ~FILE_ATTRIBUTE_ARCHIVE;
		SetFileAttributes(path, attrs);
	}
}

// Return true if we should retry; otherwise add the error to m_error_files
// and set m_cancel if we should bail out
bool PerigeeJob::HandleError(const file_info &fi)
{
	if ( m_cancel || m_options.skip_error_files || 
		m_options.overwrite == CPerigeeCopyOptions::overwrite_postpone ||
		(fi.error_code == ERROR_ACCESS_DENIED && m_skip_elevation) )
	{
		m_error_files.push_back(fi);
		return false;
	}

	CString msg = FormatError(fi);
	PrePrompt(true);
	DlgError errd(msg);
	errd.DoModal();
	PostPrompt();
	DlgError::action act = errd.GetAction();
	if ( act == DlgError::actTryAgain )
	{
		return true;
	}
	else 
	{
		m_error_files.push_back(fi);
		if ( act == DlgError::actCancel )
		{
			m_cancel = true;
		}
	}

	if ( act == DlgError::actSkipAll )
		m_options.skip_error_files = true;	// don't bother the user again

	return false;
}

CString PerigeeJob::FormatError(const file_info &fi)
{
	TCHAR sys_error[MAX_PATH] = { _T('\0') };
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		fi.error_code,
		0,
		sys_error,
		MAX_PATH,
		NULL);
		
	CString templ;
	CString msg;
	
	switch (fi.error_type)
	{
	case err_copy:
		templ.LoadString(IDS_COULD_NOT_COPY_FILE);
		msg.Format(templ, (LPCTSTR)fi.source, (LPCTSTR)fi.dest, sys_error);
		break;
	case err_move:
		templ.LoadString(IDS_COULD_NOT_MOVE_FILE);
		msg.Format(templ, (LPCTSTR)fi.source, (LPCTSTR)fi.dest, sys_error);
		break;
	case err_delete:
		templ.LoadString(IDS_COULD_NOT_DELETE_FILE);
		msg.Format(templ, (LPCTSTR)fi.source, sys_error);
		break;
	case err_create_dir:
		{
			CString dest_folder = PsAppendPath(fi.dest, PathFindFileName(fi.source));
			templ.LoadString(IDS_COULD_NOT_CREATE_DIRECTORY);
			msg.Format(templ, (LPCTSTR)dest_folder, sys_error);
		}
		break;
	case err_remove_dir:
		templ.LoadString(IDS_COULD_NOT_REMOVE_DIRECTORY);
		msg.Format(templ, (LPCTSTR)fi.source, sys_error);
		break;
	case err_read_dir:
		msg.Format(IDS_COULD_NOT_READ_DIRECTORY, (LPCTSTR)fi.source, sys_error);
		break;
	case warn_remove_file:
		templ.LoadString(IDS_WARN_COULD_NOT_REMOVE_FILE);
		msg.Format(templ, (LPCTSTR)fi.source, sys_error);
		break;
	case warn_remove_dir:
		templ.LoadString(IDS_WARN_COULD_NOT_REMOVE_DIR);
		msg.Format(templ, (LPCTSTR)fi.source, sys_error);
		break;
	case warn_move_dest_is_subfolder_of_source:
		templ.LoadString(IDS_MOVE_DEST_IS_SUBFOLDER_OF_SOURCE);
		msg.Format(templ, (LPCTSTR)fi.source, (LPCTSTR)fi.dest);
		break;
	case warn_copy_dest_is_subfolder_of_source:
		templ.LoadString(IDS_COPY_DEST_IS_SUBFOLDER_OF_SOURCE);
		msg.Format(templ, (LPCTSTR)fi.source, (LPCTSTR)fi.dest);
		break;
	default:
		ASSERT(0);
		templ.LoadString(IDS_GENERIC_ERROR);
		msg.Format(templ, (LPCTSTR)fi.source, sys_error);
	}

	return msg;
}

/* static */ bool PerigeeJob::PsRecycle(CString path)
{
	std::vector<TCHAR> dtstr(path.GetLength());
	memcpy(&dtstr[0], (LPCTSTR)path, dtstr.size() * sizeof(TCHAR));
	dtstr.push_back(0);
	dtstr.push_back(0);
	
	SHFILEOPSTRUCT shop;
	shop.hwnd = NULL;
	shop.wFunc = FO_DELETE;
	shop.pFrom = &dtstr[0];
	shop.pTo = NULL;
	shop.fFlags = FOF_ALLOWUNDO			// for recycle
		| FOF_NOCONFIRMATION			// we've already done that
		| FOF_NO_CONNECTED_ELEMENTS		// because that's retarted
		| FOF_NOERRORUI | FOF_SILENT	// we already have a UI, thankyouverymuch
		;
	shop.fAnyOperationsAborted = FALSE;
	shop.hNameMappings = NULL;
	shop.lpszProgressTitle = NULL;
	HRESULT res = SHFileOperation(&shop);
	if ( res == 0 )
		return true;

	SetLastError(res);
	return false;
}

/* static */ CString PerigeeJob::PsMakeUniqueCopyName(CString path)
{
	int suffix_number = 1;
	
	// Get the position of the file's extension
	LPTSTR szPath = path.LockBuffer();
	int insertPos = PathFindExtension(szPath) - szPath;
	path.UnlockBuffer();

	// See if the file already has a bracketed numeric suffix in its name;
	// if so, retrieve the number and erase the suffix
	if ( insertPos >= 3 )
	{
		if ( path[insertPos - 1] == _T(']') )
		{
			int openBracketPos = insertPos - 2;
			while( openBracketPos >= 0 )
			{
				if ( path[openBracketPos] == _T('[') )
				{
					int ssize = insertPos - openBracketPos;
					if ( ssize > 2 )
					{
						suffix_number = 1 + _ttoi( path.Mid(openBracketPos + 1, ssize - 2) );
						path.Delete( openBracketPos, ssize );
						insertPos = openBracketPos;
					}
					break;
				}
				else if ( ! _istdigit( path[openBracketPos] ) )
					break;

				--openBracketPos;
			}
		}
	}

	// Keep trying bigger suffixes until we get a unique filename
	for(;;)
	{
		CString suffix;
		suffix.Format( _T("[%d]"), suffix_number );

		CString candidate_filename = path;
		candidate_filename.Insert(insertPos, suffix);

		if ( !PathFileExists(candidate_filename) )
			return candidate_filename;

		++suffix_number;
	}
}

/* static */ bool PerigeeJob::DestIsSubfolderOfSource(const CString &dest, const CString &source)
{
	CString augmented_source = source + _T("\\");
	return 0 == dest.Left(augmented_source.GetLength()).CompareNoCase(augmented_source);
}

/* static */ CString PerigeeJob::FormatFileSize(__int64 sz)
{
	static CString suffix;
	if ( suffix.IsEmpty() )
		suffix.LoadString(IDS_KB_SUFFIX);

	CString retval = PerigeeJob::FormatNumber((double)sz / 1024.0);
	if ( !retval.IsEmpty() )
		return retval + suffix;

	return CString();
}


void PerigeeJob::UpdateTitleBar(CWnd &wnd, UINT action_str, __int64 count, __int64 total, bool bytes /* = false */)
{
	// update the taskbar button progress bar, if present
	if (total > 0 && mp_TaskbarList)
	{
		mp_TaskbarList->SetProgressValue(m_progress_hwnd, count, total);
	}

	// limit updates to one second to prevent XP from getting tired of us and ignoring the update
	static DWORD last_update = 0;
	DWORD now = GetTickCount();
	if ( now - last_update < 1000 )
		return;
	last_update = now;
	
	CString op, prog;
	op.LoadString(action_str);
	if ( total > 0 )
	{
		prog.Format(_T("[%d%%] "), (count * 100) / total);
	}
	else
	{
		if ( bytes )
			prog.Format(_T("[%I64d MB] "), count >> 20);
		else
			prog.Format(_T("[%I64d] "), count);
	}
	wnd.SetWindowText(prog + op);
}

bool PerigeeJob::EnableElevatePrompt()
{
	return VistaHelper::IsVistaOrNewer() && 
		!VistaHelper::IsAdmin() &&
		!m_cancel &&
		!m_skip_elevation;
}

void PerigeeJob::DoElevatePrompt(CString message)
{
	PrePrompt(false);
	DlgElevate dlg(message);
	switch(dlg.DoModal())
	{
	case DlgElevate::Elevate:
		m_retry_elevated = true;
		m_cancel = true;
		break;
	case DlgElevate::Ignore:
		m_skip_elevation = true;
		break;
	case DlgElevate::Cancel:
		m_cancel = true;
		break;
	}
	PostPrompt();
}

void PerigeeJob::UpdateProgress()
{
}

void PerigeeJob::PrePrompt(bool error_state)
{
	if (mp_TaskbarList)
	{
		mp_TaskbarList->SetProgressState(m_progress_hwnd, error_state ? TBPF_ERROR : TBPF_PAUSED);
	}
}

void PerigeeJob::PostPrompt()
{
	if (mp_TaskbarList)
	{
		mp_TaskbarList->SetProgressState(m_progress_hwnd, m_options.skip_inventory ? TBPF_INDETERMINATE : TBPF_NORMAL);
	}
}

void PerigeeJob::OnTaskbarBtnCreated()
{
	if (VistaHelper::IsSevenOrNewer())
	{
		if (mp_TaskbarList)
			mp_TaskbarList.Release();
		mp_TaskbarList.CoCreateInstance( CLSID_TaskbarList );
		if (mp_TaskbarList)
		{
			mp_TaskbarList->SetProgressState(m_progress_hwnd, m_options.skip_inventory ? TBPF_INDETERMINATE : TBPF_NORMAL);	
		}
	}
}

// This crap is necessary to allow us to receive the message processed
// in the function above when running elevated.  We have to load the
// function dynamically so as to remain runnable on OSes prior to Win7.
typedef BOOL (WINAPI *ChangeWindowMessageFilterExProc)(HWND, UINT, DWORD, LPVOID);

BOOL PerigeeJob::TaskbarProgressBarRedTape(HWND hwnd)
{
	m_progress_hwnd = hwnd;
	BOOL res = FALSE;
	HMODULE user32 = LoadLibrary(_T("user32.dll"));
	if (user32)
	{
		ChangeWindowMessageFilterExProc proc = 
			(ChangeWindowMessageFilterExProc)
			GetProcAddress(user32, "ChangeWindowMessageFilterEx");
		if (proc)
		{
			res = (*proc)(hwnd, ms_TaskbarBtnCreated, MSGFLT_ALLOW, NULL);
		}
		FreeLibrary(user32);
	}
	return res;
}

void PerigeeJob::PauseProgressBar(CWnd &ctl, bool indeterminate, bool error_state)
{
	// turn off the indeterminate marquee
	if (indeterminate && VistaHelper::IsXPOrNewer())
		ctl.SendMessage(PBM_SETMARQUEE, 0);

	// set the state to paused/error
	if (VistaHelper::IsVistaOrNewer())
	{
		WPARAM state = error_state ? PBST_ERROR : PBST_PAUSED;
		ctl.SendMessage(PBM_SETSTATE, state);
	}
}

void PerigeeJob::UnpauseProgressBar(CWnd &ctl, bool indeterminate)
{
	if (VistaHelper::IsVistaOrNewer())
		ctl.SendMessage(PBM_SETSTATE, PBST_NORMAL);

	if (indeterminate && VistaHelper::IsXPOrNewer())
		ctl.SendMessage(PBM_SETMARQUEE, 1);
}
