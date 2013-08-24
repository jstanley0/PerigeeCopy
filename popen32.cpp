#include "StdAfx.h"
#include "popen32.h"

popen32::popen32() :
	hChildStderrRd(NULL), 
	hChildStderrWr(NULL),
	hChildStdoutRd(NULL),
	hChildStdoutWr(NULL),
	hChildStdinRd(NULL),
	hChildStdinWr(NULL),
	m_error_code(NO_ERROR),
	fOpen(false)
{
}

popen32::~popen32()
{
	close();
}

BOOL popen32::open(LPCTSTR szProcess, LPCTSTR szCommandLine)
{
	close();

// Set the bInheritHandle flag so pipe handles are inherited. 
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES); 
	saAttr.bInheritHandle = TRUE; 
	saAttr.lpSecurityDescriptor = NULL; 
    
// Create pipes for the child process's STDERR, STDOUT, STDIN. 
	CreatePipe(&hChildStderrRd, &hChildStderrWr, &saAttr, 0);
	CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0); 
	CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0);


// Set up members of STARTUPINFO structure. 
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
	siStartInfo.cb = sizeof(STARTUPINFO); 
	siStartInfo.hStdError = hChildStderrWr;  
	siStartInfo.hStdInput = hChildStdinRd;  
	siStartInfo.hStdOutput = hChildStdoutWr;
	siStartInfo.wShowWindow = SW_SHOWNORMAL;
	siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

// Make a modifiable command line buffer, because Windows is retarted...
	TCHAR *szWindowsIsRetarted = (TCHAR *)alloca( (_tcslen(szCommandLine) + 1) * sizeof(TCHAR) );
	_tcscpy(szWindowsIsRetarted, szCommandLine);

// Start the process
	fOpen = CreateProcess(szProcess, szWindowsIsRetarted, &saAttr, &saAttr, TRUE, 
		DETACHED_PROCESS, NULL, NULL, 
		&siStartInfo, &piInfo);
	if ( !fOpen )
		m_error_code = GetLastError();

	// we don't need this end of these handles
	CloseHandle(hChildStderrWr);
	CloseHandle(hChildStdinRd);
	CloseHandle(hChildStdoutWr);

	if(!fOpen) // process creation failed; clean up pipes
	{
		// Clean up -- Close remaining pipes
		CloseHandle(hChildStderrRd);
		CloseHandle(hChildStdinWr);
		CloseHandle(hChildStdoutRd);

		// Close handle to thread and process
		CloseHandle(piInfo.hThread);
		CloseHandle(piInfo.hProcess);
	}
	
	return fOpen;
}

void popen32::GetError(LPTSTR buf, int len)
{
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, m_error_code, 0,
		buf, len, NULL );
}

int popen32::close()
{
	int dwPrStat = -1;
	
	if(fOpen) 
	{
		/*
		// Wait for process to terminate...
		if(WAIT_TIMEOUT == WaitForSingleObject(piInfo.hProcess, 3000))
		{
			if(IDYES == MessageBox(NULL, "Terminate process?", "Ping refuses to die!", MB_YESNO))
				TerminateProcess(piInfo.hProcess, 0);
		}
		*/

		// Save the termination code, if applicable
		GetExitCodeProcess(piInfo.hProcess, (DWORD *)&dwPrStat);
		
		// Clean up -- Close remaining pipes
		CloseHandle(hChildStderrRd);
		CloseHandle(hChildStdinWr);
		CloseHandle(hChildStdoutRd);

		// Close handle to thread and process
		CloseHandle(piInfo.hThread);
		CloseHandle(piInfo.hProcess);
		
		fOpen = false;
	}
	
	return dwPrStat;
}

int popen32::Read(void *data, int sz)
{
	DWORD cnt;
	if (ReadFile(hChildStdoutRd, data, sz, &cnt, NULL))
	{
		return cnt;
	}
	else
	{
		m_error_code = GetLastError();
		return -1;
	}
}

int popen32::ReadStderr(void *data, int sz)
{
	DWORD cnt;
	if (ReadFile(hChildStderrRd, data, sz, &cnt, NULL))
	{
		return cnt;
	}
	else
	{
		m_error_code = GetLastError();
		return -1;
	}
}

int popen32::Write(const void *data, int sz)
{
	DWORD cnt;
	if (WriteFile(hChildStdinWr, data, sz, &cnt, NULL))
	{
		return cnt;
	}
	else
	{
		m_error_code = GetLastError();
		return -1;
	}
}
	