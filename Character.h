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

// Character.h: interface for the CCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTER_H__5B5CA3B4_802B_11D4_82A8_00104B751D91__INCLUDED_)
#define AFX_CHARACTER_H__5B5CA3B4_802B_11D4_82A8_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Contract.h"	// Needed for NUM_CORP

//class CContract;
class CSystem;
class CArea;
class CNode;
class CShopItem;
class CIce;
class CProgram;
class CDSFile;
class CSource;

// Lifestyles
#define LS_STREET		0
#define	LS_LOW			1
#define LS_MED			2
#define LS_HIGH			3
#define LS_ELITE		4

#define MAX_LIFESTYLE	4

// Item Types
#define IT_SOFTWARE		0
#define IT_CHIP			1
#define IT_HARDWARE		2

// Chip types
#define CHIP_CPU			0
#define CHIP_ATTACK			1
#define CHIP_DEFENSE		2
#define CHIP_STEALTH		3
#define CHIP_ANALYSIS		4
#define CHIP_COPROCESSOR	5

#define NUM_CHIPS			6

// Hardware types
#define HW_CHIP_BURNER		0
#define HW_SURGE_SUPP		1
#define HW_NEURAL_DAMPER	2
#define HW_TRACE_MONITOR	3
#define HW_BIO_MONITOR		4
#define HW_HIGH_BW_BUS		5
#define HW_MAPPER			6
#define HW_DESIGN_ASSIST	7
#define HW_PROXY			8

#define NUM_HW				9
#define NUM_HW_RESERVED		20	// Reserved space for future expansion

#define NUM_HW_0_1			6 // Hardware in version 0.1

// Ratings for GetEffectiveRating
#define RATING_ATTACK		0
#define RATING_DEFENSE		1
#define RATING_STEALTH		2
#define RATING_ANALYSIS		3

// Load status
#define LS_LIGHT			0
#define LS_NORMAL			1
#define LS_HEAVY			2

// Maximum decoys
#define MAX_DECOYS			5

// Starting Bonus
#define BONUS_NONE			0
#define BONUS_SKILLS		1
#define BONUS_HARDWARE		2
#define BONUS_SOFTWARE		3
#define BONUS_MONEY			4

// Run flags
#define CRF_ALARMS_SET		0x00000001		// Alarms have been set


class CCharacter : public CObject  
{
public:
	CCharacter();
	virtual ~CCharacter();

	//---------------
	// Game settings
	//---------------
	// Tooltips enable
	BOOL m_bTooltips;

	// Ironman Mode
	BOOL m_bIronMan;

	//------------------
	// Basic Attributes
	//------------------
	// Character Name
	CString m_szName;
	CString m_szFileName;

	// Character Money
	int m_nCredits;

	// Lifestyle level
	int m_nLifestyle;

	// Character Reputation
	int m_nRepLevel;
	int m_nRepPoints;
	
	// Character Skills
	int m_nAttackSkill;
	int m_nDefenseSkill;
	int m_nStealthSkill;
	int m_nAnalysisSkill;
	int m_nProgrammingSkill;
	int m_nChipDesignSkill;

	// Skill points to increase skills
	int m_nSkillPoints;

	// Image in matrix
	int m_nImage;

	// Backdoors
	BYTE m_bBackdoor[NUM_CORP];

	// Systems in alert //FSO 7-24-01
	BYTE m_bSystemAlerts[NUM_CORP];

	//------------------
	// Deck Components
	//------------------
	// Chips
	int m_nChip[NUM_CHIPS];

	// Hardware
	int m_nHardware[NUM_HW_RESERVED];

	// Software
	CObList m_olSoftware;
	int m_nCurrentLoad;
	int m_nLoadStatus;

	// Default Programs
	CProgram *m_pDefAttackProgram;	// Default attack program
	CProgram *m_pDefArmorProgram;	// Default armor program
	CProgram *m_pDefShieldProgram;	// Default shield program
	CProgram *m_pDefHideProgram;	// Default hide program
	CProgram *m_pDefReflectProgram;	// Default reflect program

	//----------------------
	// Contract Information
	//----------------------
	// Available Contracts
	CObList m_olContracts;

	// Current Contract
	CContract *m_pCurrentContract;

	//------------------
	// Projects
	//------------------
	// Source Code
	CObList m_olSourceCode;

