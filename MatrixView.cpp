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

// MatrixView.cpp: implementation of the CMatrixView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "MatrixView.h"
#include "OptionsDlg.h"
#include "Character.h"
#include "System.h"
#include "Area.h"
#include "Node.h"
#include "MessageView.h"
#include "Global.h"
#include "Program.h"
#include "CharDataDialog.h"
#include "DeckDataDialog.h"
#include "ContractDataDialog.h"
#include "Ice.h"
#include "TransferDlg.h"
#include "DSFile.h"
#include "FileAccessDlg.h"
#include "Contract.h"
#include "IceDataDlg.h"
#include "MissionResultsDlg.h"
#include "HealthBar.h"
#include "Source.h"
#include "DeckerSound.h"
#include "DeckerGraphics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

//----------------------------
// General Screen Information
//----------------------------
// Screen Size
#define SCREEN_CX 640	// Screen width
#define SCREEN_CY 480	// Screen height

//------------------------
// Child Window Locations
//------------------------
// Node Window Location
#define NODE_X	194
#define NODE_Y	49

// Map Window Location
#define MAP_X	451
#define MAP_Y	12

// Map zoom button location
#define MAP_ZOOM_X 431
#define MAP_ZOOM_Y 10
#define MAP_ZOOM_CX 15
#define MAP_ZOOM_CY 24

// Software Window Location
#define SOFTWARE_X	12
#define SOFTWARE_Y	42
#define SOFTWARE_CX	165
#define SOFTWARE_CY	399

// Message Window Location
#define MSG_X		194
#define MSG_Y		367
#define MSG_SIZE_X	434
#define MSG_SIZE_Y	101

// Targetted ice name bar
#define TARGET_NAME_X	338
#define TARGET_NAME_Y	296
#define TARGET_NAME_CX	96
#define TARGET_NAME_CY	16

// Targetted ice picture location
#define TARGET_ICE_X	307
#define TARGET_ICE_Y	296

// Targetted ice status location
#define TARGET_ACTIVE_X	338
#define TARGET_ACTIVE_Y	319
#define TARGET_SLOW		0
#define TARGET_VIRUS	1
#define TARGET_CONF		2
#define TARGET_WEAK		3
#define TARGET_ANALYZED	4

// Node name bar
#define NODE_NAME_X		194
#define NODE_NAME_Y		296
#define NODE_NAME_CX	96
#define NODE_NAME_CY	16

// Node active programs bar
#define NODE_ACTIVE_X	194
#define NODE_ACTIVE_Y	319
#define NODE_SILENCE	0
#define NODE_SMOKE		1
#define NODE_SCAN		2
#define NODE_CLIENT		3

// Current alarm bar
#define ALARM_X			206
#define ALARM_Y			342
#define ALARM_CX		84
#define ALARM_CY		8

// Clock
#define CLOCK_X		132
#define CLOCK_Y		13
#define CLOCK_CX	44
#define CLOCK_CY	11

//--------------------
// Button Information
//--------------------
// Button array definitions
#define BUTTON_ATTACK			0
#define BUTTON_NORTH			1
#define BUTTON_DECEIVE			2
#define BUTTON_WEST				3
#define BUTTON_WAIT				4
#define BUTTON_EAST				5
#define BUTTON_VIEW_ICE			6
#define BUTTON_SOUTH			7
#define BUTTON_SCAN				8
#define BUTTON_CHAR				9
#define BUTTON_DECK				10
#define BUTTON_CONTRACT			11

#define BUTTON_DISCONNECT		12
#define BUTTON_OPTIONS			13

#define BUTTON_RUN_PROGRAM		14
#define BUTTON_SET_DEF_PROGRAM	15
#define BUTTON_LOAD_PROGRAM		16
#define BUTTON_UNLOAD_PROGRAM	17
//#define BUTTON_UNUSED			18
//#define BUTTON_UNUSED			19

#define BUTTON_GET_FILE			20
#define BUTTON_EDIT_FILE		21
#define BUTTON_ERASE_FILE		22
#define BUTTON_CRASH			23
#define BUTTON_USE_IO			24
#define BUTTON_GET_MAP			25
#define BUTTON_BACKDOOR			26
#define BUTTON_KILL_ALARM		27
#define BUTTON_ENTER_PORTAL		28
#define BUTTON_KILL_TRACE		29
#define BUTTON_KILL_SHUTDOWN	30

// Button locations
static const int g_iButtonCoord[MATRIX_BUTTON_COUNT][2] = {
	// Main control buttons (14)
	{439,193},	{466,193},	{493,193},				// Attack, North, Deceive
	{439,220},	{466,220},	{493,220},				// West, Wait, East
	{439,247},	{466,247},	{493,247},				// View Ice, South, Scan

	{439,274},	{466,274},	{493,274},				// Char, Deck, Contract
	{466,328},	{493,328},							// Disconnect, options
	// Program buttons (6)
	{10,446},	{37,446},	{64,446},	{91,446},	// Run, Default, Load, Unload
	{118,446},	{145,446},							// Unused

	// Node access buttons (11) These are on top of one another. Four slots.
	{439,301},	{466,301},	{493,301},				// Get, Edit, Erase (DS)
				{466,301},							// Crash (CPU), 
	{439,301},										// Use IO (IO)
	{439,301},										// Get Map (CPU/SPU)
										{439,328},	// Backdoor (CPU)
	{439,301},										// Kill Alarm (Sec)
	{439,301},										// Use Portal (PI/PO)
				{466,301},							// Kill Trace (Sec)
							{493,301},				// Kill Shutdown (CPU/SPU)
	};

// Button Bitmaps
static const int g_iButtonBitmapID[MATRIX_BUTTON_COUNT][3] = {
	// Main control buttons (18)
	{GR_MATRIX_ATTACK0,GR_MATRIX_ATTACK1,GR_MATRIX_ATTACK2},
	{GR_MATRIX_ARROW_N0,GR_MATRIX_ARROW_N1,GR_MATRIX_ARROW_N2},
	{GR_MATRIX_DECEIVE0,GR_MATRIX_DECEIVE1,GR_MATRIX_DECEIVE2},
	{GR_MATRIX_ARROW_W0,GR_MATRIX_ARROW_W1,GR_MATRIX_ARROW_W2},
	{GR_MATRIX_WAIT0,GR_MATRIX_WAIT1,GR_MATRIX_WAIT1},
	{GR_MATRIX_ARROW_E0,GR_MATRIX_ARROW_E1,GR_MATRIX_ARROW_E2},
	{GR_MATRIX_VIEW_ICE0,GR_MATRIX_VIEW_ICE1,GR_MATRIX_VIEW_ICE2},
	{GR_MATRIX_ARROW_S0,GR_MATRIX_ARROW_S1,GR_MATRIX_ARROW_S2},
	{GR_MATRIX_SCAN0,GR_MATRIX_SCAN1,GR_MATRIX_SCAN2}, // No disabled
	{GR_MATRIX_CHAR0,GR_MATRIX_CHAR1,GR_MATRIX_CHAR1}, // No disabled
	{GR_MATRIX_DECK0,GR_MATRIX_DECK1,GR_MATRIX_DECK1}, // No disabled
	{GR_MATRIX_CONTRACT0,GR_MATRIX_CONTRACT1,GR_MATRIX_CONTRACT1}, // No disabled
	{GR_MATRIX_DISCONNECT0,GR_MATRIX_DISCONNECT1,GR_MATRIX_DISCONNECT1}, // No disabled
	{GR_MATRIX_OPTIONS0,GR_MATRIX_OPTIONS1,GR_MATRIX_OPTIONS1}, // No disabled
	// Program buttons (6)
	{GR_MATRIX_RUN_PROGRAM0,GR_MATRIX_RUN_PROGRAM1,GR_MATRIX_RUN_PROGRAM2},
	{GR_MATRIX_SET_DEF0,GR_MATRIX_SET_DEF1,GR_MATRIX_SET_DEF2},
	{GR_MATRIX_LOAD0,GR_MATRIX_LOAD1,GR_MATRIX_LOAD2},
	{GR_MATRIX_UNLOAD0,GR_MATRIX_UNLOAD1,GR_MATRIX_UNLOAD2},
	{0,0,0},
	{0,0,0},
	// Node access buttons (11)
	{GR_MATRIX_GETFILE0,GR_MATRIX_GETFILE1,GR_MATRIX_GETFILE2},
	{GR_MATRIX_EDIT0,GR_MATRIX_EDIT1,GR_MATRIX_EDIT2},
	{GR_MATRIX_ERASE0,GR_MATRIX_ERASE1,GR_MATRIX_ERASE2},
	{GR_MATRIX_CRASH0,GR_MATRIX_CRASH1,GR_MATRIX_CRASH2},
	{GR_MATRIX_USE_IO0,GR_MATRIX_USE_IO1,GR_MATRIX_USE_IO2},
	{GR_MATRIX_GETMAP0,GR_MATRIX_GETMAP1,GR_MATRIX_GETMAP2},
	{GR_MATRIX_BACKDOOR0,GR_MATRIX_BACKDOOR1,GR_MATRIX_BACKDOOR2},
	{GR_MATRIX_KILL_ALARM0,GR_MATRIX_KILL_ALARM1,GR_MATRIX_KILL_ALARM2},
	{GR_MATRIX_ENTER_PORTAL0,GR_MATRIX_ENTER_PORTAL1,GR_MATRIX_ENTER_PORTAL2},
	{GR_MATRIX_KILL_TRACE0,GR_MATRIX_KILL_TRACE1,GR_MATRIX_KILL_TRACE2},
	{GR_MATRIX_KILL_SHUTDOWN0,GR_MATRIX_KILL_SHUTDOWN1,GR_MATRIX_KILL_SHUTDOWN2},
};

static const int g_iButtonID[MATRIX_BUTTON_COUNT] = {
	// Main control buttons (18)
	IDC_ATTACK,
	IDC_MATRIX_N,
	IDC_DECEIVE,
	IDC_MATRIX_W,
	IDC_WAIT1,
	IDC_MATRIX_E,
	//IDC_ANALYZE,
	IDC_VIEW_ICE,
	IDC_MATRIX_S,
	IDC_SCAN,
	IDC_MATRIX_CHAR,
	IDC_MATRIX_DECK,
	IDC_MATRIX_CONTRACT,
	IDC_MATRIX_DISCONNECT,
	IDC_OPTIONS,
	// Program buttons (6)
	IDC_RUN_PROGRAM,
	IDC_SET_DEF_PROGRAM,
	IDC_LOAD_PROGRAM,
	IDC_UNLOAD_PROGRAM,
	0,
	0,
	// Node access buttons (9)
	IDC_GET_FILE,
	IDC_EDIT_FILE,
	IDC_ERASE_FILE,
	IDC_CRASH,
	IDC_USE_IO,
	IDC_GET_MAP,
	IDC_BACKDOOR,
	IDC_KILL_ALARM,
	IDC_ENTER_PORTAL,
	IDC_KILL_TRACE,
	IDC_KILL_SHUTDOWN,
};

// Button size (height and width)
#define BUTTON_SIZE 24

//------------------------
// Health Bar Information
//------------------------
// Bar locations
static const int g_nBarCoord[NUM_BARS][2] =
{
	{548,195},	// Deck health
	{548,220},	// Mental health
	{548,234},	// Physical health
	{548,259},	// Shield
	{548,284},	// Transfer
	{548,309},	// Trace
	{354,342},	// Ice health
};

static const int g_nBarIDs[NUM_BARS] = 
{
	IDC_HEALTHBAR_DECK,
	IDC_HEALTHBAR_MENTAL,
	IDC_HEALTHBAR_LETHAL,
	IDC_HEALTHBAR_SHIELD,
	IDC_HEALTHBAR_TRANSFER,
	IDC_HEALTHBAR_TRACE,
	IDC_HEALTHBAR_ICE,
};

// Covers
#define BIO_COVER_X		525
#define BIO_COVER_Y		213
#define BIO_COVER_CX	110
#define BIO_COVER_CY	36
#define TRACE_COVER_X	525
#define TRACE_COVER_Y	302
#define TRACE_COVER_CX	110
#define TRACE_COVER_CY	22

//------------------------
// Active Bar Information
//------------------------
#define CHAR_ACTIVE_X	532
#define CHAR_ACTIVE_Y	334
#define CHAR_ARMOR		0
#define CHAR_SHIELD		1
#define CHAR_HIDE		2
#define CHAR_REFLECT	3
#define CHAR_BOOST		4

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatrixView::CMatrixView()
{
	m_ilSoftware.Create(IDB_SOFTWARE_IL,16,0,WHITE);

	m_hAccel = LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MATRIX_ACC));
}

CMatrixView::~CMatrixView()
{
}

BOOL CMatrixView::PreTranslateMessage(MSG* pMsg)
{
	if (!TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
	{
		return CWnd::PreTranslateMessage(pMsg);
	}
	return TRUE;
}


BEGIN_MESSAGE_MAP(CMatrixView,CWnd )
	//{{AFX_MSG_MAP(CMatrixView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MATRIX_N, OnMoveN)
	ON_BN_CLICKED(IDC_MATRIX_E, OnMoveE)
	ON_BN_CLICKED(IDC_MATRIX_S, OnMoveS)
	ON_BN_CLICKED(IDC_MATRIX_W, OnMoveW)
	ON_BN_CLICKED(IDC_MATRIX_DISCONNECT, OnDisconnect)
	ON_BN_CLICKED(IDC_MAP_ZOOM, OnMapZoom)
	ON_BN_CLICKED(IDC_ATTACK, OnAttack)
	ON_BN_CLICKED(IDC_DECEIVE, OnDeceive)
	ON_BN_CLICKED(IDC_WAIT1, OnWait)
	ON_BN_CLICKED(IDC_ANALYZE, OnAnalyze)
	ON_BN_CLICKED(IDC_RUN_PROGRAM, OnRunProgram)
	ON_BN_CLICKED(IDC_SET_DEF_PROGRAM, OnSetDefProgram)
	ON_BN_CLICKED(IDC_LOAD_PROGRAM, OnLoadProgram)
	ON_BN_CLICKED(IDC_UNLOAD_PROGRAM, OnUnloadProgram)
	ON_BN_CLICKED(IDC_GET_FILE, OnGetFile)
	ON_BN_CLICKED(IDC_EDIT_FILE, OnEditFile)
	ON_BN_CLICKED(IDC_ERASE_FILE, OnEraseFile)
	ON_BN_CLICKED(IDC_USE_IO, OnUseIO)
	ON_BN_CLICKED(IDC_CRASH, OnCrash)
	ON_BN_CLICKED(IDC_GET_MAP, OnGetMap)
	ON_BN_CLICKED(IDC_BACKDOOR, OnBackdoor)
	ON_BN_CLICKED(IDC_KILL_ALARM, OnKillAlarm)
	ON_BN_CLICKED(IDC_KILL_TRACE, OnKillTrace)
	ON_BN_CLICKED(IDC_KILL_SHUTDOWN, OnKillShutDown)
	ON_BN_CLICKED(IDC_ENTER_PORTAL, OnEnterPortal)
	ON_BN_CLICKED(IDC_MATRIX_CHAR, OnViewChar)
	ON_BN_CLICKED(IDC_MATRIX_DECK, OnViewDeck)
	ON_BN_CLICKED(IDC_MATRIX_CONTRACT, OnViewContract)
	ON_BN_CLICKED(IDC_VIEW_ICE, OnViewIce)
	ON_BN_CLICKED(IDC_SCAN, OnScan)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SOFTWARE_LIST, OnItemchangedSoftwareView)
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(NM_DBLCLK, IDC_SOFTWARE_LIST, OnDblclkProgramList)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_QUICK_LOAD_KEY, OnQuickLoad)
	ON_BN_CLICKED(IDC_QUICK_SAVE_KEY, OnQuickSave)
	ON_BN_CLICKED(IDC_SILENCE, OnSilence)
	ON_BN_CLICKED(IDC_EVALUATE, OnEvaluate)
	ON_BN_CLICKED(IDC_SMOKE, OnSmoke)
	ON_BN_CLICKED(IDC_MEDIC, OnMedic)
	ON_BN_CLICKED(IDC_RELOCATE, OnRelocate)
	ON_BN_CLICKED(IDC_DECRYPT, OnDecrypt)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)
	ON_COMMAND(IDC_NEXT_TARGET, OnNextTarget)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatrixView message handlers

