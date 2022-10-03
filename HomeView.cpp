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

// HomeView.cpp: implementation of the CHomeView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "HomeView.h"
#include "MatrixView.h"
#include "Character.h"
#include "OptionsDlg.h"
#include "CharDataDialog.h"
#include "DeckDataDialog.h"
#include "ContractListDialog.h"
#include "ContractDataDialog.h"
#include "Shop.h"
#include "System.h"
#include "Node.h"
#include "Global.h"
#include "Program.h"
#include "RestDlg.h"
#include "ProjectDataDlg.h"
#include "IntroDlg.h"
#include "DeckerSound.h"
#include "DeckerGraphics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

// Screen Size
#define SCREEN_CX 214	// Screen width
#define SCREEN_CY 232	// Screen height

// Button locations
static const int g_iButtonCoord[HOME_BUTTON_COUNT][2] = {
	{9,9},{9,36},{9,63},{9,90},{9,117},{9,144},{9,171},{9,198}
	};

static const int g_iButtonBitmapID[HOME_BUTTON_COUNT][2] = {
	{GR_HOME_CHAR0,		GR_HOME_CHAR1},
	{GR_HOME_DECK0,		GR_HOME_DECK1},
	{GR_HOME_CONTRACT0,	GR_HOME_CONTRACT1},
	{GR_HOME_SHOP0,		GR_HOME_SHOP1},
	{GR_HOME_PROJECT0,	GR_HOME_PROJECT1},
	{GR_HOME_REST0,		GR_HOME_REST1},
	{GR_HOME_MATRIX0,	GR_HOME_MATRIX1},
	{GR_HOME_OPTIONS0,	GR_HOME_OPTIONS1},
};

static const int g_iButtonID[HOME_BUTTON_COUNT] = {
	IDC_HOME_CHAR,
	IDC_HOME_DECK,
	IDC_HOME_CONTRACT,
	IDC_HOME_BUY,
	IDC_HOME_PROGRAM,
	IDC_HOME_REST,
	IDC_HOME_MATRIX,
	IDC_OPTIONS,
};

// Button size (height and width)
#define BUTTON_SIZE 24

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHomeView::CHomeView()
{
}

CHomeView::~CHomeView()
{
}

BEGIN_MESSAGE_MAP(CHomeView,CWnd )
	//{{AFX_MSG_MAP(CHomeView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_HOME_CHAR, OnViewChar)
	ON_BN_CLICKED(IDC_HOME_CONTRACT, OnContract)
	ON_BN_CLICKED(IDC_HOME_BUY, OnBuy)
	ON_BN_CLICKED(IDC_HOME_PROGRAM, OnProgram)
	ON_BN_CLICKED(IDC_HOME_REST, OnRest)
	ON_BN_CLICKED(IDC_HOME_DECK, OnDeck)
	ON_BN_CLICKED(IDC_HOME_MATRIX, OnMatrix)
	ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
	ON_WM_HELPINFO()
	ON_MESSAGE(WM_NEW_GAME, OnNewGame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHomeView message handlers

BOOL CHomeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.style |= WS_CAPTION;
	//cs.style |= WS_POPUP;
	//cs.style &= ~WS_BORDER;
	cs.style |= (WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX);
	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//FSO 3-29-04 Fixing this to behave better on other systems
	cs.cx = SCREEN_CX + 2*GetSystemMetrics(SM_CXFIXEDFRAME);
	cs.cy = SCREEN_CY + 2*GetSystemMetrics(SM_CYFIXEDFRAME)
					  + GetSystemMetrics(SM_CYCAPTION);
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CHomeView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Draw the background bitmap
	CPoint ptPoint(0,0);
	CSize szSize(640,480);
	dc.DrawState(ptPoint,szSize,g_hBitmaps[GR_HOME_MAIN],DST_BITMAP,DSS_NORMAL);


//	//TEST
//	MSG msg;
//	msg.hwnd = this->m_bmbButton[0].m_hWnd; 
//	msg.lParam = 0;
//	msg.wParam = 0;
//	msg.message = 0x1f;
//	MSG *pMsg = &msg;
//	HWND hWndStop = this->m_hWnd;
//
//	for (HWND hWnd = pMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
//	{
//		CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);
//		if (pWnd != NULL)
//		{
//			// target window is a C++ window
//			if (pWnd->PreTranslateMessage(pMsg))
//				return; // trapped by target window (eg: accelerators)
//		}
//
//		// got to hWndStop window without interest
//		if (hWnd == hWndStop)
//			break;
//	}
}

int CHomeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);

	// Create the bitmap buttons
	int i;
	RECT rect;
	for (i=0; i<HOME_BUTTON_COUNT; i++)
	{
		rect.left = g_iButtonCoord[i][0];
		rect.top  = g_iButtonCoord[i][1];
		rect.right = rect.left + BUTTON_SIZE - 1;
		rect.bottom = rect.top + BUTTON_SIZE - 1;
		m_bmbButton[i].Create(NULL,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,g_iButtonID[i]);
		m_bmbButton[i].SetBitmaps(	g_hBitmaps[g_iButtonBitmapID[i][0]],
									g_hBitmaps[g_iButtonBitmapID[i][1]]);
	}

	// Center the window
	CenterWindow();

	// If we have loaded a game and are on a run, go to the matrix
	if (g_pChar->m_bOnRun)
	{
		// Create the matrix view window
		CMatrixView *pvMatrix = new CMatrixView;
		pvMatrix->CreateEx(0,AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),
			"Decker - The Matrix",0,0,100,100,0,NULL/*m_hWnd*/,(HMENU)0);
		pvMatrix->CenterWindow();
		pvMatrix->ShowWindow(SW_SHOW);
		pvMatrix->UpdateWindow();

		// Hide this window
		ShowWindow(SW_HIDE);
	}
	else
	{
		ShowWindow(SW_SHOW);
	}

	return 0;
}

void CHomeView::OnViewChar()
{
	CCharDataDialog dlgChar;

	if (dlgChar.DoModal()==IDC_CHAR_VIEW_DECK)
	{
		// Switch to deck view
		OnDeck();
	}
}

void CHomeView::OnContract()
{
	if (g_pChar->m_pCurrentContract==NULL)
	{
		// No contract selected
		CContractListDialog dlgContractList;

		dlgContractList.DoModal();
	}
	else
	{
		// Contract selected
		CContractDataDialog dlgContract;

		dlgContract.m_pContract = g_pChar->m_pCurrentContract;
		dlgContract.DoModal();
	}
}

void CHomeView::OnBuy()
{
	CShop dlgShop;

	dlgShop.DoModal();
}

void CHomeView::OnProgram()
{
	CProjectDataDlg dlgProject;

	dlgProject.DoModal();
}

void CHomeView::OnRest()
{
	CRestDlg dlgRest;
	dlgRest.DoModal();

//Test
//	g_pChar->PassTime(32);
}

void CHomeView::OnDeck()
{
	CDeckDataDialog dlgDeck;

	if (dlgDeck.DoModal()==IDC_DECK_VIEW_CHAR)
	{
		// Switch to character view
		OnViewChar();
	}
}

