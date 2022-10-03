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

// ImageArray.h: interface for the CImageArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEARRAY_H__06EC0E42_9BA7_11D4_82C3_00104B751D91__INCLUDED_)
#define AFX_IMAGEARRAY_H__06EC0E42_9BA7_11D4_82C3_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CImageArray : public CObject  
{
public:
	CImageArray();
	virtual ~CImageArray();

public:
	BOOL LoadImages(CDC *pDC, LPCTSTR lpszName, int cx, int cy, COLORREF crBackground);
	BOOL DrawImage(CDC *pDC, int x, int y, int nRow, int nColumn);
	BOOL DrawImage(CDC *pDC, int x, int y, int nImage);
	BOOL DrawImageClipped(CDC *pDC, int x, int y, int xsrc, int ysrc, int cx, int cy, int nRow, int nColumn);
	BOOL DrawImageClipped(CDC *pDC, int x, int y, int xsrc, int ysrc, int cx, int cy,  int nImage);

private:
	BOOL m_bLoaded;

	HBITMAP m_hImage;
	HBITMAP m_hMask;

	CDC m_dcImages;
	CDC m_dcMask;

	int m_nImageCX;
	int m_nImageCY;

	int m_nRowCount;
	int m_nColumnCount;
};

#endif // !defined(AFX_IMAGEARRAY_H__06EC0E42_9BA7_11D4_82C3_00104B751D91__INCLUDED_)
