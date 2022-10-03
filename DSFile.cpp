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

// DSFile.cpp: implementation of the CDSFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "DSFile.h"
#include "Node.h"
#include "Area.h"
#include "Character.h"
#include "Global.h"
#include "System.h"
#include "Contract.h"
#include "Ice.h"
#include "Program.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

#define NUM_VAL_FILES 14

static const char *g_szValuableFileContents[NUM_VAL_FILES] = 
{
	"Research data",
	"Chemical formulae",
	"Personnel files",
	"Financial data",
	"Grade reports",
	"Security files",
	"Security camera recordings",
	"Blueprints",
	"Employee evaluation records",
	"Product information",
	"Medical records",
	"Records of illegal activity",
	"Test results",
	"Inventory records",
};

static const char *g_szUselessFileContents[] = 
{
	"Employee timesheets",
	"Sales brochures",
	"Employee mail",
	"Someone's poetry attempts",
	"A Christmas list",
	"Office party supply list",
	"Public service sign-up list",
	"Recording of the big game",
	"Garbage",
	"To-do list",
	"Outdated files",
	"Wedding guest list",
	"Bridal registry",
	"Grocery list",
	"Pictures of the family",
	"Pictures from office party",
	"Recordings from promotional event",
	"Yesterday's news",
	"Rejected ideas",
	"Someone's budget",
	"Garbled file",
	"Country music recordings",
	"Rock & Roll music recordings",
	"Easy listening music recordings",
	"Jazz music recordings",
	"Recording of Superbowl LXXXII",
	"Editorials",
	"Company history",
	"Educational media",
	"Hardware manual",
	"Software manual",
	"Motivational video",
	"A romance novel",
	"A sci-fi novel",
	"A mystery novel",
	"A thriller novel",
	"A romance holovid",
	"A sci-fi holovid",
	"A thriller holovid",
	"An action holovid",
	"A horror holovid",
	"Pirate copy of Starcraft 7",
	"A love letter",
	"Hate mail",
	"Joke of the day",
	"Top 10 list",
	"Letter to a relative",
	"Phonebook",
	"Travel brochures",
	"Junk mail",
	"Incomprehensible data",
	"Ramblings of a lunatic",
	"Recipe book",
	"A family tree",
};

#define NUM_USELESS_FILES (sizeof(g_szUselessFileContents) / sizeof(char*))

static const char *g_szClueString = "<Clue>";
static const char *g_szPasscodeString = "<Passcode File>";
static const char *g_szProgramString = "<Program>";
static const char *g_szSourceString = "<Source Code>";

