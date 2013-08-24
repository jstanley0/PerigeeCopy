#pragma once

class VistaHelper
{
public:
	// returns true if the running OS is Vista or newer (nt platform, major version >= 6)
	static bool IsXPOrNewer();
	static bool IsVistaOrNewer();
	static bool IsSevenOrNewer();

	// returns true if the current user is an administrator
	static bool IsAdmin();

	// create an impersonation token for the running user, for use by CheckAccess().'
	// close with CloseHandle().
	static HANDLE GetToken();

	// returns true if the current user is allowed to perform the specified actions
	// on the specified object
	static bool CheckAccess(LPCTSTR filename, DWORD desired_rights, HANDLE token);

};
