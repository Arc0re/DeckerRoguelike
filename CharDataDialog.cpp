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

// CharDataDialog.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "CharDataDialog.h"
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
// CCharDataDialog dialog


CCharDataDialog::CCharDataDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCharDataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharDataDialog)
	m_analysis = _T("");
	m_attack = _T("");
	m_chip_design = _T("");
	m_credits = _T("");
	m_defense = _T("");
	m_lifestyle = _T("");
	m_name = _T("");
	m_physical_health = _T("");
	m_programming = _T("");
	m_reputation = _T("");
	m_stealth = _T("");
	m_skill_points = _T("");
	m_lifestyle_cost = _T("");
	m_lifestyle_upgrade_cost = _T("");
	m_date = _T("");
	m_rent_due_time = _T("");
	//}}AFX_DATA_INIT
}


void CCharDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharDataDialog)
	DDX_Control(pDX, IDC_CHAR_IRONMAN, m_Ironman);
	DDX_Control(pDX, IDC_CHAR_LIFESTYLE_UPGRADE, m_Lifestyle_Upgrade);
	DDX_Control(pDX, IDC_CHAR_INCR_STEALTH, m_Incr_Stealth);
	DDX_Control(pDX, IDC_CHAR_INCR_PROG, m_Incr_Prog);
	DDX_Control(pDX, IDC_CHAR_INCR_CHIP_DES, m_Incr_Chip_Des);
	DDX_Control(pDX, IDC_CHAR_INCR_DEFENSE, m_Incr_Defense);
	DDX_Control(pDX, IDC_CHAR_INCR_ATTACK, m_Incr_Attack);
	DDX_Control(pDX, IDC_CHAR_INCR_ANALYSIS, m_Incr_Analysis);
	DDX_Control(pDX, IDC_CHAR_VIEW_DECK, m_view_deck);
	DDX_Text(pDX, IDC_CHAR_ANALYSIS, m_analysis);
	DDX_Text(pDX, IDC_CHAR_ATTACK, m_attack);
	DDX_Text(pDX, IDC_CHAR_CHIP_DESIGN, m_chip_design);
	DDX_Text(pDX, IDC_CHAR_CREDITS, m_credits);
	DDX_Text(pDX, IDC_CHAR_DEFENSE, m_defense);
	DDX_Text(pDX, IDC_CHAR_LIFESTYLE, m_lifestyle);
	DDX_Text(pDX, IDC_CHAR_NAME, m_name);
	DDX_Text(pDX, IDC_CHAR_PHYS_HEALTH, m_physical_health);
	DDX_Text(pDX, IDC_CHAR_PROGRAMMING, m_programming);
	DDX_Text(pDX, IDC_CHAR_REPUTATION, m_reputation);
	DDX_Text(pDX, IDC_CHAR_STEALTH, m_stealth);
	DDX_Text(pDX, IDC_CHAR_SKILL_POINTS, m_skill_points);
	DDX_Text(pDX, IDC_CHAR_LIFESTYLE_COST, m_lifestyle_cost);
	DDX_Text(pDX, IDC_CHAR_LIFESTYLE_UPGRADE_COST, m_lifestyle_upgrade_cost);
	DDX_Text(pDX, IDC_CHAR_DATE, m_date);
	DDX_Text(pDX, IDC_CHAR_RENT_DUE_TIME, m_rent_due_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharDataDialog, CDialog)
	//{{AFX_MSG_MAP(CCharDataDialog)
	ON_BN_CLICKED(IDC_CHAR_VIEW_DECK, OnCharViewDeck)
	ON_BN_CLICKED(IDC_CHAR_INCR_ANALYSIS, OnCharIncrAnalysis)
	ON_BN_CLICKED(IDC_CHAR_INCR_ATTACK, OnCharIncrAttack)
	ON_BN_CLICKED(IDC_CHAR_INCR_CHIP_DES, OnCharIncrChipDes)
	ON_BN_CLICKED(IDC_CHAR_INCR_DEFENSE, OnCharIncrDefense)
	ON_BN_CLICKED(IDC_CHAR_INCR_PROG, OnCharIncrProg)
	ON_BN_CLICKED(IDC_CHAR_INCR_STEALTH, OnCharIncrStealth)
	ON_BN_CLICKED(IDC_CHAR_LIFESTYLE_UPGRADE, OnCharLifestyleUpgrade)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharDataDialog message handlers

