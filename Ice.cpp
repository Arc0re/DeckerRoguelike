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

// Ice.cpp: implementation of the CIce class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "Ice.h"
#include "Global.h"
#include "MatrixView.h"
#include "Character.h"
#include "System.h"
#include "Area.h"
#include "Program.h"
#include "DSFile.h"
#include "DeckerSound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

//-------------------
// ICE graphics data
//-------------------
// Image list indices
#define IG_GEN_GATEWAY		0
#define IG_GEN_PROBE		1
#define IG_GEN_GUARDIAN		2
#define IG_GEN_TAPEWORM		3
#define IG_GEN_ATTACK		4
#define IG_GEN_TRACE		5
#define IG_GHOST1			6
#define IG_GHOST2			7
#define IG_GHOST3			8
#define IG_GHOST4			9
#define IG_GENIE			10
#define IG_WHIRLWIND		11
#define IG_BIG_GUY			12
#define IG_NINJA			13
#define IG_MONK				14
#define IG_SPHINX			15
#define IG_GOLEM			16
#define IG_EUNOCH			17
#define IG_TOUGHGUY1		18
#define IG_GARGOYLE			19
#define IG_DOG1				20
#define IG_DOG2				21
#define IG_EYE				22
#define IG_DRAGON			23
#define IG_KNIGHT1			24
#define IG_KNIGHT2			25
#define IG_TURTLE			26
#define IG_SPIDER1			27
#define IG_SPIDER2			28
#define IG_SCORPION			29
#define IG_GAZER1			30
#define IG_GAZER2			31
#define IG_GAZER3			32
#define IG_GAZER4			33
#define IG_SCOUT			34
#define IG_VINES			35
#define IG_SNAKE1			36
#define IG_SNAKE2			37
#define IG_FIREWALL			38
#define IG_GATE1			39
#define IG_FENCE			40
#define IG_GATE2			41
#define IG_BARRIER			42
#define IG_TOUGHGUY2		43
#define IG_DOORMAN			44
#define IG_POLICE1			45
#define IG_POLICE2			46
#define IG_RANGER			47
#define IG_FAIRY			48
#define IG_SHERLOCK			49
#define IG_GUARD			50
#define IG_PRIVATE_I		51
#define IG_CENTURION		52
#define IG_HIGHLANDER		53
#define IG_MARINE			54
#define IG_COWBOY			55
#define IG_SHERIFF			56
#define IG_CAVALRY			57
#define IG_BEES				58
#define IG_TANK				59
#define IG_MANTIS			60
#define IG_SAMURAI			61
#define IG_ROBOT			62
#define IG_VADER			63
#define IG_MOUTH			64
#define IG_GHOST5			65
#define IG_ATTACK_H			66
#define IG_ATTACK_P			67
#define IG_ATTACK_C			68
#define IG_DETECTIVE		69
#define IG_WORM				70
#define IG_ELECTRIC			71
#define IG_DYNAMITE			72
#define IG_BOMB				73
#define IG_TRAP				74
#define IG_TRAP_FILE		75
#define IG_DYNAMITE_FILE	76
#define IG_BOMB_FILE		77
#define IG_SNAKE3			78

//--------------
// ICE database
//--------------
// ICE database structure
typedef struct {
	char *szName;
	int nGraphic;
} ICE_DATA;

// ICE databases
const static ICE_DATA g_idGateway[20] = 
{
	{"Gateway",		IG_GEN_GATEWAY},
	{"Fence",		IG_FENCE},
	{"Barrier",		IG_BARRIER},
	{"Doorway",		IG_GATE1},
	{"Blockade",	IG_BARRIER},
	{"Checkpoint",	IG_GATE2},
	{"Bouncer",		IG_TOUGHGUY2},
	{"Doorman",		IG_DOORMAN},
	{"Gateway Mk2",	IG_GEN_GATEWAY},
	{"Reinforced Fence",IG_FENCE},
	{"Roadblock",	IG_BARRIER},
	{"Gate",		IG_GATE1},
	{"Barrier II",	IG_BARRIER},
	{"Checkpoint",	IG_GATE2},
	{"BouncerEx",	IG_TOUGHGUY2},
	{"Doorman 2.0",	IG_DOORMAN},
	{"Gateway Mk 3",IG_GEN_GATEWAY},
	{"Reinforced Door",IG_GATE1},
	{"Electric Fence",IG_FENCE},
	{"Big Bouncer",	IG_TOUGHGUY1},
};

const static ICE_DATA g_idProbe[20] = 
{
	{"Probe",		IG_GEN_PROBE},
	{"Gazer",		IG_GAZER1},
	{"Bobby",		IG_POLICE2},
	{"Snooper",		IG_GEN_PROBE},
	{"Inquisitor",	IG_GEN_PROBE},
	{"Gazer II",	IG_GAZER2},
	{"Cop",			IG_POLICE1},
	{"Probe Mk2",	IG_GEN_PROBE},
	{"Mystic Eye",	IG_EYE},
	{"Scout",		IG_SCOUT},
	{"Bobby 2.0",	IG_POLICE2},
	{"Scout II",	IG_SCOUT},
	{"Police",		IG_POLICE1},
	{"Magic Eye",	IG_EYE},
	{"Gazer III",	IG_GAZER3},
	{"Probe Mk3",	IG_GEN_PROBE},
	{"Scout III",	IG_SCOUT},
	{"SuperFuzz",	IG_POLICE1},
	{"Wizard Eye",	IG_EYE},
	{"Beholder",	IG_GAZER4},
};

const static ICE_DATA g_idGuardian[20] = 
{
	{"Guardian",	IG_GEN_GUARDIAN},
	{"Protector",	IG_TOUGHGUY1},
	{"Sentry",		IG_GOLEM},
	{"Gargoyle",	IG_GARGOYLE},
	{"Guardian Mk2",IG_GEN_GUARDIAN},
	{"Sphinx",		IG_SPHINX},
	{"Golem",		IG_GOLEM},
	{"Eunoch",		IG_EUNOCH},
	{"Protector 2",	IG_TOUGHGUY1},
	{"Guardian Mk3",IG_GEN_GUARDIAN},
	{"GynoSphinx",	IG_SPHINX},
	{"Gargoyle 2.0",IG_GARGOYLE},
	{"Golem II",	IG_GOLEM},
	{"Protector 3",	IG_TOUGHGUY1},
	{"Guardian Mk4",IG_GEN_GUARDIAN},
	{"Protector 3",	IG_TOUGHGUY1},
	{"EunochEx",	IG_EUNOCH},
	{"Guardian Mk5",IG_GEN_GUARDIAN},
	{"Golem III",	IG_GOLEM},
	{"AndroSphinx",	IG_SPHINX},
};

const static ICE_DATA g_idTapeworm[20] = 
{
	{"Tapeworm",		IG_GEN_TAPEWORM},
	{"Boa",				IG_SNAKE2},
	{"Kudzu",			IG_VINES},
	{"Anaconda",		IG_SNAKE1},
	{"Boa 2.0",			IG_SNAKE2},
	{"Tapeworm Mk2",	IG_GEN_TAPEWORM},
	{"Kudzu II",		IG_VINES},
	{"Anaconda 2.1",	IG_SNAKE1},
	{"Boa 3.0",			IG_SNAKE2},
	{"Tapeworm Mk3",	IG_GEN_TAPEWORM},
	{"Boa 3.1a",		IG_SNAKE2},
	{"Kudzu III",		IG_VINES},
	{"Anaconda 3.0",	IG_SNAKE1},
	{"Boa 3.1",			IG_SNAKE2},
	{"Tapeworm Mk4",	IG_GEN_TAPEWORM},
	{"StrangleVine",	IG_VINES},
	{"Anaconda 4.2",	IG_SNAKE1},
	{"Boa 4.0",			IG_SNAKE2},
	{"Tapeworm Mk5",	IG_GEN_TAPEWORM},
	{"StrangleVine II",	IG_VINES},
};

