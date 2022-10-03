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

// Source.cpp: implementation of the CSource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "Source.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSource::CSource()
{
}

CSource::~CSource()
{
}

void CSource::Save(CArchive &ar)
{
	ar << m_nType;
	ar << m_nClass;
	ar << m_nRating;
}

void CSource::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	ar >> m_nType;
	ar >> m_nClass;
	ar >> m_nRating;
}
