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

#if !defined(AFX_TRANSFERDLG_H__D967F041_8D98_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_TRANSFERDLG_H__D967F041_8D98_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransferDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg dialog

class CTransferDlg : public CDialog
{
// Construction
public:
	CTransferDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTransferDlg)
	enum { IDD = IDD_TRANSFER };
	CButton	m_Ok;
	CListCtrl	m_Item_List;
	CString	m_after;
	CString	m_average;
	CString	m_heavy;
	CString	m_light;
	CString	m_load;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransferDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransferDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedTransferItemList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnDblclkVcTransferItemList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void GetLoadText(CString &szStr, int nLoad);

	int m_nLightLoad;
	int m_nHeavyLoad;
	int m_nMaxLoad;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFERDLG_H__D967F041_8D98_11D4_B835_0080C8C1DB04__INCLUDED_)
