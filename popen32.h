// popen32.h: interface for the popen32 class
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POPEN32_H__BEBC54D1_4981_4886_B324_161537C467A6__INCLUDED_)
#define AFX_POPEN32_H__BEBC54D1_4981_4886_B324_161537C467A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class popen32  
{
	BOOL fOpen;

	SECURITY_ATTRIBUTES saAttr; 
	HANDLE hChildStderrRd, hChildStderrWr,
		hChildStdoutRd, hChildStdoutWr,
		hChildStdinRd, hChildStdinWr;
	PROCESS_INFORMATION piInfo;
	STARTUPINFO siStartInfo; 

	DWORD m_error_code;

public:
	popen32();
	virtual ~popen32();
	BOOL open(LPCTSTR szProcess, LPCTSTR szCommandLine);

	void GetError(LPTSTR buf, int len);
	
	int Read(void *data, int sz);
	int ReadStderr(void *data, int sz);
	int Write(const void *data, int sz);
		
	int close();
};

#endif // !defined(AFX_POPEN32_H__BEBC54D1_4981_4886_B324_161537C467A6__INCLUDED_)