const static ICE_DATA g_idDataBomb[10] = 
{
	{"Data Bomb",		IG_BOMB_FILE},
	{"Dynamyte 1.0",	IG_DYNAMITE_FILE},
	{"Trap",			IG_TRAP_FILE},
	{"Data Bomb Mk2",	IG_BOMB_FILE},
	{"Dynamyte 2.0",	IG_DYNAMITE_FILE},
	{"Trap II",			IG_TRAP_FILE},
	{"Dynamyte 2.1",	IG_DYNAMITE_FILE},
	{"Data Bomb Mk3",	IG_BOMB_FILE},
	{"Trap III",		IG_TRAP_FILE},
	{"Da Bomb",			IG_BOMB_FILE},
};

const static ICE_DATA g_idAttack[20] = 
{
	{"Attack",		IG_GEN_ATTACK},
	{"Brute",		IG_BIG_GUY},
	{"Grunt",		IG_MARINE},
	{"Centurion",	IG_CENTURION},
	{"Attack Mk2",	IG_GEN_ATTACK},
	{"Enforcer",	IG_BIG_GUY},
	{"Wolf",		IG_DOG2},
	{"Soldier",		IG_MARINE},
	{"Attack Mk3",	IG_GEN_ATTACK},
	{"Centurion II",IG_CENTURION},
	{"Dire Wolf",	IG_DOG2},
	{"Attack Mk4",	IG_GEN_ATTACK},
	{"Marine",		IG_MARINE},
	{"Worg",		IG_DOG2},
	{"Centurion III",IG_CENTURION},
	{"Barbarian",	IG_BIG_GUY},
	{"Werewolf",	IG_DOG2},
	{"Attack Mk5",	IG_GEN_ATTACK},
	{"Centurion IV",IG_CENTURION},
	{"Green Beret",	IG_MARINE},
};

const static ICE_DATA g_idAttackHardened[15] = 
{
	{"Attack-H",	IG_ATTACK_H},
	{"Knight",		IG_KNIGHT1},
	{"Tank",		IG_TANK},
	{"Turtle",		IG_TURTLE},
	{"Attack-H Mk2",IG_ATTACK_H},
	{"Knight II",	IG_KNIGHT1},
	{"Terrapin",	IG_TURTLE},
	{"Sherman",		IG_TANK},
	{"Attack-H Mk3",IG_ATTACK_H},
	{"Knight III",	IG_KNIGHT1},
	{"Tortoise",	IG_TURTLE},
	{"Attack-H Mk4",IG_ATTACK_H},
	{"Dragon Turtle",IG_TURTLE},
	{"Knight IV",	IG_KNIGHT1},
	{"Bradley",		IG_TANK},
};

const static ICE_DATA g_idAttackPhasing[15] = 
{
	{"Attack-P",	IG_ATTACK_P},
	{"Bugs",		IG_BEES},
	{"Spook",		IG_GHOST3},
	{"Neophyte",	IG_MONK},
	{"Attack-P Mk2",IG_ATTACK_P},
	{"Bees",		IG_BEES},
	{"Ghost",		IG_GHOST2},
	{"Disciple",	IG_MONK},
	{"Shade",		IG_GHOST4},
	{"Wasps",		IG_BEES},
	{"Attack-P Mk3",IG_ATTACK_P},
	{"Monk",		IG_MONK},
	{"Phantom",		IG_GHOST4},
	{"Hornets",		IG_BEES},
	{"Quai Chang Kain",	IG_MONK},
};

const static ICE_DATA g_idAttackCrash[15] = 
{
	{"Attack-C",	IG_ATTACK_C},
	{"Spider",		IG_SPIDER1},
	{"Scorpion",	IG_SCORPION},
	{"Rattler",		IG_SNAKE2},
	{"Attack-C Mk2",IG_ATTACK_C},
	{"Copperhead",	IG_SNAKE3},
	{"Scorpion 2.0",IG_SCORPION},
	{"Attack-C Mk3",IG_ATTACK_C},
	{"Spider II",	IG_SPIDER1},
	{"Scorpion 2.3",IG_SCORPION},
	{"Cottonmouth",	IG_SNAKE3},
	{"Spider III",	IG_SPIDER1},
	{"Attack-C Mk4",IG_ATTACK_C},
	{"Scorpion 3.0",IG_SCORPION},
	{"Black Widow",	IG_SPIDER1},
};

const static ICE_DATA g_idAttackLethal[10] = 
{
	{"Attack-L",		IG_GEN_ATTACK},
	{"Cowboy",			IG_COWBOY},
	{"Attack-L Mk2",	IG_GEN_ATTACK},
	{"Wrangler",		IG_COWBOY},
	{"Executioner",		IG_BIG_GUY},
	{"Sheriff",			IG_SHERIFF},
	{"Attack-L Mk3",	IG_GEN_ATTACK},
	{"Executioner II",	IG_BIG_GUY},
	{"Marshal",			IG_SHERIFF},
	{"Highlander",		IG_HIGHLANDER},
};

const static ICE_DATA g_idAttackLethalHardened[10] = 
{
	{"Attack-LH",		IG_ATTACK_H},
	{"Killbot",			IG_ROBOT},
	{"Mantis",			IG_MANTIS},
	{"Attack-LH Mk2",	IG_ATTACK_H},
	{"Mantis II",		IG_MANTIS},
	{"Crusader",		IG_KNIGHT2},
	{"Attack-LH Mk3",	IG_ATTACK_H},
	{"Mantis II",		IG_MANTIS},
	{"Paladin",			IG_KNIGHT2},
	{"Vader",			IG_VADER},
};

const static ICE_DATA g_idAttackLethalPhasing[10] = 
{
	{"Attack-LP",		IG_ATTACK_P},
	{"Wraith",			IG_GHOST1},
	{"Phase Spider",	IG_SPIDER2},
	{"Twister",			IG_WHIRLWIND},
	{"Attack-LP Mk2",	IG_ATTACK_P},
	{"Killer Bees",		IG_BEES},
	{"Spectre",			IG_GHOST5},
	{"Attack-LP Mk3",	IG_ATTACK_P},
	{"Cyclone",			IG_WHIRLWIND},
	{"Ninja",			IG_NINJA},
};

const static ICE_DATA g_idAttackLethalCrash[10] = 
{
	{"Attack-LC",		IG_ATTACK_C},
	{"Two-Step",		IG_SNAKE3},
	{"Wyvern",			IG_DRAGON},
	{"Genie",			IG_GENIE},
	{"Attack-LC Mk2",	IG_ATTACK_C},
	{"Black Widow",		IG_SPIDER2},
	{"Efreet",			IG_GENIE},
	{"Mamba",			IG_SNAKE3},
	{"Attack-LC Mk3",	IG_ATTACK_C},
	{"Dragon",			IG_DRAGON},
};

const static ICE_DATA g_idTrace[20] = 
{
	{"Trace",			IG_GEN_TRACE},
	{"Hound",			IG_DOG1},
	{"Tracker",			IG_SCOUT},
	{"Private Eye",		IG_PRIVATE_I},
	{"Trace Mk2",		IG_GEN_TRACE},
	{"Tracker II",		IG_SCOUT},
	{"Blue Tick Hound",	IG_DOG1},
	{"Private Eye 2.0",	IG_PRIVATE_I},
	{"Sherlock",		IG_SHERLOCK},
	{"Trace Mk3",		IG_GEN_TRACE},
	{"Tracker III",		IG_SCOUT},
	{"Bloodhound",		IG_DOG1},
	{"Sherlock II",		IG_SHERLOCK},
	{"Private Eye 3.0",	IG_PRIVATE_I},
	{"Trace Mk4",		IG_GEN_TRACE},
	{"Mastiff",			IG_DOG2},
	{"Tracker IV",		IG_SCOUT},
	{"Sherlock III",	IG_SHERLOCK},
	{"Trace Mk5",		IG_GEN_TRACE},
	{"Hound of the Baskervilles",	IG_DOG2},
};

