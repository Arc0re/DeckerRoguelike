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

// HomeView.h: interface for the CHomeView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOMEVIEW_H__8D3C1210_7FFF_11D4_82A8_00104B751D91__INCLUDED_)
#define AFX_HOMEVIEW_H__8D3C1210_7FFF_11D4_82A8_00104B751D91__INCLUDED_

// Number of buttons
#define HOME_BUTTON_COUNT 8

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CustomButton.h"

class CHomeView : public CWnd  
{
public:
	CHomeView();
	virtual ~CHomeView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHomeView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CHomeView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewChar();
	afx_msg void OnContract();
	afx_msg void OnBuy();
	afx_msg void OnProgram();
	afx_msg void OnRest();
	afx_msg void OnDeck();
	afx_msg void OnMatrix();
	afx_msg void OnOptions();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnNewGame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CCustomButton m_bmbButton[HOME_BUTTON_COUNT];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOMEVIEW_H__8D3C1210_7FFF_11D4_82A8_00104B751D91__INCLUDED_)
