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

// OptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "OptionsDlg.h"
#include "Character.h"
#include "Global.h"
#include "NewCharDlg.h"
#include "DeckerSound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog


COptionsDlg::COptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionsDlg)
	m_tooltips = g_pChar->m_bTooltips;
	m_sound = g_bSound;
	//}}AFX_DATA_INIT
}


void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsDlg)
	DDX_Check(pDX, IDC_OPTIONS_TOOLTIPS, m_tooltips);
	DDX_Check(pDX, IDC_OPTIONS_SOUND, m_sound);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsDlg, CDialog)
	//{{AFX_MSG_MAP(COptionsDlg)
	ON_BN_CLICKED(IDC_OPTIONS_LOAD, OnOptionsLoad)
	ON_BN_CLICKED(IDC_OPTIONS_QUIT, OnOptionsQuit)
	ON_BN_CLICKED(IDC_OPTIONS_SAVE, OnOptionsSave)
	ON_BN_CLICKED(IDC_OPTIONS_SAVEAS, OnOptionsSaveas)
	ON_BN_CLICKED(IDC_OPTIONS_NEW, OnOptionsNew)
	ON_BN_CLICKED(IDC_OPTIONS_QUICK_LOAD, OnOptionsQuickLoad)
	ON_BN_CLICKED(IDC_OPTIONS_TOOLTIPS, OnOptionsTooltips)
	ON_BN_CLICKED(IDC_OPTIONS_SOUND, OnOptionsSound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg message handlers

void COptionsDlg::OnOptionsQuickLoad() 
{
	CCharacter *pNewChar;

	if (DeckerMessageBox("Are you sure you want to lose the current game?",MB_YESNO)==IDYES)
	{
		// Create a new, temporary character
		pNewChar = new CCharacter();
		if (pNewChar->LoadGame(this, g_pChar->m_szFileName))
		{
			DeckerMessageBox("The game has been loaded.");

			// Game was loaded. Delete the old character
			delete g_pChar;

			// Set the global var to the new character
			g_pChar = pNewChar;

			CDialog::OnOK();
		}
	}
}

void COptionsDlg::OnOptionsLoad() 
{
	CCharacter *pNewChar;

	if (DeckerMessageBox("Are you sure you want to lose the current game?",MB_YESNO)==IDYES)
	{
		// Create a new, temporary character
		pNewChar = new CCharacter();
		if (pNewChar->LoadGame(this))
		{
			DeckerMessageBox("The game has been loaded.");

			// Game was loaded. Delete the old character
			delete g_pChar;

			// Set the global var to the new character
			g_pChar = pNewChar;

			CDialog::OnOK();
		}
	}
}

void COptionsDlg::OnOptionsQuit() 
{
	if (DeckerMessageBox("Are you sure you wish to quit?",MB_YESNO)==IDYES)
	{
		// Set tooltips to disable when this dialog exits
		g_pChar->m_bTooltips = FALSE;

		if (g_pChar->m_bOnRun)
		{
			GetParent()->SendMessage(WM_CLOSE);
		}
		PostQuitMessage(0);
	}
}

void COptionsDlg::OnOptionsSave() 
{
	if ((g_pChar->m_bIronMan) && (g_pChar->m_bOnRun))
	{
		DeckerMessageBox("Ironmen don't need to save in the matrix.");
		return;
	}

	// Save
	if (g_pChar->SaveGame(this))
	{
		DeckerMessageBox("The game has been saved.");
	}
}

void COptionsDlg::OnOK() 
{
	// Disable this in case use hits Enter
}

void COptionsDlg::OnOptionsSaveas() 
{
	if ((g_pChar->m_bIronMan) && (g_pChar->m_bOnRun))
	{
		DeckerMessageBox("Ironmen don't need to save in the matrix.");
		return;
	}

	// Save As...
	if (g_pChar->SaveGame(this, TRUE))
	{
		DeckerMessageBox("The game has been saved.");
	}
}

void COptionsDlg::OnOptionsNew() 
{
	CNewCharDlg dlgNewChar;

	if (dlgNewChar.DoModal()==IDOK)
	{
		// Delete the current character
		delete g_pChar;

		// Create a new character
		g_pChar = new CCharacter;

		// Initialize it
		g_pChar->CreateNew(dlgNewChar.m_bonus);
		g_pChar->m_szName = dlgNewChar.m_name;
		g_pChar->m_nImage = dlgNewChar.m_nImage;
		g_pChar->m_bIronMan = dlgNewChar.m_ironman;
		g_pChar->m_bTooltips = dlgNewChar.m_tooltips;

		CDialog::OnOK();
	}
}


void COptionsDlg::OnOptionsTooltips() 
{
	UpdateData(TRUE);

	g_pChar->m_bTooltips = m_tooltips;
}

void COptionsDlg::OnOptionsSound() 
{
	UpdateData(TRUE);

	g_bSound = m_sound;
}