const static ICE_DATA g_idTraceDump[15] = 
{
	{"Trace & Dump",	IG_GEN_TRACE},
	{"Detective",		IG_DETECTIVE},
	{"Ranger",			IG_RANGER},
	{"Investigator",	IG_PRIVATE_I},
	{"Trace & Dump Mk2",IG_GEN_TRACE},
	{"Detective 2.2",	IG_DETECTIVE},
	{"Ranger II",		IG_RANGER},
	{"Investigator",	IG_PRIVATE_I},
	{"Trace & Dump Mk3",IG_GEN_TRACE},
	{"Detective 3.1",	IG_DETECTIVE},
	{"Ranger III",		IG_RANGER},
	{"Investigator",	IG_PRIVATE_I},
	{"Trace & Dump Mk4",IG_GEN_TRACE},
	{"Detective 4.0",	IG_DETECTIVE},
	{"Ranger IV",		IG_RANGER},
};

const static ICE_DATA g_idTraceFry[10] = 
{
	{"Trace & Fry",		IG_GEN_TRACE},
	{"Mindworm",		IG_WORM},
	{"Zapp",			IG_ELECTRIC},
	{"Trace & Fry Mk2",	IG_GEN_TRACE},
	{"Mindworm 2.0",	IG_WORM},
	{"SuperZapp",		IG_ELECTRIC},
	{"Mindworm 3.1",	IG_WORM},
	{"Trace & Fry Mk3",	IG_GEN_TRACE},
	{"Mindworm 4.0",	IG_WORM},
	{"MegaZapp",		IG_ELECTRIC},
};

// Type strings
static const char *g_szType[] =
{
	"Gateway",
	"Probe",
	"Guardian",
	"Tapeworm",
	"Attack",
	"Trace",
};

const char *g_szAttackType[] = 
{
	"Attack",
	"Attack (Hardened)",
	"Attack (Phasing)",
	"Attack (Crash)",
};

const char *g_szKillerAttackType[] = 
{
	"Killer",
	"Killer (Hardened)",
	"Killer (Phasing)",
	"Killer (Crash)",
};

const char *g_szTraceType[] = 
{
	"Trace",
	"Trace & Dump",
	"Trace & Fry",
};