BOOL CMatrixView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	//cs.style |= WS_CAPTION;
	//cs.style |= WS_POPUP;
	//cs.style &= ~WS_BORDER;
	cs.style |= (WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX);
	//FSO 3-29-04 Fixing this to behave better on other systems
	cs.cx = SCREEN_CX + 2*GetSystemMetrics(SM_CXFIXEDFRAME);
	cs.cy = SCREEN_CY + 2*GetSystemMetrics(SM_CYFIXEDFRAME)
					  + GetSystemMetrics(SM_CYCAPTION);
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CMatrixView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);

	// Create the background DC
	CDC *pDC = GetDC();
	m_dcBackground.CreateCompatibleDC(pDC);
	ReleaseDC(pDC);

	//---------------------------
	// Create the bitmap buttons
	//---------------------------
	// Create the main control panel
	int i;
	RECT rect;
	for (i=0; i<MATRIX_BUTTON_COUNT; i++)
	{
		if (g_iButtonID[i]==0) continue;

		rect.left = g_iButtonCoord[i][0];
		rect.top  = g_iButtonCoord[i][1];
		rect.right = rect.left + BUTTON_SIZE - 1;
		rect.bottom = rect.top + BUTTON_SIZE - 1;
		m_bmbButton[i].Create(NULL,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,g_iButtonID[i]);
		m_bmbButton[i].SetBitmaps(	g_hBitmaps[g_iButtonBitmapID[i][0]],
									g_hBitmaps[g_iButtonBitmapID[i][1]],
									NULL,
									g_hBitmaps[g_iButtonBitmapID[i][2]]);
	}

	// Create the map zoom button
	rect.left = MAP_ZOOM_X;
	rect.top = MAP_ZOOM_Y;
	rect.right = rect.left + MAP_ZOOM_CX - 1;
	rect.bottom = rect.top + MAP_ZOOM_CY - 1;
	m_bmbMapZoom.Create(NULL,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,IDC_MAP_ZOOM);
	m_bmbMapZoom.SetBitmaps(g_hBitmaps[GR_MATRIX_MAP_ZOOM0],
							g_hBitmaps[GR_MATRIX_MAP_ZOOM1]);

	//------------------------
	// Create the health bars
	//------------------------
	for (i=0; i<NUM_BARS; i++)
	{
		rect.left = g_nBarCoord[i][0];
		rect.top = g_nBarCoord[i][1];
		rect.right = rect.left + BAR_CX - 1;
		rect.bottom = rect.top + BAR_CY - 1;
		m_hbHealthBar[i].m_nCurrentVal = 0;
		m_hbHealthBar[i].m_nMaxVal = 20;
		m_hbHealthBar[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rect,this,g_nBarIDs[i]);
	}

	//-------------------------
	// Create the name windows
	//-------------------------
	rect.left = TARGET_NAME_X;
	rect.right = TARGET_NAME_X + TARGET_NAME_CX;
	rect.top = TARGET_NAME_Y;
	rect.bottom = TARGET_NAME_Y + TARGET_NAME_CY;
	m_nbIceName.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_TARGET_NAME);

	rect.left = NODE_NAME_X;
	rect.right = NODE_NAME_X + NODE_NAME_CX;
	rect.top = NODE_NAME_Y;
	rect.bottom = NODE_NAME_Y + NODE_NAME_CY;
	m_nbNodeName.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_NODE_NAME);

	//---------------------------------
	// Create the active programs bars
	//---------------------------------
	m_abActiveChar.m_pImageList = &m_ilSoftware;
	m_abActiveChar.m_nImageIndex[CHAR_ARMOR] = PROGRAM_ARMOR;
	m_abActiveChar.m_nImageIndex[CHAR_SHIELD] = PROGRAM_SHIELD;
	m_abActiveChar.m_nImageIndex[CHAR_HIDE] = PROGRAM_HIDE;
	m_abActiveChar.m_nImageIndex[CHAR_REFLECT] = PROGRAM_REFLECT;
	rect.left = rect.right = CHAR_ACTIVE_X;
	rect.top = rect.bottom = CHAR_ACTIVE_Y;
	m_abActiveChar.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_ACTIVE_CHAR);

	m_abActiveIce.m_pImageList = &m_ilSoftware;
	m_abActiveIce.m_nImageIndex[TARGET_SLOW] = PROGRAM_SLOW;
	m_abActiveIce.m_nImageIndex[TARGET_VIRUS] = PROGRAM_VIRUS;
	m_abActiveIce.m_nImageIndex[TARGET_CONF] = PROGRAM_CONFUSE;
	m_abActiveIce.m_nImageIndex[TARGET_WEAK] = PROGRAM_WEAKEN;
	m_abActiveIce.m_nImageIndex[TARGET_ANALYZED] = PROGRAM_ANALYZE;
	rect.left = rect.right = TARGET_ACTIVE_X;
	rect.top = rect.bottom = TARGET_ACTIVE_Y;
	m_abActiveIce.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_ACTIVE_ICE);

	m_abActiveNode.m_pImageList = &m_ilSoftware;
	m_abActiveNode.m_nImageIndex[NODE_SILENCE] = PROGRAM_SILENCE;
	m_abActiveNode.m_nImageIndex[NODE_SMOKE] = PROGRAM_SMOKE;
	m_abActiveNode.m_nImageIndex[NODE_SCAN] = PROGRAM_SCAN;
	m_abActiveNode.m_nImageIndex[NODE_CLIENT] = PROGRAM_CLIENT;
	rect.left = rect.right = NODE_ACTIVE_X;
	rect.top = rect.bottom = NODE_ACTIVE_Y;
	m_abActiveNode.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_ACTIVE_NODE);

	//-------------------------
	// Create the view windows
	//-------------------------
	// Create the node view window
	m_NodeView.CreateEx(0,AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),
		NULL,WS_VISIBLE|WS_CHILD,NODE_X,NODE_Y,NODE_SIZE,NODE_SIZE,m_hWnd,(HMENU)IDC_NODE_VIEW);
	m_NodeView.ShowWindow(SW_SHOW);
	m_NodeView.UpdateWindow();

	// Create the map view window
	m_MapView.CreateEx(0,AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),
		NULL,WS_VISIBLE|WS_CHILD|WS_HSCROLL|WS_VSCROLL,MAP_X,MAP_Y,MAP_SIZE_X,MAP_SIZE_Y,m_hWnd,(HMENU)IDC_MAP_VIEW);
	m_MapView.ShowWindow(SW_SHOW);
	m_MapView.UpdateWindow();

	// Create the message view
	m_MessageView.CreateEx(0,AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),
		NULL,WS_VISIBLE|WS_CHILD|WS_HSCROLL|WS_VSCROLL,MSG_X,MSG_Y,MSG_SIZE_X,MSG_SIZE_Y,m_hWnd,(HMENU)IDC_MESSAGE_VIEW);
	m_MessageView.ShowWindow(SW_SHOW);
	m_MessageView.UpdateWindow();

	// Create the software view
	rect.left = SOFTWARE_X;
	rect.top = SOFTWARE_Y;
	rect.right = rect.left + SOFTWARE_CX /*- 1*/;
	rect.bottom = rect.top + SOFTWARE_CY /*- 1*/;
	m_SoftwareView.Create(LVS_ALIGNLEFT|LVS_NOSORTHEADER|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL,
		rect,this,IDC_SOFTWARE_LIST);
	m_SoftwareView.ShowWindow(SW_SHOW);

	// Create the columns
	m_SoftwareView.InsertColumn(0,"Program",LVCFMT_LEFT,110);
	m_SoftwareView.InsertColumn(1,"Rating",LVCFMT_LEFT,50);

	// Set the image list
	m_SoftwareView.SetImageList(&m_ilSoftware,LVSIL_SMALL);

	//---------------------------
	// Create the ICE pic window
	//---------------------------
	rect.left = TARGET_ICE_X;
	rect.right = TARGET_ICE_X + 24;
	rect.top = TARGET_ICE_Y;
	rect.bottom = TARGET_ICE_Y + 24;
	m_idIcePic.m_pImageArray = &m_NodeView.m_iaIce;
	m_idIcePic.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_TARGET_PIC);

	//----------------------
	// Create the alarm bar
	//----------------------
	rect.left = ALARM_X;
	rect.right = ALARM_X + ALARM_CX;
	rect.top = ALARM_Y;
	rect.bottom = ALARM_Y + ALARM_CY;
	m_abAlert.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_ALARM_BAR);

	//-----------------------
	// Create the clock view
	//-----------------------
	rect.left = CLOCK_X;
	rect.right = CLOCK_X + CLOCK_CX;
	rect.top = CLOCK_Y;
	rect.bottom = CLOCK_Y + CLOCK_CY;
	m_cvClock.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,rect,this,IDC_CLOCK);

	//-----------------------
	// Initialize everything
	//-----------------------
	Initialize();

	// Enable tool tips
	EnableToolTips(g_pChar->m_bTooltips);

	return 0;
}

BOOL CMatrixView::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{    
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;

	// Get the ID
	UINT nID = pNMHDR->idFrom;

    if (pTTT->uFlags & TTF_IDISHWND)
	{
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);

		// See if it is valid
		if(nID)
		{
			pTTT->lpszText = MAKEINTRESOURCE(nID);
			pTTT->hinst = AfxGetResourceHandle();
			return(TRUE);
		}
	}

	return(FALSE);
}

void CMatrixView::Initialize()
{
	//HBITMAP hBitMap;
	//HINSTANCE hInst = AfxGetInstanceHandle();

	// Load background bitmap
	m_dcBackground.SelectObject(g_hBitmaps[GR_MATRIX_MAIN]);

	// Set covers, if necessary
	if (g_pChar->m_nHardware[HW_BIO_MONITOR]==0)
	{
		CDC dcBioCover;

		dcBioCover.CreateCompatibleDC(&m_dcBackground);
		dcBioCover.SelectObject(g_hBitmaps[GR_MATRIX_BIO_COVER]);
		m_dcBackground.BitBlt(
			BIO_COVER_X, BIO_COVER_Y,
			BIO_COVER_CX, BIO_COVER_CY,
			&dcBioCover,
			0,0,
			SRCCOPY);

		m_hbHealthBar[BAR_MENTAL].ShowWindow(SW_HIDE);
		m_hbHealthBar[BAR_LETHAL].ShowWindow(SW_HIDE);
	}
	else
	{
		m_hbHealthBar[BAR_MENTAL].ShowWindow(SW_SHOW);
		m_hbHealthBar[BAR_LETHAL].ShowWindow(SW_SHOW);
	}
	if (g_pChar->m_nHardware[HW_TRACE_MONITOR]==0)
	{
		CDC dcTraceCover;

		dcTraceCover.CreateCompatibleDC(&m_dcBackground);
		dcTraceCover.SelectObject(g_hBitmaps[GR_MATRIX_TRACE_COVER]);
		m_dcBackground.BitBlt(
			TRACE_COVER_X, TRACE_COVER_Y,
			TRACE_COVER_CX, TRACE_COVER_CY,
			&dcTraceCover,
			0,0,
			SRCCOPY);

		m_hbHealthBar[BAR_TRACE].ShowWindow(SW_HIDE);
	}
	else
	{
		m_hbHealthBar[BAR_TRACE].ShowWindow(SW_SHOW);
	}

	// Show the software
	POSITION p;
	CProgram *pProgram;
	CString szText;
	int iIndex = 0;
	int iImage;

	m_SoftwareView.DeleteAllItems();

	p = g_pChar->m_olSoftware.GetHeadPosition();
	while (p!=NULL)
	{
		pProgram = (CProgram *)g_pChar->m_olSoftware.GetNext(p);

		// Ignore program if it is not loaded
		if (pProgram->m_nLoadedRating==0) continue;

		// Insert the program into the list
		iImage = pProgram->m_nClass;
		if ((pProgram == g_pChar->m_pDefAttackProgram) ||
			(pProgram == g_pChar->m_pActiveArmor) ||
			(pProgram == g_pChar->m_pActiveHide) ||
			(pProgram == g_pChar->m_pActiveShield) ||
			(pProgram == g_pChar->m_pActiveReflect))
		{
			iImage += NUM_PROGRAMS;
		}
		m_SoftwareView.InsertItem(iIndex,pProgram->m_szName,iImage);

		if (pProgram->m_nLoadedRating==pProgram->m_nRating)
		{
			szText.Format("%d",pProgram->m_nRating);
		}
		else
		{
			szText.Format("%d (%d)",pProgram->m_nLoadedRating,pProgram->m_nRating);
		}
		m_SoftwareView.SetItemText(iIndex,1,szText);

		m_SoftwareView.SetItemData(iIndex,(DWORD)pProgram);

		iIndex++;
	}

	// Show the node name
	m_nbNodeName.m_szText = g_pChar->m_pCurrentNode->m_szName;

	// Update node buttons
	UpdateNodeAccessButtons();

	// Initialize buttons
	UpdateControls();

	// Update all bars
	UpdateBar(BAR_ALL);

	// Update miscellaneous
	UpdateTarget();
	UpdateNodeIcons();
	UpdateActiveBar();
	UpdateClock();
}


void CMatrixView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	//----------------------------
	// Draw the background bitmap
	//----------------------------
	dc.BitBlt(0,0,SCREEN_CX,SCREEN_CY,&m_dcBackground,0,0,SRCCOPY);
}

void CMatrixView::OnOptions()
{
	COptionsDlg dlgOptions(this);

	if (dlgOptions.DoModal()==IDOK)
	{
		// Game has been loaded
		if (g_pChar->m_bOnRun)
		{
			// Just refresh everything
			Initialize();
			RedrawWindow();
			m_MessageView.OnClear();
			EnableToolTips(g_pChar->m_bTooltips);
			m_MapView.RedoSize();
			m_MapView.ScrollToCurrentNode();
		}
		else
		{
			// Return to the home window
			SendMessage(WM_CLOSE);
		}
	}
	else
	{
		// Game not loaded - enable tootips according to setting
		EnableToolTips(g_pChar->m_bTooltips);
	}
}

void CMatrixView::OnDestroy() 
{
	CWnd ::OnDestroy();

	delete this;
}

void CMatrixView::OnClose() 
{
	// Show the home window
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);

	CWnd::OnClose();
}

void CMatrixView::OnMoveN() 
{ 
	if (m_bmbButton[BUTTON_NORTH].IsWindowEnabled())
	{
		DoMove(DIR_NORTH); 
	}
}
void CMatrixView::OnMoveE() 
{ 
	if (m_bmbButton[BUTTON_EAST].IsWindowEnabled())
	{
		DoMove(DIR_EAST); 
	}
}
void CMatrixView::OnMoveS() 
{ 
	if (m_bmbButton[BUTTON_SOUTH].IsWindowEnabled())
	{
		DoMove(DIR_SOUTH);
	}
}
void CMatrixView::OnMoveW() 
{ 
	if (m_bmbButton[BUTTON_WEST].IsWindowEnabled())
	{
		DoMove(DIR_WEST); 
	}
}

void CMatrixView::OnDisconnect()
{
	if (DeckerMessageBox("Are you sure you wish to disconnect?",MB_YESNO)==IDYES)
	{
		DoDumpDecker(DUMP_DISCONNECT);
	}
}

void CMatrixView::UpdateControls()
{
	POSITION p;
	CIce *pIce;
	BOOL bIceGuarding[4] = {FALSE,FALSE,FALSE,FALSE};
	BOOL bHostileIce = FALSE;
	BOOL bGuardianIce = FALSE;
	BOOL bBlackIce = FALSE;
	int nIndex;
	CProgram *pProgram;
	BOOL bQueried = FALSE;

	// Look for hostile ICE and for ice guarding the exits
	p = g_pChar->m_olCurrentIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

		if (pIce->m_nState & STATE_MASK_HOSTILE)
		{
			bHostileIce = TRUE;
		}

		if (pIce->HasQueried())
		{
			bQueried = TRUE;
		}

		if (pIce->m_nType == ICE_GATEWAY)
		{
			bIceGuarding[pIce->m_nSubType] = TRUE;
		}
		else if (pIce->m_nType == ICE_GUARDIAN)
		{
			bGuardianIce = TRUE;
		}
		else if ((pIce->m_nType == ICE_ATTACK)||(pIce->m_nType == ICE_TRACE))
		{
			bBlackIce = TRUE;
		}
	}

	// Get list of ICE so we can enable/disable the buttons
	// Movement buttons
	m_bmbButton[BUTTON_NORTH].EnableWindow((g_pChar->m_pCurrentNode->m_pAdjNode[DIR_NORTH]!=NULL) &&
											((bIceGuarding[DIR_NORTH]==FALSE) || ((!bHostileIce)&&(!bQueried))));
	m_bmbButton[BUTTON_WEST].EnableWindow((g_pChar->m_pCurrentNode->m_pAdjNode[DIR_WEST]!=NULL) &&
											((bIceGuarding[DIR_WEST]==FALSE) || ((!bHostileIce)&&(!bQueried))));
	m_bmbButton[BUTTON_EAST].EnableWindow((g_pChar->m_pCurrentNode->m_pAdjNode[DIR_EAST]!=NULL) &&
											((bIceGuarding[DIR_EAST]==FALSE) || ((!bHostileIce)&&(!bQueried))));
	m_bmbButton[BUTTON_SOUTH].EnableWindow((g_pChar->m_pCurrentNode->m_pAdjNode[DIR_SOUTH]!=NULL) &&
											((bIceGuarding[DIR_SOUTH]==FALSE) || ((!bHostileIce)&&(!bQueried))));

	// These buttons are always available
	//m_bmbButton[BUTTON_WAIT				].EnableWindow(TRUE);
	//m_bmbButton[BUTTON_DISCONNECT		].EnableWindow(TRUE);
	//m_bmbButton[BUTTON_OPTIONS			].EnableWindow(TRUE);
	//m_bmbButton[BUTTON_CHAR				].EnableWindow(TRUE);
	//m_bmbButton[BUTTON_DECK		].EnableWindow(TRUE);
	//m_bmbButton[BUTTON_CONTRACT			].EnableWindow(TRUE);

	// Use default program buttons

	m_bmbButton[BUTTON_ATTACK].EnableWindow((g_pChar->m_pTargettedIce!=NULL) &&
											(g_pChar->m_pDefAttackProgram!=NULL));
	m_bmbButton[BUTTON_DECEIVE].EnableWindow((g_pChar->m_pTargettedIce!=NULL) &&
											 (g_pChar->FindProgram(PROGRAM_DECEIVE)!=NULL));
	m_bmbButton[BUTTON_VIEW_ICE].EnableWindow((g_pChar->m_pTargettedIce!=NULL) &&
											 (g_pChar->FindProgram(PROGRAM_ANALYZE)!=NULL));
	m_bmbButton[BUTTON_SCAN].EnableWindow(((g_pChar->m_pCurrentNode->m_nType == NT_DS) ||
										  (g_pChar->m_pCurrentNode->m_nType == NT_IO)) &&
										  (g_pChar->FindProgram(PROGRAM_SCAN)!=NULL));

	// Software control buttons
    nIndex=m_SoftwareView.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex==-1)
	{
		m_bmbButton[BUTTON_RUN_PROGRAM		].EnableWindow(FALSE);
		m_bmbButton[BUTTON_SET_DEF_PROGRAM	].EnableWindow(FALSE);
		m_bmbButton[BUTTON_UNLOAD_PROGRAM	].EnableWindow(FALSE);
	}
	else
	{
		pProgram = (CProgram *)m_SoftwareView.GetItemData(nIndex);
		m_bmbButton[BUTTON_RUN_PROGRAM		].EnableWindow(TRUE);
		m_bmbButton[BUTTON_SET_DEF_PROGRAM	].EnableWindow((pProgram->m_nClass == PROGRAM_ATTACK) ||
															(pProgram->m_nClass == PROGRAM_ATTACK_A) ||
															(pProgram->m_nClass == PROGRAM_ATTACK_P));
		m_bmbButton[BUTTON_UNLOAD_PROGRAM	].EnableWindow(pProgram->m_nLoadedRating != 0);
	}
	m_bmbButton[BUTTON_LOAD_PROGRAM		].EnableWindow(TRUE); // Can always load a program
	

	// Node access buttons - No access allowed if hostile guardian or black ice present,
	//	or if player has been queried.
	if (((bQueried) || (bHostileIce)) && ((bGuardianIce) || (bBlackIce)))
	{
		m_bmbButton[BUTTON_GET_FILE			].EnableWindow(FALSE);
		m_bmbButton[BUTTON_EDIT_FILE		].EnableWindow(FALSE);
		m_bmbButton[BUTTON_ERASE_FILE		].EnableWindow(FALSE);
		m_bmbButton[BUTTON_USE_IO			].EnableWindow(FALSE);
		m_bmbButton[BUTTON_CRASH			].EnableWindow(FALSE);
		m_bmbButton[BUTTON_GET_MAP			].EnableWindow(FALSE);
		m_bmbButton[BUTTON_BACKDOOR			].EnableWindow(FALSE);
		m_bmbButton[BUTTON_KILL_ALARM		].EnableWindow(FALSE);
		m_bmbButton[BUTTON_ENTER_PORTAL		].EnableWindow(FALSE);
		m_bmbButton[BUTTON_KILL_TRACE		].EnableWindow(FALSE);
		m_bmbButton[BUTTON_KILL_SHUTDOWN	].EnableWindow(FALSE);
	}
	else
	{
		m_bmbButton[BUTTON_GET_FILE			].EnableWindow(TRUE);
		m_bmbButton[BUTTON_EDIT_FILE		].EnableWindow(TRUE);
		m_bmbButton[BUTTON_ERASE_FILE		].EnableWindow(TRUE);
		m_bmbButton[BUTTON_USE_IO			].EnableWindow(!g_pChar->m_pCurrentNode->m_bActivated);
		//FSO 12-17-01 Change so that have to remove guardian ICE to crash system.
		m_bmbButton[BUTTON_CRASH			].EnableWindow((!g_pChar->m_pSystem->m_nTurnsUntilCrash) && (!bGuardianIce));
		m_bmbButton[BUTTON_GET_MAP			].EnableWindow(TRUE);
		m_bmbButton[BUTTON_BACKDOOR			].EnableWindow(TRUE);
		m_bmbButton[BUTTON_KILL_ALARM		].EnableWindow((g_pChar->m_pSystem->m_nAlert == ALERT_YELLOW) || 
															((g_pChar->m_pSystem->m_nAlert == ALERT_RED) && 
															 (!bGuardianIce))
															);
		m_bmbButton[BUTTON_ENTER_PORTAL		].EnableWindow(g_pChar->m_pCurrentNode != g_pChar->m_pSystem->m_pSystemPortalOut);
		m_bmbButton[BUTTON_KILL_TRACE		].EnableWindow(g_pChar->m_bTraced);
		m_bmbButton[BUTTON_KILL_SHUTDOWN	].EnableWindow(g_pChar->m_pSystem->m_nTurnsUntilCrash != 0);
	}
}

