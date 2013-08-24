// PerigeeCopyOptions.cpp: implementation of the CPerigeeCopyOptions class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "resource.h"
#include "PerigeeCopyOptions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const TCHAR *app_key = _T("Software\\Perigee Software\\PerigeeCopy");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPerigeeCopyOptions::CPerigeeCopyOptions()
{
	Load();
}

CPerigeeCopyOptions::~CPerigeeCopyOptions()
{

}

static DWORD GetValueImpl(HKEY key, LPCTSTR name, DWORD default_val)
{
	DWORD type;
	DWORD data;
	DWORD size = sizeof(DWORD);
	
	if ( ERROR_SUCCESS == RegQueryValueEx(key, name, NULL, &type, (LPBYTE)&data, &size) )
	{
		if ( type == REG_DWORD )
		{
			return data;
		}
	}

	return default_val;
}

static void SetValueImpl(HKEY key, LPCTSTR name, DWORD value)
{
	RegSetValueEx(key, name, 0, REG_DWORD, (CONST BYTE *)&value, sizeof(DWORD));
}

#define GET_VALUE(key, mod, var, default_val) \
	var = mod GetValueImpl(key, _T( #var ), (default_val))

#define SET_VALUE(key, var) \
	SetValueImpl(key, _T( #var ), var)


void CPerigeeCopyOptions::Load()
{
	HKEY key = NULL;
	DWORD disposition = 0;
	RegCreateKeyEx(HKEY_CURRENT_USER, app_key, 0, NULL, 0, KEY_READ, NULL, &key, &disposition);

	GET_VALUE(key, (overwrite_spec), overwrite, overwrite_prompt);

	GET_VALUE(key, !!, skip_error_files, false);
	
	GET_VALUE(key, !!, overwrite_read_only, true);
	GET_VALUE(key, !!, clear_read_only_after_copy, false);

	GET_VALUE(key, !!, only_copy_archive, false);
	GET_VALUE(key, !!, clear_archive_after_copy, false);

	GET_VALUE(key, !!, prompt_for_confirmation_when_deleting, true);
	GET_VALUE(key, !!, use_perigee_copy_by_default_when_dragging, false);
	GET_VALUE(key, !!, skip_inventory, false);

	GET_VALUE(key, !!, debug_prompt, false);

	GET_VALUE(key, !!, recycle, true);

	GET_VALUE(key, !!, wait_for_previous, false);

	GET_VALUE(key, !!, wait_for_debugger, false);

	RegCloseKey(key);
}

void CPerigeeCopyOptions::Save()
{
	HKEY key = NULL;
	DWORD disposition = 0;
	RegCreateKeyEx(HKEY_CURRENT_USER, app_key, 0, NULL, 0, KEY_WRITE, NULL, &key, &disposition);

	SET_VALUE(key, overwrite);

	SET_VALUE(key, skip_error_files);

	SET_VALUE(key, overwrite_read_only);
	SET_VALUE(key, clear_read_only_after_copy);

	SET_VALUE(key, only_copy_archive);
	SET_VALUE(key, clear_archive_after_copy);

	SET_VALUE(key, prompt_for_confirmation_when_deleting);
	SET_VALUE(key, use_perigee_copy_by_default_when_dragging);
	SET_VALUE(key, skip_inventory);

	SET_VALUE(key, debug_prompt);

	SET_VALUE(key, recycle);

	SET_VALUE(key, wait_for_previous);

	RegCloseKey(key);
}


bool CPerigeeCopyOptions::GetApplyToAll()
{
	HKEY key = NULL;
	DWORD disposition = 0;
	bool apply_to_all;
	RegCreateKeyEx(HKEY_CURRENT_USER, app_key, 0, NULL, 0, KEY_READ, NULL, &key, &disposition);
	GET_VALUE(key, !!, apply_to_all, false);
	RegCloseKey(key);
	return apply_to_all;
}

void CPerigeeCopyOptions::SetApplyToAll(bool apply_to_all)
{
	HKEY key = NULL;
	DWORD disposition = 0;
	RegCreateKeyEx(HKEY_CURRENT_USER, app_key, 0, NULL, 0, KEY_WRITE, NULL, &key, &disposition);
	SET_VALUE(key, apply_to_all);
	RegCloseKey(key);
}
