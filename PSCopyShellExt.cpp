// PSCopyShellExt.cpp : Implementation of CPSCopyShellExt
// PerigeeCopy (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "Shellext.h"
#include "PSCopyShellExt.h"
#include "pscopy/PerigeeJob.h"
#include "popen32.h"
#include <strsafe.h>

/////////////////////////////////////////////////////////////////////////////
// CPSCopyShellExt

HRESULT CPSCopyShellExt::Initialize(
    LPCITEMIDLIST pidlFolder,
    LPDATAOBJECT  pDataObj,
    HKEY          hProgID
	)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());    // init MFC
	InitCommonControls();

	m_mode = mode_none;
	m_cliptype = clip_none;
	m_dropeffect = ~0;

	// Here are our possible actions.
	//  (1) Files have been right-dragged into a folder.
	//      - The list of files is in pDataObj.
	//      - pidlFolder refers to the destination folder.
	//  (2) User right-clicked in the background of a folder
	//      to paste files there.
	//      - The source files are on the clipboard.
	//      - pDataObj is NULL.
	//      - The destination folder is in pidlFolder.
	//  (3) User right-clicked on a folder to paste files there.
	//      - The source files are on the clipboard.
	//      - The destination folder is in pDataObj.
	//      - pidlFolder is NULL.
	//  (4) User right-clicked on a bunch of files/folders
	//      to delete them.
	//      - List of files is in pDataObj.
	//      - pidlFolder is NULL.
	//  (5) User hit Ctrl+V in a folder, and Windows "helpfully"
	//      selected the DRAG menu item we set as default.
	//      This looks like (1), but a drop effect will be set.
	// note: Items (3) and (4) can be appended to the context menu 
	//       at the same time.

	// get dropped-in folder, if applicable
	m_FolderDroppedIn = GetDroppedInFolder(pidlFolder);

	// get list of dropped/right-clicked files, if applicable
	if ( pDataObj )
	{
		if ( !GetDroppedOrClickedFiles(pDataObj) )
			return E_FAIL;
	}

	// tell whether this is a drag-drop operation
	if ( !m_FolderDroppedIn.IsEmpty() && !m_lsDroppedFiles.IsEmpty() )
	{
		m_mode = mode_dragdrop;
		return S_OK;
	}
	else
	{
		m_mode = mode_contextmenu;
		GetClipboardFiles();
		return S_OK;
	}

	return E_FAIL;
}	

// Retrieves the name of the folder referred to by pidlFolder, 
// following a NetHood shortcut if necessary. The Windows shell 
// is terminally obfuscated.  It's at least an order of magnitude 
// harder to do anything than it should be.
CString CPSCopyShellExt::GetDroppedInFolder(LPCITEMIDLIST pidlFolder)
{
	if ( !pidlFolder )
		return CString();

	TCHAR buf[MAX_PATH];
	bool dereferenced_nethood_shortcut = false;

	// get the desktop folder PIDL
	IShellFolder *pDesktopFolder;
	if ( NOERROR == SHGetDesktopFolder(&pDesktopFolder) )
	{
		// get the parent folder PIDL
		LPITEMIDLIST ppidl = ILClone(pidlFolder);
		ILRemoveLastID(ppidl);

		// get an IShellFolder for the parent
		IShellFolder *pParentFolder;
		if ( S_OK == pDesktopFolder->BindToObject(ppidl, NULL, IID_IShellFolder, (LPVOID *)&pParentFolder) )
		{
			// get the relative ID for the drop target
			LPCITEMIDLIST rel_id = ILFindLastID(pidlFolder);
			ULONG attrs = SFGAO_FOLDER | SFGAO_LINK;
			if ( S_OK == pParentFolder->GetAttributesOf(1, &rel_id, &attrs) )
			{
				// test whether this is a folder shortcut
				if ( (0 != (attrs & SFGAO_FOLDER)) && (0 != (attrs & SFGAO_LINK)) )
				{
					IShellLink *pLink;
					if ( S_OK == pParentFolder->GetUIObjectOf(NULL, 1, &rel_id, IID_IShellLink,
						NULL, (LPVOID *)&pLink) )
					{
						if ( NOERROR == pLink->GetPath(buf, MAX_PATH, NULL, SLGP_UNCPRIORITY) )
						{
							dereferenced_nethood_shortcut = true;
						}
						pLink->Release();
					}
				}
			}

			// free the parent folder
			pParentFolder->Release();
		}

		// free the parent folder PIDL
		ILFree(ppidl);
		
		// free the desktop folder
		pDesktopFolder->Release();
	}

	if ( !dereferenced_nethood_shortcut )
	{
		// Use the folder itself as the destination
		if ( !SHGetPathFromIDList(pidlFolder, buf) )
			return CString();
	}
	
	return buf;
}

