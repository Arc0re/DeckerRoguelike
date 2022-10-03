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

#if !defined(AFX_RESTDLG_H__EB8AAC01_925B_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_RESTDLG_H__EB8AAC01_925B_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRestDlg dialog

class CRestDlg : public CDialog
{
// Construction
public:
	CRestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRestDlg)
	enum { IDD = IDD_REST };
	CButton	m_Hosp_Heal_All;
	CButton	m_Hosp_Heal;
	CButton	m_Heal_All;
	CButton	m_Heal;
	CString	m_cost_hosp;
	CString	m_cost_hosp_all;
	CString	m_date;
	CString	m_health;
	CString	m_money;
	CString	m_time_home;
	CString	m_time_home_all;
	CString	m_time_hosp;
	CString	m_time_hosp_all;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRestDlg)
	afx_msg void OnHeal();
	afx_msg void OnHealAll();
	afx_msg void OnHospHeal();
	afx_msg void OnHospHealAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnWait();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void RecalcAll();

	int m_nBaseTime;
	int m_nFullTime;
	int m_nBaseHospCost;
	int m_nFullHospCost;
	int m_nDamage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESTDLG_H__EB8AAC01_925B_11D4_B835_0080C8C1DB04__INCLUDED_)
