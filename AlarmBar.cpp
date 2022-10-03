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

// AlarmBar.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "AlarmBar.h"
#include "Global.h"
#include "Character.h"
#include "System.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

// Current alarm colors
static const COLORREF g_crAlarmColor[3] = {DK_GREEN,YELLOW,RED};

/////////////////////////////////////////////////////////////////////////////
// CAlarmBar

CAlarmBar::CAlarmBar()
{
}

CAlarmBar::~CAlarmBar()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CAlarmBar, CWnd)
	//{{AFX_MSG_MAP(CAlarmBar)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAlarmBar message handlers

void CAlarmBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	RECT rect;

	GetClientRect(&rect);

	COLORREF crColor;

	if (g_pChar->m_pSystem->m_nTurnsUntilCrash == 0)
		crColor = g_crAlarmColor[g_pChar->m_pSystem->m_nAlert];
	else
		crColor = PURPLE;
	dc.FillSolidRect(&rect, crColor);
}