bool CPSCopyShellExt::GetDropEffect(COleDataObject &dataobj)
{
	// get the nature of the operation (cut or copy)
	HGLOBAL hglobal = dataobj.GetGlobalData( m_preferred_drop_effect );
	if ( NULL == hglobal )
		return false;
	
	DWORD *dw = (DWORD *)GlobalLock( hglobal );
	if ( NULL == dw )
		return false;

	m_dropeffect = *dw;
	if ( *dw & 0x7fffff00 )
	{
		// WTF is it?
		ASSERT(0);
	}
	else
	{
		if ((*dw) & DROPEFFECT_MOVE)
			m_cliptype = clip_cut;
		else if ((*dw) & DROPEFFECT_COPY)
			m_cliptype = clip_copy;
	}
	
	GlobalUnlock( hglobal );
	return m_cliptype != clip_none;
}

bool CPSCopyShellExt::GetDroppedOrClickedFiles(LPDATAOBJECT pDataObj)
{
	COleDataObject dataobj;
	dataobj.Attach( pDataObj, FALSE );
	// Ctrl+V paste operations, with PSCopy set as default, will look like move operations
	// except for the drop effect
	GetDropEffect(dataobj);	
	return GetFiles(m_lsDroppedFiles, dataobj);
}

bool CPSCopyShellExt::GetClipboardFiles()
{
	COleDataObject dataobj;
	dataobj.AttachClipboard();
	if ( !GetFiles(m_lsClipboardFiles, dataobj) )
		return false;
	return GetDropEffect(dataobj);
}

bool CPSCopyShellExt::GetFiles(CStringList &list, COleDataObject &obj)
{
	HGLOBAL        hglobal;
	HDROP          hdrop;
	TCHAR          szFile[MAX_PATH];
	UINT           uFile, uNumFiles;

	hglobal = obj.GetGlobalData( CF_HDROP );
    if ( NULL == hglobal )
        return false;

    hdrop = (HDROP) GlobalLock( hglobal );
    if ( NULL == hdrop )
        return false;

	uNumFiles = DragQueryFile( hdrop, 0xFFFFFFFF, NULL, 0 );

    for ( uFile = 0; uFile < uNumFiles; uFile++ )
	{
		if ( DragQueryFile( hdrop, uFile, szFile, MAX_PATH ))
		{
			list.AddTail( szFile );
		}
	}

	GlobalUnlock( hglobal );

	return !list.IsEmpty();
}

