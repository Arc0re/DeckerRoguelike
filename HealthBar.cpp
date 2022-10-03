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

// HealthBar.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "HealthBar.h"
#include "MatrixView.h"

#define BLUE (COLORREF)0x00FF0000

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHealthBar

CHealthBar::CHealthBar()
{
}

CHealthBar::~CHealthBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CHealthBar, CWnd)
	//{{AFX_MSG_MAP(CHealthBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHealthBar message handlers

BOOL CHealthBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.cx = BAR_CX;
	cs.cy = BAR_CY;
	cs.style &= (~WS_BORDER);
	return CWnd::PreCreateWindow(cs);
}

void CHealthBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	POINT pt;
	CMatrixView *pView = (CMatrixView *)GetParent();
	
	int nLength;
	int nGreen, nRed;
	int nColor;

//	// Handle empty bar specially
//	if (m_nCurrentVal <= 0)
//	{
//		dc.FillSolidRect(0,0,BAR_CX,BAR_CY,BLUE);
//		return;
//	}

	// Calculate proportional length
	nLength = (m_nCurrentVal * BAR_CX)/m_nMaxVal;

	// Color is also according to current and max
	// Scale - 255 Green at highest, add red to middle, reduce green to end
	nGreen = ((511 * nLength)/BAR_CX) - 64;
	nRed = 765 - ((765 * nLength)/BAR_CX);
	if (nGreen>255) nGreen = 255;
	else if (nGreen<0) nGreen = 0;
	if (nRed>255) nRed = 255;
	else if (nRed<0) nRed = 0;
	nColor = ((nGreen<<8)|nRed);

	// Alternate method
	//if (nLength <= ((BAR_CX*2)/5))
	//	nColor = RED;
	//else if (nLength <= ((4*BAR_CX)/5))
	//	nColor = YELLOW;
	//else
	//	nColor = GREEN;

	// Fill the bar with the color
	dc.FillSolidRect(0,0,nLength,BAR_CY,(COLORREF)nColor);

//	// Fill the rest with blue
//	if (m_nCurrentVal != m_nMaxVal)
//	{
//		dc.FillSolidRect(nLength,0,BAR_CX-nLength,BAR_CY,BLUE);
//	}
	// Fill rest with stuff underneath
	pt.x = nLength;
	pt.y = 0;
	MapWindowPoints(pView, &pt, 1);

	dc.BitBlt(nLength,0,BAR_CX-nLength,BAR_CY,
				&pView->m_dcBackground,pt.x,pt.y,SRCCOPY);

}

