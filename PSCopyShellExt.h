// PSCopyShellExt.h : Declaration of the CPSCopyShellExt
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#ifndef __PSCOPYSHELLEXT_H_
#define __PSCOPYSHELLEXT_H_

#include <comdef.h>
#include <shlobj.h>
#include <afxole.h>
#include <vector>
#include "resource.h"       // main symbols
#include "pscopy/PerigeeCopyOptions.h"
#include "pscopy/PerigeeJob.h"

/////////////////////////////////////////////////////////////////////////////
// CPSCopyShellExt
class ATL_NO_VTABLE CPSCopyShellExt : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPSCopyShellExt, &CLSID_PSCopyShellExt>,
	public IDispatchImpl<IPSCopyShellExt, &IID_IPSCopyShellExt, &LIBID_SHELLEXTLib>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CPSCopyShellExt()
	{
		m_preferred_drop_effect = RegisterClipboardFormat( CFSTR_PREFERREDDROPEFFECT );
		m_paste_succeeded = RegisterClipboardFormat( CFSTR_PASTESUCCEEDED );
	}

DECLARE_REGISTRY_RESOURCEID(IDR_PSCOPYSHELLEXT)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CPSCopyShellExt)
	COM_INTERFACE_ENTRY(IPSCopyShellExt)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

// IPSCopyShellExt

public:
    // IShellExtInit
    STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

    // IContextMenu
    STDMETHOD(GetCommandString)(UINT_PTR, UINT, UINT*, LPSTR, UINT);
    STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
    STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);

protected:
	CPerigeeCopyOptions m_options;

    CString     m_FolderDroppedIn;
	CString     m_paste_dest;
    CStringList m_lsClipboardFiles;
	CStringList m_lsDroppedFiles;

	CString GetDroppedInFolder(LPCITEMIDLIST pidlFolder);
	bool GetDropEffect(COleDataObject &dataobj);
	bool GetDroppedOrClickedFiles(LPDATAOBJECT pDataObj);
	bool GetClipboardFiles();
	bool GetFiles(CStringList &list, COleDataObject &obj);

	enum mode { mode_none, mode_dragdrop, mode_contextmenu };
	mode m_mode;

	enum cliptype { clip_none, clip_cut, clip_copy };
	cliptype m_cliptype;
	DWORD m_dropeffect;

	enum operation { op_none, op_copy, op_move, op_paste, op_delete, op_configure };
	std::vector<operation> m_menu_items;

	void AddMenuItem(HMENU hmenu, operation op, UINT idx, UINT cmd);

	HRESULT LaunchPerigeeCopy(PerigeeJob::operation op, CStringList &source_files, CString dest_folder);

	UINT m_preferred_drop_effect, m_paste_succeeded;
};

#endif //__PSCOPYSHELLEXT_H_
