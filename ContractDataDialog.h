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

#if !defined(AFX_CONTRACTDATADIALOG_H__2847CFC1_83F2_11D4_82AB_00104B751D91__INCLUDED_)
#define AFX_CONTRACTDATADIALOG_H__2847CFC1_83F2_11D4_82AB_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContractDataDialog.h : header file
//

class CContract;

/////////////////////////////////////////////////////////////////////////////
// CContractDataDialog dialog

class CContractDataDialog : public CDialog
{
// Construction
public:
	CContractDataDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContractDataDialog)
	enum { IDD = IDD_CONTRACT_DATA };
	CStatic	m_Status2;
	CStatic	m_Status1;
	CButton	m_Accept;
	CString	m_description;
	CString	m_difficulty;
	CString	m_pay;
	CString	m_target_corp;
	CString	m_type;
	CString	m_deadline;
	CString	m_status1;
	CString	m_status2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContractDataDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContractDataDialog)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CContract *m_pContract;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTRACTDATADIALOG_H__2847CFC1_83F2_11D4_82AB_00104B751D91__INCLUDED_)
