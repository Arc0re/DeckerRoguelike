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

// Decker.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Decker.h"
#include "IntroDlg.h"
#include "HomeView.h"
#include "Character.h"
#include "Ice.h"
#include "DeckerSound.h"
#include "DeckerGraphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
CCharacter *g_pChar = NULL;


/////////////////////////////////////////////////////////////////////////////
// CDeckerApp

BEGIN_MESSAGE_MAP(CDeckerApp, CWinApp)
	//{{AFX_MSG_MAP(CDeckerApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckerApp construction

CDeckerApp::CDeckerApp()
{
	m_pHomeView = NULL;

	// Initialize the random number generator
	srand(GetTickCount());
	//srand(0x432cc1);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDeckerApp object

CDeckerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDeckerApp initialization

BOOL CDeckerApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	DWORD dwRC;
	HKEY hKeyValue;
	DWORD dwType, dwSize;
	char szDirectory[MAX_PATH];

	// Get the install directory
	dwRC = RegOpenKeyEx(	HKEY_LOCAL_MACHINE, 
							"Software\\Decker",
							0,
							KEY_READ,
  							&hKeyValue);

	if (dwRC == ERROR_SUCCESS)
	{
		/* Read the value */
		dwSize = sizeof(szDirectory);
	
		dwRC = RegQueryValueEx(	hKeyValue,
								"Directory",
								NULL,
  								&dwType,
  								(BYTE *)szDirectory,
  								&dwSize);
		RegCloseKey(hKeyValue);

		if ((dwRC != ERROR_SUCCESS) && (dwType!=REG_SZ))
		{
			strcpy(szDirectory, ".");
		}
	}
	else
	{
		strcpy(szDirectory, ".");
	}

	// Load graphics
	if (!LoadGraphics(szDirectory))
	{
		return FALSE;
	}

	// Load sounds
	LoadSounds(szDirectory);

	// Create the character
	g_pChar = new CCharacter;

	// Show the intro dialog
	CIntroDlg dlg;

	if (dlg.DoModal() == IDCANCEL)
	{
		// Since the dialog has been closed, return FALSE so that we exit the
		//  application, rather than start the application's message pump.
		delete g_pChar;
		g_pChar = NULL;
		return FALSE;
	}

	// Create and open the home window
	m_pHomeView = new CHomeView;
	m_pHomeView->CreateEx(0,AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),
		"Decker - At Home",0,0,0,100,100,NULL,0);

	m_pMainWnd = m_pHomeView;
	//m_pMainWnd->CenterWindow();
	//m_pMainWnd->ShowWindow(SW_SHOW);
	//m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CDeckerApp::ExitInstance() 
{
	if (m_pHomeView!=NULL)
	{
		delete m_pHomeView;
	}
	if (g_pChar!=NULL)
	{
		delete g_pChar;
	}

	// Unload sounds
	UnloadSounds();

	UnloadGraphics();

	return CWinApp::ExitInstance();
}

