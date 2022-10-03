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

// NameBar.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "NameBar.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNameBar

CNameBar::CNameBar()
{
}

CNameBar::~CNameBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CNameBar, CWnd)
	//{{AFX_MSG_MAP(CNameBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNameBar message handlers

void CNameBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	RECT rect;

	GetClientRect(&rect);

	//dc.FillSolidRect(&rect, WHITE);
	//dc.SetBkColor(GREEN);
	//dc.TextOut(0, 0, m_szText);

	dc.ExtTextOut(1, 0, /*ETO_CLIPPED|*/ETO_OPAQUE, &rect, m_szText, NULL);
}