const char *g_szTapewormType[] = 
{
	"Tapeworm",
	"DataBomb",
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIce::CIce()
{
	// Most data is set when ice type is chosen
	m_nSubType = 0;
	m_bResponse = FALSE;
	m_bBypassed = FALSE;
	m_nHealth = MAX_HEALTH;
	m_nSlowLevel = 0;
	m_nConfusionLevel = 0;
	m_nWeakenLevel = 0;
	m_nVirusLevel = 0;
	m_nAnalyzed = 0;
	m_bWasAccessed = FALSE;
	m_nLastDir = DIR_NONE;
}

CIce::~CIce()
{
	
}

const char *CIce::GetTypeString()
{
	if (m_nAnalyzed)
	{
		if (m_nType == ICE_ATTACK)
		{
			if (m_nSubType & AST_MASK_KILLER)
			{
				return g_szKillerAttackType[m_nSubType - AST_MASK_KILLER];
			}
			else
			{
				return g_szAttackType[m_nSubType];
			}
		}
		else if (m_nType == ICE_TRACE)
		{
			return g_szTraceType[m_nSubType];
		}
		else if (m_nType == ICE_TAPEWORM)
		{
			return g_szTapewormType[m_nSubType];
		}
	}

	// Return only basic information
	return g_szType[m_nType];
}

void CIce::GetNotes(CString &szStr)
{
	switch (m_nType)
	{
	case ICE_GATEWAY	:
		szStr = "Bars passageway to another node.";
		break;
	case ICE_PROBE	:
		szStr = "Searches for intruders in the system.";
		break;
	case ICE_GUARDIAN:
		szStr = "Guards access to the node.";
		break;
	case ICE_TAPEWORM:
		szStr = "Guards a file. Will self-destruct on illegal access, taking the file with it.";
		if (m_nAnalyzed)
		{
			if (m_nSubType == TWST_DATABOMB)
			{
				szStr += " Will attack on self destruct.";
			}
		}
		break;
	case ICE_ATTACK	:
		if (!m_nAnalyzed)
		{
			szStr = "Attacks intruders.";
		}
		else
		{
			if (m_nSubType & AST_MASK_KILLER)
				szStr = "Attacks intruders lethally.";
			else
				szStr = "Attacks intruders normally.";
			if (m_nSubType & AST_MASK_HARDENED)
			{
				szStr += " Resistant to non-piercing attacks.";
			}
			else if (m_nSubType & AST_MASK_PHASING)
			{
				szStr += " Resistant to non-area attacks.";
			}
			else if (m_nSubType & AST_MASK_CRASH)
			{
				szStr += " Can crash programs on succesful hits.";
			}
		}
		break;
	case ICE_TRACE	:
		szStr = "Attempts to trace an intruder's signal in the system.";
		if (m_nAnalyzed)
		{
			if (m_nSubType == TST_DUMP)
			{
				szStr += " Can attempt an extended trace in order to dump the decker.";
			}
			else if (m_nSubType == TST_FRY)
			{
				szStr += " Can attempt an extended trace in order to dump the decker and attempt to fry a chip.";
			}
		}
		break;
	}
}

void CIce::LoadInitData()
{
	const ICE_DATA *pIceData;

	int nInitDataIndex;

	// Get the index of the name/graphic to use
	if (m_nRating >=20)
		nInitDataIndex = 19;
	else
		nInitDataIndex = (m_nRating - 1);

	// Get the appropriate database entry
	switch (m_nType)
	{
	case ICE_GATEWAY	:
		pIceData = &(g_idGateway[nInitDataIndex]);
		break;
	case ICE_PROBE	:
		pIceData = &(g_idProbe[nInitDataIndex]);
		break;
	case ICE_GUARDIAN:
		pIceData = &(g_idGuardian[nInitDataIndex]);
		break;
	case ICE_TAPEWORM:
		switch (m_nSubType)
		{
		case TWST_NORMAL:
			pIceData = &(g_idTapeworm[nInitDataIndex]);
			break;
		case TWST_DATABOMB:
			pIceData = &(g_idDataBomb[nInitDataIndex-10]);
			break;
		}
		break;
	case ICE_ATTACK	:
		switch (m_nSubType)
		{
		case AST_NORMAL		:
			pIceData = &(g_idAttack[nInitDataIndex]);
			break;
		case AST_HARDENED	:
			pIceData = &(g_idAttackHardened[nInitDataIndex-5]);
			break;
		case AST_PHASING		:
			pIceData = &(g_idAttackPhasing[nInitDataIndex-5]);
			break;
		case AST_CRASH	:
			pIceData = &(g_idAttackCrash[nInitDataIndex-5]);
			break;
		case AST_KILLER		:
			pIceData = &(g_idAttackLethal[nInitDataIndex-10]);
			break;
		case AST_KILLER_H	:
			pIceData = &(g_idAttackLethalHardened[nInitDataIndex-10]);
			break;
		case AST_KILLER_P	:
			pIceData = &(g_idAttackLethalPhasing[nInitDataIndex-10]);
			break;
		case AST_KILLER_C	:
			pIceData = &(g_idAttackLethalCrash[nInitDataIndex-10]);
			break;
		}
		break;
	case ICE_TRACE	:
		switch (m_nSubType)
		{
		case TST_NORMAL		:
			pIceData = &(g_idTrace[nInitDataIndex]);
			break;
		case TST_DUMP	:
			pIceData = &(g_idTraceDump[nInitDataIndex-5]);
			break;
		case TST_FRY		:
			pIceData = &(g_idTraceFry[nInitDataIndex-10]);
			break;
		}
		break;
	}

	// Load the necessary items
	//m_szName = pIceData->szName;
	m_szName.Format("%s %04X",pIceData->szName, Random(0x10000));
	m_nImage = pIceData->nGraphic;
}

int CIce::GetEffectiveRating(int nType, BOOL bIncludeAnalyze)
{
	int nRating;

	// Start with base rating
	nRating = m_nRating;

	// Modify by condition
	nRating += GetConditionModifier(m_nHealth);

	// Modify by weaken level
	if (m_nWeakenLevel>0)
	{
		nRating -= 4;
	}

	// If scanned, reduce rating
	if (bIncludeAnalyze)
	{
		nRating -= m_nAnalyzed;
	}

	// Modify according to rating type
	if (nType == RATING_COMBAT)
	{
		// White ice has a penalty to combat
		if (m_nType <= MAX_WHITE)
		{
			nRating -= 2;
		}
	}
	else if (nType == RATING_SENSORS)
	{
		// White ice gets bonus to sensors if alarm set
		if (m_nType <= MAX_WHITE)
		{
			if (g_pChar->m_pSystem->m_nAlert != ALERT_GREEN)
			{
				nRating += 2;
			}
		}
//	Assume black ICE has normal rating. White gets bonus.
//		// Black ice has a penalty to sensors
//		else
//		{
//			nRating -=2;
//		}
	}

	return nRating;
}

BOOL CIce::DoAction(CMatrixView *pView)
{
	CString szTxt;
	POSITION p;
	CIce *pIce;
	int nDir;
//	int nRot;
	int nLowestDir;
	int nLowestDirVal;
	int iTargetNumber;
	int iSuccess;
	int iDamage; //FSO 7-9-01

	// If we are not in the player's node, clear the bypassed flag
	if (m_pCurrentNode != g_pChar->m_pCurrentNode)
	{
		m_bBypassed = NULL;
	}
	
	//------------------------------------------
	// Handle any special conditions on the ICE
	//------------------------------------------
	// See if we are slowed
	if (m_nSlowLevel)
	{
		m_nSlowLevel--;
		if (m_nSlowLevel==0)
		{
			szTxt.Format("%s is no longer slowed", m_szName);
			pView->m_MessageView.AddMessage(szTxt,BLACK);
		}
		else
		{
			if (m_bSkippedLastTurn)
			{
				// We can do this turn
				m_bSkippedLastTurn = FALSE;
			}
			else
			{
				// Skip this turn
				m_bSkippedLastTurn = TRUE;
				return FALSE;
			}
		}
	}

	// Do we have a virus?
	if (m_nVirusLevel>0)
	{
		m_nVirusLevel--;

		if (m_nVirusLevel==0)
		{
			szTxt.Format("%s is no longer infected", m_szName);
			pView->m_MessageView.AddMessage(szTxt,BLACK);
		}
		else
		{
			// Take a point of damage
			m_nHealth--;
			if (m_nHealth==0)
			{
				pView->DoCrashIce(this);
				return FALSE;
			}
		}

	}

	// Are we confused?
	if (m_nConfusionLevel>0)
	{
		m_nConfusionLevel--;

		if (m_nConfusionLevel==0)
		{
			szTxt.Format("%s is no longer confused", m_szName);
			pView->m_MessageView.AddMessage(szTxt,BLACK);
		}
	}

	// Are we weakened?
	if (m_nWeakenLevel>0)
	{
		m_nWeakenLevel--;

		if (m_nWeakenLevel==0)
		{
			szTxt.Format("%s is no longer weakened", m_szName);
			pView->m_MessageView.AddMessage(szTxt,BLACK);
		}
	}

	//----------------------------------
	// Do the ICE actions for this turn
	//----------------------------------
	// If we are confused, choose a random action
	if (m_nConfusionLevel>0)
	{
		CObList m_olOtherIce;

		// If this is the active trace, 25% chance of cancelling trace
		if (g_pChar->m_pTraceIce == this)
		{
			if (Random(4)==0)
			{
				g_pChar->m_pTraceIce = NULL;

				szTxt.Format("%s cancels its trace!", m_szName);
				pView->m_MessageView.AddMessage(szTxt, BLUE);
			}
		}

		// Choose random action
		int iRoll = Random(5);
		switch (iRoll)
		{
		case 0: // Damage
			iSuccess = DoDieRoll(10);
			if (iSuccess>0)
			{
				// The ice has damaged itself
				iDamage = ComputeDamage(iSuccess);
				szTxt.Format("%s does %d%% damage to itself", m_szName, (iDamage*5));
				pView->m_MessageView.AddMessage(szTxt, BLUE);

				// Apply the damage
				m_nHealth -= iDamage;
				if (m_nHealth <1)
				{
					pView->DoCrashIce(this);
				}
			}
			return FALSE;
		case 1: // Attack player, if in node
			if (((m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE) || (m_nType == ICE_PROBE))
				&& (g_pChar->m_pCurrentNode == m_pCurrentNode))
			{
				// Go to the attack code below
				m_nState = STATE_ATTACKING;
				break;
			}
			else
			{
				// Non-attack will just set an alarm
				pView->DoSetAlert(this, ALERT_RED);
			}
			// Fall through to code below
		case 2: // Attack other ice in node
			if (m_nType == ICE_ATTACK)
			{
				// Build an ice list for this node
				p = g_pChar->m_pSystem->m_olIceList.GetHeadPosition();
				while (p!=NULL)
				{
					pIce = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);
					if ((pIce != this) &&(pIce->m_pCurrentNode == m_pCurrentNode))
					{
						m_olOtherIce.AddTail(pIce);
					}
				}
				if (!m_olOtherIce.IsEmpty())
				{
					// Pick a random ice to attack
					iRoll = Random(m_olOtherIce.GetCount());
					pIce = (CIce *)m_olOtherIce.GetAt(m_olOtherIce.FindIndex(iRoll));

					// Attack the ice
					iTargetNumber = 10 - GetEffectiveRating(RATING_COMBAT, FALSE) + pIce->GetEffectiveRating(RATING_COMBAT, FALSE);

					// Roll the die
					iSuccess = DoDieRoll(iTargetNumber);

					if (iSuccess > 0)
					{
						// Did some damage. Send a message.
						iDamage = ComputeDamage(iSuccess);
						szTxt.Format("%s attacks %s for %d%% damage!", m_szName, pIce->m_szName, (iDamage*5));
						pView->m_MessageView.AddMessage(szTxt, BLUE);

						// Apply the damage
						pIce->m_nHealth -= iDamage;
						if (pIce->m_nHealth <1)
						{
							pView->DoCrashIce(pIce);
						}
					}
					else
					{
						// Missed. Send a message.
						szTxt.Format("%s attacks %s but missed.", m_szName, pIce->m_szName);
						pView->m_MessageView.AddMessage(szTxt, BLACK);
					}

					return FALSE;
				}
			}
			// Fall through to code below
		case 3:
			// Wander, or do nothing
			if ((m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE) || (m_nType == ICE_PROBE))
			{
				// If we fell through from the code above, 50/50 chance wandering/sleeping
				if ((iRoll==3)||(Random(2)==0))
				{
					// Wander
					DoWander(pView);
				}
			}

		//case 4:
		default:
			// Do nothing
			return FALSE;
		}
	}

	// Put in loop in case state changes
	for (;;)
	{
		// Process according to current state
		switch (m_nState)
		{
		case STATE_FOLLOWING:
			// This is a probe ICE following the player to query him
			// If the player is in this node, query him
			if (m_pCurrentNode == g_pChar->m_pCurrentNode)
			{
				// Query the player.
				if (!DoQuery(pView))
				{
					// Wander
					m_nState = STATE_SEARCHING;
					DoWander(pView);

					return FALSE;
				}
			}
			else
			{
				// Is the player in adjacent node?
				// If node is smoked, we won't know
				if (m_pCurrentNode->m_nActiveSmokeLevel==0)
				{
					for (nDir = 0; nDir < 4; nDir++)
					{
						if (g_pChar->m_pCurrentNode == m_pCurrentNode->m_pAdjNode[nDir])
						{
							// Move to this node
							DoMove(nDir, pView);

							return FALSE;
						}
					}
				}

				// Player is not near. Give up the chase
				m_nState = STATE_SEARCHING;
				DoWander(pView);
			}
			break;

		case STATE_MOVING_H:
			// Try to signal an alarm, because we are hostile
			pView->DoSetAlert(this, ALERT_RED);

			// Fall through to the code below
		case STATE_MOVING:
			// If this is black ice returning home in green/yellow alert, go inactive
			if (((m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE)) &&
				(g_pChar->m_pSystem->m_nAlert != ALERT_RED) &&
				(m_nState != STATE_MOVING_H))
			{
				// Ignore player
			}
			else
			{
				// If the player is in this node, query him
				if ((m_pCurrentNode == g_pChar->m_pCurrentNode) &&
					(!m_bBypassed) &&
					((m_nType == ICE_ATTACK) || (m_nType == ICE_PROBE) || (m_nType == ICE_TRACE)))
				{
					// See if we notice him
					if (NoticedPlayer(pView))
					{
						if (DoQuery(pView))
						{
							// We queried, or are waiting, so quit
							return FALSE;
						}
					}
				}
			}

			// Are we at our node?
			if (m_pCurrentNode == m_pTargetNode)
			{
				// If this is black ice returning home in green/yellow alert, go inactive
				if (((m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE)) &&
					(g_pChar->m_pSystem->m_nAlert != ALERT_RED))
				{
					// Go dormant
					m_nState = STATE_INACTIVE;
				}
				// Response IC should wander looking for intruders.
				else if (m_bResponse)
				{
					DoWander(pView);
				}
				// For non-response, guard
				else if ((m_nState & STATE_MASK_HOSTILE) && (m_pCurrentNode == g_pChar->m_pCurrentNode)) //FSO 8-7-01
				{
					m_nState = STATE_GUARDING_H;
				}
				else
				{
					m_nState = STATE_GUARDING;
				}
			}
			else
			{
				// Calculate the shortest path to the target node
				m_pTargetNode->m_pParentArea->CalculatePathsToNode(m_pTargetNode);
				nLowestDirVal = 1000;
				for (nDir=0; nDir<4; nDir++)
				{
					if ((m_pCurrentNode->m_pAdjNode[nDir]!=NULL) &&
						(m_pCurrentNode->m_pAdjNode[nDir]->m_bProcessed[0]<nLowestDirVal))
					{
						nLowestDirVal = m_pCurrentNode->m_pAdjNode[nDir]->m_bProcessed[0];
						nLowestDir = nDir;
					}
				}

				// Clear the hostile flag
				m_nState = STATE_MOVING;
				
				// Move towards the target node
				DoMove(nLowestDir, pView);
			}
			break;

		case STATE_SEARCHING:
			// If the player is in this node, query him
			if ((m_pCurrentNode == g_pChar->m_pCurrentNode) && (!m_bBypassed))
			{
				// See if we notice the player
				if (NoticedPlayer(pView))
				{
					if (DoQuery(pView))
					{
						// Waiting for response
						return FALSE;
					}
				}
			}

			// Wander randomly
			DoWander(pView);
			break;

		case STATE_QUERIED1:
		case STATE_QUERIED2:
		case STATE_QUERIED3:
			// Have we been bypassed?
			if (!m_bBypassed)
			{
				// Ice has not been bypassed.

				// If this is the last time, or the player has left the node, sound a yellow alert.
				// If it is the red alert, go hostile
				if ((m_nState == STATE_QUERIED3) || (g_pChar->m_pCurrentNode != m_pCurrentNode))
				{
					// Sound a yellow alert. May change our state  if we are silenced,
					// or if it goes to red, but only if we are in the player's node.
					pView->DoSetAlert(this, ALERT_YELLOW);
				}
				else
				{
					// Give the player some more time to respond
					m_nState++;
					return FALSE;
				}
				
				// If we successfully set a yellow alert, we can query again
				if ((g_pChar->m_pSystem->m_nAlert == ALERT_YELLOW) &&
					(m_pCurrentNode->m_nActiveSilenceLevel == 0))
				{
					// Query again
					// We may be in another node if the player
					// left without answering the query, so handle this.
					if (m_pCurrentNode!=g_pChar->m_pCurrentNode)
					{
						if ((m_nType == ICE_PROBE) || (m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE))
						{
							m_nState = STATE_FOLLOWING;
						}
						else
						{
							m_nState = STATE_GUARDING;
						}
					}
					else
					{
						// Query again
						DoQuery(pView);

						// If we are waiting for someone else to query, fix state.
						if ((m_nState != STATE_QUERIED1) && (!(m_nState & STATE_MASK_HOSTILE)))
						{
							if ((m_nType == ICE_PROBE) || (m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE))
							{
								m_nState = STATE_FOLLOWING;
							}
							else
							{
								m_nState = STATE_GUARDING;
							}
						}
						return FALSE;
					}
				}
				else
				{
					// Go hostile
					// We may be in another node if the player
					// left without answering the query, so handle this.
					if (m_pCurrentNode!=g_pChar->m_pCurrentNode)
					{
						if ((m_nType == ICE_PROBE) || (m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE))
						{
							m_nState = STATE_ATTACKING;
						}
						else
						{
							m_nState = STATE_GUARDING;
						}
					}
					else
					{
						// Mark all ice in the node as hostile
						MarkIceAsHostile();

						// It this is a tapeworm set it to destroy the file
						if (m_nType == ICE_TAPEWORM)
						{
							m_nState = STATE_DESTROYING;
						}
					}
				}

				// Redo action with new state
				continue;
			}
   
			// If we are here, we were bypassed

			// If we are gate/guard/tape, go back to guarding
			if ((m_nType == ICE_GATEWAY) || (m_nType == ICE_GUARDIAN) || (m_nType == ICE_TAPEWORM))
			{
				m_nState = STATE_GUARDING;
			}
			// For probe or response IC, wander
			else if ((m_nType == ICE_PROBE) || (m_bResponse))
			{
				m_nState = STATE_SEARCHING;
				DoWander(pView);
			}
			// For non-response black, return home
			else
			{
				m_nState = STATE_MOVING;
				m_pTargetNode = m_pHomeNode;

				// Redo action
				continue;
			}
			break;

		case STATE_DESTROYING:
			// Try to signal an alarm, because we are hostile
			pView->DoSetAlert(this, ALERT_RED);

			// This is a tapeworm destroying a file
			m_pFile->m_nState &= (~STATE_IN_NODE);

			// If this is a databomb, attack
			if ((m_nSubType == TWST_DATABOMB) &&
				(m_pCurrentNode == g_pChar->m_pCurrentNode))
			{
				// Do an attack with no benefit from decoys. First, get a target number
				// Target number is lower than normal
				iTargetNumber = 6 - ((2*g_pChar->m_pSystem->m_nRating) + GetEffectiveRating(RATING_COMBAT)) + (g_pChar->GetEffectiveRating(RATING_DEFENSE));
				if (g_pChar->m_pActiveArmor!=NULL) 
				{
					iTargetNumber += g_pChar->m_pActiveArmor->m_nLoadedRating;
				}

				// Roll the die
				iSuccess = DoDieRoll(iTargetNumber);

				if (iSuccess>0)
				{
					// Ignore decoys
					// It hit. Compute damage
					iDamage = ComputeDamage(iSuccess);

					// Print out a message saying so
					szTxt.Format("%s explodes for %d%% damage.", m_szName, (iDamage*5));
					pView->m_MessageView.AddMessage(szTxt, BLACK);

					// Draw the damage
					pView->m_NodeView.DrawDamage(this);
					pView->m_NodeView.DrawDamage(NULL);

					// Do damage to shield first
					if (g_pChar->m_pActiveShield!=NULL)
					{
						if (g_pChar->m_pActiveShield->m_nLoadedRating > iDamage)
						{
							// The shield will take the damage
							g_pChar->m_pActiveShield->m_nLoadedRating -= iDamage;
							iDamage = 0;
							pView->UpdateProgramRating(g_pChar->m_pActiveShield);
						}
						else
						{
							// The shield will crash
							iDamage -= g_pChar->m_pActiveShield->m_nLoadedRating;
							g_pChar->m_pActiveShield->m_nLoadedRating = 0;
							szTxt.Format("Your shield program %s has crashed.", g_pChar->m_pActiveShield->m_szName);
							pView->m_MessageView.AddMessage(szTxt, RED);
							pView->DoRemoveProgram(g_pChar->m_pActiveShield);
						}
						pView->UpdateBar(BAR_SHIELD);
					}
					
					// Do we still have damage to do?
					if (iDamage>0)
					{
						// Do damage to the deck
						g_pChar->m_nDamageDeck += iDamage;
					}
				}
				else
				{
					// We missed. Print out a message saying so
					szTxt.Format("%s exploded.", m_szName);
					pView->m_MessageView.AddMessage(szTxt, BLACK);
				}
			}

			PlayGameSound(SOUND_FILEDESTROYED);

			// Print a message
			szTxt.Format("%s destroys file %s.", m_szName, m_pFile->m_szName);
			pView->m_MessageView.AddMessage(szTxt, ORANGE);

			// Self destruct
			pView->DoCrashIce(this);
			break;

		case STATE_ATTACKING:
			// Try to signal an alarm, because we are hostile
			pView->DoSetAlert(this, ALERT_RED);

			// If the player is not in the node, try to follow him
			if (m_pCurrentNode != g_pChar->m_pCurrentNode)
			{
				// Is the player in adjacent node? If smoked, we won't know
				if (m_pCurrentNode->m_nActiveSmokeLevel==0)
				{
					for (nDir = 0; nDir < 4; nDir++)
					{
						if (g_pChar->m_pCurrentNode == m_pCurrentNode->m_pAdjNode[nDir])
						{
							// Move to this node
							DoMove(nDir, pView);

							return FALSE;
						}
					}
				}

				// Player is not near. Give up the chase.
				if ((m_nType == ICE_PROBE) || (m_bResponse))
				{
					// Probe and response IC will try to search for intruders
					m_nState = STATE_SEARCHING;
					DoWander(pView);
				}
				else
				{
					// Non-response ice will go home and guard
					m_nState = STATE_MOVING;
					m_pTargetNode = m_pHomeNode;
					
					// Redo move
					continue;
				}
			}
			else
			{
				// Player is in this node. Attack base on ICE type
				if (m_nType == ICE_ATTACK)
				{
					// Do a normal attack. First, get a target number
					iTargetNumber = 10 - ((2*g_pChar->m_pSystem->m_nRating) + GetEffectiveRating(RATING_COMBAT)) + (g_pChar->GetEffectiveRating(RATING_DEFENSE));
					if (g_pChar->m_pActiveArmor!=NULL) 
					{
						iTargetNumber += g_pChar->m_pActiveArmor->m_nLoadedRating;
					}
					if (m_nSubType & AST_MASK_KILLER)
					{
						// Killer ice. Take neural damper into account
						iTargetNumber += g_pChar->m_nHardware[HW_NEURAL_DAMPER];
					}

					// Roll the die
					iSuccess = DoDieRoll(iTargetNumber);

					if (iSuccess>0)
					{
						PlayGameSound(SOUND_ATTACKED);

						// If there are decoys, see if we actually hit
						if (g_pChar->m_nDecoyCount > 0)
						{
							if (Random(g_pChar->m_nDecoyCount+1)!=0)
							{
								// Hit the decoy
								szTxt.Format("%s hits a decoy and destroys it.", m_szName);
								pView->m_MessageView.AddMessage(szTxt, BLACK);

								// Erase the decoy
								g_pChar->m_nDecoyCount--;
								pView->m_NodeView.EraseGrid(g_pChar->m_ptDecoyLocation[g_pChar->m_nDecoyCount]);
								return FALSE;
							}
						}

						// It hit. Compute damage
						iDamage = ComputeDamage(iSuccess);

						// Print out a message saying so
						szTxt.Format("%s hits for %d%% damage.", m_szName, (iDamage*5));
						pView->m_MessageView.AddMessage(szTxt, BLACK);

						// Check for reflect
						if (g_pChar->m_pActiveReflect != NULL)
						{
							// Roll to see reflected percent
							iTargetNumber = 10 - ((2*g_pChar->m_pSystem->m_nRating) + GetEffectiveRating(RATING_COMBAT)) + (g_pChar->GetEffectiveRating(RATING_DEFENSE) + g_pChar->m_pActiveReflect->m_nLoadedRating);

							// Roll the die
							iSuccess = DoDieRoll(iTargetNumber);
							if (iSuccess == -1)
							{
								g_pChar->m_pActiveReflect->m_nLoadedRating = 0;
								szTxt.Format("Your reflect program %s has crashed.", g_pChar->m_pActiveReflect->m_szName);
								pView->m_MessageView.AddMessage(szTxt, RED);
								pView->DoRemoveProgram(g_pChar->m_pActiveReflect);
							}
							else
							{
								// 20% damage reflected per success
								int iReflectedDamage = ((iDamage * iSuccess)/5);

								if (iReflectedDamage > 0)
								{
									// Some damage has been reflected
									szTxt.Format("%d%% damage has been reflected.", iReflectedDamage*5);
									pView->m_MessageView.AddMessage(szTxt, RED);

									// Do damage to the ice
									pView->m_NodeView.DrawDamage(this);
									m_nHealth -= iReflectedDamage;
									if (m_nHealth <= 0)
									{
										pView->DoCrashIce(this);
									}

									// Reduce the damage taken
									iDamage -= iReflectedDamage;
									if (iDamage == 0) break;
								}
							}
						}

						// Draw the damage
						pView->m_NodeView.DrawDamage(NULL);

						// Do damage to shield first
						if (g_pChar->m_pActiveShield!=NULL)
						{
							if (g_pChar->m_pActiveShield->m_nLoadedRating > iDamage)
							{
								// The shield will take the damage
								g_pChar->m_pActiveShield->m_nLoadedRating -= iDamage;
								iDamage = 0;
								pView->UpdateProgramRating(g_pChar->m_pActiveShield);
							}
							else
							{
								// The shield will crash
								iDamage -= g_pChar->m_pActiveShield->m_nLoadedRating;
								g_pChar->m_pActiveShield->m_nLoadedRating = 0;
								szTxt.Format("Your shield program %s has crashed.", g_pChar->m_pActiveShield->m_szName);
								pView->m_MessageView.AddMessage(szTxt, RED);
								pView->DoRemoveProgram(g_pChar->m_pActiveShield);
							}
							pView->UpdateBar(BAR_SHIELD);
						}
						
						// Do we still have damage to do?
						if (iDamage>0)
						{
							// Modify the player's health health
							if (m_nSubType & AST_MASK_KILLER)
							{
								// Damage the player
								g_pChar->m_nDamageMental += iDamage;
							}
							else
							{
								// Do damage to the deck
								g_pChar->m_nDamageDeck += iDamage;
							}

							// If this is crash ice, handle that
							if (m_nSubType & AST_MASK_CRASH)
							{
								// 10% chance per damage point of crashing a program
								if (Random(10) < iDamage)
								{
									// Crash a program
									CObList olProgs;
									POSITION p;
									CProgram *pProgram;

									// First, get a list of programs
									p = g_pChar->m_olSoftware.GetHeadPosition();
									while (p!=NULL)
									{
										pProgram = (CProgram *)g_pChar->m_olSoftware.GetNext(p);
										if (pProgram->m_nLoadedRating>0)
										{
											olProgs.AddTail(pProgram);
										}
									}

									// See if it is empty
									if (!olProgs.IsEmpty())
									{
										// Choose a random one
										p = olProgs.FindIndex(Random(olProgs.GetCount()));
										pProgram = (CProgram *)olProgs.GetAt(p);

										// Send a message
										szTxt.Format("%s has crashed your %s program!", m_szName, pProgram->m_szName);
										pView->m_MessageView.AddMessage(szTxt, RED);

										// Crash it
										pProgram->m_nLoadedRating = 0;
										pView->DoRemoveProgram(pProgram);
									}
								}
							}
						}
					}
					else
					{
						// We missed. Print out a message saying so
						szTxt.Format("%s program missed.", m_szName);
						pView->m_MessageView.AddMessage(szTxt, BLACK);
					}
				}
				else if (m_nType == ICE_TRACE)
				{
					// If there is already a trace active, 
					// or if we are a normal trace and the trace is complete, just wait
					if ((g_pChar->m_pTraceIce == NULL) &&
						((m_nSubType != TST_NORMAL) || (!g_pChar->m_bTraced)))
					{
						// Do the attack.
						// Do a normal attack. First, get a target number
						iTargetNumber = 10 - ((2*g_pChar->m_pSystem->m_nRating) + GetEffectiveRating(RATING_COMBAT)) + (g_pChar->GetEffectiveRating(RATING_DEFENSE));
						if (g_pChar->m_pActiveArmor!=NULL) 
						{
							iTargetNumber += g_pChar->m_pActiveArmor->m_nLoadedRating;
						}

						// Reduce by 1 for each attacking trace
						p = g_pChar->m_olCurrentIceList.GetHeadPosition();
						while (p!=NULL)
						{
							pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

							if ((pIce != this) && (pIce->m_nType == ICE_TRACE))
							{
								iTargetNumber--;
							}
						}

						// Roll the die
						iSuccess = DoDieRoll(iTargetNumber);

						if (iSuccess>0)
						{
							// Set us up as the tracing ice (12-(success/2) => 10..2)
							g_pChar->m_pTraceIce = this;
							g_pChar->m_nTraceInitialTime = (12 - (iSuccess<<1));
							g_pChar->m_nTraceTurnsLeft = g_pChar->m_nTraceInitialTime;
							if (!g_pChar->m_bTraced)
							{
								// Normal trace
								g_pChar->m_nTraceType = TST_NORMAL;
							}
							else
							{
								// Type is our type
								g_pChar->m_nTraceType = m_nSubType;
							}

							PlayGameSound(SOUND_TRACESTART);

							// The trace has started
							szTxt.Format("%s has started a trace! Trace will complete in %d turns.", m_szName, g_pChar->m_nTraceInitialTime);
							pView->m_MessageView.AddMessage(szTxt, RED);

							pView->UpdateBar(BAR_TRACE);
						}
					}
				}
				// Probe ice will just continue to follow
			}
			break;

		case STATE_GUARDING	:
			// If we are black ICE and are in the same node as player, query him
			if ((m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE))
			{
				if ((m_pCurrentNode == g_pChar->m_pCurrentNode) && (!m_bBypassed))
				{
					if (NoticedPlayer(pView))
					{
						// Query the player
						DoQuery(pView);
					}
				}
				// Otherwise, just sleep
			}
			else if (m_bWasAccessed)
			{
				// Character accessed us. Query the player
				DoQuery(pView);
			}
			break;

		case STATE_GUARDING_H:
			// Try to signal an alarm, because we are hostile
			pView->DoSetAlert(this, ALERT_RED);
		
			// Then, do nothing
			break;

		//case STATE_INACTIVE	:
		//default:
			// Do nothing
		} // End of state case

		// End of loop, assume done
		return FALSE;
	} // End of loop
}

