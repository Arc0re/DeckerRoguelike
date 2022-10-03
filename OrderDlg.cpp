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

// OrderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "OrderDlg.h"
#include "Character.h"
#include "Program.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// COrderDlg dialog


COrderDlg::COrderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderDlg)
	m_class = 0;
	m_cost = _T("");
	m_rating = 0;
	m_time = _T("");
	m_type = 0;
	//}}AFX_DATA_INIT
}


void COrderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderDlg)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDC_ORDER_TYPE, m_Type);
	DDX_Control(pDX, IDC_ORDER_RATING, m_Rating);
	DDX_Control(pDX, IDC_ORDER_CLASS, m_Class);
	DDX_CBIndex(pDX, IDC_ORDER_CLASS, m_class);
	DDX_Text(pDX, IDC_ORDER_COST, m_cost);
	DDX_CBIndex(pDX, IDC_ORDER_RATING, m_rating);
	DDX_Text(pDX, IDC_ORDER_TIME, m_time);
	DDX_CBIndex(pDX, IDC_ORDER_TYPE, m_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderDlg, CDialog)
	//{{AFX_MSG_MAP(COrderDlg)
	ON_CBN_SELCHANGE(IDC_ORDER_CLASS, OnSelchangeOrderClass)
	ON_CBN_SELCHANGE(IDC_ORDER_RATING, OnSelchangeOrderRating)
	ON_CBN_SELCHANGE(IDC_ORDER_TYPE, OnSelchangeOrderType)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderDlg message handlers

BOOL COrderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Fill in the class list. This will handle the rest
	FillInClassList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void COrderDlg::FillInClassList()
{
	int i;
	m_Class.ResetContent();

	if (m_type == IT_SOFTWARE)
	{
		// Program
		for (i=0; i<NUM_PROGRAMS; i++)
		{
			m_Class.AddString(GetProgramClassName(i));
		}
	}
	else if (m_type == IT_CHIP)
	{
		// Chip
		for (i=0; i<NUM_CHIPS; i++)
		{
			m_Class.AddString(GetChipName(i));
		}
	}
	else //IT_HARDWARE
	{
		// Hardware
		for (i=0; i<NUM_HW; i++)
		{
			m_Class.AddString(GetHardwareName(i));
		}
	}

	// Default to first item of class
	m_class = 0;

	FillInRatingList();
}

void COrderDlg::FillInRatingList()
{
	int i;
	CString szTmp;
	int nMax;

	m_Rating.ResetContent();

	// Get the maximum rating
	if ((m_type == IT_SOFTWARE) || (m_type == IT_CHIP))
	{
		// For software and chips, max level is dictated by lifestyle.
		// Note that this is the max that will appear in the shop.
		nMax = 6 + (2*g_pChar->m_nLifestyle);
	}
	else
	{
		// Max is dictated by hardware type
		nMax = GetMaxHardwareRating(m_class);
	}

	// Fill in the list box
	for (i = 0; i < nMax; i++)
	{
		szTmp.Format("%d", i+1);
		m_Rating.AddString(szTmp);
	}

	// Select an initial rating of 1
	m_rating = 0;

	// Update source and time
	FillInMisc();
}

void COrderDlg::FillInMisc()
{
	//------------------
	// Fill in the cost
	//------------------
	// Get base price according to type
	if (m_type == IT_SOFTWARE)
	{
		// Program
		m_nPrice = GetSoftwarePrice(m_class, m_rating+1);
	}
	else if (m_type == IT_CHIP)
	{
		// Chip
		m_nPrice = GetChipPrice(m_class, m_rating+1);
	}
	else //IT_HARDWARE
	{
		// Hardware
		m_nPrice = GetHardwarePrice(m_class, m_rating+1);
	}

	// Now, double it because we are ordering
	m_nPrice *= 2;
	m_cost.Format("%d cr", m_nPrice);

	// If user doesn't have enough money, disable the OK button
	m_Ok.EnableWindow(g_pChar->m_nCredits >= m_nPrice);

	//------------------
	// Fill in the time
	//------------------
	// Time is dependant on price
	if (m_nPrice < 500)
		m_nTime = 2;
	else if (m_nPrice < 1000)
		m_nTime = 3;
	else if (m_nPrice < 5000)
		m_nTime = 4;
	else if (m_nPrice < 10000)
		m_nTime = 5;
	else if (m_nPrice < 50000)
		m_nTime = 6;
	else
		m_nTime = 7;

	// Software is quicker than the others
	if (m_type == IT_SOFTWARE)
	{
		m_nTime--;
	}

	m_time.Format("%d days", m_nTime);

	UpdateData(FALSE);
}

void COrderDlg::OnSelchangeOrderType() 
{
	UpdateData(TRUE);
	FillInClassList();
}

void COrderDlg::OnSelchangeOrderClass() 
{
	UpdateData(TRUE);
	FillInRatingList();
}

void COrderDlg::OnSelchangeOrderRating() 
{
	UpdateData(TRUE);
	FillInMisc();
}

void COrderDlg::OnOK() 
{
	int nRating = m_rating+1;

	// Make sure this is usable
	if (m_type == IT_SOFTWARE)
	{
		// Program
		POSITION p;
		CProgram *pProgram;

		p = g_pChar->m_olSoftware.GetHeadPosition();
		while (p!=NULL)
		{
			pProgram = (CProgram *)g_pChar->m_olSoftware.GetNext(p);

			if ((pProgram->m_nClass == m_class) &&
				(pProgram->m_nRating == nRating))
			{
				DeckerMessageBox("You already have a program of this rating.");
				return;
			}
		}
	}
	else if (m_type == IT_CHIP)
	{
		// Chip
		if (g_pChar->m_nChip[m_class] >= nRating)
		{
			DeckerMessageBox("You already have a this chip of equal or higher rating.");
			return;
		}
	}
	else //IT_HARDWARE
	{
		// Hardware
		// Chip
		if (g_pChar->m_nHardware[m_class] >= nRating)
		{
			DeckerMessageBox("You already have this item of equal or higher rating.");
			return;
		}
	}

	// Set the order information
	g_pChar->m_nOrderItemType = m_type;
	g_pChar->m_nOrderItemClass = m_class;
	g_pChar->m_nOrderItemRating = nRating;
	g_pChar->m_nOrderTimeLeft = m_nTime;

	// Subtract the money
	g_pChar->m_nCredits -= m_nPrice;

	// Show a message
	DeckerMessageBox("Order has been placed.");
	
	CDialog::OnOK();
}

BOOL COrderDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_SHOP);
	return TRUE;
}
