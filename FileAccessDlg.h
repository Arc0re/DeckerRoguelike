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

#if !defined(AFX_FILEACCESSDLG_H__D1A64C51_8E26_11D4_82B4_00104B751D91__INCLUDED_)
#define AFX_FILEACCESSDLG_H__D1A64C51_8E26_11D4_82B4_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileAccessDlg.h : header file
//

// Types of operations for this window
#define FO_VIEW		0	// Just show the files
#define FO_GET		1	// Get (download) a file
#define	FO_EDIT		2	// Edit a file
#define FO_ERASE	3	// Erase a file

/////////////////////////////////////////////////////////////////////////////
// CFileAccessDlg dialog

class CFileAccessDlg : public CDialog
{
// Construction
public:
	CFileAccessDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileAccessDlg)
	enum { IDD = IDD_TRANSFER };
	CStatic	m_After_Text;
	CStatic	m_Load_Text;
	CStatic	m_Load;
	CStatic	m_Light_Text;
	CStatic	m_Light;
	CStatic	m_Heavy_Text;
	CStatic	m_Heavy;
	CStatic	m_Average_Text;
	CStatic	m_Average;
	CStatic	m_After;
	CListCtrl	m_Item_List;
	CButton	m_Ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileAccessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFileAccessDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnItemchangedTransferItemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkVcTransferItemList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nOperationType;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEACCESSDLG_H__D1A64C51_8E26_11D4_82B4_00104B751D91__INCLUDED_)