// TRUE - Queried, or waiting for other's query
// FALSE - Bypassed, choose new state
BOOL CIce::DoQuery(CMatrixView *pView)
{
	POSITION p;
	CIce *pIce;

	// If we have been bypassed, ignore
	if (m_bBypassed)
	{
		return FALSE;
	}

	// If the player has already deceived in this node, ignore
	if (m_nRating <= g_pChar->m_nHighestDeceivedIce)
	{
		m_bBypassed = TRUE;
		return FALSE;
	}

	// Look for a higher-rated ice who has queried or been bypassed
	p = g_pChar->m_pSystem->m_olIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);

		// If this is us, we will do it (because ice are sorted by rating)
		if (pIce == this) break;

// Remove this, because HighestDeceivedIce code above handles
//		// If the other ice has been bypassed, we will accept it
//		if (pIce->m_bBypassed)
//		{
//			m_bBypassed = TRUE;
//			return FALSE;
//		}

		if (pIce->HasQueried())
		{
			// Wait for the other one to get an answer
			// Keep the same state
			return TRUE;
		}
	}

	// If the decker has a decoy active, set an alert
	if (g_pChar->m_nDecoyCount>0)
	{
		// Try to signal an alarm, because we are hostile
		pView->DoSetAlert(this, ALERT_RED);
		return TRUE;
	}

	PlayGameSound(SOUND_QUERIED);

	// Do the query
	pView->DoQuery(this);
	m_nState = STATE_QUERIED1;

	// Set all below us who have queried to not queried, so that we
	// only have one at a time who are querying
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);

		if (pIce->HasQueried())
		{
			// Change the state
			if ((pIce->m_nType == ICE_GATEWAY) || (pIce->m_nType == ICE_GUARDIAN) || (pIce->m_nType == ICE_TAPEWORM))
			{
				m_nState = STATE_GUARDING;
			}
			else
			{
				pIce->m_nState = STATE_FOLLOWING;
			}
		}
	}

	return TRUE;
}

