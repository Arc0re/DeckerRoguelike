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

#if !defined(AFX_MISSIONRESULTSDLG_H__5D84F035_9080_11D4_82B7_00104B751D91__INCLUDED_)
#define AFX_MISSIONRESULTSDLG_H__5D84F035_9080_11D4_82B7_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MissionResultsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMissionResultsDlg dialog

class CMissionResultsDlg : public CDialog
{
// Construction
public:
	CMissionResultsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMissionResultsDlg)
	enum { IDD = IDD_MISSION_RESULTS };
	CString	m_contract;
	CString	m_files;
	CString	m_pay;
	CString	m_rep;
	CString	m_result;
	CString	m_skill_pts;
	CString	m_summary;
	CString	m_total;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMissionResultsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMissionResultsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MISSIONRESULTSDLG_H__5D84F035_9080_11D4_82B7_00104B751D91__INCLUDED_)
