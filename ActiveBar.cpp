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

// ActiveBar.cpp : implementation file
//

#include "stdafx.h"
#include "ActiveBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ACTIVE_IMAGE_SIZE 16

/////////////////////////////////////////////////////////////////////////////
// CActiveBar

CActiveBar::CActiveBar()
{
	for (int i=0; i<MAX_ACTIVE; i++)
	{
		m_bActive[i] = FALSE;
	}
}

CActiveBar::~CActiveBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CActiveBar, CWnd)
	//{{AFX_MSG_MAP(CActiveBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CActiveBar message handlers

BOOL CActiveBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.cx = ACTIVE_IMAGE_SIZE * MAX_ACTIVE;
	cs.cy = ACTIVE_IMAGE_SIZE;
	cs.style &= (~WS_BORDER);
	
	return CWnd::PreCreateWindow(cs);
}

void CActiveBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Blank out the background
	dc.FillSolidRect(0,0,ACTIVE_IMAGE_SIZE * MAX_ACTIVE, ACTIVE_IMAGE_SIZE, (COLORREF)0x00FFFFFF);

	CPoint ptOrigin(0,0);
	CPoint ptPoint(0,0);
	CSize szSize(ACTIVE_IMAGE_SIZE,ACTIVE_IMAGE_SIZE);

	for (int i=0; i<MAX_ACTIVE; i++)
	{
		if (m_bActive[i])
		{
			m_pImageList->DrawIndirect(&dc,m_nImageIndex[i],ptPoint,szSize,ptOrigin);
		}
		ptPoint.x += ACTIVE_IMAGE_SIZE;
	}
}
