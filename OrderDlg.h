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

#if !defined(AFX_ORDERDLG_H__C3679BF1_9602_11D4_82BD_00104B751D91__INCLUDED_)
#define AFX_ORDERDLG_H__C3679BF1_9602_11D4_82BD_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COrderDlg dialog

class COrderDlg : public CDialog
{
// Construction
public:
	COrderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COrderDlg)
	enum { IDD = IDD_ORDER_DLG };
	CButton	m_Ok;
	CComboBox	m_Type;
	CComboBox	m_Rating;
	CComboBox	m_Class;
	int		m_class;
	CString	m_cost;
	int		m_rating;
	CString	m_time;
	int		m_type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeOrderClass();
	afx_msg void OnSelchangeOrderRating();
	afx_msg void OnSelchangeOrderType();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void FillInClassList();
	void FillInRatingList();
	void FillInMisc();

	int m_nPrice;
	int m_nTime;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERDLG_H__C3679BF1_9602_11D4_82BD_00104B751D91__INCLUDED_)