void CMatrixView::DoMove(int nDir)
{
	CIce *pIce;
	POSITION p;
	CString szTxt;

	// Make sure that direction is open
	if (g_pChar->m_pCurrentNode->m_pAdjNode[nDir] == NULL)
	{
		return;
	}

	// See if there is ICE blocking this direction
	p = g_pChar->m_olCurrentIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);
		
		if ((pIce->m_nType == ICE_GATEWAY) && (pIce->m_nSubType == nDir))
		{
			// There is a gateway. See if it has been deceived.
			if (!pIce->m_bBypassed)
			{
				// If we have a HIDE program active, try it
				if (g_pChar->m_pActiveHide!=NULL)
				{
					// Try to use the hide program to bypass the ice
					if (DoRunProgramVsIce(g_pChar->m_pActiveHide, pIce))
					{
						// Hide succeeded. Print out a message saying so
						szTxt.Format("%s was successfully bypassed by the %s program.", pIce->m_szName, g_pChar->m_pActiveHide->m_szName);
						m_MessageView.AddMessage(szTxt, BLACK);

						// Continue on to move code below.
						break;
					}

					// We failed. Print out a message saying so
					szTxt.Format("%s was not fooled by %s program.", pIce->m_szName, g_pChar->m_pActiveHide->m_szName);
					m_MessageView.AddMessage(szTxt, BLACK);

					pIce->m_bWasAccessed = TRUE;
				}
				else
				{
					// Put out a message and mark the ice as needing to query
					szTxt.Format("%s blocks you passage.", pIce->m_szName);
					m_MessageView.AddMessage(szTxt, BLACK);
				}

				DoEndPlayerTurn();
				return;
			}
		}
	}

	// Cancel any file transfer in progress (but not programs)
	if (g_pChar->m_pTransferFile != NULL)
	{
		g_pChar->m_pTransferFile = NULL;
		m_MessageView.AddMessage("File transfer cancelled.", BLACK);
		UpdateBar(BAR_TRANSFER);
	}

	// Cancel any scan/eval in progress
	if (g_pChar->m_pActiveScan!=NULL)
	{
		g_pChar->m_pActiveScan = NULL;
		UpdateNodeIcons();
	}

	// Cancel any client program execution
	if (g_pChar->m_nClientProgramStatus != 0)
	{
		g_pChar->m_nClientProgramStatus = 0;
		m_MessageView.AddMessage("Client program execution cancelled.", BLACK);
		UpdateNodeIcons();
	}

	// Change the state of all ice in the node we are leaving
	p = g_pChar->m_olCurrentIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

		switch (pIce->m_nState)
		{
		case STATE_MOVING:
			// Black ice moving. If not bypassed, they will want to query
			if (!pIce->m_bBypassed)
			{
				pIce->m_nState = STATE_SEARCHING;
			}
			break;
		case STATE_GUARDING_H:
			// Change them back to guarding
			pIce->m_nState = STATE_GUARDING;
			break;
		//case STATE_INACTIVE:
		//case STATE_GUARDING:
		//case STATE_FOLLOWING:
		//case STATE_SEARCHING:
		//case STATE_QUERIED:
		//case STATE_DESTROYING
		//case STATE_ATTACKING:
			// All others stay the same
		}
	}

	PlayGameSound(SOUND_MOVE);

	// Choose a location for the character in the new node and move him there
	DoEnterNode(g_pChar->m_pCurrentNode->m_pAdjNode[nDir], OppDir(nDir));

	// End the turn
	DoEndPlayerTurn();
}

void CMatrixView::OnMapZoom()
{
	m_MapView.DoZoom();
}

void CMatrixView::OnAttack()
{
	// FSO 6-20-01 Bug fix
	if (g_pChar->m_pDefAttackProgram == NULL)
	{
		DeckerMessageBox("No default attack program specified.");
	}
	else
	{
		// Run the default attack program
		DoRunProgram(g_pChar->m_pDefAttackProgram);
	}
}

void CMatrixView::OnDeceive()
{
	CProgram *pProgram;

	// Run the highest rated deceive program
	pProgram = g_pChar->FindProgram(PROGRAM_DECEIVE);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Deceive program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}

void CMatrixView::OnWait()
{
	// Just wait
	DoEndPlayerTurn();
}

void CMatrixView::OnAnalyze()
{
	CProgram *pProgram;

	// Run the highest rated analyze program
	pProgram = g_pChar->FindProgram(PROGRAM_ANALYZE);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Analyze program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}

void CMatrixView::OnDblclkProgramList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnRunProgram();
	
	*pResult = 0;
}

void CMatrixView::OnRunProgram()
{
	int nIndex;
	CProgram *pProgram;

	// Get the selected program
    nIndex = m_SoftwareView.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex==-1) return;
	pProgram = (CProgram *)m_SoftwareView.GetItemData(nIndex);

	// Run the program
	DoRunProgram(pProgram);
}


void CMatrixView::OnSetDefProgram()
{
	int nIndex;
	CProgram *pProgram;

	// Get the selected program
    nIndex = m_SoftwareView.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex==-1) return;
	pProgram = (CProgram *)m_SoftwareView.GetItemData(nIndex);

	// Change the image for the old default attack program
	if (g_pChar->m_pDefAttackProgram != NULL)
	{
		int nIndex2;

		// Find what index this item is at
		nIndex2 = GetProgramIndex(g_pChar->m_pDefAttackProgram);
		if (nIndex2!=-1)
		{
			// Change the image
			//m_SoftwareView.SetItemState(nIndex2, g_pChar->m_pDefProgram->m_nClass, LVIF_IMAGE);
			m_SoftwareView.SetItem(nIndex2, 0, LVIF_IMAGE, NULL, g_pChar->m_pDefAttackProgram->m_nClass, 0, 0, 0);
		}
	}

	// Now, set the new program as the default
	g_pChar->m_pDefAttackProgram = pProgram;

	// Change the image
	//m_SoftwareView.SetItemState(nIndex, pProgram->m_nClass + NUM_PROGRAMS, LVIF_IMAGE);
	m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, pProgram->m_nClass + NUM_PROGRAMS, 0, 0, 0);
}


void CMatrixView::OnLoadProgram()
{
	CTransferDlg m_dlgTransfer(this);

	if (m_dlgTransfer.DoModal()==IDOK)
	{
		// Note - bar will update at DoEndPlayerTurn
		//UpdateBar(BAR_TRANSFER);
		m_MessageView.AddMessage("Transfer started.",BLACK);
		DoEndPlayerTurn();
	}
}


void CMatrixView::OnUnloadProgram()
{
	int nIndex;
	CProgram *pProgram;

	// Get the index of the selected program
    nIndex = m_SoftwareView.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex==-1) return;
	pProgram = (CProgram *)m_SoftwareView.GetItemData(nIndex);

	PlayGameSound(SOUND_UNLOADPROGRAM);

	pProgram->m_nLoadedRating = 0;
	DoRemoveProgram(pProgram);
}

