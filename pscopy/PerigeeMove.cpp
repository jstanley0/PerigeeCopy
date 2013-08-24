// PerigeeMove.cpp: implementation of the PerigeeMove class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "PerigeeMove.h"
#include "DialogFailedFiles.h"
#include "DlgConfirmDelete.h"
#include "VistaHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PerigeeMove::PerigeeMove(CPerigeeCopyOptions &opts, CString dest_folder, CStringList &source_files) :
	PerigeeJob(opts)
{
	AppendFileList(m_files, source_files, dest_folder);
	m_progress.Create(DlgMoveProgress::IDD);
}

PerigeeMove::~PerigeeMove()
{
}

bool PerigeeMove::Run()
{
	if ( m_options.debug_prompt )
	{
		DlgConfirmDelete dcd(m_files);
		CString csCaption;
		CString csTempl;
		CString csPrompt;

		csCaption.LoadString(IDS_CONFIRM_MOVE);
		csTempl.LoadString(IDS_CONFIRM_MOVE_PROMPT_TEMPL);
		csPrompt.Format(csTempl, (LPCTSTR)m_files[0].dest);

		dcd.ChangeCaption(csCaption, csPrompt);

		if ( IDCANCEL == dcd.DoModal() )
		{
			return false;
		}
	}
	
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

		// if we're in "postpone" mode, enable prompting now
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

bool PerigeeMove::InitIt()
{
	// on Windows Vista, verify we have permissions to do this, and prompt to elevate otherwise
	if (!m_options.skip_inventory && 
		VistaHelper::IsVistaOrNewer() && !VistaHelper::IsAdmin())
	{	
		CString prompt;
		prompt.LoadString( IDS_PREPARING_TO_MOVE_FILES );
		m_progress.SetSourceDest(prompt, _T(""));
		int count = 0;
		for(file_list::iterator it = m_files.begin(); it != m_files.end(); ++it)
		{
			if (!VistaHelper::CheckAccess(it->source, GENERIC_READ | DELETE, m_token))
			{
				CString message;
				message.Format(IDS_ELEVATION_REQUIRED_TO_MOVE_FILE, it->source);
				DoElevatePrompt(message);
				if (m_cancel)
					return false;
			}
			if (0 == (++count & 0x3f))
			{
				ProcessMessages();
				m_cancel = m_cancel || m_progress.m_CancelRequest;
			}
		}
	}

	return true;
}

bool PerigeeMove::DoIt()
{
	file_list::size_type num = 0, cnt = m_files.size();
	m_progress.m_Progress.SetRange32(0, cnt);
	m_progress.m_Progress.SetPos(0);
	m_progress.m_Progress.SetStep(1);
	for(file_list::iterator it = m_files.begin(); it != m_files.end(); ++it)
	{
		CString dest_name = PsAppendPath(it->dest, PathFindFileName(it->source));
		m_progress.SetSourceDest(it->source, dest_name);
		
		// first make sure the destination isn't the source; otherwise, there's nothing to do
		if (0 != it->source.CompareNoCase(dest_name))
		{
			// check whether the destination is a subfolder of the source, and skip
			if ( PathIsDirectory(it->source) && DestIsSubfolderOfSource(dest_name, it->source) )
			{
				file_info fi;
				fi.source = it->source;
				fi.dest = dest_name;
				fi.error_code = ERROR_INVALID_PARAMETER;
				fi.error_type = warn_move_dest_is_subfolder_of_source;
				m_error_files.push_back(fi);
				continue;
			}

			TargetState state = CheckTarget(dest_name, it->source);
			if ( targetCanBeOverwritten == state )
			{
				if ( m_options.overwrite_read_only )
					PrepareOverwrite(dest_name);

				if ( m_options.recycle )
					PsRecycle(dest_name);
				else
					DeleteFile(dest_name);
			}
			else if ( targetCannotBeOverwritten == state )
			{
				if (m_options.overwrite == CPerigeeCopyOptions::overwrite_postpone) {
					// put the file in the error list for now
					file_info fi;
					fi.source = it->source;
					fi.dest   = it->dest;
					fi.error_code = ERROR_FILE_EXISTS;
					fi.error_type = err_move;
					m_error_files.push_back(fi);
					continue;
				} else {
					// exclude the file from the job
					--cnt;
					m_progress.m_Progress.SetRange32(0, cnt);
					ProcessMessages();
					m_cancel = m_cancel || m_progress.m_CancelRequest;
					if ( m_cancel )
						break;
					continue;
				}
			}

			while ( !MoveFile(it->source, dest_name) )
			{
				file_info fi;
				fi.source = it->source;
				fi.dest   = it->dest;
				fi.error_code = GetLastError();
				fi.error_type = err_move;
				if ( !HandleError(fi) )
					break;
			}
		}

		m_progress.m_Progress.StepIt();
		UpdateTitleBar(m_progress, IDS_MOVING_FILES, num++, cnt);
		ProcessMessages();
		m_cancel = m_cancel || m_progress.m_CancelRequest;

		if ( m_cancel )
			break;
	}

	return m_error_files.empty();
}

void PerigeeMove::PrePrompt(bool error_state)
{
	PerigeeJob::PrePrompt(error_state);
	PauseProgressBar(m_progress.m_Progress, false, error_state);
}

void PerigeeMove::PostPrompt()
{
	PerigeeJob::PostPrompt();
	UnpauseProgressBar(m_progress.m_Progress, false);
}