void CHomeView::OnMatrix()
{
	// Make sure we have a contract
	if (g_pChar->m_pCurrentContract==NULL)
	{
		DeckerMessageBox("No contract has been accepted.");
		return;
	}

	// Play the sound effect
	PlayGameSound(SOUND_ENTERMATRIX);

	// FSO 7-24-01
	// Generate the system for this contract
	g_pChar->m_pSystem = new CSystem();
	g_pChar->m_pSystem->Generate(g_pChar->m_pCurrentContract->m_nCorporation, g_pChar->m_pCurrentContract->m_nSystemSize,0);

	// Reset some run-time data
	g_pChar->m_dwRunFlags = 0;
	g_pChar->m_nHealthMental = MAX_HEALTH;
	g_pChar->m_nHealthDeck = MAX_HEALTH;
	g_pChar->m_pTraceIce = NULL;
	g_pChar->m_nTraceInitialTime = 0;
	g_pChar->m_nTraceTurnsLeft = 0;
	g_pChar->m_pTargettedIce = NULL;
	g_pChar->m_pTraceIce = NULL;
	g_pChar->m_bTraced = FALSE;
	g_pChar->m_pTransferProgram = NULL;
	g_pChar->m_pTransferFile = NULL;
	g_pChar->m_pActiveScan = NULL;
	g_pChar->m_pActiveBoost = NULL;
	g_pChar->m_nRunTime = 0;
	g_pChar->m_nDecoyCount = 0;
	g_pChar->m_nDamageMental = 0;
	g_pChar->m_nDamageDeck = 0;

	// Verify that the default attack program is loaded
	if (g_pChar->m_pDefAttackProgram != NULL)
	{
		g_pChar->m_pDefAttackProgram->m_bLoadByDefault = TRUE;
	}

	// Load the default programs
	g_pChar->CalcCurrentLoad();

	// Set the active programs
	g_pChar->m_pActiveArmor = g_pChar->m_pDefArmorProgram;
	g_pChar->m_pActiveShield = g_pChar->m_pDefShieldProgram;
	g_pChar->m_pActiveHide = g_pChar->m_pDefHideProgram;
	g_pChar->m_pActiveReflect = g_pChar->m_pDefReflectProgram;

	// Get the entry node
	CNode *pEntryNode = g_pChar->m_pSystem->m_pSystemPortalOut;

	if (g_pChar->m_bBackdoor[g_pChar->m_pCurrentContract->m_nCorporation])
	{
		if (AfxMessageBox("You have a backdoor to this system. Use it?",MB_YESNO)==IDYES)
		{
			pEntryNode = g_pChar->m_pSystem->m_pSystemCPU;
		}
	}

	// Set current node so map view won't crash
	g_pChar->m_pCurrentNode = pEntryNode;

	// Put the user in the matrix
	g_pChar->m_bOnRun = TRUE;

	// Create the matrix view window
	CMatrixView *pvMatrix = new CMatrixView;
	pvMatrix->CreateEx(0,AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),
		"Decker - The Matrix",0,0,100,100,0,NULL/*m_hWnd*/,(HMENU)0);

	// Hide this window (do this after the matrix window creation, as it taks some time)
	ShowWindow(SW_HIDE);

	// Show the matrix window
	pvMatrix->CenterWindow();
	pvMatrix->ShowWindow(SW_SHOW);
	pvMatrix->UpdateWindow();
	pvMatrix->DoEnterNode(pEntryNode, DIR_CENTER);
}

void CHomeView::OnOptions()
{
	COptionsDlg dlgOptions;

	if (dlgOptions.DoModal()==IDOK)
	{
		// Game has been loaded
		if (g_pChar->m_bOnRun)
		{
			// Create the matrix view window
			CMatrixView *pvMatrix = new CMatrixView;
			pvMatrix->CreateEx(0,AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),
				"Decker - The Matrix",0,0,100,100,0,NULL/*m_hWnd*/,(HMENU)0);
			pvMatrix->CenterWindow();
			pvMatrix->ShowWindow(SW_SHOW);
			pvMatrix->UpdateWindow();

			// Hide this window
			ShowWindow(SW_HIDE);
		}
	}
}



BOOL CHomeView::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_HOME);
	return TRUE;
}

void CHomeView::OnNewGame()
{
	CIntroDlg dlg;

	// Hide this window
	ShowWindow(SW_HIDE);

	// Delete the current character
	if (g_pChar)
	{
		delete g_pChar;
	}

	// Create the character
	g_pChar = new CCharacter;

	// Show the intro dialog
	if (dlg.DoModal() == IDCANCEL)
	{
		// User said quit
		delete g_pChar;
		g_pChar = NULL;
		PostQuitMessage(0);
	}
	else
	{
		// User said new/load
		// Show this window
		ShowWindow(SW_SHOW);
	}
}