	// Current project
	int m_nProjectType;
	int m_nProjectClass;
	int m_nProjectRating;
	int m_nProjectInitialTime;
	int m_nProjectTimeLeft;

	// Chip being burned
	CSource *m_pChipBurning;
	int m_nChipBurnTimeLeft;

	//---------------
	// Items at Shop
	//---------------
	CObList m_olShopList;

	// Item on order
	int m_nOrderItemType;
	int m_nOrderItemClass;
	int m_nOrderItemRating;
	int m_nOrderTimeLeft;

	//------------------
	// Game State
	//------------------
	// Game time
	int m_nDayOfMonth;
	int m_nMonth;
	int m_nYear;
	int m_nDayOfWeek;

	// Are we on a run?
	BOOL m_bOnRun;
	int m_nRunTime;	// Current time in the run (seconds)
	DWORD m_dwRunFlags;		// Flags for use during run

	// Current health
	int m_nHealthPhysical;
	int m_nHealthMental;
	int m_nHealthDeck;

	// Damage (temporary)
	int m_nDamageMental;
	int m_nDamageDeck;
	
	// Current System
	CSystem *m_pSystem;
	CNode *m_pCurrentNode;	// The node we are in
	CPoint m_ptNodeLoc;		// Location within the node
	CPoint m_ptScreenLoc;	// The screen coordinates

	CIce *m_pTargettedIce;	// ICE which is targetted
	CObList m_olCurrentIceList; //ICE list for current node
	int	m_nHighestDeceivedIce;	//Highest rating of ICE deceived

	// Active Programs
	CProgram *m_pActiveArmor;
	CProgram *m_pActiveShield;
	CProgram *m_pActiveHide;
	CProgram *m_pActiveScan;
	CProgram *m_pActiveReflect;
	CProgram *m_pActiveBoost;
	int m_nDecoyCount;
	CPoint m_ptDecoyLocation[MAX_DECOYS];
	// NOTE: Silence and smoke are maintained at the node level
	int m_nClientProgramStatus; //FSO 3-30-04

	// Trace information
	CIce *m_pTraceIce;			// The ice that started the trace
	int m_nTraceInitialTime;	// Total time for the trace to complete
	int m_nTraceTurnsLeft;		// Number of turns left on trace
	int m_nTraceType;			// Type of trace (Use trace sub-types)
	BOOL m_bTraced;				// Have we been traced

	// Transfer details
	CProgram *m_pTransferProgram;
	CDSFile *m_pTransferFile;
	int m_nTransferInitialTime;
	int m_nTransferTurnsLeft;
	int m_nFileTransferType; // Download or edit

	// Scan/Eval status
	int m_nCurrentScanMP;
	POSITION m_posScanFile;

	//-----------
	// Functions
	//-----------
	void CreateNew(int nBonus = BONUS_NONE);
	const char * GetLifestyleString();
	void CalcCurrentLoad();
	void GenerateContracts();
	void ClearContracts();
	void ClearCurrentContract();
	void GenerateShopItems();
	void ClearShopItems();
	BOOL FindShopItem(CShopItem *pItem);
	CProgram *FindProgram(int nType, BOOL bLoaded = TRUE);
	int GetEffectiveRating(int nType);
	int GetEffectiveLevel();
	BOOL PassTime(int nDays);
	void GetLoadRatings(int &nLight, int &nHeavy, int &nMax);
	BOOL SaveGame(CWnd *pWnd, BOOL bAskName = FALSE);
	BOOL LoadGame(CWnd *pWnd, const char *szFileName = NULL);
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion);
	int GetCurrentItemLevel(int nType, int nSubType);
};

//------------------
// Global Functions
//------------------
// In Character.cpp
void GetHardwareString(CString &szStr, int nHardware, int nRating);
const char * GetHardwareName(int nType);
int GetChipComplexity(int nClass);

// In ShopItem.cpp
int GetHardwarePrice(int nType, int nRating);
const char * GetChipName(int nType);
int GetChipPrice(int nType, int nRating);
int GetMaxHardwareRating(int nType);

// In Contract.cpp
int GetFileTypePerCorp();
int GetContractStatus(CString &szOverall, CString &szBreakdown, BOOL bInProgress, BOOL bCrash);


#endif // !defined(AFX_CHARACTER_H__5B5CA3B4_802B_11D4_82A8_00104B751D91__INCLUDED_)
