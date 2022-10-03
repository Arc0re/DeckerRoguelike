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

// Decker.h : main header file for the DECKER application
//

#if !defined(AFX_DECKER_H__8D3C1206_7FFF_11D4_82A8_00104B751D91__INCLUDED_)
#define AFX_DECKER_H__8D3C1206_7FFF_11D4_82A8_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Decker.hm"		// Help context ID's

// Version
#define MAJOR_VERSION	1
#define MINOR_VERSION	12

class CHomeView;

// New game message
#define WM_NEW_GAME		(WM_USER + 4)

/////////////////////////////////////////////////////////////////////////////
// CDeckerApp:
// See Decker.cpp for the implementation of this class
//

class CDeckerApp : public CWinApp
{
public:
	CDeckerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDeckerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CHomeView * m_pHomeView;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKER_H__8D3C1206_7FFF_11D4_82A8_00104B751D91__INCLUDED_)
