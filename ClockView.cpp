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

// ClockView.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "ClockView.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClockView

CClockView::CClockView()
{
	m_ilTime.Create(IDB_IL_TIME,7,0,WHITE);
}

CClockView::~CClockView()
{
	DestroyWindow();
}


BEGIN_MESSAGE_MAP(CClockView, CWnd)
	//{{AFX_MSG_MAP(CClockView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CClockView message handlers

void CClockView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	int iTime = m_nTime;
	int iHours10;
	int iHours1;
	int iMinutes10;
	int iMinutes1;
	int iSeconds10;
	int iSeconds1;
	CPoint ptPoint(0,0);
	CSize szSize(7,11);
	CPoint ptOrigin(0,0);

	// Calculate all of the numbers
	iSeconds1 = (iTime%10);
	iTime/=10;
	iSeconds10 = (iTime%6);
	iTime/=6;
	iMinutes1 = (iTime%10);
	iTime/=10;
	iMinutes10 = (iTime%6);
	iTime/=6;
	iHours1 = (iTime%10);
	iTime/=10;
	iHours10 = (iTime%6);
		
	m_ilTime.DrawIndirect(&dc,iHours10,ptPoint,szSize,ptOrigin,ILD_ROP,SRCCOPY);
	ptPoint.x += 7;
	m_ilTime.DrawIndirect(&dc,iHours1,ptPoint,szSize,ptOrigin,ILD_ROP,SRCCOPY);
	ptPoint.x += 8;
	m_ilTime.DrawIndirect(&dc,iMinutes10,ptPoint,szSize,ptOrigin,ILD_ROP,SRCCOPY);
	ptPoint.x += 7;
	m_ilTime.DrawIndirect(&dc,iMinutes1,ptPoint,szSize,ptOrigin,ILD_ROP,SRCCOPY);
	ptPoint.x += 8;
	m_ilTime.DrawIndirect(&dc,iSeconds10,ptPoint,szSize,ptOrigin,ILD_ROP,SRCCOPY);
	ptPoint.x += 7;
	m_ilTime.DrawIndirect(&dc,iSeconds1,ptPoint,szSize,ptOrigin,ILD_ROP,SRCCOPY);
}