void CMatrixView::OnGetFile()
{
	if (AttemptNodeAccess())
	{
		CFileAccessDlg dlgFileAccess(this);

		dlgFileAccess.m_nOperationType = FO_GET;
		if (dlgFileAccess.DoModal()==IDOK)
		{
			PlayGameSound(SOUND_DOWNLOADFILE);
			m_MessageView.AddMessage("Transfer started.",BLACK);
		}
		else
		{
			// This doesn't count as an action
			return;
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnEditFile()
{
	if (AttemptNodeAccess())
	{
		CFileAccessDlg dlgFileAccess(this);

		dlgFileAccess.m_nOperationType = FO_EDIT;
		if (dlgFileAccess.DoModal()==IDOK)
		{
			PlayGameSound(SOUND_EDITFILE);
			m_MessageView.AddMessage("Transfer started.",BLACK);
		}
		else
		{
			// This doesn't count as an action
			return;
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnEraseFile()
{
	if (AttemptNodeAccess())
	{
		CFileAccessDlg dlgFileAccess(this);

		dlgFileAccess.m_nOperationType = FO_ERASE;
		if (dlgFileAccess.DoModal()==IDOK)
		{
			PlayGameSound(SOUND_ERASEFILE);
			m_MessageView.AddMessage("File erased.",BLACK);
		}
		else
		{
			// This doesn't count as an action
			return;
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnUseIO()
{
	CString szTxt;

	if (AttemptNodeAccess())
	{
		PlayGameSound(SOUND_USEIO);

		// Process depending on type
		switch(g_pChar->m_pCurrentNode->m_nSubType)
		{
		case NST_IO_ALARM:
			m_MessageView.AddMessage("External alarms have been deactivated.",BLUE);
			g_pChar->m_pSystem->m_bExternalAlarmsDeactivated = TRUE;
			g_pChar->m_pCurrentNode->m_bActivated = TRUE;
			break;
		case NST_IO_ICE_PORT:
			m_MessageView.AddMessage("Ice port has been closed. No reinforcements will appear.",BLUE);
			g_pChar->m_pCurrentNode->m_pParentArea->m_pIceIONode = NULL;
			g_pChar->m_pCurrentNode->m_bActivated = TRUE;
			break;
		case NST_IO_MATRIX:
			m_MessageView.AddMessage("Connection made to deck. High speed I/O now available in this area.",BLUE);
			g_pChar->m_pCurrentNode->m_bActivated = TRUE;
			break;
		case NST_IO_QUEST_NODE:
			if (g_pChar->m_pCurrentContract->m_nType != CONT_RUN_PROGRAM)
			{
				if (g_pChar->m_pCurrentContract->m_nType == CONT_DEACTIVATE_IO)
				{
					szTxt.Format("You have deactivated the %s.", g_pChar->m_pCurrentContract->GetTargetObjectText());
				}
				else if (g_pChar->m_pCurrentContract->m_nType == CONT_ACTIVATE_IO)
				{
					szTxt.Format("You have activated the %s.", g_pChar->m_pCurrentContract->GetTargetObjectText());
				}
				else //CONT_SABOTAGE_IO
				{
					szTxt.Format("You have sabotaged the %s.", g_pChar->m_pCurrentContract->GetTargetObjectText());
				}

				m_MessageView.AddMessage(szTxt,BLUE);
				g_pChar->m_pCurrentNode->m_bActivated = TRUE;

				//FSO 4-8-02 Get rid of this
				//// Set the quest completion time
				//g_pChar->m_pCurrentContract->m_nCompletionTime = g_pChar->m_nRunTime;
				break;
			}
			// Else this is a run-program node, so fall through
		default:
			// Useless node. Chance of setting alarm
			if (Random(3)==0)
			{
				// Set an alert
				m_MessageView.AddMessage("You have accidentally triggered an alarm!", RED);

				// Do a red alert
				DoSetAlert(NULL, ALERT_RED);
			}
			else
			{
				// Tell the user that nothing happened
				m_MessageView.AddMessage("Nothing happens!", BLUE);
			}
			break;
		}

		UpdateControls();
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnCrash()
{
	int iTargetNumber;
	int iSuccess;

	// Attampt node access
	if (AttemptNodeAccess())
	{
		// Make a roll of attack skill versus the system rating
		iTargetNumber = 10 + (2*g_pChar->m_pSystem->m_nRating) - g_pChar->GetEffectiveRating(RATING_ATTACK);
		
		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		// Check for success
		if (iSuccess>0)
		{
			PlayGameSound(SOUND_CRASHSYSTEM);

			// Crash the system
			DoDumpDecker(DUMP_SYS_CRASH);
			return;
		}
		else
		{
			// A failed attempt at crashing always sets an alert
			m_MessageView.AddMessage("System crash failed. You have triggered an alert.", RED);
			DoSetAlert(NULL, ALERT_RED);
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnGetMap()
{
	POSITION p;
	CNode *pNode;

	// Attempt node access
	if (AttemptNodeAccess())
	{
		PlayGameSound(SOUND_GETMAP);

		// Map all of the nodes in this area
		p = g_pChar->m_pCurrentNode->m_pParentArea->m_olNodeList.GetHeadPosition();
		while (p!=NULL)
		{
			pNode = (CNode *)g_pChar->m_pCurrentNode->m_pParentArea->m_olNodeList.GetNext(p);

			pNode->m_bMapped = TRUE;

			// Handle special map icons //FSO 6-20-01
			if (pNode->m_nType == NT_IO)
			{
				if (pNode->m_nSubType == NST_IO_ALARM)
				{
					pNode->m_nSpecialImage = NSI_IO_ALARM;
				}
				else if (pNode->m_nSubType == NST_IO_ICE_PORT)
				{
					pNode->m_nSpecialImage = NSI_IO_ICE_PORT;
				}
				else if (pNode->m_nSubType == NST_IO_MATRIX)
				{
					pNode->m_nSpecialImage = NSI_IO_MATRIX;
				}
			}
			else if (pNode->m_nType == NT_COP)
			{
				if (pNode->m_nSubType == NST_COP_SECURITY)
				{
					pNode->m_nSpecialImage = NSI_COP_SECURITY;
				}
			}
		}
		m_MapView.RedrawWindow();

		m_MessageView.AddMessage("Area map aquired.", BLACK);
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnBackdoor()
{
	int iTargetNumber;
	int iSuccess;

	// Attempt node access
	if (AttemptNodeAccess())
	{
		// Make a roll of analysis skill versus the system rating
		iTargetNumber = 12 + (2*g_pChar->m_pSystem->m_nRating) - g_pChar->GetEffectiveRating(RATING_ANALYSIS);
		
		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		// Check for success
		if (iSuccess>0)
		{
			PlayGameSound(SOUND_CREATEBACKDOOR);

			m_MessageView.AddMessage("Backdoor successfully created.", BLUE);
			g_pChar->m_pSystem->m_bBackdoor = TRUE;
			g_pChar->m_bBackdoor[g_pChar->m_pCurrentContract->m_nCorporation] = iSuccess;
		}
		else 
		{
			m_MessageView.AddMessage("Backdoor creation failed.", BLACK);

			if (iSuccess == -1)
			{
				// Critical failure. Set alarm
				m_MessageView.AddMessage("Oops! You have triggered an alert.", RED);
				DoSetAlert(NULL, ALERT_RED);
			}
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnKillAlarm()
{
	int iTargetNumber;
	int iSuccess;

	// If green alert, nothing to do
	if (g_pChar->m_pSystem->m_nAlert == ALERT_GREEN)
	{
		m_MessageView.AddMessage("There is no alert to cancel.", BLACK);
		return;
	}

	// Attempt node access
	if (AttemptNodeAccess())
	{
		// Make a roll of attack skill versus the system rating
		iTargetNumber = 10 + (2*g_pChar->m_pSystem->m_nRating) - g_pChar->GetEffectiveRating(RATING_ATTACK);
		
		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		// Check for success
		if (iSuccess>0)
		{
			PlayGameSound(SOUND_CANCELALERT);

			m_MessageView.AddMessage("Alert cancelled.",BLUE);

			// If red alert, send all ICE to home nodes
			if (g_pChar->m_pSystem->m_nAlert == ALERT_RED)
			{
				POSITION p;
				p = g_pChar->m_pSystem->m_olIceList.GetHeadPosition();
				while (p != NULL)
				{
					CIce *pIce = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);
					if ((pIce->m_nType == ICE_ATTACK) || (pIce->m_nType == ICE_TRACE))
					{
						pIce->m_nState = STATE_MOVING;
						pIce->m_pTargetNode = pIce->m_pHomeNode;
					}
					else if (pIce->m_nType == ICE_PROBE)
					{
						pIce->m_nState = STATE_SEARCHING;
					}
					else
					{
						// Gateway, guardian, tapeworm should already be at home or
						// going there. Just set to non-hostile
						pIce->m_nState &= (~STATE_MASK_HOSTILE);
					}
				}

				// Downgrade alert
				g_pChar->m_pSystem->m_nAlert = ALERT_YELLOW;
			}
			else
			{
				// Go from yellow to green
				g_pChar->m_pSystem->m_nAlert = ALERT_GREEN;
			}

			// Do the graphic
			m_abAlert.RedrawWindow();
		}
		else
		{
			m_MessageView.AddMessage("Failed to cancel alert.", BLACK);

			if (iSuccess == -1)
			{
				// Critical failure. Set alarm
				m_MessageView.AddMessage("Oops! You have triggered an alert.", RED);
				DoSetAlert(NULL, ALERT_RED);
			}
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnKillShutDown()
{
	int iTargetNumber;
	int iSuccess;

	// Attempt node access
	if (AttemptNodeAccess())
	{
		// Make a roll of attack skill versus the system rating
		iTargetNumber = 8 + (2*g_pChar->m_pSystem->m_nRating) - g_pChar->GetEffectiveRating(RATING_ATTACK);
		
		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		// Check for success
		if (iSuccess>0)
		{
			PlayGameSound(SOUND_CANCELSHUTDOWN);

			m_MessageView.AddMessage("System shutdown halted.", BLUE);
			g_pChar->m_pSystem->m_nTurnsUntilCrash = 0;

			// Do the graphic
			m_abAlert.RedrawWindow();
		}
		else
		{
			m_MessageView.AddMessage("Failed to cancel shutdown.", BLACK);

			if (iSuccess == -1)
			{
				// Critical failure. Set alarm
				m_MessageView.AddMessage("Oops! You have triggered an alert.", RED);
				DoSetAlert(NULL, ALERT_RED);
			}
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnKillTrace()
{
	int iTargetNumber;
	int iSuccess;

	// Attempt node access
	if (AttemptNodeAccess())
	{
		// Make a roll of attack skill versus the system rating
		iTargetNumber = 10 + (2*g_pChar->m_pSystem->m_nRating) - g_pChar->GetEffectiveRating(RATING_ATTACK);
		
		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		// Check for success
		if (iSuccess>0)
		{
			PlayGameSound(SOUND_CANCELTRACE);

			m_MessageView.AddMessage("Trace has been removed.", BLUE);
			g_pChar->m_bTraced = FALSE;

			// Do the graphic
			m_abAlert.RedrawWindow();
		}
		else
		{
			m_MessageView.AddMessage("Failed to remove trace.", BLACK);

			if (iSuccess == -1)
			{
				// Critical failure. Set alarm
				m_MessageView.AddMessage("Oops! You have triggered an alert.", RED);
				DoSetAlert(NULL, ALERT_RED);
			}
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnEnterPortal()
{
	POSITION p;
	CArea *pArea;

	// Attempt node access
	if (AttemptNodeAccess())
	{
		PlayGameSound(SOUND_ENTERPORTAL);

		// Find the current area in the list
		p = g_pChar->m_pSystem->m_olAreaList.Find(g_pChar->m_pCurrentNode->m_pParentArea);

		// Is this a portal in or portal out
		if (g_pChar->m_pCurrentNode == g_pChar->m_pCurrentNode->m_pParentArea->m_pAreaPortalIn)
		{
			// Go inwards
			g_pChar->m_pSystem->m_olAreaList.GetPrev(p);
			pArea = (CArea *)g_pChar->m_pSystem->m_olAreaList.GetAt(p);
			DoEnterNode(pArea->m_pAreaPortalOut, DIR_CENTER);
		}
		else
		{
			// Go outwards
			g_pChar->m_pSystem->m_olAreaList.GetNext(p);
			pArea = (CArea *)g_pChar->m_pSystem->m_olAreaList.GetAt(p);
			DoEnterNode(pArea->m_pAreaPortalIn, DIR_CENTER);
		}
	}
	DoEndPlayerTurn();
}

void CMatrixView::OnViewChar()
{
	CCharDataDialog dlgChar(this);

	if (dlgChar.DoModal()==IDC_CHAR_VIEW_DECK)
	{
		// Switch to deck view
		OnViewDeck();
	}
}

void CMatrixView::OnViewDeck()
{
	CDeckDataDialog dlgDeck(this);

	if (dlgDeck.DoModal()==IDC_DECK_VIEW_CHAR)
	{
		// Switch to character view
		OnViewChar();
	}
}

void CMatrixView::OnViewContract()
{
	CContractDataDialog dlgContract(this);

	dlgContract.m_pContract = g_pChar->m_pCurrentContract;
	dlgContract.DoModal();
}

void CMatrixView::OnItemchangedSoftwareView(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	//
	//if (m_SoftwareView.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED)!=-1)
	//{
	//}
	UpdateControls();
	
	*pResult = 0;
}

void CMatrixView::DoEnterNode(CNode *pNode, int nDirFrom)
{
	POSITION p;
	CIce *pIce;
	CString szTmp;
	int i;

	// Generate a location for the player, and put him in it
	pNode->ChooseLocation(g_pChar->m_ptNodeLoc, nDirFrom);
	g_pChar->m_pCurrentNode = pNode;

	// Choose locations for decoys
	for (i=0; i<g_pChar->m_nDecoyCount; i++)
	{
		pNode->ChooseLocation(g_pChar->m_ptDecoyLocation[i], nDirFrom);
	}

	// Clear the targetted ice
	g_pChar->m_pTargettedIce = NULL;
	UpdateTarget();

	// Clear the highest deceived ICE
	g_pChar->m_nHighestDeceivedIce = 0;

	// Mark the node as mapped
	g_pChar->m_pCurrentNode->m_bMapped = TRUE;

	// If mapper hardware installed, mark adjacent nodes as mapped
	if (g_pChar->m_nHardware[HW_MAPPER]>0)
	{
		for (i = 0; i < 4; i++)
		{
			if (g_pChar->m_pCurrentNode->m_pAdjNode[i] != NULL)
			{
				g_pChar->m_pCurrentNode->m_pAdjNode[i]->m_bMapped = TRUE;
			}
		}
	}

	// Build an ICE list for the node
	g_pChar->m_olCurrentIceList.RemoveAll();
	p = g_pChar->m_pSystem->m_olIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);

		if (pIce->m_pCurrentNode == pNode)
		{
			g_pChar->m_olCurrentIceList.AddTail(pIce);

			// Reset some flags on the ICE
			pIce->m_bWasAccessed = FALSE;
			pIce->m_bBypassed = FALSE;
		}
	}

	// If we have been traced, mark the ice as hostile
	if (g_pChar->m_bTraced)
	{
		MarkIceAsHostile();
	}

	// Update the node access buttons
	UpdateNodeAccessButtons();

	// Output a message
	szTmp.Format("Entering node %s.", pNode->m_szName);
	m_MessageView.AddMessage(szTmp, BLACK);

	// Show the node name
	//CClientDC dc(this);
	//dc.FillSolidRect(NODE_NAME_X, NODE_NAME_Y, NODE_NAME_CX, NODE_NAME_CY, WHITE);
	//dc.TextOut(NODE_NAME_X, NODE_NAME_Y, g_pChar->m_pCurrentNode->m_szName);
	m_nbNodeName.m_szText = g_pChar->m_pCurrentNode->m_szName;

	// Redraw the node
	m_NodeView.RedrawWindow();

	// Redraw the map
	m_MapView.DoMove();

	// Redraw the node name
	this->m_nbNodeName.RedrawWindow();

	// Update the controls
	UpdateControls();
}

BOOL CMatrixView::DoRunProgramVsIce(CProgram *pProgram, CIce *pIce)
{
	int iTargetNumber;
	int iSuccess;
	CString szTxt;
	POSITION p;
	CIce *pTmpIce;
	BOOL bLessDamage = FALSE;
	BOOL bReturn = TRUE;
	int iDamage; //FSO 7-9-01

	// Process according to program type
	switch (pProgram->m_nClass)
	{
	//-----------------
	// Attack Programs
	//-----------------
	case PROGRAM_ATTACK:
		// Get the target number
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_COMBAT)) - (g_pChar->GetEffectiveRating(RATING_ATTACK) + pProgram->m_nLoadedRating);
		
		// Modify if ice is hardened/area/etc
		if ((pIce->m_nType==ICE_ATTACK) &&
			((pIce->m_nSubType&AST_MASK_HARDENED)||
			 (pIce->m_nSubType&AST_MASK_PHASING)))
		{
			iTargetNumber += 3;
		}

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// We hit. Cpmpute damage
			iDamage = ComputeDamage(iSuccess);

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Print out a message saying so
			szTxt.Format("%s program does %d%% damage to %s.",pProgram->m_szName, (iDamage*5), pIce->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
			
			// Draw the damage
			m_NodeView.DrawDamage(pIce);

			// Modify the Ice's health
			pIce->m_nHealth -= iDamage;
			if (pIce->m_nHealth<=0)
			{
				DoCrashIce(pIce);
			}
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program missed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}

		// Need to mark all ice as hostile
		MarkIceAsHostile();

		// It this is a tapeworm set it to destroy the file
		if (pIce->m_nType == ICE_TAPEWORM)
		{
			pIce->m_nState = STATE_DESTROYING;
		}
		break;

	case PROGRAM_ATTACK_A:
		// Area attack effects everyone in node
		p = g_pChar->m_olCurrentIceList.GetHeadPosition();
		while (p!=NULL)
		{
			pTmpIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

			// FSO 5-31-04
			bLessDamage = FALSE;

			// Attack this ice
			// Get the target number
			iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pTmpIce->GetEffectiveRating(RATING_COMBAT)) - (g_pChar->GetEffectiveRating(RATING_ATTACK) + pProgram->m_nLoadedRating);

			// Modify if ice is hardened/area/etc
			if (pTmpIce->m_nType==ICE_ATTACK)
			{
				if (pTmpIce->m_nSubType&AST_MASK_HARDENED)
				{
					// Piercing sucks against hardened
					iTargetNumber += 6;
					bLessDamage = TRUE;
				}
				else if ((pTmpIce->m_nSubType&AST_MASK_PHASING)==0)
				{
					// Against normal targets, target number is increased by 2
					iTargetNumber += 2;
				}
			}
			else
			{
				// Against normal targets, target number is increased by 2
				iTargetNumber += 2;
			}

			// Roll the die
			iSuccess = DoDieRoll(iTargetNumber);

			if (iSuccess>0)
			{
				// We hit. Compute damage
				iDamage = ComputeDamage(iSuccess);

				// If we are reducing damage, do it
				if (bLessDamage)
				{
					// Divide damage in half (round up)
					iDamage = ((iDamage+1)/2);
				}
				
				// Play the program sound effect
				PlayGameSound(pProgram->m_nSound);

				// Print out a message saying so
				szTxt.Format("%s program does %d%% damage to %s.",pProgram->m_szName, (iDamage*5), pTmpIce->m_szName);
				m_MessageView.AddMessage(szTxt, BLACK);

				// Draw the damage
				m_NodeView.DrawDamage(pTmpIce);

				// Modify the Ice's health
				pTmpIce->m_nHealth -= iDamage;
				if (pTmpIce->m_nHealth<=0)
				{
					DoCrashIce(pTmpIce);
				}
			}
			else
			{
				// We missed. Print out a message saying so
				szTxt.Format("%s program missed.",pProgram->m_szName);
				m_MessageView.AddMessage(szTxt, BLACK);
			}

			// It this is a tapeworm set it to destroy the file
			if (pTmpIce->m_nType == ICE_TAPEWORM)
			{
				pTmpIce->m_nState = STATE_DESTROYING;
			}
		}

		// Need to mark all ice as hostile
		MarkIceAsHostile();
		break;

	case PROGRAM_ATTACK_P:
		// Get the target number
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_COMBAT)) - (g_pChar->GetEffectiveRating(RATING_ATTACK) + pProgram->m_nLoadedRating);

		// Modify if ice is hardened/area/etc
		if (pIce->m_nType==ICE_ATTACK)
		{
			if (pIce->m_nSubType&AST_MASK_PHASING)
			{
				// Piercing sucks against phasing
				iTargetNumber += 6;
				bLessDamage = TRUE;
			}
			else if ((pIce->m_nSubType&AST_MASK_HARDENED)==0)
			{
				// Against normal targets, target number is reduced by 2, but damage is also reduced
				iTargetNumber -= 3;
				bLessDamage = TRUE;
			}
		}
		else
		{
			// Against normal targets, target number is reduced by 2, but damage is also reduced
			iTargetNumber -= 3;
			bLessDamage = TRUE;
		}

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// We hit. Compute damage
			iDamage = ComputeDamage(iSuccess);

			// If we are reducing damage, do it
			if (bLessDamage)
			{
				// Divide damage in half
				//FSO 5-31-04
				iDamage = ((iDamage+1)/2);
			}
			
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Print out a message saying so
			szTxt.Format("%s program does %d%% damage to %s.",pProgram->m_szName, (iDamage*5), pIce->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);

			// Draw the damage
			m_NodeView.DrawDamage(pIce);

			// Modify the Ice's health
			pIce->m_nHealth -= iDamage;
			if (pIce->m_nHealth<=0)
			{
				DoCrashIce(pIce);
			}
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program missed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}

		// It this is a tapeworm set it to destroy the file
		if (pIce->m_nType == ICE_TAPEWORM)
		{
			pIce->m_nState = STATE_DESTROYING;
		}

		// Need to mark all ice as hostile
		MarkIceAsHostile();
		break;

	case PROGRAM_SLOW:
		p = g_pChar->m_olCurrentIceList.GetHeadPosition();
		while (p!=NULL)
		{
			pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

			// Get the target number
			iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_COMBAT)) - (g_pChar->GetEffectiveRating(RATING_ATTACK) + pProgram->m_nLoadedRating);

			// Roll the die
			iSuccess = DoDieRoll(iTargetNumber);

			if (iSuccess>0)
			{
				// Slow lasts for 4 turns per success
				iSuccess *= 4;

				// Play the program sound effect
				PlayGameSound(pProgram->m_nSound);

				// Success. Print out a message saying so
				szTxt.Format("%s program succeeds. %s is slowed for %d turns.",pProgram->m_szName, pIce->m_szName, iSuccess);
				m_MessageView.AddMessage(szTxt, BLACK);

				// Add one to the slow level because it is reduced at the first of the turn
				iSuccess++;
				
				// Modify the Ice's status only if higher
				if (iSuccess > pIce->m_nSlowLevel) 
				{
					pIce->m_nSlowLevel = iSuccess;
				}
			}
			else
			{
				// We missed. Print out a message saying so
				szTxt.Format("%s is unaffected by %s.",pIce->m_szName, pProgram->m_szName);
				m_MessageView.AddMessage(szTxt, BLACK);
			}

			// It this is a tapeworm set it to destroy the file
			if (pIce->m_nType == ICE_TAPEWORM)
			{
				pIce->m_nState = STATE_DESTROYING;
			}
		}

		// Need to mark all ice as hostile
		MarkIceAsHostile();
		break;

	case PROGRAM_VIRUS:
		// Get the target number
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_COMBAT)) - (g_pChar->GetEffectiveRating(RATING_ATTACK) + pProgram->m_nRating);

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// Virus level is success level * 4
			iSuccess *= 4;

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds. %s is infected for %d turns.",pProgram->m_szName, pIce->m_szName, iSuccess);
			m_MessageView.AddMessage(szTxt, BLACK);
			
			// Add one to the virus level because it is reduced at the first of the turn
			iSuccess++;

			// Modify the Ice's status only if higher
			if (iSuccess > pIce->m_nVirusLevel) 
			{
				pIce->m_nVirusLevel = iSuccess;
			}
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}

		// Make a second roll versus stealth to see if the ice noticed it
		if (!(pIce->m_nState & STATE_MASK_HOSTILE))
		{
			iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_SENSORS)) - (g_pChar->GetEffectiveRating(RATING_STEALTH) + pProgram->m_nLoadedRating);

			// Roll the die
			iSuccess = DoDieRoll(iTargetNumber);

			if (iSuccess>0)
			{
				m_MessageView.AddMessage("The virus attack goes unnoticed.", BLACK);
			}
			else
			{
				m_MessageView.AddMessage("The virus attack is noticed!.", BLACK);

				// It this is a tapeworm set it to destroy the file
				if (pIce->m_nType == ICE_TAPEWORM)
				{
					pIce->m_nState = STATE_DESTROYING;
				}

				// Need to mark all ice as hostile
				MarkIceAsHostile();
			}
		}
		break;

	case PROGRAM_CONFUSE:
		// Get the target number
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_COMBAT)) - (g_pChar->GetEffectiveRating(RATING_ATTACK) + pProgram->m_nLoadedRating);

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// Confusion lasts for 4 turns per success
			iSuccess *= 4;

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds. %s is confused for %d turns.",pProgram->m_szName, pIce->m_szName, iSuccess);
			m_MessageView.AddMessage(szTxt, BLACK);
			
			// Add one to the confusion level because it is reduced at the first of the turn
			iSuccess++;

			// Modify the Ice's status only if higher
			if (iSuccess > pIce->m_nConfusionLevel) 
			{
				pIce->m_nConfusionLevel = iSuccess;
			}
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}

		// Only mark ice as hostile if other programs were in the node, or the attack failed
		if ((g_pChar->m_olCurrentIceList.GetCount()>1) || (iSuccess<1))
		{
			// It this is a tapeworm set it to destroy the file
			if (pIce->m_nType == ICE_TAPEWORM)
			{
				pIce->m_nState = STATE_DESTROYING;
			}

			// Need to mark all ice as hostile
			MarkIceAsHostile();
		}
		break;

	case PROGRAM_WEAKEN:
		// Get the target number
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_COMBAT)) - (g_pChar->GetEffectiveRating(RATING_ATTACK) + pProgram->m_nLoadedRating);

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// Weaken lasts for 4 turns per success
			iSuccess *= 4;

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds. %s is weakened for %d turns.",pProgram->m_szName, pIce->m_szName, iSuccess);
			m_MessageView.AddMessage(szTxt, BLACK);
			
			// Add one to the weaken level because it is reduced at the first of the turn
			iSuccess++;

			// Modify the Ice's status only if higher
			if (iSuccess > pIce->m_nWeakenLevel) 
			{
				pIce->m_nWeakenLevel = iSuccess;
			}
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}

		// It this is a tapeworm set it to destroy the file
		if (pIce->m_nType == ICE_TAPEWORM)
		{
			pIce->m_nState = STATE_DESTROYING;
		}

		// Need to mark all ice as hostile
		MarkIceAsHostile();
		break;

	//------------------
	// Stealth Programs
	//------------------
	case PROGRAM_HIDE:
		// Special Case: This is an always active program, so if this function is being called,
		// we must be trying to bypass a gateway/guardian with a hide program active,
		// or else a piece of ice is trying to notice us.
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_SENSORS)) - (g_pChar->GetEffectiveRating(RATING_STEALTH) + pProgram->m_nLoadedRating);

		// If decoys are in use, big penalty
		if (g_pChar->m_nDecoyCount>0)
		{
			iTargetNumber += 4;
		}

		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			bReturn = TRUE;
		}
		else
		{
			bReturn = FALSE;
		}
		break;

	case PROGRAM_DECEIVE:
		// If the ICE is hostile, it automatically fails
		if (pIce->m_nState & STATE_MASK_HOSTILE)
		{
			szTxt.Format("%s already knows you are hostile. Program  %s has no effect.", pIce->m_szName ,pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);
			return FALSE;
		}

		// Get the target number
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_SENSORS)) - (g_pChar->GetEffectiveRating(RATING_STEALTH) + pProgram->m_nLoadedRating);
		// If the user has accessed a passcode file, reduce by 4 (20%)
		if (g_pChar->m_pSystem->m_bPasscodeAccessed)
		{
			iTargetNumber -= 4;
		}

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds. %s is deceived.",pProgram->m_szName, pIce->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);

			m_NodeView.DrawBypass(pIce);
			
			// Modify the Ice's status
			pIce->m_bBypassed = TRUE;

			// Set the highest deceived level
			if (pIce->m_nRating > g_pChar->m_nHighestDeceivedIce)
			{
				g_pChar->m_nHighestDeceivedIce = pIce->m_nRating;
			}

			// Now, mark everyone at a lower or same rating as decieved
//			// If this was a querying ice, mark all other ice as deceived
//			if (pIce->HasQueried())
//			{
				p = g_pChar->m_olCurrentIceList.GetHeadPosition();
				while (p!=NULL)
				{
					pTmpIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

					// Only mark if rating is same or higher
					if (pTmpIce->m_nRating <= pIce->m_nRating)
					{
						pTmpIce->m_bBypassed = TRUE;
					}
				}
//			}

			return TRUE;
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed. %s was not deceived.",pProgram->m_szName, pIce->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);

			// Trigger an alert - only if the ice is not in the query state. If it is in 
			// the query state, it will trigger an alert on its next action.
			if (pIce->HasQueried())
			{
				pIce->m_nState = STATE_QUERIED3;
			}
			else
			{
				DoSetAlert(pIce, ALERT_YELLOW);
				// It this is a tapeworm set it to destroy the file
				if ((g_pChar->m_pSystem->m_nAlert == ALERT_RED) && 
					(pIce->m_nType == ICE_TAPEWORM))
				{
					pIce->m_nState = STATE_DESTROYING;
				}
			}
		}
		break;

	case PROGRAM_RELOCATE:
		// Note - At this point, should have already checked to see
		//        that a trace is actually occurring.

		// Get the target number
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_COMBAT)) - (g_pChar->GetEffectiveRating(RATING_STEALTH) + pProgram->m_nLoadedRating);

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
			
			// Remove the trace
			DoClearTrace();

			return TRUE;
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}
		break;

	//-------------------
	// Analysis Programs
	//-------------------
	case PROGRAM_ANALYZE:
		// Get the target number (low for analyze)
		iTargetNumber = 8 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_NORMAL)) - (g_pChar->GetEffectiveRating(RATING_ANALYSIS) + pProgram->m_nLoadedRating);

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		//FSO 8-7-01
		//if (iSuccess>0)
		if (iSuccess > pIce->m_nAnalyzed)
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s analyzed (%d%%)", pIce->m_szName, iSuccess*20);
			m_MessageView.AddMessage(szTxt, BLACK);

			//FSO 8-7-01
			//pIce->m_bAnalyzed = TRUE;
			pIce->m_nAnalyzed = iSuccess;

			return TRUE;
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);

			return FALSE;
		}
		break;

	case PROGRAM_DECRYPT:
		// Make sure this is a tapeworm (should have already been done)
		if (pIce->m_nType!=ICE_TAPEWORM)
		{
			szTxt.Format("%s program only works against tapeworms.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
			return FALSE;
		}

		// Get the target number - Decrypt has a base 60% chance of working.
		//   Lower target number so player will want to use this program over deceive.
		iTargetNumber = 10 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_NORMAL)) - (g_pChar->GetEffectiveRating(RATING_ANALYSIS) + pProgram->m_nLoadedRating);
		// If the user has accessed a passcode file, reduce by 2 (10%)
		if (g_pChar->m_pSystem->m_bPasscodeAccessed)
		{
			iTargetNumber -= 2;
		}

		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);

		if (iSuccess>0)
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds. %s decrypted.",pProgram->m_szName, pIce->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);

			DoCrashIce(pIce);

			return TRUE;
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);

			// Make a second roll versus stealth to see if the ice noticed it
			iTargetNumber = 8 + ((2*g_pChar->m_pSystem->m_nRating) + pIce->GetEffectiveRating(RATING_SENSORS)) - (g_pChar->GetEffectiveRating(RATING_STEALTH) + pProgram->m_nLoadedRating);

			// Roll the die
			iSuccess = DoDieRoll(iTargetNumber);

			if (iSuccess<=0)
			{
				szTxt.Format("%s noticed the decrypt attempt!.", pIce->m_szName); //FSO 6-26-01 Bug fix
				m_MessageView.AddMessage(szTxt, RED);

				// Need to mark all ice as hostile
				MarkIceAsHostile();
				pIce->m_nState = STATE_DESTROYING;
			}

			return FALSE;
		}
		break;

	//---------------------------------------------
	// These do not target ice, and so are illegal
	//---------------------------------------------
	//case PROGRAM_SILENCE:
	//case PROGRAM_SHIELD:
	//case PROGRAM_SMOKE:
	//case PROGRAM_DECOY:
	//case PROGRAM_MEDIC:
	//case PROGRAM_ARMOR:
	//case PROGRAM_SCAN:
	//case PROGRAM_EVALUATE:
	}

	// Done
	return bReturn;
}

