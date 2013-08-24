// pscopy.cpp : Entry point for the PerigeeCopy executable.
//
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.
//
// Normally invoked by the PerigeeCopy.dll shell extension (not directly invoked
// by the end user).
//
// The operation, destination, and source files (if applicable) are given
// on stdin, in TCHARs (this means you can't mix the Win9x DLL with the Win2k
// EXE or vice versa).  Lines are terminated by a single \n.
//
// This information may also be provided in a file or named pipe specified on the
// command line: /request filename
//
// The first line specifies the command:
//     config  (show configure dialog)
//     copy    (copy files)
//     move    (move files)
//     delete  (delete files)
// The second line specifies the destination directory.
// Subsequent lines provide source files, one file per line.
// An empty line terminates the argument list.


#include "stdafx.h"
#include "pscopy.h"
#include "PerigeeCopyOptions.h"
#include "DialogOptions.h"
#include "PerigeeJob.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString GetLine(HANDLE hIn)
{
	CString csResult;

	for(;;)
	{
		DWORD read;
		TCHAR c;
		BOOL res = ReadFile(hIn, &c, sizeof(TCHAR), &read, NULL);
		if ( !res || (read < sizeof(TCHAR)) )
			return csResult;

		if ( c == _T('\n') )
			return csResult;
		
		csResult += c;
	}
}

