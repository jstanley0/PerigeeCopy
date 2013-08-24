// SizeHelper.cpp: implementation of the SizeHelper class.
// (C) 2004 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#include "stdafx.h"
#include "SizeHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SizeHelper::SizeHelper()
{
	m_hDlg = NULL;
}

SizeHelper::~SizeHelper()
{
}

void SizeHelper::SetDialog(HWND hDlg, int min_width /*= -1*/, int min_height /*= -1*/)
{
	m_hDlg = hDlg;
	GetWindowRect(m_hDlg, &m_orig_size);
	m_min_width  = (min_width  < 0) ? m_orig_size.right  - m_orig_size.left : min_width;
	m_min_height = (min_height < 0) ? m_orig_size.bottom - m_orig_size.top  : min_height;

	GetClientRect(m_hDlg, &m_orig_size);
}

void SizeHelper::AddWidget(HWND hWidget, double size_h /*= 1.0*/, double size_v /*= 1.0*/, double anc_r /*= 0.0*/, double anc_b /*= 0.0*/)
{
	WidgetInfo wi;
	wi.hwnd = hWidget;
	wi.size_h = size_h;
	wi.size_v = size_v;
	wi.anchor_right = anc_r;
	wi.anchor_bottom = anc_b;

	POINT pt;
	GetWindowRect(hWidget, &wi.orig_pos);
	
	pt.x = wi.orig_pos.left;
	pt.y = wi.orig_pos.top;
	ScreenToClient(m_hDlg, &pt);
	wi.orig_pos.left = pt.x;
	wi.orig_pos.top  = pt.y;

	pt.x = wi.orig_pos.right;
	pt.y = wi.orig_pos.bottom;
	ScreenToClient(m_hDlg, &pt);
	wi.orig_pos.right  = pt.x;
	wi.orig_pos.bottom = pt.y;

	m_widgets.push_back(wi);
}

void SizeHelper::AddWidget(HWND hDlg, UINT id, double size_h /*= 1.0*/, double size_v /*= 1.0*/, double anc_r /*= 0.0*/, double anc_b /*= 0.0*/)
{
	AddWidget(GetDlgItem(hDlg, id), size_h, size_v, anc_r, anc_b);
}

void SizeHelper::OnSize()
{
	if ( !m_hDlg )
		return;

	for(std::vector<WidgetInfo>::size_type i = 0; i < m_widgets.size(); i++)
	{
		FixWidget(m_widgets[i]);
	}

	InvalidateRect(m_hDlg, NULL, FALSE);
}

void SizeHelper::OnGetMinMaxInfo(LPMINMAXINFO lpmmi)
{
	if ( !m_hDlg )
		return;

	lpmmi->ptMinTrackSize.x = m_min_width;
	lpmmi->ptMinTrackSize.y = m_min_height;
}

void SizeHelper::FixWidget(WidgetInfo &wi)
{
	RECT new_size;
	GetClientRect(m_hDlg, &new_size);
	int dW = new_size.right  - m_orig_size.right;
	int dH = new_size.bottom - m_orig_size.bottom;

	MoveWindow(wi.hwnd, 
		wi.orig_pos.left + (int)(wi.anchor_right  * dW), 
		wi.orig_pos.top  + (int)(wi.anchor_bottom * dH), 
		(wi.orig_pos.right - wi.orig_pos.left) + (int)(wi.size_h * dW), 
		(wi.orig_pos.bottom - wi.orig_pos.top) + (int)(wi.size_v * dH), 
		TRUE);
}