void CMatrixView::DoSetAlert(CIce *pIce, int nAlertLevel)
{
	CString szTxt;

	// If the node is silenced, we cannot set an alarm
	if ((pIce!=NULL) && (pIce->m_pCurrentNode->m_nActiveSilenceLevel))
	{
		// Automatically set ice to hostile
		if (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode)
		{
			MarkIceAsHostile();
		}
		return;
	}

	// If we already are at red alert, ignore
	if (g_pChar->m_pSystem->m_nAlert == ALERT_RED)
	{
		// If the ice is in the same node as the player, or if the ice is chasing the player,
		// the route the IC to the player's node
		if ((pIce == NULL) || (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode))
		{
			g_pChar->m_pSystem->RouteResponseIC(g_pChar->m_pCurrentNode);
			MarkIceAsHostile();
		}
		return;
	}

	// New alert depends on current alert and requested alert
	if ((g_pChar->m_pSystem->m_nAlert == ALERT_GREEN) && (nAlertLevel == ALERT_YELLOW))
	{
		// Do a yellow alert only
		PlayGameSound(SOUND_YELLOWALERT);
		g_pChar->m_pSystem->m_nAlert = ALERT_YELLOW;

		// Send a message
		if (pIce!=NULL)
		{
			szTxt.Format("%s in node %s signals a yellow alert.", pIce->m_szName, pIce->m_pCurrentNode->m_szName);
			m_MessageView.AddMessage(szTxt, ORANGE);
		}
	}
	else
	{
		// Do a red alert
		PlayGameSound(SOUND_REDALERT);
		g_pChar->m_pSystem->m_nAlert = ALERT_RED;

		// Set flag that alert was set
		g_pChar->m_dwRunFlags |= CRF_ALARMS_SET;

		// Send a message
		if (pIce!=NULL)
		{
			szTxt.Format("%s in node %s signals a red alert!", pIce->m_szName, pIce->m_pCurrentNode->m_szName);
			m_MessageView.AddMessage(szTxt, RED);
		}

		// Activate all IC
		POSITION p;
		CIce *pIce2;
		p = g_pChar->m_pSystem->m_olIceList.GetHeadPosition();
		while (p!=NULL)
		{
			pIce2 = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);
			if (pIce2->m_nState == STATE_INACTIVE)
			{
				// Activate this ice
				pIce2->m_nState = STATE_GUARDING;
			}
		}

		// Send the resonse IC to the ice's node.
		if (pIce!=NULL)
			g_pChar->m_pSystem->RouteResponseIC(pIce->m_pCurrentNode);
		else
			g_pChar->m_pSystem->RouteResponseIC(g_pChar->m_pCurrentNode);

		// Mark the ice as hostile
		if ((pIce == NULL) || (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode))
		{
			MarkIceAsHostile();
		}
	}

	// Show the graphic
	if ((pIce != NULL) && (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode))
	{
		m_NodeView.DrawAlert(pIce);
	}
	
	// Do the graphic
	m_abAlert.RedrawWindow();
}

void CMatrixView::DoClearTrace()
{
	// Send a message
	m_MessageView.AddMessage("Trace has been cancelled.", BLUE);

	// Clear the trace data
	g_pChar->m_pTraceIce = NULL;

	// Update the bar
	UpdateBar(BAR_TRACE);
}

void CMatrixView::DoCrashIce(CIce *pIce)
{
	CString szTxt;

	// Ice is crashed. Send a message.
	// Check to see if ice is in same node. If not, no message.
	if (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode)
	{
		// Don't play sound if tapeworm, because has own sound
		if (pIce->m_nType != ICE_TAPEWORM)
		{
			PlayGameSound(SOUND_ICECRASH);
		}

		szTxt.Format("%s has crashed!", pIce->m_szName);
		m_MessageView.AddMessage(szTxt, DK_BLUE);

		// Erase the ice from the node view
		m_NodeView.EraseIce(pIce);
	}

	// If this was the targetted ice, clear the target
	if (pIce == g_pChar->m_pTargettedIce)
	{
		g_pChar->m_pTargettedIce = NULL;
		UpdateTarget();
		UpdateControls();
	}

	// If this was a tracing ice, cancel the trace
	if (pIce == g_pChar->m_pTraceIce)
	{
		DoClearTrace();
	}

	// Remove the ICE from the node list
	if (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode)
	{
		g_pChar->m_olCurrentIceList.RemoveAt(g_pChar->m_olCurrentIceList.Find(pIce));
	}

	// Remove the ICE from the system list
	g_pChar->m_pSystem->m_olIceList.RemoveAt(g_pChar->m_pSystem->m_olIceList.Find(pIce));

	// If this is a tapeworm ice, set it to not be guarding a file,
	// then delete it (tapeworms do not respawn).
	if (pIce->m_nType == ICE_TAPEWORM)
	{
		pIce->m_pFile->m_pTapeworm = NULL;
		delete pIce;
	}
	else
	{
		// Move this ice to the dead ice list
		g_pChar->m_pSystem->m_olDeadIceList.AddTail(pIce);
	}
}

