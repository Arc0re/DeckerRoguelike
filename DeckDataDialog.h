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

#if !defined(AFX_DECKDATADIALOG_H__3AA7B781_8233_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_DECKDATADIALOG_H__3AA7B781_8233_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeckDataDialog.h : header file
//

class CProgram;

class COEMBitmapButton : public CBitmapButton
{
	DECLARE_DYNAMIC(COEMBitmapButton)
public:
	COEMBitmapButton() {};

	BOOL LoadBitmaps(UINT nIDBitmapResource,
			UINT nIDBitmapResourceSel = 0,
			UINT nIDBitmapResourceFocus = 0,
			UINT nIDBitmapResourceDisabled = 0);
};


/////////////////////////////////////////////////////////////////////////////
// CDeckDataDialog dialog

class CDeckDataDialog : public CDialog
{
// Construction
public:
	CDeckDataDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeckDataDialog)
	enum { IDD = IDD_DECK_DATA };
	CButton	m_Sound;
	CButton	m_Rename;
	CButton	m_Trash;
	CButton	m_Default;
	COEMBitmapButton	m_Up;
	COEMBitmapButton	m_Down;
	CButton	m_Clear;
	CListCtrl	m_Software;
	CButton	m_Load;
	CString	m_analysis;
	CString	m_attack;
	CString	m_cpu;
	CString	m_current_load;
	CString	m_defense;
	CString	m_other_hw;
	CString	m_stealth;
	CString	m_cop;
	CString	m_load_average;
	CString	m_load_heavy;
	CString	m_load_light;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeckDataDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeckDataDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeckLoad();
	afx_msg void OnDeckViewChar();
	afx_msg void OnDeckClear();
	afx_msg void OnItemchangedDeckSoftware(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeckUp();
	afx_msg void OnDeckDown();
	afx_msg void OnDeckDefault();
	afx_msg void OnDeckTrash();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnDeckRename();
	afx_msg void OnDeckSound();
	afx_msg void OnColumnclickDeckSoftware(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_nDefaultLoad;
	int m_nLightLoad;
	int m_nHeavyLoad;
	int m_nMaxLoad;

	void UpdateLoad();
	void InsertProgram(int nIndex, CProgram *pProgram);
	void UpdateControls();

	CImageList m_ilSoftware;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECKDATADIALOG_H__3AA7B781_8233_11D4_B835_0080C8C1DB04__INCLUDED_)
