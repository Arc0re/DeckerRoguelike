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

// MapView.h: interface for the CMapView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPVIEW_H__C46DB88A_85CA_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_MAPVIEW_H__C46DB88A_85CA_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapView : public CScrollView  
{
public:
	CMapView();
	virtual ~CMapView();
	DECLARE_DYNCREATE(CMapView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL m_bUseBig;
	int m_nGridSize;
	CSize m_szBorderOffset;
	CImageList m_ilNodesLarge;
	CImageList m_ilNodesSmall;
	CImageList m_ilSpecialNodesLarge;
	CImageList m_ilSpecialNodesSmall;
	CSize m_sizeTotal;

public:
	void RedoSize();
	void ScrollToCurrentNode();
	void DoMove();
	void DoZoom();
};

#endif // !defined(AFX_MAPVIEW_H__C46DB88A_85CA_11D4_B835_0080C8C1DB04__INCLUDED_)
