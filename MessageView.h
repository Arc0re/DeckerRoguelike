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

#if !defined(AFX_MESSAGEVIEW_H__FF21F1E1_8821_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_MESSAGEVIEW_H__FF21F1E1_8821_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageView view

class CMessageView : public CScrollView
{
public:
	CMessageView();           // protected constructor used by dynamic creation
	virtual ~CMessageView();
	DECLARE_DYNCREATE(CMessageView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void RedoSize();
	void ScrollToBottom();

	CObList m_olMessageList;
	int m_dx;
	int m_dy;
	CSize m_sizeTotal;
	CMenu m_popup_menu;

public:
	void OnClear();
	void AddMessage(const char *szStr, COLORREF crColor);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEVIEW_H__FF21F1E1_8821_11D4_B835_0080C8C1DB04__INCLUDED_)
