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

// CustomButton.cpp: implementation of the CCustomButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "CustomButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CCustomButton, CButton)


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomButton::CCustomButton()
{
	m_hbitmap = NULL;
	m_hbitmapSel = NULL;
	m_hbitmapFocus = NULL;
	m_hbitmapDisabled = NULL;
	m_bDeleteBitmaps = FALSE;
}

CCustomButton::~CCustomButton()
{
	if (m_bDeleteBitmaps)
	{
		if (m_hbitmap != NULL)
		{
			DeleteObject(m_hbitmap);
			m_hbitmap = NULL;
		}
		if (m_hbitmapSel != NULL)
		{
			DeleteObject(m_hbitmapSel);
			m_hbitmapSel = NULL;
		}
		if (m_hbitmapFocus != NULL)
		{
			DeleteObject(m_hbitmapFocus);
			m_hbitmapFocus = NULL;
		}
		if (m_hbitmapDisabled != NULL)
		{
			DeleteObject(m_hbitmapDisabled);
			m_hbitmapDisabled = NULL;
		}
	}
}

BOOL CCustomButton::LoadBitmapsFromFiles(
			LPCTSTR lpszBitmapFile,
			LPCTSTR lpszBitmapFileSel, 
			LPCTSTR lpszBitmapFileFocus,
			LPCTSTR lpszBitmapFileDisabled)
{
	// delete old bitmaps (if present)
	if (m_bDeleteBitmaps)
	{
		if (m_hbitmap != NULL)
		{
			DeleteObject(m_hbitmap);
			m_hbitmap = NULL;
		}
		if (m_hbitmapSel != NULL)
		{
			DeleteObject(m_hbitmapSel);
			m_hbitmapSel = NULL;
		}
		if (m_hbitmapFocus != NULL)
		{
			DeleteObject(m_hbitmapFocus);
			m_hbitmapFocus = NULL;
		}
		if (m_hbitmapDisabled != NULL)
		{
			DeleteObject(m_hbitmapDisabled);
			m_hbitmapDisabled = NULL;
		}

		m_bDeleteBitmaps = FALSE;
	}

	// Load the bitmaps
	m_hbitmap = (HBITMAP)LoadImage(NULL, lpszBitmapFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (m_hbitmap == NULL)
	{
		TRACE0("Failed to load bitmap for normal image.\n");
		return FALSE;   // need this one image
	}

	m_bDeleteBitmaps = TRUE;

	BOOL bAllLoaded = TRUE;

	if (lpszBitmapFileSel != NULL)
	{
		m_hbitmapSel = (HBITMAP)LoadImage(NULL, lpszBitmapFileSel, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (m_hbitmapSel == NULL)
		{
			TRACE0("Failed to load bitmap for selected image.\n");
			bAllLoaded = FALSE;
		}
	}

	if (lpszBitmapFileFocus != NULL)
	{
		m_hbitmapFocus = (HBITMAP)LoadImage(NULL, lpszBitmapFileFocus, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (m_hbitmapFocus == NULL)
		{
			TRACE0("Failed to load bitmap for focused image.\n");
			bAllLoaded = FALSE;
		}
	}

	if (lpszBitmapFileDisabled != NULL)
	{
		m_hbitmapDisabled = (HBITMAP)LoadImage(NULL, lpszBitmapFileDisabled, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (m_hbitmapDisabled == NULL)
		{
			TRACE0("Failed to load bitmap for selected image.\n");
			bAllLoaded = FALSE;
		}
	}

	return bAllLoaded;
}

void CCustomButton::SetBitmaps(
			HBITMAP hBitmap,
			HBITMAP hBitmapSel, 
			HBITMAP hBitmapFocus,
			HBITMAP hBitmapDisabled)
{
	// delete old bitmaps (if present)
	if (m_bDeleteBitmaps)
	{
		if (m_hbitmap != NULL)
		{
			DeleteObject(m_hbitmap);
			m_hbitmap = NULL;
		}
		if (m_hbitmapSel != NULL)
		{
			DeleteObject(m_hbitmapSel);
			m_hbitmapSel = NULL;
		}
		if (m_hbitmapFocus != NULL)
		{
			DeleteObject(m_hbitmapFocus);
			m_hbitmapFocus = NULL;
		}
		if (m_hbitmapDisabled != NULL)
		{
			DeleteObject(m_hbitmapDisabled);
			m_hbitmapDisabled = NULL;
		}

		m_bDeleteBitmaps = FALSE;
	}

	m_hbitmap = hBitmap;
	m_hbitmapSel = hBitmapSel;
	m_hbitmapFocus = hBitmapFocus;
	m_hbitmapDisabled = hBitmapDisabled;
}


// Draw the appropriate bitmap
void CCustomButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS != NULL);
	// must have at least the first bitmap loaded before calling DrawItem
	ASSERT(m_hbitmap != NULL);     // required

	// use the main bitmap for up, the selected bitmap for down
	CBitmap* pBitmap = CBitmap::FromHandle(m_hbitmap);

	UINT state = lpDIS->itemState;
	if ((state & ODS_SELECTED) && m_hbitmapSel != NULL)
		pBitmap = CBitmap::FromHandle(m_hbitmapSel);
	else if ((state & ODS_FOCUS) && m_hbitmapFocus != NULL)
		pBitmap = CBitmap::FromHandle(m_hbitmapFocus);   // third image for focused
	else if ((state & ODS_DISABLED) && m_hbitmapDisabled != NULL)
		pBitmap = CBitmap::FromHandle(m_hbitmapDisabled);   // last image for disabled

	// draw the whole button
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOld = memDC.SelectObject(pBitmap);
	if (pOld == NULL)
		return;     // destructors will clean up

	CRect rect;
	rect.CopyRect(&lpDIS->rcItem);
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		&memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOld);
}

