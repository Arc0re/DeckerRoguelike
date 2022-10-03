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

// ShopItem.cpp: implementation of the CShopItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "ShopItem.h"
#include "Character.h"
#include "Program.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

// Maximum levels of hardware
static const int g_nMaxHWLevel[NUM_HW] = 
{
		4, // HW_CHIP_BURNER
		5, // HW_SURGE_SUPP
		5, // HW_NEURAL_DAMPER
		3, // HW_TRACE_MONITOR
		2, // HW_BIO_MONITOR
		5, // HW_HIGH_BW_BUS
		1, // HW_MAPPER
		3, // HW_DESIGN_ASSIST
		1, // HW_PROXY
};

// Base cost of level 1 hardware. Actual cost is Base * 2^(Rating-1)
static const int g_nHWBaseCost[NUM_HW] = 
{
		1000,	// HW_CHIP_BURNER
		500,	// HW_SURGE_SUPP
		1000,	// HW_NEURAL_DAMPER
		250,	// HW_TRACE_MONITOR
		500,	// HW_BIO_MONITOR
		500,	// HW_HIGH_BW_BUS
		2000,	// HW_MAPPER
		2000,	// HW_DESIGN_ASSIST
		1500,	// HW_PROXY
};

// Base cost of level 1 chip. Actual cost is Base * Rating^2
static const int g_nChipBaseCost[NUM_CHIPS] = 
{
		150,	// CHIP_CPU		
		100,	// CHIP_ATTACK		
		100,	// CHIP_DEFENSE	
		100,	// CHIP_STEALTH	
		100,	// CHIP_ANALYSIS	
		125,	// CHIP_COPROCESSOR
};

// Names of chips
static const char *g_szChipNames[NUM_CHIPS] = 
{
	"CPU",
	"Attack Firmware",
	"Defense Firmware",
	"Stealth Firmware",
	"Analysis Firmware",
	"Coprocessor",
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShopItem::CShopItem()
{
}

CShopItem::~CShopItem()
{
}

void CShopItem::Generate(int nType)
{
	int i;

	// Set the type
	m_nType = nType;
	
	switch (nType)
	{
	case IT_SOFTWARE:
		// Type is random
		m_nSubType = Random(NUM_PROGRAMS);

		// Make sure it is not 'special'
		while (m_nSubType == PROGRAM_CLIENT)
		{
			m_nSubType = Random(NUM_PROGRAMS);
		}

		// Rating is 1..6 + 2*lifestyle (0..4)
		m_nRating = (2*g_pChar->m_nLifestyle) + Random(6) + 1;

		// Alternate method: Rating = 1..4 + 4*LS
		// Base = Random(11)
		//   0..3 = 1
		//   4..6 = 2
		//   7..8 = 3
		//   9 = 4
		//   10 = Random(LS*4), or 1 if LS = 1 (Lower value programs)

		// Get the price
		m_nPrice = GetSoftwarePrice(m_nSubType,m_nRating);

		// Get the text
		GetSoftwareText(m_szText,m_nSubType,m_nRating);
		break;
	case IT_HARDWARE:
		// Type is random
		m_nSubType = Random(NUM_HW);

		// Generate the rating (1..1+LS)
		m_nRating = 1 + Random(g_pChar->m_nLifestyle+1);
		if (m_nRating > g_nMaxHWLevel[m_nSubType]) 
		{
			m_nRating = g_nMaxHWLevel[m_nSubType];
		}

		// Get the cost
		m_nPrice = GetHardwarePrice(m_nSubType,m_nRating);

		// Get the description
		GetHardwareString(m_szText, m_nSubType, m_nRating);

		break;
	case IT_CHIP:
		// Choose random type
		m_nSubType = Random(NUM_CHIPS);

		// Rating is 2..7 + 2*lifestyle (0..4)
		// Minimum is 2 because everyone already has at least rating 1 installed.
		m_nRating = (2*g_pChar->m_nLifestyle) + Random(6) + 2;

		// Get the price according to type
		m_nPrice = GetChipPrice(m_nSubType, m_nRating);
		
		// Get the description
		m_szText.Format("%s  Level %d", g_szChipNames[m_nSubType], m_nRating);
		break;
	}

	// Modify the price by a random factor (+/- 0..15%)
	i = Random(31) - 15; // -15..15%
	m_nPrice += ((m_nPrice * i)/100);
}

void CShopItem::Save(CArchive &ar)
{
	ar << m_szText;
	ar << m_nType;
	ar << m_nSubType;
	ar << m_nRating;
	ar << m_nPrice;
}

void CShopItem::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	ar >> m_szText;
	ar >> m_nType;
	ar >> m_nSubType;
	ar >> m_nRating;
	ar >> m_nPrice;
}

// Global Functions
int GetHardwarePrice(int nType, int nRating)
{
	return (g_nHWBaseCost[nType] * (1<<(nRating-1)));
}

const char * GetChipName(int nType)
{
	return g_szChipNames[nType];
}

int GetChipPrice(int nType, int nRating)
{
	return (nRating * nRating * g_nChipBaseCost[nType]);
}

int GetMaxHardwareRating(int nType)
{
	return g_nMaxHWLevel[nType];
}
