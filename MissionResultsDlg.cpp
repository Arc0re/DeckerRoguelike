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

// MissionResultsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "MissionResultsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMissionResultsDlg dialog


CMissionResultsDlg::CMissionResultsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMissionResultsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMissionResultsDlg)
	m_contract = _T("");
	m_files = _T("");
	m_pay = _T("");
	m_rep = _T("");
	m_result = _T("");
	m_skill_pts = _T("");
	m_summary = _T("");
	m_total = _T("");
	//}}AFX_DATA_INIT
}


void CMissionResultsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMissionResultsDlg)
	DDX_Text(pDX, IDC_MR_CONTRACT, m_contract);
	DDX_Text(pDX, IDC_MR_FILES, m_files);
	DDX_Text(pDX, IDC_MR_PAY, m_pay);
	DDX_Text(pDX, IDC_MR_REP, m_rep);
	DDX_Text(pDX, IDC_MR_RESULT, m_result);
	DDX_Text(pDX, IDC_MR_SKILL_PTS, m_skill_pts);
	DDX_Text(pDX, IDC_MR_SUMMARY, m_summary);
	DDX_Text(pDX, IDC_MR_TOTAL, m_total);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMissionResultsDlg, CDialog)
	//{{AFX_MSG_MAP(CMissionResultsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMissionResultsDlg message handlers

BOOL CMissionResultsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