void CMatrixView::DoRunProgram(CProgram *pProgram)
{
	CString szTxt;
	int nIndex;
	CDSFile *pFile;
	int iTargetNumber;
	int iSuccess;

	// Process according to what type of program it is
	switch (pProgram->m_nClass)
	{
	case PROGRAM_ATTACK:
	case PROGRAM_ATTACK_P:
	case PROGRAM_VIRUS:
	case PROGRAM_CONFUSE:
	case PROGRAM_WEAKEN:
	case PROGRAM_DECEIVE:
	case PROGRAM_DECRYPT:
		// Just make sure we have a target ice
		if (g_pChar->m_pTargettedIce == NULL)
		{
			szTxt.Format("%s cannot run without a target selected.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
			return;
		}

	// These are are-effect
	case PROGRAM_ATTACK_A:
	case PROGRAM_SLOW:
		// Run the program
		DoRunProgramVsIce(pProgram, g_pChar->m_pTargettedIce);
		break;

	case PROGRAM_RELOCATE:
		// Make sure we have a trace going
		if (g_pChar->m_pTraceIce == NULL)
		{
			// No trace active
			szTxt.Format("%s cannot run without an active trace.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
			return;
		}
		else
		{
			// Run the program
			DoRunProgramVsIce(pProgram, g_pChar->m_pTraceIce);
		}
		break;

	case PROGRAM_ANALYZE:
		// Just make sure we have a target ice
		if (g_pChar->m_pTargettedIce == NULL)
		{
			szTxt.Format("%s cannot run without a target selected.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
			return;
		}

//FSO 8-7-01 Now allow multiple analyises to get better results
//		// See if this ice has already been analyzed
//		if (g_pChar->m_pTargettedIce->m_nAnalyzed)
//		{
//			// Automatically view (doesn't take turn)
//			OnViewIce();
//			return;
//		}

		// Run the program
		if (!DoRunProgramVsIce(pProgram, g_pChar->m_pTargettedIce))
		{
			// Analyze failed
			break;
		}

		OnViewIce();

		break;

	case PROGRAM_SHIELD:
		// See if this is the active shield
		if (pProgram == g_pChar->m_pActiveShield)
		{
			szTxt.Format("%s already active.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}
		else
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Remove the old shield
			if (g_pChar->m_pActiveShield!=NULL)
			{
				// Find what index this item is at
				nIndex = GetProgramIndex(g_pChar->m_pActiveShield);

				// Change the image
				//m_SoftwareView.SetItemState(nIndex, g_pChar->m_pActiveShield->m_nClass, LVIF_IMAGE);
				m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, g_pChar->m_pActiveShield->m_nClass, 0, 0, 0);
			}

			// Now, set the new program as the default
			g_pChar->m_pActiveShield = pProgram;

			// Change the image
			nIndex = GetProgramIndex(pProgram);

			//m_SoftwareView.SetItemState(nIndex, pProgram->m_nClass + NUM_PROGRAMS, LVIF_IMAGE);
			m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, pProgram->m_nClass + NUM_PROGRAMS, 0, 0, 0);

			szTxt.Format("%s activated.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);

			// Show the 'Shield Active' symbol
			UpdateBar(BAR_SHIELD);
			UpdateActiveBar();
		}
		break;

	case PROGRAM_SILENCE:
		// Get the target number
		iTargetNumber = 10 + (3*g_pChar->m_pSystem->m_nRating) - (g_pChar->GetEffectiveRating(RATING_STEALTH) + pProgram->m_nLoadedRating);
		
		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);
		
		if (iSuccess>=0) //Note - only fails on critical failure, but may only be one round
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds. Node is silenced.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);
			
			// Set the node's silence level (At least 2 turns)
			g_pChar->m_pCurrentNode->m_nActiveSilenceLevel = 2 + (2*iSuccess);
			UpdateNodeIcons();
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, RED);
		}
		break;

	case PROGRAM_SMOKE:
		// Get the target number
		iTargetNumber = 10 + (3*g_pChar->m_pSystem->m_nRating) - (g_pChar->GetEffectiveRating(RATING_STEALTH) + pProgram->m_nLoadedRating);
		
		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);
		
		if (iSuccess>=0) //Note - only fails on critical failure, but may only be one round
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds. Node is smoked.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);
			
			// Set the node's silence level (At least two turns)
			g_pChar->m_pCurrentNode->m_nActiveSmokeLevel = 2 + (2*iSuccess);
			UpdateNodeIcons();
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, RED);
		}

		break;

	case PROGRAM_DECOY:
		// Get the target number
		iTargetNumber = 8 + (3*g_pChar->m_pSystem->m_nRating) - (g_pChar->GetEffectiveRating(RATING_STEALTH) + pProgram->m_nLoadedRating);
		
		// Roll the die
		iSuccess = DoDieRoll(iTargetNumber);
		
		if (iSuccess>0)
		{
			// Set the number of decoys
			if (iSuccess > MAX_DECOYS)
			{
				iSuccess = MAX_DECOYS;
			}

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			// Success. Print out a message saying so
			szTxt.Format("%s program succeeds. %d duplicates created.",pProgram->m_szName, iSuccess);
			m_MessageView.AddMessage(szTxt, BLACK);

			// Add the decoys (keep ones already active
			while (g_pChar->m_nDecoyCount < iSuccess)
			{
				// Choose a location
				g_pChar->m_pCurrentNode->ChooseLocation(g_pChar->m_ptDecoyLocation[g_pChar->m_nDecoyCount], DIR_NONE);

				// Draw a decoy
				m_NodeView.DrawDecoy(g_pChar->m_ptDecoyLocation[g_pChar->m_nDecoyCount]);

				// Increment the decoy count
				g_pChar->m_nDecoyCount++;
			}
			
		}
		else
		{
			// We missed. Print out a message saying so
			szTxt.Format("%s program failed.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);
		}
		break;

	case PROGRAM_MEDIC:
		// Medic automatically works, but loses effectiveness
		if (g_pChar->m_nHealthDeck == MAX_HEALTH)
		{
			szTxt.Format("%s failed because you are not damaged.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}
		else
		{
			// Get target number
			iTargetNumber = 8 + (3*g_pChar->m_pSystem->m_nRating) - (pProgram->m_nLoadedRating + g_pChar->GetEffectiveRating(RATING_DEFENSE));

			// Roll the die
			iSuccess = DoDieRoll(iTargetNumber);

			// Did we succeed?
			if (iSuccess>0)
			{
				// Play the program sound effect
				PlayGameSound(pProgram->m_nSound);

				// Heal 4*success (4..20)
				g_pChar->m_nHealthDeck += (iSuccess<<2);
				if (g_pChar->m_nHealthDeck > MAX_HEALTH)
				{
					g_pChar->m_nHealthDeck = MAX_HEALTH;
				}

				// Print a message
				szTxt.Format("%s restored your deck to %d%% health.", pProgram->m_szName, (100/MAX_HEALTH)*g_pChar->m_nHealthDeck);
				m_MessageView.AddMessage(szTxt, BLACK);

				// Succeeded. Just reduce the rating by 1
				pProgram->m_nLoadedRating--;
			}
			else if (iSuccess == -1)
			{
				// Crash on catastrophic failure
				pProgram->m_nLoadedRating = 0;
			}
			else
			{
				// Print a message
				szTxt.Format("%s failed.", pProgram->m_szName);
				m_MessageView.AddMessage(szTxt, BLACK);
			}

			// Crash the program if necessary
			if (pProgram->m_nLoadedRating == 0)
			{
				szTxt.Format("%s has crashed.", pProgram->m_szName);
				m_MessageView.AddMessage(szTxt, BLACK);

				DoRemoveProgram(pProgram);
			}
			else
			{
				// Update the program's rating in the view
				this->UpdateProgramRating(pProgram);
			}

			UpdateBar(BAR_DECK);
		}
		break;

	case PROGRAM_ARMOR:
		// See if this is the active armor
		if (pProgram == g_pChar->m_pActiveArmor)
		{
			szTxt.Format("%s already active.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}
		else
		{
			// Remove the old armor
			if (g_pChar->m_pActiveArmor!=NULL)
			{
				// Find what index this item is at
				nIndex = GetProgramIndex(g_pChar->m_pActiveArmor);

				// Change the image
				//m_SoftwareView.SetItemState(nIndex, g_pChar->m_pActiveArmor->m_nClass, LVIF_IMAGE);
				m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, g_pChar->m_pActiveArmor->m_nClass, 0, 0, 0);
			}

			// Now, set the new program as the default
			g_pChar->m_pActiveArmor = pProgram;

			// Change the image
			nIndex = GetProgramIndex(pProgram);
			//m_SoftwareView.SetItemState(nIndex, pProgram->m_nClass + NUM_PROGRAMS, LVIF_IMAGE);
			m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, pProgram->m_nClass + NUM_PROGRAMS, 0, 0, 0);

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			szTxt.Format("%s activated.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);

			// Show the 'Armor Active' symbol
			UpdateActiveBar();
		}
		break;

	case PROGRAM_HIDE:
		// See if this is the active hide
		if (pProgram == g_pChar->m_pActiveHide)
		{
			szTxt.Format("%s already active.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}
		else
		{
			// Remove the old hide
			if (g_pChar->m_pActiveHide!=NULL)
			{
				// Find what index this item is at
				nIndex = GetProgramIndex(g_pChar->m_pActiveHide);

				// Change the image
				//m_SoftwareView.SetItemState(nIndex, g_pChar->m_pActiveHide->m_nClass, LVIF_IMAGE);
				m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, g_pChar->m_pActiveHide->m_nClass, 0, 0, 0);
			}

			// Now, set the new program as the default
			g_pChar->m_pActiveHide = pProgram;

			// Change the image
			nIndex = GetProgramIndex(pProgram);
			//m_SoftwareView.SetItemState(nIndex, pProgram->m_nClass + NUM_PROGRAMS, LVIF_IMAGE);
			m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, pProgram->m_nClass + NUM_PROGRAMS, 0, 0, 0);

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			szTxt.Format("%s activated.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);

			// Show the 'Hide Active' symbol
			UpdateActiveBar();
		}
		break;

	case PROGRAM_SCAN:
		// What type of node is this
		if (g_pChar->m_pCurrentNode->m_nType == NT_DS)
		{
			// Get the first program to scan
			g_pChar->m_posScanFile = g_pChar->m_pCurrentNode->m_olFileList.GetHeadPosition();
			while (g_pChar->m_posScanFile!=NULL)
			{
				pFile = (CDSFile *)g_pChar->m_pCurrentNode->m_olFileList.GetAt(g_pChar->m_posScanFile);

				// Is this file valid?
				if (pFile->IsValidForScanEval(PROGRAM_SCAN))
				{
					break;
				}

				g_pChar->m_pCurrentNode->m_olFileList.GetNext(g_pChar->m_posScanFile);
			}

			if (g_pChar->m_posScanFile == NULL)
			{
				// No scannable files
				szTxt.Format("There are no valid targets for the %s program in this node.",pProgram->m_szName);
				g_pChar->m_pActiveScan = NULL;
				m_MessageView.AddMessage(szTxt, BLACK);
				UpdateNodeIcons();
			}
			else
			{
				// Start a scan
				g_pChar->m_pActiveScan = pProgram;
				g_pChar->m_nCurrentScanMP = 0;

				// Play the program sound effect
				PlayGameSound(pProgram->m_nSound);

				m_MessageView.AddMessage("Scan started.", BLACK);
				UpdateNodeIcons();
			}
		}
		else if (g_pChar->m_pCurrentNode->m_nType == NT_IO)
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			switch (g_pChar->m_pCurrentNode->m_nSubType)
			{
			case NST_IO_QUEST_NODE	:
				if (g_pChar->m_pCurrentContract->m_nType != CONT_RUN_PROGRAM)
				{
					szTxt.Format("This node controls the %s you are searching for.", g_pChar->m_pCurrentContract->GetTargetObjectText());
					g_pChar->m_pCurrentNode->m_nSpecialImage = NSI_IO_QUEST;
					m_MapView.RedrawWindow();
					m_MessageView.AddMessage(szTxt, BLACK);
					break;
				}
				// For Run Program contracts, fall through
			case NST_IO_USELESS		:
				m_MessageView.AddMessage("This node controls unimportant IO.", BLACK);
				break;
			case NST_IO_ALARM		:
				m_MessageView.AddMessage("This node handles system administrator alarm notification.", BLACK);
				g_pChar->m_pCurrentNode->m_nSpecialImage = NSI_IO_ALARM;
				m_MapView.RedrawWindow();
				break;
			case NST_IO_ICE_PORT	:
				m_MessageView.AddMessage("This node is the entry point for ICE into the system.", BLACK);
				g_pChar->m_pCurrentNode->m_nSpecialImage = NSI_IO_ICE_PORT;
				m_MapView.RedrawWindow();
				break;
			case NST_IO_MATRIX		:
				m_MessageView.AddMessage("This node contains a high-speed connection to cyberspace.", BLACK);
				g_pChar->m_pCurrentNode->m_nSpecialImage = NSI_IO_MATRIX;
				m_MapView.RedrawWindow();
				break;
			}
		}
		else if (g_pChar->m_pCurrentNode->m_nType == NT_IO)
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			switch (g_pChar->m_pCurrentNode->m_nSubType)
			{
			case NST_COP_NORMAL:
				m_MessageView.AddMessage("This is a standard coprocessor.", BLACK);
				break;
			case NST_COP_SECURITY:
				m_MessageView.AddMessage("This is the area security node.", BLACK);
				g_pChar->m_pCurrentNode->m_nSpecialImage = NSI_COP_SECURITY;
				m_MapView.RedrawWindow();
				break;
			}
		}
		else
		{
			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			m_MessageView.AddMessage("There is nothing in this node to scan.", BLACK);
		}
		break;

	case PROGRAM_EVALUATE:
		// Get the first program to evaluate
		g_pChar->m_posScanFile = g_pChar->m_pCurrentNode->m_olFileList.GetHeadPosition();
		while (g_pChar->m_posScanFile!=NULL)
		{
			pFile = (CDSFile *)g_pChar->m_pCurrentNode->m_olFileList.GetAt(g_pChar->m_posScanFile);

			// Is this file valid?
			if (pFile->IsValidForScanEval(PROGRAM_EVALUATE))
			{
				break;
			}

			g_pChar->m_pCurrentNode->m_olFileList.GetNext(g_pChar->m_posScanFile);
		}

		if (g_pChar->m_posScanFile == NULL)
		{
			// No scannable files
			szTxt.Format("There are no valid targets for the %s program in this node.",pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
			g_pChar->m_pActiveScan = NULL;
			UpdateNodeIcons();
		}
		else
		{
			// Start a scan
			g_pChar->m_pActiveScan = pProgram;
			g_pChar->m_nCurrentScanMP = 0;

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			m_MessageView.AddMessage("Evaluation started.", BLACK);
		}
		break;

	case PROGRAM_REFLECT:
		// See if this is the active reflect
		if (pProgram == g_pChar->m_pActiveReflect)
		{
			szTxt.Format("%s already active.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}
		else
		{
			// Remove the old reflect
			if (g_pChar->m_pActiveReflect!=NULL)
			{
				// Find what index this item is at
				nIndex = GetProgramIndex(g_pChar->m_pActiveReflect);

				// Change the image
				//m_SoftwareView.SetItemState(nIndex, g_pChar->m_pActiveReflect->m_nClass, LVIF_IMAGE);
				m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, g_pChar->m_pActiveReflect->m_nClass, 0, 0, 0);
			}

			// Now, set the new program as the default
			g_pChar->m_pActiveReflect = pProgram;

			// Change the image
			nIndex = GetProgramIndex(pProgram);
			//m_SoftwareView.SetItemState(nIndex, pProgram->m_nClass + NUM_PROGRAMS, LVIF_IMAGE);
			m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, pProgram->m_nClass + NUM_PROGRAMS, 0, 0, 0);

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			szTxt.Format("%s activated.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);

			// Show the 'Reflect Active' symbol
			UpdateActiveBar();
		}
		break;

	case PROGRAM_ATTACK_BOOST:
	case PROGRAM_DEFENSE_BOOST:
	case PROGRAM_STEALTH_BOOST:
	case PROGRAM_ANALYSIS_BOOST:
		// See if this is the active boost
		if (pProgram == g_pChar->m_pActiveBoost)
		{
			szTxt.Format("%s already active.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);
		}
		else
		{
			// Remove the old boost
			if (g_pChar->m_pActiveBoost != NULL)
			{
				// Find what index this item is at
				nIndex = GetProgramIndex(g_pChar->m_pActiveBoost);

				// Change the image
				//m_SoftwareView.SetItemState(nIndex, g_pChar->m_pActiveReflect->m_nClass, LVIF_IMAGE);
				m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, g_pChar->m_pActiveBoost->m_nClass, 0, 0, 0);
			}

			// Now, set the new program as the default
			g_pChar->m_pActiveBoost = pProgram;

			// Change the image
			nIndex = GetProgramIndex(pProgram);
			//m_SoftwareView.SetItemState(nIndex, pProgram->m_nClass + NUM_PROGRAMS, LVIF_IMAGE);
			m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, pProgram->m_nClass + NUM_PROGRAMS, 0, 0, 0);

			// Play the program sound effect
			PlayGameSound(pProgram->m_nSound);

			szTxt.Format("%s activated.", pProgram->m_szName);
			m_MessageView.AddMessage(szTxt, BLUE);

			// Show the 'Boost Active' symbol
			UpdateActiveBar();
		}
		break;

	case PROGRAM_CLIENT:
		// Are we in a target node?
		if (((g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_CPU) && (g_pChar->m_pCurrentNode->m_nType == NT_CPU)) ||
			((g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_IO) && (g_pChar->m_pCurrentNode->m_nType == NT_IO) && (g_pChar->m_pCurrentNode->m_nSubType == NST_IO_QUEST_NODE)) ||
			((g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_DS) && (g_pChar->m_pCurrentNode->m_nType == NT_DS) && (g_pChar->m_pCurrentNode->m_nSubType == NST_DS_QUEST_NODE)))
		{
			// We are in the correct node. 
			// Is the program already running?
			if (g_pChar->m_nClientProgramStatus != 0)
			{
				// Program is already running.
				m_MessageView.AddMessage("Client program is already running.", BLUE);
			}
			else
			{
				// Start program running.
				g_pChar->m_nClientProgramStatus = 6 + (pProgram->m_nRating/2);

				// Change the image
				nIndex = GetProgramIndex(pProgram);
				m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, pProgram->m_nClass + NUM_PROGRAMS, 0, 0, 0);

				// Play the program sound effect
				PlayGameSound(pProgram->m_nSound);

				szTxt.Format("Client program activated. %d rounds until completion.", g_pChar->m_nClientProgramStatus-1);
				m_MessageView.AddMessage(szTxt, BLUE);

				// Show the 'Special Active' symbol
				UpdateNodeIcons();
			}
		}
		else
		{
			// Show where the correct nodes are
			CObList olNodes;
			CNode *pNode;

			if (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_IO)
			{
				g_pChar->m_pSystem->BuildNodeList(olNodes, NT_IO, TRUE);
			}
			else if (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_DS)
			{
				g_pChar->m_pSystem->BuildNodeList(olNodes, NT_DS, TRUE);
			}
			else //if (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_CPU)
			{
				olNodes.AddTail(g_pChar->m_pSystem->m_pSystemCPU);
			}

			// Go through the list of nodes, looking for the IO node
			while (!olNodes.IsEmpty())
			{
				pNode = (CNode *)olNodes.RemoveHead();

				if (!pNode->m_bActivated)
				{
					// This node has not been activated. Let the user know
					szTxt.Format("Target node %s located", pNode->m_szName);
					m_MessageView.AddMessage(szTxt, BLUE);
				}

				pNode->m_bMapped = TRUE;

				if (pNode->m_nType == NT_IO)
				{
					pNode->m_nSpecialImage = NSI_IO_QUEST;
				}
				else if (pNode->m_nType == NT_DS)
				{
					pNode->m_nSpecialImage = NSI_DS_QUEST;
				}
			}

			m_MapView.RedrawWindow();
		}
	}

	// End the turn
	DoEndPlayerTurn();
}

int CMatrixView::GetProgramIndex(CProgram *pProgram)
{
	LVFINDINFO lvFindInfo;

	memset(&lvFindInfo,0,sizeof(LVFINDINFO));
	lvFindInfo.flags = LVFI_PARAM;
	lvFindInfo.lParam = (LPARAM)pProgram;
	return m_SoftwareView.FindItem(&lvFindInfo);
}

void CMatrixView::DoRemoveProgram(CProgram *pProgram)
{
	int iIndex;

	// Remove the program from the software list
	iIndex = GetProgramIndex(pProgram);
	m_SoftwareView.DeleteItem(iIndex);

	// Check to see if this was a default/active program
	if (pProgram == g_pChar->m_pDefAttackProgram)
	{
		g_pChar->m_pDefAttackProgram = NULL;
		UpdateControls();
	}
	else if (pProgram == g_pChar->m_pActiveArmor)
	{
		g_pChar->m_pActiveArmor = NULL;
		UpdateActiveBar();
	}
	else if (pProgram == g_pChar->m_pActiveShield)
	{
		g_pChar->m_pActiveShield = NULL;
		UpdateBar(BAR_SHIELD);
		UpdateActiveBar();
	}
	else if (pProgram == g_pChar->m_pActiveHide)
	{
		g_pChar->m_pActiveHide = NULL;
		UpdateActiveBar();
	}
	else if (pProgram == g_pChar->m_pActiveScan)
	{
		g_pChar->m_pActiveScan = NULL;
		UpdateActiveBar();
	}
	else if (pProgram == g_pChar->m_pActiveReflect)
	{
		g_pChar->m_pActiveReflect = NULL;
		UpdateActiveBar();
	}
	else if (pProgram == g_pChar->m_pActiveBoost)
	{
		g_pChar->m_pActiveBoost = NULL;
		UpdateActiveBar();
	}
	else if ((pProgram->m_nClass == PROGRAM_CLIENT) && (g_pChar->m_nClientProgramStatus != 0))
	{
		g_pChar->m_nClientProgramStatus = 0;
		UpdateNodeIcons();
	}

	// Recalculate the deck load
	g_pChar->CalcCurrentLoad();

	// Update the program controls
	UpdateControls();
}

void CMatrixView::UpdateProgramRating(CProgram *pProgram)
{
	int iIndex;
	CString szText;

	// Get the index of the program
	iIndex = GetProgramIndex(pProgram);
	if (iIndex==-1) return;

	if (pProgram->m_nLoadedRating==pProgram->m_nRating)
	{
		szText.Format("%d",pProgram->m_nRating);
	}
	else
	{
		szText.Format("%d (%d)",pProgram->m_nLoadedRating,pProgram->m_nRating);
	}
	m_SoftwareView.SetItemText(iIndex,1,szText);
}

void CMatrixView::UpdateActiveBar()
{
	// Armor
	m_abActiveChar.m_bActive[CHAR_ARMOR] = (g_pChar->m_pActiveArmor != NULL);
	
	// Shield
	m_abActiveChar.m_bActive[CHAR_SHIELD] = (g_pChar->m_pActiveShield != NULL);

	// Hide
	m_abActiveChar.m_bActive[CHAR_HIDE] = (g_pChar->m_pActiveHide != NULL);

	// Reflect
	m_abActiveChar.m_bActive[CHAR_REFLECT] = (g_pChar->m_pActiveReflect != NULL);

	// Boost
	if (g_pChar->m_pActiveBoost == NULL)
	{
		m_abActiveChar.m_bActive[CHAR_BOOST] = FALSE;
	}
	else
	{
		m_abActiveChar.m_nImageIndex[CHAR_BOOST] = g_pChar->m_pActiveBoost->m_nClass;
		m_abActiveChar.m_bActive[CHAR_BOOST] = TRUE;
	}

	m_abActiveChar.RedrawWindow();
}

void CMatrixView::DoEndPlayerTurn()
{
	POSITION p;
	CIce *pIce, *pIce2;
	CObList m_olTmp;
	CString szTxt;
	int nIndex;
	CDSFile *pFile;
	CString szTmp;

	// Update target ice health
	UpdateTarget();

	//------------------
	// Handle transfers
	//------------------
	if (g_pChar->m_pTransferProgram!=NULL)
	{
		// Decrement the transfer time
		g_pChar->m_nTransferTurnsLeft--;

		// Check for completion
		if (g_pChar->m_nTransferTurnsLeft == 0)
		{
			szTxt.Format("Program %s has finished loading.", g_pChar->m_pTransferProgram->m_szName);
			m_MessageView.AddMessage(szTxt,BLUE);

			// Adjust the loaded rating
			g_pChar->m_pTransferProgram->m_nLoadedRating = g_pChar->m_pTransferProgram->m_nRating;

			// Add/update the program
			nIndex = GetProgramIndex(g_pChar->m_pTransferProgram);
			if (nIndex == -1)
			{
				// Add it to the list.
				// First, find out where to insert it
				p = g_pChar->m_olSoftware.Find(g_pChar->m_pTransferProgram);
				for (;;)
				{
					g_pChar->m_olSoftware.GetPrev(p);
					if (p==NULL)
					{
						// First of list
						nIndex = 0;
						break;
					}
					else
					{
						nIndex = GetProgramIndex((CProgram *)g_pChar->m_olSoftware.GetAt(p));
						if (nIndex!=-1)
						{
							nIndex++;
							break;
						}
					}
				}
				m_SoftwareView.InsertItem(
					nIndex,
					g_pChar->m_pTransferProgram->m_szName,
					g_pChar->m_pTransferProgram->m_nClass);

				m_SoftwareView.SetItemData(nIndex,(DWORD)g_pChar->m_pTransferProgram);

				g_pChar->CalcCurrentLoad();
			}
			else
			{
				// Update in list will occur below.
				// If we are reloading the active shield, update the bar
				if (g_pChar->m_pTransferProgram == g_pChar->m_pActiveShield)
				{
					UpdateBar(BAR_SHIELD);
				}
			}

			PlayGameSound(SOUND_PROGRAMLOADED);

			szTxt.Format("%d",g_pChar->m_pTransferProgram->m_nRating);
			m_SoftwareView.SetItemText(nIndex,1,szTxt);

			g_pChar->m_pTransferProgram = NULL;
		}

	}
	else if (g_pChar->m_pTransferFile!=NULL)
	{
		// Decrement the transfer time
		g_pChar->m_nTransferTurnsLeft--;

		// Check for completion
		if (g_pChar->m_nTransferTurnsLeft == 0)
		{
			// Transfer complete
			if (g_pChar->m_nFileTransferType == FO_EDIT)
			{
				// Edit complete
				PlayGameSound(SOUND_EDITCOMPLETE);
				szTxt.Format("File %s has been edited.", g_pChar->m_pTransferFile->m_szName);
				g_pChar->m_pTransferFile->m_nState |= STATE_EDITED_N;
			}
			else
			{
				// Download complete
				PlayGameSound(SOUND_DOWNLOADCOMPLETE);
				szTxt.Format("File %s has been downloaded.", g_pChar->m_pTransferFile->m_szName);
				g_pChar->m_pTransferFile->m_nState |= STATE_IN_DECK;
				if (g_pChar->m_pTransferFile->m_nState & STATE_EDITED_N)
				{
					// An edited file which is downloaded will have no value (even as a quest item)
					g_pChar->m_pTransferFile->m_nState |= STATE_EDITED_D;
				}
			}
			m_MessageView.AddMessage(szTxt,BLUE);

			g_pChar->m_pTransferFile = NULL;
		}
	}
	UpdateBar(BAR_TRANSFER);

	//---------------------------
	// Handles scans/evaluations
	//---------------------------
	if (g_pChar->m_pActiveScan!=NULL)
	{
		POSITION posOriginal = g_pChar->m_posScanFile;

		// Increment the amount scanned (Rating * 5mp per turn)
		g_pChar->m_nCurrentScanMP += (5 * g_pChar->m_pActiveScan->m_nLoadedRating);

		// Make sure the current file is still ok, and if not, get the next legal scan file
		pFile = (CDSFile *)g_pChar->m_pCurrentNode->m_olFileList.GetAt(g_pChar->m_posScanFile);
		while (!pFile->IsValidForScanEval(g_pChar->m_pActiveScan->m_nClass))
		{
			g_pChar->m_pCurrentNode->m_olFileList.GetNext(g_pChar->m_posScanFile);
			if (g_pChar->m_posScanFile == NULL)
			{
				g_pChar->m_posScanFile = g_pChar->m_pCurrentNode->m_olFileList.GetHeadPosition();
			}
			if (g_pChar->m_posScanFile == posOriginal)
			{
				g_pChar->m_posScanFile = NULL;
				break;
			}
			pFile = (CDSFile *)g_pChar->m_pCurrentNode->m_olFileList.GetAt(g_pChar->m_posScanFile);
		}

		// If we reached the end, terminate the scan
		if (g_pChar->m_posScanFile == NULL)
		{
			// No valid files left
			if (g_pChar->m_pActiveScan->m_nClass == PROGRAM_SCAN)
			{
				PlayGameSound(SOUND_SCANCOMPLETE);
			}
			else
			{
				PlayGameSound(SOUND_EVALCOMPLETE);
			}
			szTxt.Format("%s has completed.", g_pChar->m_pActiveScan->m_szName);
			m_MessageView.AddMessage(szTxt,BLUE);
			g_pChar->m_pActiveScan = NULL;
			UpdateNodeIcons();
		}
		else
		{
			// Check to see if the file is scanned
			if (pFile->m_nSize <= g_pChar->m_nCurrentScanMP)
			{
				// Mark file as scanned/evaluated, then get the next file
				if (g_pChar->m_pActiveScan->m_nClass == PROGRAM_SCAN)
				{
					// Mark file as scanned
					pFile->m_nState |= STATE_SCAN;

					// Send a message
					szTxt.Format("File %s has been scanned. (%s)", pFile->m_szName, pFile->GetScanString());
					m_MessageView.AddMessage(szTxt,BLACK);

					// Check for target file //FSO 6-20-01
					if (pFile->m_nType == FT_QUEST)
					{
						g_pChar->m_pCurrentNode->m_nSpecialImage = NSI_DS_QUEST;
						m_MapView.RedrawWindow();
					}
				}
				else
				{
					// Mark file as evaluated
					pFile->m_nState |= STATE_EVAL;

					// Process according to type
					if (pFile->m_nType == FT_VALUABLE)
					{
						// Send a message
						szTxt.Format("File %s has been evaluated. (%d)", pFile->m_szName, pFile->m_nValue);
					}
					else if (pFile->m_nType == FT_CLUE)
					{
						// Send a message
						g_pChar->m_pSystem->MarkContractNodes(this);
						m_MapView.RedrawWindow();

						szTxt.Format("File %s has been evaluated. <Clue>", pFile->m_szName);
					}
					else if (pFile->m_nType == FT_PASSCODE)
					{
						g_pChar->m_pSystem->m_bPasscodeAccessed = TRUE;

						m_MessageView.AddMessage("Passcode file accessed!", BLUE);
						szTxt.Format("File %s has been evaluated. <Passcode>", pFile->m_szName);
					}
					else if (pFile->m_nType == FT_PROGRAM)
					{
						GetSoftwareText(szTmp, pFile->m_nContents, pFile->m_nValue);
						szTxt.Format("File %s has been evaluated. <%s>", pFile->m_szName, szTmp);
					}
					else if (pFile->m_nType == FT_P_SOURCE)
					{
						GetSoftwareText(szTmp, pFile->m_nContents, pFile->m_nValue);
						szTxt.Format("File %s has been evaluated. <%s Source>", pFile->m_szName, szTmp);
					}
					else if (pFile->m_nType == FT_C_SOURCE)
					{
						szTxt.Format("File %s has been evaluated. <%s %d Source>", pFile->m_szName, GetChipName(pFile->m_nContents), pFile->m_nValue);
					}
					m_MessageView.AddMessage(szTxt,BLACK);
				}

				// Get the next file
				while (!pFile->IsValidForScanEval(g_pChar->m_pActiveScan->m_nClass))
				{
					g_pChar->m_pCurrentNode->m_olFileList.GetNext(g_pChar->m_posScanFile);
					if (g_pChar->m_posScanFile == NULL)
					{
						g_pChar->m_posScanFile = g_pChar->m_pCurrentNode->m_olFileList.GetHeadPosition();
					}
					if (g_pChar->m_posScanFile == posOriginal)
					{
						g_pChar->m_posScanFile = NULL;
						break;
					}
					pFile = (CDSFile *)g_pChar->m_pCurrentNode->m_olFileList.GetAt(g_pChar->m_posScanFile);
				}

				// See if we are done
				if (g_pChar->m_posScanFile == NULL)
				{
					// No valid files left
					if (g_pChar->m_pActiveScan->m_nClass == PROGRAM_SCAN)
					{
						PlayGameSound(SOUND_SCANCOMPLETE);
					}
					else
					{
						PlayGameSound(SOUND_EVALCOMPLETE);
					}
					szTxt.Format("%s has completed.", g_pChar->m_pActiveScan->m_szName);
					m_MessageView.AddMessage(szTxt,BLUE);
					g_pChar->m_pActiveScan = NULL;
					UpdateNodeIcons();
				}
				else
				{
					// Reduce the points scanned for the next file
					g_pChar->m_nCurrentScanMP -= pFile->m_nSize;
				}
			}
		}
	}

	//-------------------------
	// Handle special programs
	//-------------------------
	if (g_pChar->m_nClientProgramStatus != 0)
	{
		// Decrement the timer
		g_pChar->m_nClientProgramStatus--;
		if (g_pChar->m_nClientProgramStatus == 0)
		{
			CProgram *pProgram;

			// Program complete
			m_MessageView.AddMessage("Client program has completed execution.",BLUE);

			// Mark node as accessed
			g_pChar->m_pCurrentNode->m_bActivated = TRUE;

			pProgram = g_pChar->FindProgram(PROGRAM_CLIENT, TRUE);
			nIndex = GetProgramIndex(pProgram);
			m_SoftwareView.SetItem(nIndex, 0, LVIF_IMAGE, NULL, pProgram->m_nClass, 0, 0, 0);
		}
	}

	//-----------------------
	// Handle trace attempts
	//-----------------------
	if (g_pChar->m_pTraceIce!=NULL)
	{
		g_pChar->m_nTraceTurnsLeft--;

		if (g_pChar->m_nTraceTurnsLeft == 0)
		{
			// Trace is complete
			if (g_pChar->m_nTraceType==TST_NORMAL)
			{
				szTxt.Format("%s has completed its trace. All ice will now be hostile.", g_pChar->m_pTraceIce->m_szName);
				m_MessageView.AddMessage(szTxt,RED);
				g_pChar->m_bTraced = TRUE;
				g_pChar->m_pTraceIce = NULL; //FSO 8-7-01 Bug fix

				// Mark ice in current node as hostile
				MarkIceAsHostile();
			}
			else if (g_pChar->m_nTraceType == TST_DUMP)
			{
				// Ice dumps the decker
				DoDumpDecker(DUMP_TRACE);
				return;
			}
			else //TRACE & FRY
			{
				// Ice gets to randomly fry a chip
				DoDumpDecker(DUMP_TRACE_FRY);
				return;
			}
		}
	}
	UpdateBar(BAR_TRACE);

	//-----------------
	// Handle the ICE. 
	//-----------------
	// First, sort according to sensor strength so that the higher rated ICE goes first
	// This allows the strongest (sensor-wise) ice to query the player firs. This also
	// allows the strongest trace program to attack first.
	m_olTmp.AddTail(&g_pChar->m_pSystem->m_olIceList);
	g_pChar->m_pSystem->m_olIceList.RemoveAll();

	while (!m_olTmp.IsEmpty())
	{
		// Remove the first ice
		pIce = (CIce *)m_olTmp.RemoveHead();

		// Add it to the list
		// If this ICE has queried, add it to the head of the list
		if (pIce->HasQueried())
		{
			p = NULL;
		}
		else
		{
			p = g_pChar->m_pSystem->m_olIceList.GetTailPosition();
			while (p!=NULL)
			{
				pIce2 = (CIce *)g_pChar->m_pSystem->m_olIceList.GetAt(p);

				if (pIce->GetEffectiveRating(RATING_SENSORS, FALSE) >= pIce2->GetEffectiveRating(RATING_SENSORS, FALSE))
				{
					break;
				}

				g_pChar->m_pSystem->m_olIceList.GetPrev(p);
			}
		}

		if (p==NULL)
		{
			// Insert at head
			g_pChar->m_pSystem->m_olIceList.AddHead(pIce);
		}
		else
		{
			// Insert after position
			g_pChar->m_pSystem->m_olIceList.InsertAfter(p, pIce);
		}
	}

	// Now, do the ICE movement
	p = g_pChar->m_pSystem->m_olIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);

		if (pIce->DoAction(this))
		{
			// Run has ended
			return;
		}

		// Reset some ice stuff
		pIce->m_bWasAccessed = FALSE;
	}

	//---------------
	// Handle damage
	//---------------
	// Do mental damage
	//FSO 5-31-04 Bug fix
	g_pChar->m_nHealthMental -= g_pChar->m_nDamageMental;
	UpdateBar(BAR_MENTAL);

	// Check for dump
	if (g_pChar->m_nHealthMental < 1)
	{
		// Modify physical health
		g_pChar->m_nHealthPhysical += g_pChar->m_nHealthMental;
		UpdateBar(BAR_LETHAL);

		// Dump the decker
		if (g_pChar->m_nHealthPhysical < 1)
		{
			DoDumpDecker(DUMP_DEATH);
		}
		else
		{
			DoDumpDecker(DUMP_UNCONS);
		}
		return;
	}

	// Do deck damage
	g_pChar->m_nHealthDeck -= g_pChar->m_nDamageDeck;
	UpdateBar(BAR_DECK);

	if (g_pChar->m_nHealthDeck < 1)
	{
		// Extra deck damage becomes physical damage
		g_pChar->m_nHealthPhysical += g_pChar->m_nHealthDeck;
		UpdateBar(BAR_LETHAL);

		// Dump the decker
		if (g_pChar->m_nHealthPhysical < 1)
		{
			DoDumpDecker(DUMP_DEATH);
		}
		else
		{
			DoDumpDecker(DUMP_DECK_DAMAGE);
		}
		return;
	}

	// Reset damage
	g_pChar->m_nDamageMental = 0;
	g_pChar->m_nDamageDeck = 0;

	//----------------------
	// Handle node programs
	//----------------------
	p = g_pChar->m_pSystem->m_olAreaList.GetHeadPosition();
	while (p!=NULL)
	{
		CArea *pArea = (CArea *)g_pChar->m_pSystem->m_olAreaList.GetNext(p);

		POSITION p2 = pArea->m_olNodeList.GetHeadPosition();
		while (p2!=NULL)
		{
			CNode *pNode = (CNode *)pArea->m_olNodeList.GetNext(p2);

			// Check silence
			if (pNode->m_nActiveSilenceLevel)
			{
				pNode->m_nActiveSilenceLevel--;
				if (pNode->m_nActiveSilenceLevel == 0)
				{
					szTxt.Format("Node %s is no longer silenced.", pNode->m_szName);
					m_MessageView.AddMessage(szTxt,ORANGE);
				}
			}

			// Check smoke
			if (pNode->m_nActiveSmokeLevel)
			{
				pNode->m_nActiveSmokeLevel--;
				if (pNode->m_nActiveSmokeLevel == 0)
				{
					szTxt.Format("Node %s is no longer smoked.", pNode->m_szName);
					m_MessageView.AddMessage(szTxt,ORANGE);
				}
			}
		}
	}

	//-----------------------
	// Handle boost programs
	//-----------------------
	if (g_pChar->m_pActiveBoost != NULL)
	{
		g_pChar->m_pActiveBoost->m_nLoadedRating--;
		if (g_pChar->m_pActiveBoost->m_nLoadedRating == 0)
		{
			// Crash the program
			szTxt.Format("Your boost program %s has expired.", g_pChar->m_pActiveBoost->m_szName);
			m_MessageView.AddMessage(szTxt, RED);
			DoRemoveProgram(g_pChar->m_pActiveBoost);
			UpdateActiveBar();
		}
		else
		{
			// Just update it
			UpdateProgramRating(g_pChar->m_pActiveBoost);
		}
	}

	//---------------------------
	// Check for system shutdown
	//---------------------------
	// If the system is in red alert and the external alarm system has not been deactivated,
	// then there is a chance that the system may be taken offline.
	if ((g_pChar->m_pSystem->m_nAlert == ALERT_RED) && 
		(g_pChar->m_pSystem->m_nTurnsUntilCrash==0) &&
		(!g_pChar->m_pSystem->m_bExternalAlarmsDeactivated))
	{
		// Chance of shutdown relative to system level - .1% per system rating per turn
		if (Random(1000) < g_pChar->m_pSystem->m_nRating)
		{
			// System alert has been noticed. System will be taken offline in 15*#Areas seconds (turns)
			PlayGameSound(SOUND_SHUTDOWN_COMMENCE);

			g_pChar->m_pSystem->m_nTurnsUntilCrash = (((g_pChar->m_pSystem->m_nRating+3)/4) * 15);
			szTxt.Format("System shutdown commencing. System will be offline in %d seconds.", g_pChar->m_pSystem->m_nTurnsUntilCrash);
			m_MessageView.AddMessage(szTxt, RED);
			m_abAlert.RedrawWindow();
		}
	}
	// Check for shutdown in progress
	else if (g_pChar->m_pSystem->m_nTurnsUntilCrash)
	{
		g_pChar->m_pSystem->m_nTurnsUntilCrash --;

		if (g_pChar->m_pSystem->m_nTurnsUntilCrash == 30)
			m_MessageView.AddMessage("30 seconds until system shutdown.", RED);
		else if (g_pChar->m_pSystem->m_nTurnsUntilCrash == 10)
			m_MessageView.AddMessage("10 seconds until system shutdown.", RED);
		else if (g_pChar->m_pSystem->m_nTurnsUntilCrash==0)
		{
			// Time for shutdown
			DoDumpDecker(DUMP_SYS_OFFLINE);
			return;
		}
	}

	//-------------------------
	// Handle ICE reactivation
	//-------------------------
	if (!g_pChar->m_pSystem->m_olDeadIceList.IsEmpty())
	{
		// Check the reactivate time
		if (g_pChar->m_pSystem->m_nIceReactivateTime == 0)
		{
			// Reset it
			g_pChar->m_pSystem->SetReactivateTime(FALSE);
		}
		else
		{
			// Just decrement
			g_pChar->m_pSystem->m_nIceReactivateTime--;

			if (g_pChar->m_pSystem->m_nIceReactivateTime == 0)
			{
				// Time to reactivate something
				for (;;)
				{
					// Remove the lead ice from the list
					pIce = (CIce *)g_pChar->m_pSystem->m_olDeadIceList.RemoveHead();

					// See if his area ice portal is active
					if (pIce->m_pHomeNode->m_pParentArea->m_pIceIONode != NULL)
					{
						// Activate this ice
						pIce->m_nHealth = MAX_HEALTH;
						pIce->m_bBypassed = FALSE;
						pIce->m_nSlowLevel = 0;
						pIce->m_bSkippedLastTurn = FALSE;
						pIce->m_nConfusionLevel = 0;
						pIce->m_nWeakenLevel = FALSE;
						pIce->m_nVirusLevel = FALSE;
						pIce->m_pCurrentNode = pIce->m_pHomeNode->m_pParentArea->m_pIceIONode;
						if ((pIce->m_nType == ICE_PROBE) || (pIce->m_bResponse))
							pIce->m_nState = STATE_SEARCHING;
						else
							pIce->m_nState = STATE_MOVING;
						pIce->m_bWasAccessed = FALSE;
						pIce->m_pTargetNode = pIce->m_pHomeNode;

						g_pChar->m_pSystem->m_olIceList.AddTail(pIce);

						pIce->DoMove(DIR_NONE, this);
						break;
					}
					else
					{
						// Delete this ice
						delete pIce;
					}

					// If the ice list is empty, we are done
					if (g_pChar->m_pSystem->m_olDeadIceList.IsEmpty())
					{
						break;
					}
				}

				// If the dead ice list is not empty, reset the time
				if (!g_pChar->m_pSystem->m_olDeadIceList.IsEmpty())
				{
					g_pChar->m_pSystem->SetReactivateTime(TRUE);
				}
			}
		}
	}

	//--------------
	// Handle clock
	//--------------
	g_pChar->m_nRunTime++;
	UpdateClock();

	//--------------------
	// Update other items
	//--------------------
	// Enable the buttons
	UpdateControls();
	UpdateTarget(); // In case ice was harmed by virus or confusion
	UpdateNodeIcons();
}