void CCharDataDialog::OnCharViewDeck() 
{
	EndDialog(IDC_CHAR_VIEW_DECK);
}

BOOL CCharDataDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set the initial values
	m_name = g_pChar->m_szName;
	m_credits.Format("%d",g_pChar->m_nCredits);
	m_reputation = g_szRepLevelString[g_pChar->m_nRepLevel];
	if (g_pChar->m_nRepLevel == ((g_pChar->m_nLifestyle + 1) * 4))
	{
		m_reputation += " (Max)";
	}
	m_attack.Format("%d",g_pChar->m_nAttackSkill);
	m_defense.Format("%d",g_pChar->m_nDefenseSkill);
	m_stealth.Format("%d",g_pChar->m_nStealthSkill);
	m_analysis.Format("%d",g_pChar->m_nAnalysisSkill);
	m_chip_design.Format("%d",g_pChar->m_nChipDesignSkill);
	m_programming.Format("%d",g_pChar->m_nProgrammingSkill);
	m_physical_health.Format("%d%%",g_pChar->m_nHealthPhysical*HEALTH_INCREMENT);
	m_skill_points.Format("%d",g_pChar->m_nSkillPoints);

	m_date.Format("%s %d, %d",
					g_szMonthNames[g_pChar->m_nMonth],
					g_pChar->m_nDayOfMonth+1,
					g_pChar->m_nYear);

	m_lifestyle = g_pChar->GetLifestyleString();
	m_lifestyle_cost.Format("%d", g_nLifestyleMonthlyCost[g_pChar->m_nLifestyle]);
	m_rent_due_time.Format("%d days", GetDays(g_pChar->m_nMonth,g_pChar->m_nYear) - g_pChar->m_nDayOfMonth);
	if (g_pChar->m_nLifestyle!=MAX_LIFESTYLE)
	{
		m_lifestyle_upgrade_cost.Format("%d", g_nLifestyleMonthlyCost[g_pChar->m_nLifestyle+1] * LIFESTYLE_UPGRADE_FACTOR);
	}
	else
	{
		m_lifestyle_upgrade_cost = "-";
	}

	UpdateData(FALSE);

	if (g_pChar->m_bIronMan)
	{
		m_Ironman.ShowWindow(SW_SHOW);
	}
	else
	{
		m_Ironman.ShowWindow(SW_HIDE);
	}

	UpdateControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CCharDataDialog::OnCharIncrAttack() 
{
	g_pChar->m_nSkillPoints -= g_pChar->m_nAttackSkill;
	g_pChar->m_nAttackSkill++;
	UpdateControls();
	m_attack.Format("%d",g_pChar->m_nAttackSkill);
	m_skill_points.Format("%d",g_pChar->m_nSkillPoints);
	UpdateData(FALSE);
}

void CCharDataDialog::OnCharIncrDefense() 
{
	g_pChar->m_nSkillPoints -= g_pChar->m_nDefenseSkill;
	g_pChar->m_nDefenseSkill++;
	UpdateControls();
	m_defense.Format("%d",g_pChar->m_nDefenseSkill);
	m_skill_points.Format("%d",g_pChar->m_nSkillPoints);
	UpdateData(FALSE);
}

void CCharDataDialog::OnCharIncrStealth() 
{
	g_pChar->m_nSkillPoints -= g_pChar->m_nStealthSkill;
	g_pChar->m_nStealthSkill++;
	UpdateControls();
	m_stealth.Format("%d",g_pChar->m_nStealthSkill);
	m_skill_points.Format("%d",g_pChar->m_nSkillPoints);
	UpdateData(FALSE);
}

void CCharDataDialog::OnCharIncrAnalysis() 
{
	g_pChar->m_nSkillPoints -= g_pChar->m_nAnalysisSkill;
	g_pChar->m_nAnalysisSkill++;
	UpdateControls();
	m_analysis.Format("%d",g_pChar->m_nAnalysisSkill);
	m_skill_points.Format("%d",g_pChar->m_nSkillPoints);
	UpdateData(FALSE);
}

