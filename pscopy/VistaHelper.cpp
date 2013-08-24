#include "stdafx.h"
#include "VistaHelper.h"

// return the version as major * 1000 + minor

static int GetWindowsVersion()
{
	static int version = 0;
	if (version == 0)
	{
		OSVERSIONINFO verinfo;
		ZeroMemory(&verinfo, sizeof(verinfo));
		verinfo.dwOSVersionInfoSize = sizeof(verinfo);
		if (GetVersionEx(&verinfo) && verinfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			version = verinfo.dwMajorVersion * 1000 + verinfo.dwMinorVersion;
		}
		else
		{
			version = -1;
		}
	}
	return version;
}

bool VistaHelper::IsXPOrNewer()
{
	return GetWindowsVersion() >= 5001;
}

bool VistaHelper::IsVistaOrNewer()
{
	return GetWindowsVersion() >= 6000;
}

bool VistaHelper::IsSevenOrNewer()
{
	return GetWindowsVersion() >= 6001;
}

bool VistaHelper::IsAdmin()
{
	bool s_check = false;
	bool s_admin = false;

	if (!s_check)
	{
		BOOL b;
		SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
		PSID AdministratorsGroup; 
		b = AllocateAndInitializeSid(
			&NtAuthority,
			2,
			SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS,
			0, 0, 0, 0, 0, 0,
			&AdministratorsGroup); 
		if(b) 
		{
			if (!CheckTokenMembership(NULL, AdministratorsGroup, &b)) 
			{
				 b = FALSE;
			} 
			FreeSid(AdministratorsGroup); 
		}
		s_admin = !!b;
		s_check = true;
	}

	return s_admin;
}

HANDLE VistaHelper::GetToken()
{
	// Get the primary token
	HANDLE hToken;
	BOOL ret = OpenThreadToken(GetCurrentThread(), TOKEN_DUPLICATE, TRUE, &hToken);
	if (!ret && GetLastError() == ERROR_NO_TOKEN)
		ret = OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE, &hToken);
	if (!ret)
		return INVALID_HANDLE_VALUE;

	// Now make an impersonation token out of it
	HANDLE hImpersonationToken;
	ret = DuplicateToken(hToken, SecurityImpersonation, &hImpersonationToken);
	CloseHandle(hToken);
	return ret ? hImpersonationToken : INVALID_HANDLE_VALUE;
}

bool VistaHelper::CheckAccess(LPCTSTR filename, DWORD rights_desired, HANDLE token)
{
	// If we don't have a valid impersonation token, then skip the check
	// and don't force a UAC elevation
	if (token == INVALID_HANDLE_VALUE)
		return true;

	// Get the security descriptor for the object.
	size_t len = 1024;
	PSECURITY_DESCRIPTOR sd = (PSECURITY_DESCRIPTOR)new char[len];
	DWORD needed;
	for(;;)
	{
		BOOL ret = GetFileSecurity(filename, 
			OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION,
			sd, len, &needed);
		DWORD err = GetLastError();
		if (ret)
		{
			break;
		}
		else
		{
			if (err == ERROR_INSUFFICIENT_BUFFER)
			{
				delete [] sd;
				len = needed;
				sd = (PSECURITY_DESCRIPTOR)new char[len];
				continue;
			}
			else if (err == ERROR_ACCESS_DENIED)
			{
				// if I don't even have READ_CONTROL, then I probably do not have
				// read, write, or delete either, so fail.
				return false;
			}

			// if we're failing for some other reason, then let that reason come up
			// when the copy is attempted - don't force a bogus UAC elevation
			return true;
		}
	}	

	// test whether this security descriptor allows us the rights we want.
	// first, generic rights must be mapped to specific rights...
	// these mappings for files are provided on http://msdn.microsoft.com/en-us/library/aa364399(VS.85).aspx
	// except for GENERIC_ALL, which is a guess...
	GENERIC_MAPPING gmap;
	gmap.GenericExecute = FILE_READ_ATTRIBUTES | STANDARD_RIGHTS_EXECUTE | SYNCHRONIZE ;
	gmap.GenericRead = FILE_READ_ATTRIBUTES | FILE_READ_DATA | FILE_READ_EA | STANDARD_RIGHTS_READ | SYNCHRONIZE ;
	gmap.GenericWrite = FILE_APPEND_DATA | FILE_WRITE_ATTRIBUTES | FILE_WRITE_DATA | FILE_WRITE_EA | STANDARD_RIGHTS_WRITE | SYNCHRONIZE ;
	gmap.GenericAll = gmap.GenericExecute | gmap.GenericRead | gmap.GenericWrite | STANDARD_RIGHTS_REQUIRED;
	MapGenericMask(&rights_desired, &gmap);

	BOOL bResult;
	DWORD granted = 0;
	char privilege_buf[1024];
	ZeroMemory(&privilege_buf[0], sizeof(privilege_buf));
	PRIVILEGE_SET *privs = (PRIVILEGE_SET *)&privilege_buf[0];
	privs->PrivilegeCount = 0;
	DWORD cbprivs = sizeof(privilege_buf);
	if (!AccessCheck(sd, token, rights_desired, &gmap, privs, &cbprivs, &granted, &bResult))
	{
		// again, if the check itself fails, don't do a UAC elevation
		bResult = TRUE;
	}

	// clean up
	delete [] sd;

	return bResult;
}
