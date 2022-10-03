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

// RestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "RestDlg.h"
#include "Character.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// CRestDlg dialog


CRestDlg::CRestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRestDlg)
	m_cost_hosp = _T("");
	m_cost_hosp_all = _T("");
	m_date = _T("");
	m_health = _T("");
	m_money = _T("");
	m_time_home = _T("");
	m_time_home_all = _T("");
	m_time_hosp = _T("");
	m_time_hosp_all = _T("");
	//}}AFX_DATA_INIT
}


void CRestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRestDlg)
	DDX_Control(pDX, IDC_HOSP_HEAL_ALL, m_Hosp_Heal_All);
	DDX_Control(pDX, IDC_HOSP_HEAL, m_Hosp_Heal);
	DDX_Control(pDX, IDC_HEAL_ALL, m_Heal_All);
	DDX_Control(pDX, IDC_HEAL, m_Heal);
	DDX_Text(pDX, IDC_REST_COST_HOSP, m_cost_hosp);
	DDX_Text(pDX, IDC_REST_COST_HOSP_ALL, m_cost_hosp_all);
	DDX_Text(pDX, IDC_REST_DATE, m_date);
	DDX_Text(pDX, IDC_REST_HEALTH, m_health);
	DDX_Text(pDX, IDC_REST_MONEY, m_money);
	DDX_Text(pDX, IDC_REST_TIME_HOME, m_time_home);
	DDX_Text(pDX, IDC_REST_TIME_HOME_ALL, m_time_home_all);
	DDX_Text(pDX, IDC_REST_TIME_HOSP, m_time_hosp);
	DDX_Text(pDX, IDC_REST_TIME_HOSP_ALL, m_time_hosp_all);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRestDlg, CDialog)
	//{{AFX_MSG_MAP(CRestDlg)
	ON_BN_CLICKED(IDC_HEAL, OnHeal)
	ON_BN_CLICKED(IDC_HEAL_ALL, OnHealAll)
	ON_BN_CLICKED(IDC_HOSP_HEAL, OnHospHeal)
	ON_BN_CLICKED(IDC_HOSP_HEAL_ALL, OnHospHealAll)
	ON_BN_CLICKED(IDC_REST_WAIT, OnWait)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRestDlg message handlers

BOOL CRestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Fill in all data
	RecalcAll();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRestDlg::RecalcAll()
{
	// Put basic stuff
	m_date.Format("%s %d, %d",
					g_szMonthNames[g_pChar->m_nMonth],
					g_pChar->m_nDayOfMonth+1,
					g_pChar->m_nYear);
	m_health.Format("%d%%",g_pChar->m_nHealthPhysical * HEALTH_INCREMENT);
	m_money.Format("%d",g_pChar->m_nCredits);

	// Calculate the healing times and costs
	m_nDamage = MAX_HEALTH - g_pChar->m_nHealthPhysical;
	m_nBaseTime = m_nDamage;
	//FSO 7-25-01 Cut hospital time from 1/2 to 1/3
	m_nFullTime = ((m_nDamage * (m_nDamage + 2))/3);
	m_nBaseHospCost = 100 * m_nBaseTime;
	m_nFullHospCost = 100 * m_nFullTime;

	m_time_home.Format("%d", m_nBaseTime);
	m_time_home_all.Format("%d", m_nFullTime);
	m_time_hosp.Format("%d", (m_nBaseTime+1)/2);
	m_time_hosp_all.Format("%d", (m_nFullTime+1)/2);
	m_cost_hosp.Format("%d", m_nBaseHospCost);
	m_cost_hosp_all.Format("%d", m_nFullHospCost);

	UpdateData(FALSE);

	// Enable/disable controls
	m_Heal.EnableWindow(m_nDamage != 0);
	m_Heal_All.EnableWindow(m_nDamage != 0);
	m_Hosp_Heal.EnableWindow((m_nDamage > 1) && (g_pChar->m_nCredits >= m_nBaseHospCost));
	m_Hosp_Heal_All.EnableWindow((m_nDamage > 1) && (g_pChar->m_nCredits >= m_nFullHospCost));
}

void CRestDlg::OnHeal() 
{
	if (g_pChar->PassTime(m_nBaseTime))
	{
		SendMessage(WM_CLOSE);
		return;
	}
	g_pChar->m_nHealthPhysical++;
	RecalcAll();
}

void CRestDlg::OnHealAll() 
{
	if (g_pChar->PassTime(m_nFullTime))
	{
		SendMessage(WM_CLOSE);
		return;
	}
	g_pChar->m_nHealthPhysical = MAX_HEALTH;
	RecalcAll();
}

void CRestDlg::OnHospHeal() 
{
	if (g_pChar->PassTime((m_nBaseTime+1)/2))
	{
		SendMessage(WM_CLOSE);
		return;
	}
	g_pChar->m_nHealthPhysical++;
	g_pChar->m_nCredits -= m_nBaseHospCost;
	RecalcAll();
}

void CRestDlg::OnHospHealAll() 
{
	if (g_pChar->PassTime((m_nFullTime+1)/2))
	{
		SendMessage(WM_CLOSE);
		return;
	}
	g_pChar->m_nHealthPhysical = MAX_HEALTH;
	g_pChar->m_nCredits -= m_nFullHospCost;
	RecalcAll();
}

void CRestDlg::OnWait() 
{
	if (g_pChar->PassTime(1))
	{
		SendMessage(WM_CLOSE);
		return;
	}
	RecalcAll();
}

BOOL CRestDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_REST);
	return TRUE;
}
