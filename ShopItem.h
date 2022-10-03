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

// ShopItem.h: interface for the CShopItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPITEM_H__D419A652_84B6_11D4_82AC_00104B751D91__INCLUDED_)
#define AFX_SHOPITEM_H__D419A652_84B6_11D4_82AC_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShopItem : public CObject  
{
public:
	// Construction
	CShopItem();
	virtual ~CShopItem();

	// Member Functions
	void Generate(int nType);
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion);

	// Member Data
	CString m_szText;	// Item description
	int m_nType;		// Item type
	int m_nSubType;		// Item subtype
	int m_nRating;		// Item rating
	int m_nPrice;		// Price type
};

// Global functions
int GetHardwarePrice(int nType, int nRating);

#endif // !defined(AFX_SHOPITEM_H__D419A652_84B6_11D4_82AC_00104B751D91__INCLUDED_)
