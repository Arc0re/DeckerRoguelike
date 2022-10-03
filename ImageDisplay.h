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

#if !defined(AFX_IMAGEDISPLAY_H__6D8E9887_A21D_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_IMAGEDISPLAY_H__6D8E9887_A21D_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageDisplay.h : header file
//

class CImageArray;

/////////////////////////////////////////////////////////////////////////////
// CImageDisplay window

class CImageDisplay : public CWnd
{
// Construction
public:
	CImageDisplay();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageDisplay)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageDisplay();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageDisplay)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CImageArray *m_pImageArray;
	int m_nImage;
	COLORREF m_crBackground;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEDISPLAY_H__6D8E9887_A21D_11D4_B835_0080C8C1DB04__INCLUDED_)
