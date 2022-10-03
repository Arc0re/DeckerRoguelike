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

// BuildDialog.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "BuildDialog.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBuildDialog dialog


CBuildDialog::CBuildDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBuildDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBuildDialog)
	m_load = TRUE;
	m_text = _T("");
	m_trash = TRUE;
	m_unload = FALSE;
	//}}AFX_DATA_INIT

	m_bAllowLoad = TRUE;
}


void CBuildDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBuildDialog)
	DDX_Control(pDX, IDC_BUILD_UNLOAD, m_Unload);
	DDX_Control(pDX, IDC_BUILD_TRASH, m_Trash);
	DDX_Control(pDX, IDC_BUILD_LOAD, m_Load);
	DDX_Check(pDX, IDC_BUILD_LOAD, m_load);
	DDX_Text(pDX, IDC_BUILD_TEXT, m_text);
	DDX_Check(pDX, IDC_BUILD_TRASH, m_trash);
	DDX_Check(pDX, IDC_BUILD_UNLOAD, m_unload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBuildDialog, CDialog)
	//{{AFX_MSG_MAP(CBuildDialog)
	ON_BN_CLICKED(IDC_BUILD_TRASH, OnBuildTrash)
	ON_BN_CLICKED(IDC_BUILD_UNLOAD, OnBuildUnload)
	ON_BN_CLICKED(IDC_BUILD_LOAD, OnBuildLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuildDialog message handlers

BOOL CBuildDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (!m_bAllowLoad)
	{
		m_load = FALSE;
		m_unload = FALSE;
		m_trash = FALSE;
		m_Load.EnableWindow(FALSE);
		m_Unload.EnableWindow(FALSE);
		m_Trash.EnableWindow(FALSE);
	}
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBuildDialog::OnBuildLoad() 
{
	UpdateData(TRUE);

	if (m_load)
	{
		m_Unload.EnableWindow(TRUE);
		m_Trash.EnableWindow(TRUE);
	}
	else
	{
		m_unload = FALSE;
		m_trash = FALSE;
		UpdateData(FALSE);
		m_Unload.EnableWindow(FALSE);
		m_Trash.EnableWindow(FALSE);
	}
}

void CBuildDialog::OnBuildTrash() 
{
	UpdateData(TRUE);

	if (m_trash)
	{
		m_unload = FALSE;
		UpdateData(FALSE);
	}
}

void CBuildDialog::OnBuildUnload() 
{
	UpdateData(TRUE);

	if (m_unload)
	{
		m_trash = FALSE;
		UpdateData(FALSE);
	}
}

void CBuildDialog::OnOK() 
{
	UpdateData(TRUE);

	if (m_text.IsEmpty())
	{
		DeckerMessageBox("Please enter a name.");
		return;
	}
	
	CDialog::OnOK();
}

