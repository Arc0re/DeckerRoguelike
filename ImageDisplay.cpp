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

// ImageDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "ImageDisplay.h"
#include "ImageArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageDisplay

CImageDisplay::CImageDisplay()
{
	m_nImage = -1;
}

CImageDisplay::~CImageDisplay()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CImageDisplay, CWnd)
	//{{AFX_MSG_MAP(CImageDisplay)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImageDisplay message handlers

void CImageDisplay::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	RECT rect;

	GetClientRect(&rect);

	dc.FillSolidRect(&rect, m_crBackground);

	if (m_nImage != -1)
	{
		m_pImageArray->DrawImage(&dc, 0, 0, m_nImage);
	}
}
