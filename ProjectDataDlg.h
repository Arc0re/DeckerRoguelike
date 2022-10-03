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

#if !defined(AFX_PROJECTDATADLG_H__AABB0081_93A4_11D4_82B9_00104B751D91__INCLUDED_)
#define AFX_PROJECTDATADLG_H__AABB0081_93A4_11D4_82B9_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectDataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjectDataDlg dialog

class CProjectDataDlg : public CDialog
{
// Construction
public:
	CProjectDataDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProjectDataDlg)
	enum { IDD = IDD_PROJECT };
	CButton	m_Trash;
	CButton	m_Work_Week;
	CButton	m_Work_Day;
	CListCtrl	m_Source;
	CButton	m_New;
	CButton	m_Finish;
	CButton	m_Build;
	CString	m_date;
	CString	m_time;
	CString	m_type;
	CString	m_chip_time;
	CString	m_chip_type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProjectDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnProjBuild();
	afx_msg void OnProjFinish();
	afx_msg void OnProjNew();
	afx_msg void OnItemchangedProjSource(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnProjWorkDay();
	afx_msg void OnProjWorkWeek();
	afx_msg void OnProjTrash();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnColumnclickProjSource(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void UpdateProject();
	void UpdateChip(BOOL bUpdateSource = FALSE);
	void DoCompleteProject();

	CImageList m_ilItems;
	int m_nSkill;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTDATADLG_H__AABB0081_93A4_11D4_82B9_00104B751D91__INCLUDED_)