HRESULT CPSCopyShellExt::QueryContextMenu(
    HMENU hmenu,
    UINT  uMenuIndex,
    UINT  uidFirstCmd,
    UINT  uidLastCmd,
    UINT  uFlags
	)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());    // init MFC

    // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
    if ( uFlags & CMF_DEFAULTONLY )
	{
        return MAKE_HRESULT( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
	}

#ifdef _DEBUG
{
	CString line;
	line.Format(_T("QueryContextMenu %08x\n"), uFlags);
	OutputDebugString(line);
	int defIdx = GetMenuDefaultItem(hmenu, TRUE, 0);
	int count = GetMenuItemCount(hmenu);
	for(int i = 0; i < count; ++i)
	{
		TCHAR str[MAX_PATH];
		GetMenuString(hmenu, i, str, MAX_PATH, MF_BYPOSITION);
		line.Format(_T("%c%c%d %s\n"), 
			(defIdx == i) ? _T('*') : _T(' '),
			(uMenuIndex == i) ? _T('>') : _T(' '), 
			GetMenuItemID(hmenu, i),
			str);
		if (str[0])
			OutputDebugString(line);
	}
	OutputDebugString(_T("\n"));
}
#endif

    // Add the menu item.
	UINT uCmd = uidFirstCmd;
	CString mitem;

	UINT uIdx = uMenuIndex;
	if ( uIdx == (DWORD)-1 ) uIdx = 0; // Explorer thinks it's funny, but I'd prefer to keep my menu items together

	if ( m_mode == mode_dragdrop )
	{
		// add copy and move items
		AddMenuItem(hmenu, op_copy, uIdx++, uCmd++);
		AddMenuItem(hmenu, op_move, uIdx++, uCmd++);

		// set Perigee item to default if applicable
		if ( 0 == (uFlags & CMF_NODEFAULT) &&
			m_options.use_perigee_copy_by_default_when_dragging )
		{
			int newDefault = -1;
			int defItem = GetMenuDefaultItem(hmenu, FALSE, 0);
			switch(defItem)
			{
			case 1: /* copy */
				newDefault = uIdx - 2;
				break;
			case 2:	/* move */
				newDefault = uIdx - 1;
				break;
			}
			if (newDefault >= 0)
				SetMenuDefaultItem(hmenu, newDefault, MF_BYPOSITION);
		}
	}
	else
	{
		// add paste, delete, and/or configure, whichever applies
		if ( !m_lsClipboardFiles.IsEmpty() && (m_cliptype != clip_none) )
		{
			bool ok_to_paste = false;
			
			if ( m_FolderDroppedIn.IsEmpty() )
			{
				// paste by right clicking on a folder
				// (don't allow right-clicking on a file, or multiple anythings)
				if ( m_lsDroppedFiles.GetCount() == 1 )
				{
					m_paste_dest = m_lsDroppedFiles.GetHead();
					DWORD attrs = GetFileAttributes(m_paste_dest);
					ok_to_paste = ( (attrs != INVALID_FILE_ATTRIBUTES) && (0 != (attrs & FILE_ATTRIBUTE_DIRECTORY)) );
				}
			}
			else
			{
				// paste by right clicking in folder window background 
				m_paste_dest = m_FolderDroppedIn;
				ok_to_paste = true;
			}

			if ( ok_to_paste )
			{
				AddMenuItem(hmenu, op_paste, uIdx++, uCmd++);
			}
		}

		if ( !m_lsDroppedFiles.IsEmpty() )
		{
			AddMenuItem(hmenu, op_delete, uIdx++, uCmd++);
		}
	}

	InsertMenu(hmenu, uIdx++, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);

	ASSERT( uCmd <= uidLastCmd );

    // Return the number of menu items added
    return MAKE_HRESULT( SEVERITY_SUCCESS, FACILITY_NULL, uCmd - uidFirstCmd );
}

void CPSCopyShellExt::AddMenuItem(HMENU hmenu, operation op, UINT idx, UINT cmd)
{
	UINT res_idx;
	
	switch(op)
	{
	case op_copy:
		res_idx = IDS_CONTEXT_MENU_ITEM_COPY;
		break;
	case op_move:
		res_idx = IDS_CONTEXT_MENU_ITEM_MOVE;
		break;
	case op_delete:
		res_idx = IDS_CONTEXT_MENU_ITEM_DELETE;
		break;
	case op_paste:
		res_idx = IDS_CONTEXT_MENU_ITEM_PASTE;
		break;
	case op_configure:
		res_idx = IDS_CONTEXT_MENU_ITEM_OPTIONS;
		break;
	default:
		ASSERT(0);
		return;
	}

	CString mitem;
	mitem.LoadString(res_idx);
	if ( mitem.IsEmpty() )
	{
		ASSERT(0);
		return;
	}
	
	InsertMenu( hmenu, idx, MF_STRING | MF_BYPOSITION, cmd,	mitem );
	m_menu_items.push_back(op);
}

HRESULT CPSCopyShellExt::GetCommandString ( 
    UINT_PTR  idCmd,
    UINT  uFlags,
    UINT* pwReserved,
    LPSTR pszName,
    UINT  cchMax )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());    // init MFC

	USES_CONVERSION;

	if ( idCmd >= m_menu_items.size() )
	{
		return E_INVALIDARG;
	}

	UINT res_idx;

	switch( m_menu_items[idCmd] )
	{
	case op_paste:
		res_idx = IDS_TOOLTIP_PASTE;
		break;
	case op_delete:
		res_idx = IDS_TOOLTIP_DELETE;
		break;
	case op_configure:
		res_idx = IDS_TOOLTIP_OPTIONS;
		break;
	default:
		ASSERT(0);
		return E_NOTIMPL;
	}

	CString tooltip;
	tooltip.LoadString(res_idx);
	if ( tooltip.IsEmpty() )
	{
		ASSERT(0);
		return E_FAIL;
	}

	if ( uFlags & GCS_HELPTEXT )
	{
		if ( uFlags & GCS_UNICODE )
		{
			StringCchCopyNW( (LPWSTR)pszName, cchMax, T2CW(tooltip), cchMax );
		}
		else
		{
			StringCchCopyNA( (LPSTR)pszName, cchMax, T2CA(tooltip), cchMax );
		}

		return S_OK;
	}

	return E_INVALIDARG;
}

