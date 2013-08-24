// PerigeeCopyOptions.h: interface for the CPerigeeCopyOptions class.
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_PERIGEECOPYOPTIONS_H__FF74C5F7_C274_4D7B_8809_D42733925ABA__INCLUDED_)
#define AFX_PERIGEECOPYOPTIONS_H__FF74C5F7_C274_4D7B_8809_D42733925ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPerigeeCopyOptions  
{
public:
	CPerigeeCopyOptions();
	virtual ~CPerigeeCopyOptions();

	void Load();
	void Save();

	static bool GetApplyToAll();
	static void SetApplyToAll(bool apply);

	enum overwrite_spec {
		overwrite_always,
		overwrite_never,
		overwrite_if_newer,
		overwrite_prompt,
		make_unique_name,
		overwrite_postpone
	};
	
	overwrite_spec overwrite;

	bool skip_error_files;

	bool overwrite_read_only;
	bool clear_read_only_after_copy;

	bool only_copy_archive;
	bool clear_archive_after_copy;

	bool prompt_for_confirmation_when_deleting;
	bool use_perigee_copy_by_default_when_dragging;
	bool skip_inventory;

	bool debug_prompt;

	bool recycle;

	bool wait_for_previous;

	bool wait_for_debugger;
};

#endif // !defined(AFX_PERIGEECOPYOPTIONS_H__FF74C5F7_C274_4D7B_8809_D42733925ABA__INCLUDED_)
