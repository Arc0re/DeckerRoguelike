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

// Ice.h: interface for the CIce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICE_H__0F6155C1_8370_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_ICE_H__0F6155C1_8370_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ICE Classes
#define ICE_GATEWAY		0	// White
#define ICE_PROBE		1
#define ICE_GUARDIAN	2
#define ICE_TAPEWORM	3
#define ICE_ATTACK		4	// Black
#define ICE_TRACE		5

#define MAX_WHITE		3	// Maximum white ice number

// Attack ice sub-types
#define AST_NORMAL			0x0000	// Attacks deck
#define AST_HARDENED		0x0001
#define AST_PHASING			0x0002
#define AST_CRASH			0x0004
#define AST_KILLER			0x1000	// Attacks decker
#define AST_KILLER_H		0x1001
#define AST_KILLER_P		0x1002
#define AST_KILLER_C		0x1004

#define AST_MASK_KILLER		0x1000
#define AST_MASK_HARDENED	0x0001
#define AST_MASK_PHASING	0x0002
#define AST_MASK_CRASH		0x0004

// Trace ice sub-types
#define TST_NORMAL		0	// Normal trace
#define TST_DUMP		1	// Extended trace & dump
#define TST_FRY			2	// Extended trace & dump & fry

// Tapeworm ice subtypes
#define TWST_NORMAL		0	// Normal tapeworm
#define TWST_DATABOMB	1	// Data bomb

// Number of ICE graphics
#define ICE_GRAPHIC_COUNT 6

// Minimum levels
#define MIN_LEVEL_ATTACK_HARDENED	5
#define MIN_LEVEL_ATTACK_PHASING	5
#define MIN_LEVEL_ATTACK_CRASH_BURN	5
#define MIN_LEVEL_ATTACK_LETHAL		10
#define MIN_LEVEL_TRACE_DUMP		5
#define MIN_LEVEL_TRACE_FRY			10

// Rating Types
#define RATING_NORMAL	0	// Base rating
#define RATING_COMBAT	1	// Combat rating
#define RATING_SENSORS	2	// Sensor rating

//------------
// ICE States
//------------
// Non-hostile
#define STATE_INACTIVE		0x00	// Black ice which is not active
#define STATE_GUARDING		0x01	// Normal ice which are sitting in their home node
#define STATE_FOLLOWING		0x02	// Following player to query
#define STATE_MOVING		0x03	// Going to a target node. (Black only)
									//	Response: Going to search. 
									//  Other: Going home to guard.
#define STATE_SEARCHING		0x04	// Searching for intruders (Black/Probe)
#define STATE_DESTROYING	0x05	// Destroying a datafile - tapeworm only
#define STATE_QUERIED1		0x06	// Queried player, waiting for response
#define STATE_QUERIED2		0x07	// Queried player, waiting for response
#define STATE_QUERIED3		0x08	// Queried player, waiting for response
// Hostile
#define STATE_GUARDING_H	0x11	// Guarding, but knows hostile
#define STATE_ATTACKING		0x12	// Black ice attacking/chasing the player,
#define STATE_MOVING_H		0x13	// White ice returning to home node

#define STATE_MASK_HOSTILE	0x10	// Mask to show hostility

class CSystem;
class CNode;
class CMatrixView;
class CDSFile;

class CIce : public CObject  
{
public:
	// Construction
	CIce();
	virtual ~CIce();

	// Member Fuctions
	const char *GetTypeString();
	void GetNotes(CString &szStr);
	void LoadInitData();
	int GetEffectiveRating(int nType, BOOL bIncludeAnalyze = TRUE);
	BOOL DoAction(CMatrixView *pView);
	BOOL DoQuery(CMatrixView *pView);
	void DoMove(int nDir, CMatrixView *pView);
	BOOL NoticedPlayer(CMatrixView *pView);
	BOOL HasQueried();
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion, CSystem *pSystem);
	void DoWander(CMatrixView *pView);

	// Data Members
	// Fixed data
	CString m_szName;
	int m_nType;
	int m_nSubType;	// For gateway, gives direction
	int m_nRating;
	int m_nImage;
	BOOL m_bResponse;	// Response IC flag for black ice
	CNode *m_pHomeNode;		// The home node for this ICE
	CDSFile *m_pFile;		// File for tapeworms

	//--------------------
	// Current state data
	//--------------------
	// Health status
	int m_nHealth;
	int m_nSlowLevel;		// Affected by slow program - level is # turns slowed
	BOOL m_bSkippedLastTurn;	// Did we skip the last turn because of the slow?
	int m_nConfusionLevel;	// Affected by confusion program - level is # turns confused
	int m_nWeakenLevel;		// Affected by weaken program - level is # turns weakened
	int m_nVirusLevel;		// Affected by virus program - level is number of turns to take damage
	int m_nAnalyzed;		// Has this ice been analyzed?

	// Location
	CNode *m_pCurrentNode;
	CPoint m_ptNodeLoc;
	CPoint m_ptScreenLoc;

	// Behavior flags
	int m_nState;			// Current state
	BOOL m_bBypassed;		// Was this ice bypassed?
	BOOL m_bWasAccessed;	// Was this ice accessed by the player
	CNode *m_pTargetNode;	// Node we are moving to
	int m_nLastDir;			// Direction we moved last time
};

#endif // !defined(AFX_ICE_H__0F6155C1_8370_11D4_B835_0080C8C1DB04__INCLUDED_)