void WriteLine(HANDLE hOut, const CString &ln)
{
	DWORD written;
	WriteFile(hOut, (LPCTSTR)ln, ln.GetLength() * sizeof(TCHAR), &written, NULL);
	WriteFile(hOut, _T("\n"), sizeof(TCHAR), &written, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CPscopyApp

BEGIN_MESSAGE_MAP(CPscopyApp, CWinApp)
	//{{AFX_MSG_MAP(CPscopyApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPscopyApp construction

CPscopyApp::CPscopyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPscopyApp object

CPscopyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPscopyApp initialization

BOOL CPscopyApp::InitInstance()
{
	AfxEnableControlContainer();

	CPerigeeCopyOptions options;

#ifdef _DEBUG
	if (options.wait_for_debugger)
	{
		while( !IsDebuggerPresent() )
			Sleep(500);
		DebugBreak();
	}
#endif

	// Open our input handle 
	
	HANDLE hIn = NULL;

	// check the command line for a pipe name
	int argc;
	LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	CString pipe_name;
	bool noprompt = false;
	for(int i = 0; i < argc; ++i)
	{
		if (i < argc - 1 && 0 == wcscmp(argv[i], L"/request"))
			pipe_name = argv[++i];
		else if (0 == wcscmp(argv[i], L"/noprompt"))
			noprompt = true;
	}

	if (!pipe_name.IsEmpty())
	{
		// read request from named pipe
		hIn = CreateFile( 
			 pipe_name,           // pipe name 
			 GENERIC_READ,	      // read access 
			 0,                   // no sharing 
			 NULL,                // default security attributes
			 OPEN_EXISTING,       // opens existing pipe 
			 0,                   // default attributes 
			 NULL);               // no template file 
		if (hIn == INVALID_HANDLE_VALUE)
		{
			TCHAR buf[MAX_PATH];
			FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
				NULL, GetLastError(), 0, buf, MAX_PATH, NULL );
			CString error_message;
			error_message.Format(_T("Failed to read request from %s: %s"), (LPCWSTR)pipe_name, buf);
			MessageBox(NULL, error_message, NULL, MB_ICONERROR | MB_OK);
			return FALSE;
		}
	}
	else
	{
		// read request from stdin
		hIn = GetStdHandle(STD_INPUT_HANDLE);
		if ( !hIn || (hIn == INVALID_HANDLE_VALUE) )
		{
			// We were launched independently; just configure
			CDialogOptions dlgOpts(options);
			dlgOpts.DoModal();
			return FALSE;
		}
	}
	
	// turn off confirmation prompts if /noprompt was called, and we're not configuring
	// this is used when relaunching elevated - the user already saw it
	if (noprompt) {
		options.prompt_for_confirmation_when_deleting = false;
		options.debug_prompt = false;
	}

	PerigeeJob::operation op;

	// Get the command
	CString csCommand( GetLine(hIn) );
	if ( csCommand == _T("config") )
	{
		CDialogOptions dlgOpts(options);
		dlgOpts.DoModal();
		return FALSE;
	}		
	else if ( csCommand == _T("copy") )
	{
		op = PerigeeJob::op_copy;
	}
	else if ( csCommand == _T("move") )
	{
		op = PerigeeJob::op_move;
	}
	else if ( csCommand == _T("delete") )
	{
		op = PerigeeJob::op_delete;
	}
	else
	{
		CString err;
		if ( csCommand.IsEmpty() )
		{
			// if no command was specified (PerigeeCopy.exe was run from a console standalone),
			// just run config
			CDialogOptions dlgOpts(options);
			dlgOpts.DoModal();
			return FALSE;
		}
		else
		{
			err = _T("Unrecognized command: ") + csCommand;
		}
		MessageBox( NULL, err, _T("PerigeeCopy"), MB_ICONERROR );
		
		return FALSE;
	}

	// Get the destination folder
	CString csDestFolder( GetLine(hIn) );

	// Get the list of source files
	CStringList source_files;
	for(;;)
	{
		CString cs = GetLine(hIn);
		if ( cs.IsEmpty() )
			break;
		source_files.AddTail(cs);
	}

	// Initialize COM, so we can do cool taskbar nonsense
	CoInitialize(NULL);
	
	// Create the job
	PerigeeJob *job = PerigeeJob::Create( csDestFolder, source_files, op, options );
	m_pMainWnd = job->GetMainWindow();

	// Run the job
	job->Run();

	// Check whether the job was cancelled because we need to elevate to administrator
	if (job->ShouldRetryElevated())
	{
		// Create a named pipe, which we will use to send the request to the new instance
		CString pipe_name;
		pipe_name.Format(_T("\\\\.\\pipe\\pscopy%08x"), GetTickCount());
		HANDLE hPipe = CreateNamedPipe(pipe_name, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_WAIT, 1,
			4096, 4096, 5000, NULL);
		if (INVALID_HANDLE_VALUE != hPipe)
		{
			TCHAR szModName[MAX_PATH];
			GetModuleFileName(AfxGetInstanceHandle(), szModName, MAX_PATH);

			CString params;
			params.Format(_T("/noprompt /request %s"), (LPCTSTR)pipe_name);

			// Launch the elevated process
			SHELLEXECUTEINFO sei;
			ZeroMemory(&sei, sizeof(sei));
			sei.cbSize = sizeof(sei);
			sei.hwnd = NULL;
			sei.lpVerb = L"runas";
			sei.lpFile = szModName;
			sei.lpParameters = (LPCTSTR)params;
			sei.nShow = SW_SHOWNORMAL;
			if (ShellExecuteEx(&sei))
			{
				// Wait for it to connect to the pipe
				// ... also deal with the possibility that the client already connected to the pipe
				BOOL res = ConnectNamedPipe(hPipe, NULL);
				DWORD err = GetLastError();
				if (res || err == ERROR_PIPE_CONNECTED)
				{
					// Send the request
					WriteLine(hPipe, csCommand);
					WriteLine(hPipe, csDestFolder);
					for(POSITION pos = source_files.GetHeadPosition(); pos; )
						WriteLine(hPipe, source_files.GetNext(pos));
				}
				else
				{
					CString msg;
					msg.Format(_T("Failed to connect to the elevated child process (%d)"), err);
					MessageBox( NULL, msg, _T("PerigeeCopy"), MB_ICONERROR );				
				}
			}
			else
			{
				DWORD err = GetLastError();
				CString msg;
				msg.Format(_T("Failed to launch the elevated child process (%d)"), err);
				MessageBox( NULL, msg, _T("PerigeeCopy"), MB_ICONERROR );				
			}

			// Clean up and exit
			CloseHandle(hPipe);
		}
		else
		{
			DWORD err = GetLastError();
			CString msg;
			msg.Format(_T("Failed to create an elevation request pipe (%d)"), err);
			MessageBox( NULL, msg, _T("PerigeeCopy"), MB_ICONERROR );
		}
	}

	// Clean up
	delete job;
	CoUninitialize();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
