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

#if !defined(AFX_CHARDATADIALOG_H__39C33343_81D8_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_CHARDATADIALOG_H__39C33343_81D8_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharDataDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharDataDialog dialog

class CCharDataDialog : public CDialog
{
// Construction
public:
	CCharDataDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCharDataDialog)
	enum { IDD = IDD_CHAR_DATA };
	CStatic	m_Ironman;
	CButton	m_Lifestyle_Upgrade;
	CButton	m_Incr_Stealth;
	CButton	m_Incr_Prog;
	CButton	m_Incr_Chip_Des;
	CButton	m_Incr_Defense;
	CButton	m_Incr_Attack;
	CButton	m_Incr_Analysis;
	CButton	m_view_deck;
	CString	m_analysis;
	CString	m_attack;
	CString	m_chip_design;
	CString	m_credits;
	CString	m_defense;
	CString	m_lifestyle;
	CString	m_name;
	CString	m_physical_health;
	CString	m_programming;
	CString	m_reputation;
	CString	m_stealth;
	CString	m_skill_points;
	CString	m_lifestyle_cost;
	CString	m_lifestyle_upgrade_cost;
	CString	m_date;
	CString	m_rent_due_time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharDataDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCharDataDialog)
	afx_msg void OnCharViewDeck();
	virtual BOOL OnInitDialog();
	afx_msg void OnCharIncrAnalysis();
	afx_msg void OnCharIncrAttack();
	afx_msg void OnCharIncrChipDes();
	afx_msg void OnCharIncrDefense();
	afx_msg void OnCharIncrProg();
	afx_msg void OnCharIncrStealth();
	afx_msg void OnCharLifestyleUpgrade();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARDATADIALOG_H__39C33343_81D8_11D4_B835_0080C8C1DB04__INCLUDED_)
