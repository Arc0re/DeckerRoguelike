//////////////////////////////////////////////////////////////////////
//Decker - A Cyberspace RPG
//Copyright (C) 2001  Shawn Overcash
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLOCKVIEW_H__DDF96181_A2DC_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_CLOCKVIEW_H__DDF96181_A2DC_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClockView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClockView window

class CClockView : public CWnd
{
// Construction
public:
	CClockView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClockView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClockView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClockView)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CImageList m_ilTime;

public:
	int m_nTime;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLOCKVIEW_H__DDF96181_A2DC_11D4_B835_0080C8C1DB04__INCLUDED_)
