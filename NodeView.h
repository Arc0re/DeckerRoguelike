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

// NodeView.h: interface for the CNodeView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEVIEW_H__E2655283_857E_11D4_82AD_00104B751D91__INCLUDED_)
#define AFX_NODEVIEW_H__E2655283_857E_11D4_82AD_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "ImageArray.h"

class CIce;

class CNodeView : public CWnd  
{
public:
	CNodeView();
	virtual ~CNodeView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeView)
	protected:
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CNodeView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HBITMAP m_hNodeBitMap[NUM_NODES];
	HBITMAP m_hLineBitMap[4];
	CPoint m_ptLineLocation[4];
	CSize m_szLineSize[4];

public:
	//CImageList m_ilICE;
	CImageArray m_iaIce;
	CImageList m_ilChar;
	CImageList m_ilMisc;

	CDC m_dcBackground;

	void DoCharMove();
	void EraseIce(CIce *pIce);
	void DrawIce(CIce *pIce);
	void DrawDecoy(CPoint &ptLoc);
	void EraseGrid(CPoint &ptLoc);
	void DrawDamage(CIce *pIce);
	void DrawQuery(CIce *pIce);
	void DrawAlert(CIce *pIce);
	void DrawBypass(CIce *pIce);

private:
	void DrawMisc(CIce *pIce, int nImage);
};

#endif // !defined(AFX_NODEVIEW_H__E2655283_857E_11D4_82AD_00104B751D91__INCLUDED_)
