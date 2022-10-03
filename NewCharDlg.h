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

#if !defined(AFX_NEWCHARDLG_H__5B5CA3B5_802B_11D4_82A8_00104B751D91__INCLUDED_)
#define AFX_NEWCHARDLG_H__5B5CA3B5_802B_11D4_82A8_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewCharDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewCharDlg dialog

class CNewCharDlg : public CDialog
{
// Construction
public:
	CNewCharDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewCharDlg)
	enum { IDD = IDD_NEW_CHAR };
	CButton	m_Image;
	CSpinButtonCtrl	m_Image_Spin;
	CString	m_name;
	int		m_bonus;
	BOOL	m_ironman;
	BOOL	m_tooltips;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewCharDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewCharDlg)
	afx_msg void OnKillfocusNewCharName();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnDeltaposNewCharImageSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnNewCharIronman();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CImageList m_ilChar;

public:
	int m_nImage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWCHARDLG_H__5B5CA3B5_802B_11D4_82A8_00104B751D91__INCLUDED_)
