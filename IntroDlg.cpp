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

// DeckerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Decker.h"
#include "IntroDlg.h"
#include "NewCharDlg.h"
#include "Character.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// CIntroDlg dialog

CIntroDlg::CIntroDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIntroDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIntroDlg)
	m_version = _T("");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIntroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIntroDlg)
	DDX_Text(pDX, IDC_VERSION, m_version);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIntroDlg, CDialog)
	//{{AFX_MSG_MAP(CIntroDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INTRO_LOAD, OnIntroLoad)
	ON_BN_CLICKED(IDC_INTRO_NEW, OnIntroNew)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIntroDlg message handlers

BOOL CIntroDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	m_version.Format("Version %u.%u",MAJOR_VERSION,MINOR_VERSION);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIntroDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CIntroDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIntroDlg::OnIntroLoad() 
{
	if (g_pChar->LoadGame(this))
	{
		//DeckerMessageBox("The game has been loaded.");

		CDialog::OnOK();
	}
}

void CIntroDlg::OnIntroNew() 
{
	CNewCharDlg dlgNewChar;

	if (dlgNewChar.DoModal()==IDOK)
	{
		// Create a new character
		g_pChar->CreateNew(dlgNewChar.m_bonus);
		g_pChar->m_szName = dlgNewChar.m_name;
		g_pChar->m_nImage = dlgNewChar.m_nImage;
		g_pChar->m_bIronMan = dlgNewChar.m_ironman;
		g_pChar->m_bTooltips = dlgNewChar.m_tooltips;

		CDialog::OnOK();
	}
}

void CIntroDlg::OnCancel() 
{
	CDialog::OnCancel();
}


BOOL CIntroDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_INTRO);
	return TRUE;
}