void CIce::DoMove(int nDir, CMatrixView *pView)
{
	CString szTxt;

	// Special case for ice being re-entered into the system
	if ((nDir != DIR_NONE) && (nDir != DIR_CENTER))
	{
		// Are we leaving the player's node?
		if (m_pCurrentNode == g_pChar->m_pCurrentNode)
		{
			// Remove us from the ICE list
			g_pChar->m_olCurrentIceList.RemoveAt(g_pChar->m_olCurrentIceList.Find(this));

			// If this was the last ICE to leave the node, clear the 'highest deceived level'
			if (g_pChar->m_olCurrentIceList.IsEmpty())
			{
				g_pChar->m_nHighestDeceivedIce = 0;
			}

			PlayGameSound(SOUND_ICELEAVENODE);

			// Send a message
			szTxt.Format("%s has left the node.", m_szName);
			pView->m_MessageView.AddMessage(szTxt,BLACK);

			// Were we the targetted ice?
			if (g_pChar->m_pTargettedIce == this)
			{
				g_pChar->m_pTargettedIce = NULL;
				pView->UpdateControls();
			}

			// Erase the ice
			pView->m_NodeView.EraseIce(this);
		}

		// Change our node
		m_pCurrentNode = m_pCurrentNode->m_pAdjNode[nDir];
	}

	// Save our direction
	m_nLastDir = nDir;

	// Choose a location
	// If this is an ice returning to its home node, put it in the correct place
	if (m_pCurrentNode == m_pHomeNode)
	{
		// Put gateways at the door they will guard
		if (m_nType == ICE_GATEWAY)
		{
			m_pCurrentNode->ChooseLocation(m_ptNodeLoc, m_nSubType);
		}
		// If this is anyone else's home node, put in middle (guard)
		else
		{
			m_pCurrentNode->ChooseLocation(m_ptNodeLoc, DIR_CENTER);
		}
	}
	// Check for entering by non-standard method (i.e. reactivation)
	else if ((nDir == DIR_NONE)||(nDir == DIR_CENTER))
	{
		m_pCurrentNode->ChooseLocation(m_ptNodeLoc, nDir);
	}
	// Normal entry
	else
	{
		m_pCurrentNode->ChooseLocation(m_ptNodeLoc, OppDir(nDir));
	}

	// Are we now in the player's node?
	if (m_pCurrentNode == g_pChar->m_pCurrentNode)
	{
		PlayGameSound(SOUND_ICEENTERNODE);

		// Send a message
		szTxt.Format("%s has entered the node.", m_szName);
		pView->m_MessageView.AddMessage(szTxt,BLACK);

		// Add us to the current ice list
		g_pChar->m_olCurrentIceList.AddTail(this);

		// Draw the picture
		pView->m_NodeView.DrawIce(this);

		// Don't do this, as it ignores hide program. Let DoAction handle it
		//// If we are searching, set us to following
		//if (m_nState == STATE_SEARCHING)
		//{
		//	// Just set to following.
		//	m_nState = STATE_FOLLOWING;
		//}

		// If this is a hostile ice, mark all ice in node as hostile
		if ((m_nState & STATE_MASK_HOSTILE) || (g_pChar->m_bTraced))
		{
			MarkIceAsHostile();
		}
		// If there are hostile ice in the node, mark this one as hostile
		else 
		{
			BOOL bHostile;

			bHostile = g_pChar->m_bTraced;

			if (!bHostile)
			{
				POSITION p = g_pChar->m_olCurrentIceList.GetHeadPosition();
				while (p!=NULL)
				{
					CIce *pTmpIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);
					if (pTmpIce->m_nState & STATE_MASK_HOSTILE)
					{
						bHostile = TRUE;
						break;
					}
				}
			}

			if (bHostile)
			{
				// Mark us as hostile
				if ((m_nType == ICE_PROBE) || (m_nType == ICE_ATTACK) || (m_nType == ICE_TRACE))
				{
					m_nState = STATE_ATTACKING;
				}
				else
				{
					// We must be guardian or gateway moving into position
					m_nState = STATE_MOVING_H;
				}
			}
			else
			{
				// If player has deceived in this node, we may be bypassed
				if (g_pChar->m_nHighestDeceivedIce >= m_nRating)
				{
					m_bBypassed = TRUE;
				}
			}
		}
	}
}