// File scan strings
static const char *g_szScanString[] = {
	"Worthless",
	"Valuable",
	"*Passcode*",
	"*Clue*",
	"*Contract*",
	"*Program*",
	"*Source Code*",
	"*Source Code*",
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDSFile::CDSFile()
{
	//m_szName;
	m_nType = FT_USELESS;
	m_nValue = 0;
	m_pTapeworm = NULL;
	m_nState = STATE_IN_NODE;
}

CDSFile::~CDSFile()
{

}

const char * CDSFile::GetContentString()
{
	switch (m_nType)
	{
	case FT_USELESS:
		return g_szUselessFileContents[m_nContents];
	case FT_CLUE:
		return g_szClueString;
	case FT_PASSCODE:
		return g_szPasscodeString;
	case FT_PROGRAM:
		return g_szProgramString;
	case FT_P_SOURCE:
	case FT_C_SOURCE:
		return g_szSourceString;
	//case FT_VALUABLE:
	//case FT_QUEST:
	default:
		return g_szValuableFileContents[m_nContents];
	}
}

void CDSFile::Generate(CNode *pParentNode)
{
	int nBase;
	int nRoll;

	// Choose contents
	switch (m_nType)
	{
	case FT_USELESS:
		// There is a 25% chance that this is a valuable file type with no value
		if (Random(4)==0)
		{
			// Change type to valuable
			m_nType = FT_VALUABLE;
			m_nContents = GetFileTypePerCorp();
		}
		else
		{
			m_nContents = Random(NUM_USELESS_FILES);
		}
		break;
	case FT_VALUABLE:
		m_nContents = GetFileTypePerCorp();

		// Set value according to difficulty (5d9 + (10*Diff), +/- 15%) Averages to 25+(10*Diff)
		m_nValue = (5+Random2(5,9)) + (10 * g_pChar->m_pSystem->m_nRating);
		m_nValue += ((m_nValue * (Random(31) - 15))/100);
		break;
	case FT_QUEST:
		m_nContents = g_pChar->m_pCurrentContract->m_nTargetObject;
		break;
	case FT_CLUE:
		// No value here
		break;
	case FT_PROGRAM:
	case FT_P_SOURCE:
	case FT_C_SOURCE:
		if (m_nType == FT_C_SOURCE)
		{
			// Contents is actually chip type
			m_nContents = Random(NUM_CHIPS);
		}
		else
		{
			// Contents is actually program type
			//FSO 5-31-04 Bug fix - no client programs allowed
			do
			{
				m_nContents = Random(NUM_PROGRAMS);
			} while (m_nContents == PROGRAM_CLIENT);
		}
		// Value is rating
		nRoll = Random(100);
		if (nRoll < 15)
		{
			m_nValue = g_pChar->m_pSystem->m_nRating - 2;
		}
		else if (nRoll < 45)
		{
			m_nValue = g_pChar->m_pSystem->m_nRating - 1;
		}
		else if (nRoll < 85)
		{
			m_nValue = g_pChar->m_pSystem->m_nRating;
		}
		else if (nRoll < 95)
		{
			m_nValue = g_pChar->m_pSystem->m_nRating + 1;
		}
		else //if (nRoll < 100)
		{
			m_nValue = g_pChar->m_pSystem->m_nRating + 2;
		}

		if (m_nValue < 1)
		{
			m_nValue = 1;
		}
		// No maximum size
		break;
	}

	// Compute size
	if (m_nType == FT_PROGRAM)
	{
		m_nSize = GetProgramSize(m_nContents, m_nValue);
	}
	else if (m_nType == FT_P_SOURCE)
	{
		m_nSize = 2 * GetProgramSize(m_nContents, m_nValue);
	}
	else if (m_nType == FT_C_SOURCE)
	{
		m_nSize = 2 * GetChipComplexity(m_nContents) * m_nValue;
	}
	else
	{
		// Size is (Diff*2)d4, averages to 5*diff
		nBase = 2*g_pChar->m_pSystem->m_nRating;
		m_nSize = nBase+Random2(nBase,4);
	}

	// Generate the file name
	m_szName.Format("%s-%04X",(const char *)pParentNode->m_szName,(UINT)Random(0x10000));
}

int CDSFile::GetLoadTime()
{
	int nTime;

	// If have a high-speed connection, time is 1 turn
	if ((g_pChar->m_pCurrentNode->m_pParentArea->m_pHighSpeedIONode != NULL) &&
		(g_pChar->m_pCurrentNode->m_pParentArea->m_pHighSpeedIONode->m_bActivated))
	{
		// One turn download
		nTime = 1;
	}
	else
	{
		// Time is size / (2^(bus size))
		int nRate = (1 << (g_pChar->m_nHardware[HW_HIGH_BW_BUS]));
		nTime =  ((m_nSize + nRate - 1) / nRate);
		if (nTime<1) 
		{
			nTime = 1;
		}
	}

	return nTime;
}
BOOL CDSFile::IsValidForScanEval(int nProgramClass)
{
	if (nProgramClass == PROGRAM_SCAN)
	{
		// Is this file valid? (Not scanned, not protected, not deleted)
		if (((m_nState & STATE_SCAN)==0) && 
			(m_nState & STATE_IN_NODE) &&
			((m_pTapeworm == NULL) || (m_pTapeworm->m_bBypassed)))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else //if (nProgramClass == PROGRAM_EVAL)
	{
		// Is this file valid? (Scanned, not evaluated, not protected, not deleted, not quest or worthless)
		if ((m_nState & STATE_SCAN) && 
			((m_nState & STATE_EVAL)==0) && 
			(m_nState & STATE_IN_NODE) &&
			(m_nType != FT_USELESS)  && (m_nType != FT_QUEST) &&
			(m_pTapeworm == NULL))// || (m_pTapeworm->m_bBypassed))) //No evaluation on tapeworms
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

void CDSFile::Save(CArchive &ar)
{
	ar << m_szName;
	ar << m_nType;
	ar << m_nValue;
	ar << m_nContents;
	ar << m_nState;
	// Note, we skip the tapeworm. It will be handled when ice is saved
	ar << m_nSize;
}

void CDSFile::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	ar >> m_szName;
	ar >> m_nType;
	ar >> m_nValue;
	ar >> m_nContents;
	ar >> m_nState;
	// Note, we skip the tapeworm. It will be handled when ice is loaded
	ar >> m_nSize;
}

const char * CDSFile::GetScanString()
{
	return g_szScanString[m_nType];
}