void CMatrixView::DoQuery(CIce *pIce)
{
	CString szTxt;

	// Send a message
	szTxt.Format("%s has queried you.",pIce->m_szName);
	m_MessageView.AddMessage(szTxt,RED);

	// Show the graphic
	m_NodeView.DrawQuery(pIce);
}

void CMatrixView::DoDumpDecker(int nCause)
{
	CMissionResultsDlg dlgResult(this);
	CString szTxt;
	int iTargetNumber;
	int iSuccess;
	int iChip;
	int nSound;
	BOOL bTrace = FALSE;

	//-----------------------
	// Show the dump message
	//-----------------------
	switch (nCause)
	{
	case DUMP_DECK_DAMAGE:
		nSound = SOUND_DUMPDECKDAMAGE;
		szTxt = "You have been dumped!";
		if ((g_pChar->m_pTraceIce != NULL) && 
			(g_pChar->m_pTraceIce->m_nSubType != TST_NORMAL) &&
			(g_pChar->m_bTraced))
		{
			bTrace = TRUE;
		}
		break;
	case DUMP_UNCONS:
		nSound = SOUND_DUMPUNCONSCIOUS;
		szTxt = "You have fallen unconscious, and been dumped!";
		if ((g_pChar->m_pTraceIce != NULL) && 
			(g_pChar->m_pTraceIce->m_nSubType != TST_NORMAL) &&
			(g_pChar->m_bTraced))
		{
			bTrace = TRUE;
		}
		break;
	case DUMP_DEATH:
		nSound = SOUND_DUMPDEATH;
		if (g_pChar->m_nHardware[HW_BIO_MONITOR] >= 2)
		{
			// Saved by autodump
			szTxt = "You have been save by your BioMonitor's AutoDump.";
			g_pChar->m_nHealthPhysical = 1;
		}
		else
		{
			// Death!
			DeckerMessageBox("You have died!\n(Does anyone smell grey matter burning?)");
			SendMessage(WM_CLOSE);
			PostQuitMessage(0);
			return;
		}
		break;
	case DUMP_TRACE:
		nSound = SOUND_DUMPTRACE;
		szTxt = "An extended trace has completed and you have been dumped as a result.";
		bTrace = TRUE;
		break;
	case DUMP_TRACE_FRY:
		nSound = SOUND_DUMPTRACE;

		// Choose a random chip
		iChip = Random(NUM_CHIPS);

		// If chip is already 0, no chance to fry
		if (g_pChar->m_nChip[iChip]!=0)
		{
			// Calculate the target number 
			// Note that this is a roll of purely ICE vs (chip + surge protector)
			iTargetNumber = 10 - g_pChar->m_pTraceIce->GetEffectiveRating(RATING_COMBAT, FALSE) + 
								  (g_pChar->m_nChip[iChip] + g_pChar->m_nHardware[HW_SURGE_SUPP]);

			// Roll the die
			iSuccess = DoDieRoll(iTargetNumber);
		}
		else 
		{
			iSuccess = 0;
		}

		if (iSuccess < 1)
		{
			// Fry was unsuccessful
			szTxt = "An extended trace has completed and you have been dumped as a result.";
		}
		else
		{
			if (iSuccess > g_pChar->m_nChip[iChip])
			{
				iSuccess = g_pChar->m_nChip[iChip];
			}

			szTxt.Format("An extended trace has completed and you have been dumped as a result. In addition, your %s chip has been fried. The rating has been reduced by %d.",
							GetChipName(iChip), iSuccess);
			g_pChar->m_nChip[iChip] -= iSuccess;
		}
		bTrace = TRUE;
		break;
	case DUMP_SYS_CRASH:
		nSound = SOUND_DUMPSYSCRASH;
		szTxt = "You have crashed the system.";
		break;
	case DUMP_SYS_OFFLINE:
		nSound = SOUND_DUMPSYSOFFLINE;
		szTxt = "The system has been taken offline due to an intruder alert.";
		break;
	case DUMP_DISCONNECT:
		nSound = SOUND_DUMPDISCONNECT;
		szTxt = "You have disconnected from the matrix.";
		break;
	}
	PlayGameSound(nSound);
	DeckerMessageBox(szTxt);

	//----------------------------
	// Check for completed traces
	//----------------------------
	// If trace completed, there is a 10% chance per system level of calling police
	if (bTrace) 
	{
		// Check for Anti-trace Proxy
		if (g_pChar->m_nHardware[HW_PROXY])
		{
			DeckerMessageBox("An extended trace was completed against you. Your AntiTrace Proxy has destroyed itself to defeat the trace.");
			g_pChar->m_nHardware[HW_PROXY] = 0;
		}
		else if (Random(10) <= (g_pChar->m_pSystem->m_nCorporation/5))
		{
			DeckerMessageBox("The police have been sent to arrest you. You are forced to abandon your home to avoid capture. Your lifestyle has dropped to poverty.");
			g_pChar->m_nLifestyle = 0;

			// Clear all contracts, then generate new ones based on new lifestyle
			g_pChar->ClearContracts();
			g_pChar->GenerateContracts();
		}
	}

	//-------------------
	// Check for success
	//-------------------
	int nResult;
	CString szOverall;
	CString szBreakdown;

	// Compute the results of the run
	nResult = GetContractStatus(szOverall, szBreakdown, FALSE, (nCause == DUMP_SYS_CRASH));

	// We now have success/failure. Compute other things
	int nPay;
	int nFileValue;
	int nTotal;
	int nSkillPoints;
	int nRepChange;
	CObList olFiles;
	CDSFile *pFile;

	// Built a list of all files for money calculation
	g_pChar->m_pSystem->BuildFileList(olFiles, FALSE);

	// Get total values of files
	nFileValue = 0;
	while (!olFiles.IsEmpty())
	{
		pFile = (CDSFile *)olFiles.RemoveHead();
		if ((pFile->m_nState & STATE_IN_DECK) && 
			((pFile->m_nState & STATE_EDITED_D)==0))
		{
			// File is downloaded unedited. What type is it?
			if (pFile->m_nType == FT_VALUABLE)
			{
				// Just add to credits
				nFileValue += pFile->m_nValue;
			}
			else if (pFile->m_nType == FT_PROGRAM)
			{
				// Add the program to the deck
				CProgram *pProgram;
				pProgram = new CProgram;
				pProgram->GetInitValues(pFile->m_nContents, pFile->m_nValue);
				g_pChar->m_olSoftware.AddTail(pProgram);
			}
			else if (pFile->m_nType == FT_P_SOURCE)
			{
				CSource *pSource;

				pSource = new CSource;
				pSource->m_nType = IT_SOFTWARE;
				pSource->m_nClass = pFile->m_nContents;
				pSource->m_nRating = pFile->m_nValue;
				g_pChar->m_olSourceCode.AddTail(pSource);
			}
			else if (pFile->m_nType == FT_C_SOURCE)
			{
				CSource *pSource;

				pSource = new CSource;
				pSource->m_nType = IT_CHIP;
				pSource->m_nClass = pFile->m_nContents;
				pSource->m_nRating = pFile->m_nValue;
				g_pChar->m_olSourceCode.AddTail(pSource);
			}
		}
	}

	// Compute the effects on reputation, skill points, and money
	g_pChar->m_pCurrentContract->GetTypeText(dlgResult.m_contract);
	if (nResult == -1)
	{
		nPay = 0;
		nSkillPoints = 0;

		// Lose points equal to run
		g_pChar->m_nRepPoints -= g_pChar->m_pCurrentContract->m_nDifficulty;
		if (g_pChar->m_nRepPoints<0)
		{
			g_pChar->m_nRepPoints = 0;
		}

		if ((g_pChar->m_nRepLevel>0) &&
			(g_pChar->m_nRepPoints < CalcRepPointsForNextLevel(g_pChar->m_nRepLevel-1)))
		{
			g_pChar->m_nRepLevel--;
			nRepChange = -1;
		}
		else
		{
			nRepChange = 0;
		}
	}
	else if (nResult == 0)
	{
		nPay = 0;
		nSkillPoints = 0;

		// Lose points equal to run/2
		g_pChar->m_nRepPoints -= (g_pChar->m_pCurrentContract->m_nDifficulty/2);
		if (g_pChar->m_nRepPoints<0)
		{
			g_pChar->m_nRepPoints = 0;
		}

		if ((g_pChar->m_nRepLevel>0) &&
			(g_pChar->m_nRepPoints < CalcRepPointsForNextLevel(g_pChar->m_nRepLevel-1)))
		{
			g_pChar->m_nRepLevel--;
			nRepChange = -1;
		}
		else
		{
			nRepChange = 0;
		}
	}
	else //if (nResult == 1)
	{
		nPay = g_pChar->m_pCurrentContract->m_nPay;

		// Old way was  Points = Diff - (Skill - Diff). This gives away too many points
		// for low difficulty missions at higher skill levels.
		//nSkillPoints = 1 + (2 * g_pChar->m_pCurrentContract->m_nDifficulty) - g_pChar->GetEffectiveLevel();
		//if (nSkillPoints<0) nSkillPoints = 0;

		// New method is Diff - 25% per level below skill
		nSkillPoints = g_pChar->m_pCurrentContract->m_nDifficulty;
		int iDiff = g_pChar->GetEffectiveLevel() - g_pChar->m_pCurrentContract->m_nDifficulty;
		if (iDiff == 1)
		{
			// One level below: 75%
			nSkillPoints -= (nSkillPoints>>2);
		}
		else if (iDiff == 2)
		{
			// Two levels below: 50%
			nSkillPoints >>= 1;
		}
		else if (iDiff == 3)
		{
			// Three levels below: 25%
			nSkillPoints >>= 2;
		}
		else if (iDiff>=4)
		{
			// Four or more levels below: 0
			nSkillPoints = 0;
		}

		g_pChar->m_nRepPoints += g_pChar->m_pCurrentContract->m_nDifficulty;
		int nRepForNextLevel = CalcRepPointsForNextLevel(g_pChar->m_nRepLevel);
		if (g_pChar->m_nRepPoints >= nRepForNextLevel)
		{
			// Make sure we are not capped
			if (g_pChar->m_nRepLevel < ((g_pChar->m_nLifestyle + 1) * 4))
			{
				g_pChar->m_nRepLevel++;
				nRepChange = 1;
			}
			else
			{
				g_pChar->m_nRepPoints = (nRepForNextLevel - 1);
				nRepChange = 2;
			}
		}
		else
		{
			nRepChange = 0;
		}
	}
	nTotal = nFileValue + nPay;

	// Fill in the dialog
	dlgResult.m_summary = szBreakdown;
	dlgResult.m_result = szOverall;
	dlgResult.m_pay.Format("%d",nPay);
	dlgResult.m_total.Format("%d",nTotal);
	dlgResult.m_files.Format("%d",nFileValue);
	dlgResult.m_skill_pts.Format("%d",nSkillPoints);
	if (nRepChange == 0)
		dlgResult.m_rep.Format("No change");
	else if (nRepChange == 1)
		dlgResult.m_rep.Format("Raised to %s", g_szRepLevelString[g_pChar->m_nRepLevel]);
	else if (nRepChange == 2)
		dlgResult.m_rep.Format("Capped at %s", g_szRepLevelString[g_pChar->m_nRepLevel]);
	else
		dlgResult.m_rep.Format("Falls to %s", g_szRepLevelString[g_pChar->m_nRepLevel]);

	dlgResult.DoModal();

	// Update the character information
	g_pChar->m_nSkillPoints += nSkillPoints;
	g_pChar->m_nCredits += nTotal;

	// Hide the window so redraw won't crash
	ShowWindow(SW_HIDE);

	// Set system alerts, if necessary
	if (g_pChar->m_pSystem->m_nAlert == ALERT_YELLOW)
	{
		// Stay in yellow for one day per (level/5)
		g_pChar->m_bSystemAlerts[g_pChar->m_pSystem->m_nCorporation] = ((g_pChar->m_pSystem->m_nRating+4)/5) + 1;
	}
	else if (g_pChar->m_pSystem->m_nAlert == ALERT_RED)
	{
		// Stay in yellow for one week per (level/5)
		g_pChar->m_bSystemAlerts[g_pChar->m_pSystem->m_nCorporation] = (((g_pChar->m_pSystem->m_nRating+4)/5)*7) + 1;
	}
	else
	{
		// Leaving in green, so leave it that way
		g_pChar->m_bSystemAlerts[g_pChar->m_pSystem->m_nCorporation] = 0;
	}

	// Delete the current contract failed or succeeded
	if (nResult != 0)
	{
		g_pChar->ClearCurrentContract();
	}
	// Delete the system
	delete g_pChar->m_pSystem;
	g_pChar->m_pSystem = NULL;
	g_pChar->m_olCurrentIceList.RemoveAll();
	g_pChar->m_bOnRun = FALSE;

	// Let some time pass //FSO 7-24-01 Changed to 1 day
	if (g_pChar->PassTime(1))
	{
		// Close this window
		SendMessage(WM_CLOSE);
		return;
	}

	//FSO 7-24-01
	// If contract was completed successfully, add a new one
	if (nResult == 1)
	{
		// Add a new contract
		CContract * pContract = new CContract;
		pContract->Generate();
		g_pChar->m_olContracts.AddTail(pContract);
	}

	// Close this window
	SendMessage(WM_CLOSE);
}