BOOL CIce::NoticedPlayer(CMatrixView *pView)
{
	CString szTxt;

	// See if a hide program is active
	if (g_pChar->m_pActiveHide != NULL)
	{
		if (pView->DoRunProgramVsIce(g_pChar->m_pActiveHide, this))
		{
			// Not noticed
			szTxt.Format("%s fails to notice you.", m_szName);
			pView->m_MessageView.AddMessage(szTxt, BLACK);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CIce::HasQueried()
{
	if ((m_nState == STATE_QUERIED1) ||
		(m_nState == STATE_QUERIED2) ||
		(m_nState == STATE_QUERIED3))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CIce::Save(CArchive &ar)
{
	int nIndex;

	// Write out the fixed data
	ar << m_szName;
	ar << m_nType;
	ar << m_nSubType;
	ar << m_nRating;
	ar << m_nImage;
	ar << m_bResponse;

	// Home node
	nIndex = FindListIndex(m_pHomeNode, g_pChar->m_pSystem->m_olNodeList);
	ar << nIndex;

	// File
	nIndex = FindListIndex(m_pFile, m_pHomeNode->m_olFileList);
	ar << nIndex;

	// Write health status
	ar << m_nHealth;
	ar << m_nSlowLevel;
	ar << m_bSkippedLastTurn;
	ar << m_nConfusionLevel;
	ar << m_nWeakenLevel;
	ar << m_nVirusLevel;
	ar << m_nAnalyzed;

	// Write location data
	nIndex = FindListIndex(m_pCurrentNode, g_pChar->m_pSystem->m_olNodeList);
	ar << nIndex;

	ar << m_ptNodeLoc;
	// Skip screen coordinates

	// Behavior flags
	ar << m_nState;			// Current state
	ar << m_bBypassed;		// Was this ice bypassed?
	ar << m_bWasAccessed;	// Was this ice accessed by the player
	ar << m_nLastDir;		// Direction we moved last time

	// Target Node
	nIndex = FindListIndex(m_pTargetNode, g_pChar->m_pSystem->m_olNodeList);
	ar << nIndex;
}

void CIce::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion, CSystem *pSystem)
{
	int nIndex;

	// Read out the fixed data
	ar >> m_szName;
	ar >> m_nType;
	ar >> m_nSubType;
	ar >> m_nRating;
	ar >> m_nImage;
	ar >> m_bResponse;

	// Home node
	ar >> nIndex;
	m_pHomeNode = (CNode *)FindListObject(nIndex, pSystem->m_olNodeList);

	// File
	ar >> nIndex;
	m_pFile = (CDSFile *)FindListObject(nIndex, m_pHomeNode->m_olFileList);
	if (m_pFile != NULL)
	{
		m_pFile->m_pTapeworm = this;
	}

	// Read health status
	ar >> m_nHealth;
	ar >> m_nSlowLevel;
	ar >> m_bSkippedLastTurn;
	ar >> m_nConfusionLevel;
	ar >> m_nWeakenLevel;
	ar >> m_nVirusLevel;
	if ((wMajorVersion == 0) && (wMinorVersion <7))
	{
		BOOL bAnalyzed;
		ar >> bAnalyzed;
		if (bAnalyzed)
		{
			m_nAnalyzed = 2;
		}
	}
	else
	{
		ar >> m_nAnalyzed;
	}

	// Read location data
	ar >> nIndex;
	m_pCurrentNode = (CNode *)FindListObject(nIndex, pSystem->m_olNodeList);

	ar >> m_ptNodeLoc;
	// Skip screen coordinates

	// Behavior flags
	ar >> m_nState;			// Current state
	ar >> m_bBypassed;		// Was this ice bypassed?
	ar >> m_bWasAccessed;	// Was this ice accessed by the player
	ar >> m_nLastDir;		// Direction we moved last time

	// Target Node
	ar >> nIndex;
	m_pTargetNode = (CNode *)FindListObject(nIndex, pSystem->m_olNodeList);
}

void CIce::DoWander(CMatrixView *pView)
{
	int nDir;
	int nCount = 0;
	int nDirOpen[4];

	// Get the directions available
	for (nDir = 0; nDir < 4; nDir++)
	{
		// Skip last direction
		if (m_nLastDir == OppDir(nDir)) continue;

		// If open, go
		if (m_pCurrentNode->m_pAdjNode[nDir] != NULL)
		{
			nDirOpen[nCount++] = nDir;
		}
	}

	// If nothing open, go back where we came from
	if (nCount == 0)
	{
		// Last direction is only direction,
		DoMove(OppDir(m_nLastDir), pView);
	}
	else
	{
		// Choose a random direction from the ones available
		nDir = nDirOpen[Random(nCount)];

		// Move to this node
		DoMove(nDir, pView);
	}
}