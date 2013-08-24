// SizeHelper.h: interface for the SizeHelper class.
// (C) 2004-2010 Jeremy Stanley
// This program is licensed under the terms of the GNU GPL; see license.txt for details.

#if !defined(AFX_SIZEHELPER_H__D84CC51A_8360_490A_B8D1_364817CBE682__INCLUDED_)
#define AFX_SIZEHELPER_H__D84CC51A_8360_490A_B8D1_364817CBE682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <vector>

class SizeHelper  
{
public:
	SizeHelper();
	virtual ~SizeHelper();

	// default minimum size is original size
	void SetDialog(HWND hDlg, int min_width = -1, int min_height = -1);

	// width = original width + (change in dialog width * size_h)
	// x = x0 + (change in dialog width * anc_r)
	// and so forth...
	void AddWidget(HWND hWidget, double size_h = 0.0, double size_v = 0.0, double anc_r = 0.0, double anc_b = 0.0);
	void AddWidget(HWND hDlg, UINT id, double size_h = 0.0, double size_v = 0.0, double anc_r = 0.0, double anc_b = 0.0);

	// call in response to dialog events
	void OnSize();
	void OnGetMinMaxInfo(LPMINMAXINFO lpmmi);


private:
	struct WidgetInfo {
		HWND hwnd;
		RECT orig_pos;
		double size_h;
		double size_v;
		double anchor_right;
		double anchor_bottom;
	};

	HWND m_hDlg;
	RECT m_orig_size;

	std::vector<WidgetInfo> m_widgets;
	int m_min_width;
	int m_min_height;

	void FixWidget(WidgetInfo &wi);


};

#endif // !defined(AFX_SIZEHELPER_H__D84CC51A_8360_490A_B8D1_364817CBE682__INCLUDED_)