void CMatrixView::UpdateBar(int nBar)
{
	if ((nBar == BAR_DECK) || (nBar == BAR_ALL))
	{
		m_hbHealthBar[BAR_DECK].m_nCurrentVal = g_pChar->m_nHealthDeck;
		m_hbHealthBar[BAR_DECK].RedrawWindow();
	}

	if ((nBar == BAR_MENTAL) || (nBar == BAR_ALL))
	{
		m_hbHealthBar[BAR_MENTAL].m_nCurrentVal = g_pChar->m_nHealthMental;
		m_hbHealthBar[BAR_MENTAL].RedrawWindow();
	}

	if ((nBar == BAR_LETHAL) || (nBar == BAR_ALL))
	{
		m_hbHealthBar[BAR_LETHAL].m_nCurrentVal = g_pChar->m_nHealthPhysical;
		m_hbHealthBar[BAR_LETHAL].RedrawWindow();
	}

	if ((nBar == BAR_SHIELD) || (nBar == BAR_ALL))
	{
		if (g_pChar->m_pActiveShield == NULL)
		{
			m_hbHealthBar[BAR_SHIELD].m_nCurrentVal = 0;
		}
		else
		{
			m_hbHealthBar[BAR_SHIELD].m_nCurrentVal = g_pChar->m_pActiveShield->m_nLoadedRating;
			m_hbHealthBar[BAR_SHIELD].m_nMaxVal = g_pChar->m_pActiveShield->m_nRating;
		}
		m_hbHealthBar[BAR_SHIELD].RedrawWindow();
	}
	if ((nBar == BAR_TRANSFER) || (nBar == BAR_ALL))
	{
		if ((g_pChar->m_pTransferFile == NULL) && (g_pChar->m_pTransferProgram == NULL))
		{
			m_hbHealthBar[BAR_TRANSFER].m_nCurrentVal = 0;
		}
		else
		{
			m_hbHealthBar[BAR_TRANSFER].m_nCurrentVal = g_pChar->m_nTransferTurnsLeft;
			m_hbHealthBar[BAR_TRANSFER].m_nMaxVal = g_pChar->m_nTransferInitialTime;
		}
		m_hbHealthBar[BAR_TRANSFER].RedrawWindow();
	}
	if ((nBar == BAR_TRACE) || (nBar == BAR_ALL))
	{
		if (g_pChar->m_pTraceIce == NULL)
		{
			m_hbHealthBar[BAR_TRACE].m_nCurrentVal = 0;
		}
		else
		{
			m_hbHealthBar[BAR_TRACE].m_nCurrentVal = g_pChar->m_nTraceTurnsLeft;
			m_hbHealthBar[BAR_TRACE].m_nMaxVal = g_pChar->m_nTraceInitialTime;
		}
		m_hbHealthBar[BAR_TRACE].RedrawWindow();
	}
	if ((nBar == BAR_ICE) || (nBar == BAR_ALL))
	{
		if (g_pChar->m_pTargettedIce == NULL)
		{
			m_hbHealthBar[BAR_ICE].m_nCurrentVal = 0;
		}
		else
		{
			m_hbHealthBar[BAR_ICE].m_nCurrentVal = g_pChar->m_pTargettedIce->m_nHealth;
		}
		m_hbHealthBar[BAR_ICE].RedrawWindow();
	}
}

BOOL CMatrixView::AttemptNodeAccess()
{
	POSITION p;
	CIce *pIce;
	CIce *pGuardIce = NULL;
	CString szTxt;

	// See if there is a guardian or active black ice present
	p = g_pChar->m_olCurrentIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

		// Is this a guardian
		if (pIce->m_nType == ICE_GUARDIAN)
		{
			// Always use guardian ice
			pGuardIce = pIce;
			break;
		}
		else if ((pIce->m_nState == STATE_GUARDING) && (pIce->m_nType == ICE_ATTACK))
		{
			// This is a guarding black ice. Use the highest rated if no guardian is present
			if ((pGuardIce==NULL) || (pGuardIce->GetEffectiveRating(RATING_SENSORS, FALSE) < pIce->GetEffectiveRating(RATING_SENSORS, FALSE)))
			{
				pGuardIce = pIce;
			}
		}
	}

	// If no ice, or if we have bypassed it, we can access
	if ((pGuardIce == NULL) || (pGuardIce->m_bBypassed))
	{
		return TRUE;
	}

	// See if there is a hide program active
	if (g_pChar->m_pActiveHide != NULL)
	{
		// Try to use the hide program to bypass the ice
		if (DoRunProgramVsIce(g_pChar->m_pActiveHide, pGuardIce))
		{
			// Hide succeeded. Print out a message saying so
			szTxt.Format("%s was successfully bypassed by the %s program.", pGuardIce->m_szName, g_pChar->m_pActiveHide->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);

			return TRUE;
		}
		else
		{
			// We failed. Print out a message saying so
			szTxt.Format("%s was not fooled by %s program.", pGuardIce->m_szName, g_pChar->m_pActiveHide->m_szName);
			m_MessageView.AddMessage(szTxt, BLACK);

			pGuardIce->m_bWasAccessed = TRUE;
		}
	}
	else
	{
		szTxt.Format("%s blocks your access.", pGuardIce->m_szName);
		m_MessageView.AddMessage(szTxt, BLACK);
	}
	return FALSE;
}

void CMatrixView::UpdateTarget()
{
	if (g_pChar->m_pTargettedIce != NULL)
	{
		// Show the name
		m_nbIceName.m_szText = g_pChar->m_pTargettedIce->m_szName;

		// Show picture
		if (g_pChar->m_pTargettedIce->m_nState & STATE_MASK_HOSTILE)
			m_idIcePic.m_crBackground = RED;
		else if (g_pChar->m_pTargettedIce->HasQueried())
			m_idIcePic.m_crBackground = YELLOW;
		else
			m_idIcePic.m_crBackground = GREEN;
		m_idIcePic.m_nImage = g_pChar->m_pTargettedIce->m_nImage;

		// Is the ice slowed?
		m_abActiveIce.m_bActive[TARGET_SLOW] = (g_pChar->m_pTargettedIce->m_nSlowLevel);

		// Is the ice infected?
		m_abActiveIce.m_bActive[TARGET_VIRUS] = (g_pChar->m_pTargettedIce->m_nVirusLevel);

		// Is the ice confused?
		m_abActiveIce.m_bActive[TARGET_CONF] = (g_pChar->m_pTargettedIce->m_nConfusionLevel);

		// Is the ice weakened?
		m_abActiveIce.m_bActive[TARGET_WEAK] = (g_pChar->m_pTargettedIce->m_nWeakenLevel);

		// Is the ice analyzed?
		m_abActiveIce.m_bActive[TARGET_ANALYZED] = (g_pChar->m_pTargettedIce->m_nAnalyzed);
	}
	else
	{
		m_abActiveIce.m_bActive[TARGET_SLOW] = FALSE;
		m_abActiveIce.m_bActive[TARGET_VIRUS] = FALSE;
		m_abActiveIce.m_bActive[TARGET_CONF] = FALSE;
		m_abActiveIce.m_bActive[TARGET_WEAK] = FALSE;
		m_abActiveIce.m_bActive[TARGET_ANALYZED] = FALSE;
		m_nbIceName.m_szText.Empty();
		m_idIcePic.m_nImage = -1;
		m_idIcePic.m_crBackground = BLUE;
	}
	m_abActiveIce.RedrawWindow();
	m_nbIceName.RedrawWindow();
	m_idIcePic.RedrawWindow();

	// Show health bar
	UpdateBar(BAR_ICE);

	// Update the buttons
	UpdateControls();
}

void CMatrixView::UpdateNodeIcons()
{
	// Silence
	m_abActiveNode.m_bActive[NODE_SILENCE] = (g_pChar->m_pCurrentNode->m_nActiveSilenceLevel);

	// Smoke
	m_abActiveNode.m_bActive[NODE_SMOKE] = (g_pChar->m_pCurrentNode->m_nActiveSmokeLevel);

	// Scan/Eval
	if (g_pChar->m_pActiveScan==NULL)
	{
		m_abActiveNode.m_bActive[NODE_SCAN] = FALSE;
	}
	else
	{
		m_abActiveNode.m_bActive[NODE_SCAN] = TRUE;
		m_abActiveNode.m_nImageIndex[NODE_SCAN] = g_pChar->m_pActiveScan->m_nClass;
	}

	// Special
	m_abActiveNode.m_bActive[NODE_CLIENT] = (g_pChar->m_nClientProgramStatus);

	m_abActiveNode.RedrawWindow();
}

void CMatrixView::OnViewIce()
{
	if (g_pChar->m_pTargettedIce == NULL)
	{
		return;
	}

	CIceDataDlg dlgIceData(this);

	// Set the icon
	//dlgIceData.m_pIceIL = &m_NodeView.m_ilICE;
	dlgIceData.m_pIceIA = &m_NodeView.m_iaIce;
	dlgIceData.DoModal();
}

void CMatrixView::OnScan()
{
	CProgram *pProgram;

	// Run the highest rated scan program
	pProgram = g_pChar->FindProgram(PROGRAM_SCAN);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Scan program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}

void CMatrixView::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (nID == SC_CLOSE)
	{
		OnDisconnect();
	}
	else
	{
		CWnd ::OnSysCommand(nID, lParam);
	}
}

void CMatrixView::UpdateClock()
{
	m_cvClock.m_nTime = g_pChar->m_nRunTime;
	m_cvClock.RedrawWindow();
}

void CMatrixView::UpdateNodeAccessButtons()
{
	// Show the node access buttons
	int nShowDS = SW_HIDE;
	int nShowIO = SW_HIDE;
	int nShowCPU = SW_HIDE;
	int nShowSPU = SW_HIDE;
	int nShowPortal = SW_HIDE;
	int nShowSecurity = SW_HIDE;

	switch (g_pChar->m_pCurrentNode->m_nType)
	{
	case NT_CPU:
		nShowCPU = SW_SHOW;
		// Fall through
	case NT_SPU:
		nShowSPU = SW_SHOW;
		break;
	case NT_DS:
		nShowDS = SW_SHOW;
		break;
	case NT_IO:
		nShowIO = SW_SHOW;
		break;
	case NT_PORTAL_IN:
	case NT_PORTAL_OUT:
		nShowPortal = SW_SHOW;
		break;
	case NT_COP:
		if (g_pChar->m_pCurrentNode->m_nSubType == NST_COP_SECURITY)
		{
			nShowSecurity = SW_SHOW;
		}
		break;
	//case NT_JUNC:
		//No options
	}
	m_bmbButton[BUTTON_GET_FILE			].ShowWindow(nShowDS);
	m_bmbButton[BUTTON_EDIT_FILE		].ShowWindow(nShowDS);
	m_bmbButton[BUTTON_ERASE_FILE		].ShowWindow(nShowDS);
	m_bmbButton[BUTTON_USE_IO			].ShowWindow(nShowIO);
	m_bmbButton[BUTTON_CRASH			].ShowWindow(nShowCPU);
	m_bmbButton[BUTTON_GET_MAP			].ShowWindow(nShowSPU);
	m_bmbButton[BUTTON_BACKDOOR			].ShowWindow(nShowCPU);
	m_bmbButton[BUTTON_KILL_SHUTDOWN	].ShowWindow(nShowSPU);
	m_bmbButton[BUTTON_KILL_ALARM		].ShowWindow(nShowSecurity);
	m_bmbButton[BUTTON_KILL_TRACE		].ShowWindow(nShowSecurity);
	m_bmbButton[BUTTON_ENTER_PORTAL		].ShowWindow(nShowPortal);
}

// Global Functions
void MarkIceAsHostile()
{
	POSITION p;
	CIce *pIce;

	// Mark all the ICE in this node as hostile.
	p = g_pChar->m_olCurrentIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

		// Mark as not bypassed
		pIce->m_bBypassed = FALSE;

		// If a tapeworm trying to erase a file, don't change
		if (pIce->m_nState == STATE_DESTROYING) continue;

		// NOTE: Ice will try to sound alarm on its next action
		if ((pIce->m_nType==ICE_GATEWAY)||(pIce->m_nType==ICE_GUARDIAN)||(pIce->m_nType==ICE_TAPEWORM))
		{
			// These ICE should always be in home node. If not, move there
			if (pIce->m_pCurrentNode == pIce->m_pHomeNode)
			{
				// ICE is home. Set to hostile guarding
				pIce->m_nState = STATE_GUARDING_H;
			}
			else
			{
				// ICE is not home. Set to moving
				pIce->m_nState = STATE_MOVING_H;
				pIce->m_pTargetNode = pIce->m_pHomeNode;
			}

		}
		else
		{
			// Attack, trace, and probe will attack
			pIce->m_nState = STATE_ATTACKING;
		}
	}
}


BOOL CMatrixView::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_MATRIX);
	return TRUE;
}

void CMatrixView::OnNextTarget()
{
	POSITION p;
	CIce *pIce;

	// If no ice targetted, go to first
	if (g_pChar->m_pTargettedIce != NULL)
	{
		p = g_pChar->m_olCurrentIceList.GetHeadPosition();
		while (p != NULL)
		{
			pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

			if (pIce == g_pChar->m_pTargettedIce)
			{
				// Target the next ice
				if (p != NULL)
				{
					// Get next
					g_pChar->m_pTargettedIce = (CIce *)g_pChar->m_olCurrentIceList.GetAt(p);
					m_NodeView.RedrawWindow();
					UpdateTarget();
					return;
				}
			}
		}
	}

	// Get the head, if there is one
	if (!g_pChar->m_olCurrentIceList.IsEmpty())
	{
		g_pChar->m_pTargettedIce = (CIce *)g_pChar->m_olCurrentIceList.GetHead();
		m_NodeView.RedrawWindow();
		UpdateTarget();
	}
}

void CMatrixView::OnQuickLoad()
{
	CCharacter *pNewChar;

	// Create a new, temporary character
	pNewChar = new CCharacter();
	if (pNewChar->LoadGame(this, g_pChar->m_szFileName))
	{
		// Game was loaded. Delete the old character
		delete g_pChar;

		// Set the global var to the new character
		g_pChar = pNewChar;

		//FSO 8-21-01
		if (g_pChar->m_bOnRun)
		{
			Initialize();
			RedrawWindow();
			m_MessageView.OnClear();

			m_MessageView.AddMessage("Game has been loaded.", BLUE);
		}
		else
		{
			// Return to the home window
			SendMessage(WM_CLOSE);
		}
	}
}

void CMatrixView::OnQuickSave()
{
	if (g_pChar->m_bIronMan)
	{
		DeckerMessageBox("Ironmen don't need to save in the matrix.");
		return;
	}

	if (g_pChar->SaveGame(this))
	{
		m_MessageView.AddMessage("Game has been saved.", BLUE);
	}
}

void CMatrixView::OnSilence()
{
	CProgram *pProgram;

	// Run the highest rated scan program
	pProgram = g_pChar->FindProgram(PROGRAM_SILENCE);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Silence program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}

void CMatrixView::OnEvaluate()
{
	CProgram *pProgram;

	// Run the highest rated scan program
	pProgram = g_pChar->FindProgram(PROGRAM_EVALUATE);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Evaluate program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}
void CMatrixView::OnSmoke()
{
	CProgram *pProgram;

	// Run the highest rated scan program
	pProgram = g_pChar->FindProgram(PROGRAM_SMOKE);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Smoke program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}
void CMatrixView::OnMedic()
{
	CProgram *pProgram;

	// Run the highest rated scan program
	pProgram = g_pChar->FindProgram(PROGRAM_MEDIC);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Medic program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}
void CMatrixView::OnRelocate()
{
	CProgram *pProgram;

	// Run the highest rated scan program
	pProgram = g_pChar->FindProgram(PROGRAM_RELOCATE);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Relocate program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}
void CMatrixView::OnDecrypt()
{
	CProgram *pProgram;

	// Run the highest rated scan program
	pProgram = g_pChar->FindProgram(PROGRAM_DECRYPT);
	if (pProgram == NULL)
	{
		DeckerMessageBox("No Decrypt program loaded.");
	}
	else
	{
		DoRunProgram(pProgram);
	}
}