HRESULT CPSCopyShellExt::InvokeCommand( LPCMINVOKECOMMANDINFO pInfo )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());    // init MFC

    // Double-check that we're getting called for our own menu item 
	// HIWORD(lpVerb) must be 0
    if ( 0 != HIWORD(pInfo->lpVerb) )
	{
        return E_INVALIDARG;
	}

	UINT cmd = LOWORD(pInfo->lpVerb);
	if ( cmd >= m_menu_items.size() )
	{
		return E_INVALIDARG;
	}

	operation op = m_menu_items[cmd];

	if ( op == op_configure )
	{
		CStringList dummy;
		return LaunchPerigeeCopy( PerigeeJob::op_configure, dummy, _T("") );
	}

	CStringList *source_files = NULL;
	CString dest_folder;
	PerigeeJob::operation pc_op = PerigeeJob::op_none;

	switch( op )
	{
	case op_copy:
	case op_move:
		source_files = &m_lsDroppedFiles;
		dest_folder = m_FolderDroppedIn;
		pc_op = (op == op_copy) ? PerigeeJob::op_copy : PerigeeJob::op_move;
		break;

	case op_paste:
		source_files = &m_lsClipboardFiles;
		dest_folder = m_paste_dest;
		pc_op = (m_cliptype == clip_copy) ? PerigeeJob::op_copy : PerigeeJob::op_move;
		break;

	case op_delete:
		source_files = &m_lsDroppedFiles;
		pc_op = PerigeeJob::op_delete;
		break;

	default:
		ASSERT(0);
		return E_FAIL;
	}
	
	if ( !source_files )
	{
		ASSERT(0);
		return E_FAIL;
	}

	if ( pc_op == PerigeeJob::op_none )
	{
		ASSERT(0);
		return E_FAIL;
	}
		
	return LaunchPerigeeCopy( pc_op, *source_files, dest_folder );
}

void WriteLine(popen32 &pop, const CString &ln)
{
	pop.Write( (LPCTSTR)ln, ln.GetLength() * sizeof(TCHAR) );
	pop.Write( _T("\n"), sizeof(TCHAR) );
}

HRESULT CPSCopyShellExt::LaunchPerigeeCopy(PerigeeJob::operation op, CStringList &source_files, CString dest_folder)
{
	// Figure out the name of the executable; it's the same path and name as the DLL,
	// but with a .EXE extension.
	TCHAR szModName[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(), szModName, MAX_PATH);
	CString csModName( szModName );
	CString csExt = csModName.Right(4);
	if ( 0 == csExt.CompareNoCase(_T(".dll")) )
	{
		csModName = csModName.Left( csModName.GetLength() - 4 ) + _T(".exe");
	}
	else return E_FAIL;

	// Create the process
	popen32 pop;
	if ( !pop.open(csModName, CString()) )
	{
		TCHAR szError[MAX_PATH];
		pop.GetError(szError, MAX_PATH);
		AfxMessageBox(szError);
		return E_FAIL;
	}

	// Send the command
	CString csCommand;
	switch ( op )
	{
	case PerigeeJob::op_copy:
		csCommand = _T("copy");
		break;
	case PerigeeJob::op_move:
		csCommand = _T("move");
		break;
	case PerigeeJob::op_delete:
		csCommand = _T("delete");
		break;
	case PerigeeJob::op_configure:
		csCommand = _T("config");
		break;
	default:
		return E_FAIL;
	}
	WriteLine( pop, csCommand );

	// Send the destination folder
	WriteLine( pop, dest_folder );
	
	// Pipe the filename list to the new process
	for( POSITION pos = source_files.GetHeadPosition(); pos; )
	{
		WriteLine( pop, source_files.GetNext(pos) );
	}
	
	// Send the terminator
	WriteLine( pop, CString() );
	
	return S_OK;
}
