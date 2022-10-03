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

// EntryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "EntryDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntryDlg dialog


CEntryDlg::CEntryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEntryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntryDlg)
	m_text = _T("");
	//}}AFX_DATA_INIT
}


void CEntryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntryDlg)
	DDX_Text(pDX, IDC_TEXT, m_text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntryDlg, CDialog)
	//{{AFX_MSG_MAP(CEntryDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntryDlg message handlers

void CEntryDlg::OnOK() 
{
	UpdateData(TRUE);

	if (m_text.IsEmpty())
	{
		DeckerMessageBox("Please enter a name.");
		return;
	}
	
	CDialog::OnOK();
}

BOOL CEntryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
