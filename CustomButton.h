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

// CustomButton.h: interface for the CCustomButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMBUTTON_H__696A59E3_2084_11D6_8474_00104B751D91__INCLUDED_)
#define AFX_CUSTOMBUTTON_H__696A59E3_2084_11D6_8474_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCustomButton : public CButton  
{
	DECLARE_DYNAMIC(CCustomButton)
public:
// Construction
	CCustomButton();
	virtual ~CCustomButton();

	BOOL LoadBitmapsFromFiles(
			LPCTSTR lpszBitmapFile,
			LPCTSTR lpszBitmapFileSel = NULL, 
			LPCTSTR lpszBitmapFileFocus = NULL,
			LPCTSTR lpszBitmapFileDisabled = NULL);
	void SetBitmaps(
			HBITMAP hBitmap,
			HBITMAP hBitmapSel = NULL, 
			HBITMAP hBitmapFocus = NULL,
			HBITMAP hBitmapDisabled = NULL);

// Implementation:
protected:
	// all bitmaps must be the same size
	HBITMAP m_hbitmap;           // normal image (REQUIRED)
	HBITMAP m_hbitmapSel;        // selected image (OPTIONAL)
	HBITMAP m_hbitmapFocus;      // focused but not selected (OPTIONAL)
	HBITMAP m_hbitmapDisabled;   // disabled bitmap (OPTIONAL)

	BOOL m_bDeleteBitmaps;

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};

#endif // !defined(AFX_CUSTOMBUTTON_H__696A59E3_2084_11D6_8474_00104B751D91__INCLUDED_)
