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

// MatrixView.h: interface for the CMatrixView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIXVIEW_H__5B5CA3B3_802B_11D4_82A8_00104B751D91__INCLUDED_)
#define AFX_MATRIXVIEW_H__5B5CA3B3_802B_11D4_82A8_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeView.h"
#include "MapView.h"
#include "MessageView.h"
#include "HealthBar.h"
#include "ActiveBar.h"
#include "NameBar.h"
#include "ImageDisplay.h"
#include "AlarmBar.h"
#include "ClockView.h"
#include "CustomButton.h"

class CProgram;
class CIce;

// Size of the node window (square)
#define NODE_SIZE 240

// Size of the map window
#define MAP_SIZE_X	177
#define MAP_SIZE_Y	166

// Number of buttons
#define MATRIX_BUTTON_COUNT 31

// Reasons for player dumping
#define DUMP_DECK_DAMAGE	0	// Dumped due to normal (deck) damage
#define DUMP_UNCONS			1	// Dumped due to unconsciousness
#define DUMP_DEATH			2	// Dumped due to death
#define DUMP_TRACE			3	// Dumped due to a trace completing
#define DUMP_TRACE_FRY		4	// Dumped due to a trace completing, get to fry a chip
#define DUMP_SYS_CRASH		5	// Dumped due to the decker crashing the system
#define DUMP_SYS_OFFLINE	6	// Dumped due to system going offline from a red alert
#define DUMP_DISCONNECT		7	// Decker disconnected

// Health bars to update
#define BAR_ALL				100
#define BAR_DECK			0	// Deck damage
#define BAR_MENTAL			1	// Stun damage
#define BAR_LETHAL			2	// Lethal damage
#define BAR_SHIELD			3	// Shield bar
#define BAR_TRANSFER		4	// Upload/download completion bar
#define BAR_TRACE			5	// Trace completion bar
#define BAR_ICE				6	// Ice health

#define NUM_BARS			7

class CMatrixView : public CWnd  
{
public:
	CMatrixView();
	virtual ~CMatrixView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatrixView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CMatrixView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOptions();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg void OnMoveN();
	afx_msg void OnMoveE();
	afx_msg void OnMoveS();
	afx_msg void OnMoveW();
	afx_msg void OnDisconnect();
	afx_msg void OnMapZoom();
	afx_msg void OnAttack();
	afx_msg void OnDeceive();
	afx_msg void OnWait();
	afx_msg void OnAnalyze();
	afx_msg void OnRunProgram();
	afx_msg void OnSetDefProgram();
	afx_msg void OnLoadProgram();
	afx_msg void OnUnloadProgram();
	afx_msg void OnGetFile();
	afx_msg void OnEditFile();
	afx_msg void OnEraseFile();
	afx_msg void OnUseIO();
	afx_msg void OnCrash();
	afx_msg void OnGetMap();
	afx_msg void OnBackdoor();
	afx_msg void OnKillAlarm();
	afx_msg void OnKillTrace();
	afx_msg void OnKillShutDown();
	afx_msg void OnEnterPortal();
	afx_msg void OnViewChar();
	afx_msg void OnViewDeck();
	afx_msg void OnViewContract();
	afx_msg void OnViewIce();
	afx_msg void OnScan();
	afx_msg void OnItemchangedSoftwareView(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDblclkProgramList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSilence();
	afx_msg void OnEvaluate();
	afx_msg void OnSmoke();
	afx_msg void OnMedic();
	afx_msg void OnRelocate();
	afx_msg void OnDecrypt();
	//}}AFX_MSG
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()

public:
	void Initialize();
	void DoMove(int nDir);
	void UpdateControls();
	BOOL DoRunProgramVsIce(CProgram *pProgram, CIce *pIce);
	void DoSetAlert(CIce *pIce, int nAlertLevel);
	void DoClearTrace();
	void DoCrashIce(CIce *pIce);
	void DoEndPlayerTurn();
	void DoRunProgram(CProgram *pProgram);
	int GetProgramIndex(CProgram *pProgram);
	void DoRemoveProgram(CProgram *pProgram);
	void UpdateProgramRating(CProgram *pProgram);
	void UpdateActiveBar();
	void DoQuery(CIce *pIce);
	void DoEnterNode(CNode *pNode, int nDirFrom);
	void DoDumpDecker(int nCause);
	void UpdateBar(int nBar);
	BOOL AttemptNodeAccess();
	void UpdateTarget();
	void UpdateNodeIcons();
	void UpdateClock();
	void UpdateNodeAccessButtons();
	void OnNextTarget();
	void OnQuickLoad();
	void OnQuickSave();

	// Member Data
	CDC m_dcBackground;
	CCustomButton m_bmbButton[MATRIX_BUTTON_COUNT];
	CCustomButton m_bmbMapZoom;
	CImageList m_ilSoftware;

	CNodeView m_NodeView;
	CMapView m_MapView;
	CListCtrl m_SoftwareView;
	CMessageView m_MessageView;

	CHealthBar m_hbHealthBar[NUM_BARS];
	CActiveBar m_abActiveChar;
	CActiveBar m_abActiveIce;
	CActiveBar m_abActiveNode;
	CNameBar m_nbIceName;
	CNameBar m_nbNodeName;
	CImageDisplay m_idIcePic;
	CAlarmBar m_abAlert;
	CClockView m_cvClock;

private:
	HACCEL m_hAccel;
};

// Global Functions
void MarkIceAsHostile();

#endif // !defined(AFX_MATRIXVIEW_H__5B5CA3B3_802B_11D4_82A8_00104B751D91__INCLUDED_)
