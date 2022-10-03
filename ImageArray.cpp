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

// ImageArray.cpp: implementation of the CImageArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageArray::CImageArray()
{
	m_bLoaded = FALSE;
}

CImageArray::~CImageArray()
{
	if (m_bLoaded)
	{
		DeleteObject(m_hImage);
		DeleteObject(m_hMask);
	}
}

BOOL CImageArray::LoadImages(CDC *pDC, LPCTSTR lpszName, int cx, int cy, COLORREF crBackground)
{
	SIZE size;

	HINSTANCE hInst = AfxGetInstanceHandle();

	// Unload old stuff
	if (m_bLoaded)
	{
		DeleteObject(m_hImage);
		DeleteObject(m_hMask);
		m_bLoaded = FALSE;
	}

	// Save the sizes
	m_nImageCX = cx;
	m_nImageCY = cy;

	// Load the image
	m_hImage = (HBITMAP)LoadImage(hInst, lpszName, IMAGE_BITMAP, 0, 0, 0);
	if (m_hImage == NULL)
	{
		return FALSE;
	}

	// Get the image size
	//if (!GetBitmapDimensionEx(m_hImage, &size))
	//{
	//	DeleteObject(m_hImage);
	//	return FALSE;
	//}
	BITMAPV4HEADER Header;
	memset(&Header,0,sizeof(BITMAPV4HEADER));
	Header.bV4Size = sizeof(BITMAPV4HEADER);

	GetDIBits(pDC->m_hDC, m_hImage, 0, 0, NULL, (BITMAPINFO *)&Header, DIB_PAL_COLORS);
	size.cx = Header.bV4Width;
	size.cy = Header.bV4Height;

	// Calculate the number of elements in a row/column
	m_nColumnCount = (size.cx / cx);
	m_nRowCount = (size.cy / cy);

	if ((m_nColumnCount == 0) || (m_nRowCount == 0))
	{
		DeleteObject(m_hImage);
		return FALSE;
	}

	// Create the mask image
	m_hMask = (HBITMAP)CopyImage(m_hImage, IMAGE_BITMAP, size.cx, size.cy, 0);
	if (m_hMask == NULL)
	{
		DeleteObject(m_hImage);
		return FALSE;
	}

	// Create the bitmap dc
	if (!m_dcImages.CreateCompatibleDC(pDC))
	{
		DeleteObject(m_hImage);
		DeleteObject(m_hMask);
		return FALSE;
	}

	// Create the mask dc
	if (!m_dcMask.CreateCompatibleDC(pDC))
	{
		DeleteObject(m_hImage);
		DeleteObject(m_hMask);
		return FALSE;
	}

	// Load the images into the DC
	m_dcImages.SelectObject(m_hImage);
	m_dcMask.SelectObject(m_hMask);

	// Create the mask
	for (int i = 0; i < size.cx; i++)
	{
		for (int j  =0; j < size.cy; j++)
		{
			if (m_dcMask.GetPixel(i,j)==crBackground)
			{
				m_dcMask.SetPixel(i,j,(COLORREF)0x00FFFFFF);
				m_dcImages.SetPixel(i,j,(COLORREF)0x00000000);
			}
			else
			{
				m_dcMask.SetPixel(i,j,(COLORREF)0x00000000);
			}
		}
	}

	m_bLoaded = TRUE;

	return TRUE;
}

BOOL CImageArray::DrawImage(CDC *pDC, int x, int y, int nRow, int nColumn)
{
	if (!m_bLoaded)
	{
		return FALSE;
	}

	int nOffsetX = (nColumn * m_nImageCX);
	int nOffsetY = (nRow * m_nImageCY);

	pDC->BitBlt(x, y, m_nImageCX, m_nImageCY, &m_dcMask, nOffsetX, nOffsetY, SRCAND);
	pDC->BitBlt(x, y, m_nImageCX, m_nImageCY, &m_dcImages, nOffsetX, nOffsetY, SRCPAINT);

	return TRUE;
}

BOOL CImageArray::DrawImage(CDC *pDC, int x, int y, int nImage)
{
	return DrawImage(pDC, x, y, (nImage / m_nColumnCount), (nImage % m_nColumnCount));
}

BOOL CImageArray::DrawImageClipped(CDC *pDC, int x, int y, int xsrc, int ysrc, 
								   int cx, int cy, int nRow, int nColumn)
{
	if (!m_bLoaded)
	{
		return FALSE;
	}

	int nOffsetX = (nColumn * m_nImageCX) + xsrc;
	int nOffsetY = (nRow * m_nImageCY) + xsrc;

	pDC->BitBlt(x, y, cx, cy, &m_dcMask, nOffsetX, nOffsetY, SRCAND);
	pDC->BitBlt(x, y, cx, cy, &m_dcImages, nOffsetX, nOffsetY, SRCPAINT);

	return TRUE;
}

BOOL CImageArray::DrawImageClipped(CDC *pDC, int x, int y, int xsrc, int ysrc, 
								   int cx, int cy, int nImage)
{
	return DrawImageClipped(pDC, x, y, xsrc, ysrc, cx, cy, (nImage / m_nColumnCount), (nImage % m_nColumnCount));
}





