void CCharDataDialog::OnCharIncrProg() 
{
	g_pChar->m_nSkillPoints -= g_pChar->m_nProgrammingSkill;
	g_pChar->m_nProgrammingSkill++;
	UpdateControls();
	m_programming.Format("%d",g_pChar->m_nProgrammingSkill);
	m_skill_points.Format("%d",g_pChar->m_nSkillPoints);
	UpdateData(FALSE);
}

void CCharDataDialog::OnCharIncrChipDes() 
{
	g_pChar->m_nSkillPoints -= g_pChar->m_nChipDesignSkill;
	g_pChar->m_nChipDesignSkill++;
	UpdateControls();
	m_chip_design.Format("%d",g_pChar->m_nChipDesignSkill);
	m_skill_points.Format("%d",g_pChar->m_nSkillPoints);
	UpdateData(FALSE);
}

void CCharDataDialog::UpdateControls()
{
	if (g_pChar->m_bOnRun)
	{
		//Disable everything
		m_Lifestyle_Upgrade.EnableWindow(FALSE);
		m_Incr_Attack.EnableWindow(FALSE);
		m_Incr_Defense.EnableWindow(FALSE);
		m_Incr_Stealth.EnableWindow(FALSE);
		m_Incr_Analysis.EnableWindow(FALSE);
		m_Incr_Prog.EnableWindow(FALSE);
		m_Incr_Chip_Des.EnableWindow(FALSE);
	}
	else
	{
		m_Lifestyle_Upgrade.EnableWindow((g_pChar->m_nLifestyle!=MAX_LIFESTYLE) && (g_pChar->m_nCredits >= (g_nLifestyleMonthlyCost[g_pChar->m_nLifestyle+1] * LIFESTYLE_UPGRADE_FACTOR)));
		m_Incr_Attack.EnableWindow(g_pChar->m_nAttackSkill <= g_pChar->m_nSkillPoints);
		m_Incr_Defense.EnableWindow(g_pChar->m_nDefenseSkill <= g_pChar->m_nSkillPoints);
		m_Incr_Stealth.EnableWindow(g_pChar->m_nStealthSkill <= g_pChar->m_nSkillPoints);
		m_Incr_Analysis.EnableWindow(g_pChar->m_nAnalysisSkill <= g_pChar->m_nSkillPoints);
		m_Incr_Prog.EnableWindow(g_pChar->m_nProgrammingSkill <= g_pChar->m_nSkillPoints);
		m_Incr_Chip_Des.EnableWindow(g_pChar->m_nChipDesignSkill <= g_pChar->m_nSkillPoints);
	}
}

void CCharDataDialog::OnCharLifestyleUpgrade() 
{
	g_pChar->m_nLifestyle++;
	g_pChar->m_nCredits -= (g_nLifestyleMonthlyCost[g_pChar->m_nLifestyle] * LIFESTYLE_UPGRADE_FACTOR);

	m_lifestyle = g_pChar->GetLifestyleString();
	m_credits.Format("%d",g_pChar->m_nCredits);
	m_lifestyle_cost.Format("%d", g_nLifestyleMonthlyCost[g_pChar->m_nLifestyle]);
	if (g_pChar->m_nLifestyle!=MAX_LIFESTYLE)
	{
		m_lifestyle_upgrade_cost.Format("%d", g_nLifestyleMonthlyCost[g_pChar->m_nLifestyle+1] * LIFESTYLE_UPGRADE_FACTOR);
	}
	else
	{
		m_lifestyle_upgrade_cost = "-";
	}

	m_Lifestyle_Upgrade.EnableWindow((g_pChar->m_nLifestyle!=MAX_LIFESTYLE) && (g_pChar->m_nCredits >= (g_nLifestyleMonthlyCost[g_pChar->m_nLifestyle+1] * LIFESTYLE_UPGRADE_FACTOR)));

	UpdateData(FALSE);

	AfxMessageBox("Lifestyle upgraded. New contracts and shop items are now available.");

	g_pChar->GenerateShopItems();
	g_pChar->ClearContracts();
	g_pChar->GenerateContracts();
}

BOOL CCharDataDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_CHAR);
	return TRUE;
}
