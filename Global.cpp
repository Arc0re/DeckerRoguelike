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

#include "stdafx.h"
#include "Resource.h"
#include "Global.h"
#include "IntroDlg.h"

//------------------
// Global Constants
//------------------
const char *g_szFilter = "Decker Save Games (*.DSG)|*.dsg|All Files (*.*)|*.*||";
const char *g_szExtension = "DSG";

const char *g_szMonthNames[12] = 
{
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

const char *g_szLifestyleString[] = 
{
	"Poverty","Lower Class","Middle Class","Upper Class","Elite" 
};

const int g_nLifestyleMonthlyCost[] = 
{
	500,1000,2000,4000,10000
};

static const int g_nDaysPerMonth[12] = 
{
	31,	//"January",
	28,	//"February",
	31,	//"March",
	30,	//"April",
	31,	//"May",
	30,	//"June",
	31,	//"July",
	31,	//"August",
	30,	//"September",
	31,	//"October",
	30,	//"November",
	31,	//"December"
};

const char *g_szRepLevelString[] = 
{
	"Nobody",
	"Wannabe",
	"Cyber Surfer",
	"Matrix Runner",
	"Newbie Hacker",
	"Journeyman Hacker",
	"Competent Hacker",
	"Experienced Hacker",
	"Hacker Extraordinaire",
	"Cyber Thief",
	"Cyber Sleuth",
	"Cyber Warrior",
	"Cyber Wizard",
	"Ice Crusher",
	"Node Master",
	"System Master",
	"Ghost in the Machine",
	"Digital Dream",
	"Digital Nightmare",
	"Master of the Matrix",
	"Matrix God", //FSO 6-20-01
};

//-----------
// Functions
//-----------

int Random(int nMax)
{
	return (rand()%nMax);
}

int Random2(int nCount, int nMax)
{
	int nData = 0;
	while (nCount--)
	{
		nData += (rand()%nMax);
	}
	return nData;
}

int ChooseRot()
{
	int nRot;
	if (rand()&1)
		nRot = 1; //Clockwise
	else
		nRot = -1; //Counterclockwise
	return nRot;
}


HPALETTE LoadPalette(const char *szFileName, int iNumColors)
{
	CFile cfPalFile;
	LOGPALETTE *pPalette;
	int i;
	HPALETTE hPal;

	// Open the palette file
	if (!cfPalFile.Open(szFileName,CFile::modeRead|CFile::shareExclusive))
	{
		DeckerMessageBox("Failed to load palette");
		return NULL;
	}

	// Jump to the start of the color data (at 24 bytes from start)
	cfPalFile.Seek(24,CFile::begin);

	// Allocate the palette structure
	pPalette = (LOGPALETTE *)malloc(sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * (iNumColors-1)));

	pPalette->palVersion = 0x300; //(?)
	pPalette->palNumEntries = iNumColors;

	for (i=0; i<iNumColors; i++)
	{
		cfPalFile.Read(&(pPalette->palPalEntry[0]), iNumColors * sizeof(PALETTEENTRY));
	}

	cfPalFile.Close();

	// Create a pallette from the information
	hPal = CreatePalette(pPalette);

	free(pPalette);

	return hPal;
}

// Note on this function:
// It returns the success level of running a program. 
// -1 = Catastrophic failure
// 0 = Failure
// 1+ = Success level
int DoDieRoll(int iTargetNumber)
{
	int iRoll;
	int iDiff;
	int iSuccess;

	// Clamp off the target value: 1 is always a failure, 20 is always a success
	if (iTargetNumber > 20) iTargetNumber = 20;
	//else if (iTargetNumber < 2) iTargetNumber = 2;

	// Roll the die
	iRoll = 1+Random(20);

	// Check for critical failure
	if (iRoll == 1)
	{
		iSuccess = -1;
	}
	else
	{
		// Calculate the success level and return it
		iDiff = iRoll-iTargetNumber;
		if (iDiff < 0)
		{
			iSuccess = 0; // Failure
		}
		// FSO Changing the way success is calculated
		//else if (iDiff <= 1)
		//	iSuccess = 1;
		//else if (iDiff <= 4)
		//	iSuccess = 2;
		//else if (iDiff <= 8)
		//	iSuccess = 3;
		//else if (iDiff <= 13)
		//	iSuccess = 4;
		//else // iDiff <= 19
		//	iSuccess = 5;
		else
		{
			iSuccess = ((iDiff+4)/4);
			if (iSuccess > 5)
			{
				iSuccess = 5;
			}
		}
	}

	return iSuccess;
}

int GetConditionModifier(int nCondition)
{
	int nMod;

	//if (nCondition<=4)
	//	nMod = -6;
	//else if (nCondition<=7)
	//	nMod = -4;
	//else if (nCondition<=9)
	//	nMod = -2;
	//else
	//	nMod = 0;
	nMod = ((nCondition - MAX_HEALTH)/4);

	return nMod;
}

int CalcRepPointsAfterLoss(int nNewLevel)
{
	int x = (nNewLevel+1);

	return (((x*x)*REP_LEVEL_FACTOR)/2);
}

int CalcRepPointsForNextLevel(int nCurrentLevel)
{
	int x = (nCurrentLevel+1);

	return ((REP_LEVEL_FACTOR * x * (x+1)) / 2);
}

int GetDays(int nMonth, int nYear)
{
	int nDays;

	nDays = g_nDaysPerMonth[nMonth];

	// Check for leap year
	if ((nMonth == 1) && ((nYear&3)==0))
	{
		nDays++;
	}
	return nDays;
}

int FindListIndex(CObject *pObj, CObList &olList)
{
	int nIndex;
	POSITION p;

	// If null, return -1
	if (pObj==NULL)
	{
		nIndex = -1;
	}
	else
	{
		nIndex = 0;

		p = olList.GetHeadPosition();
		for (;;)
		{
			if (p==NULL)
			{
				nIndex = -1;
				break;
			}

			if (pObj == olList.GetNext(p))
			{
				break;
			}

			nIndex++;
		}
	}

	return nIndex;
}

CObject *FindListObject(int nIndex, CObList &olList)
{
	// If -1, return null
	if (nIndex == -1)
	{
		return NULL;
	}
	else
	{
		// FSO 5-7-02
		POSITION p = olList.FindIndex(nIndex);
		if (p == NULL)
		{
			return NULL;
		}
		else
		{
			return olList.GetAt(p);
		}
	}
}

int DeckerMessageBox( LPCTSTR lpszText, UINT nType)
{
	return ::MessageBox(::GetActiveWindow(),lpszText, "Decker", nType);
}

int ComputeDamage(int iSuccess)
{
	// Original - Damage = success (too low)
	// Now - Damage = sucess squared (too high)
	return (iSuccess * iSuccess);
}

int VersionCompare(WORD wFileMajorVersion, WORD wFileMinorVersion,
				   WORD wNeededMajorVersion, WORD wNeededMinorVersion)
{
	if (wFileMajorVersion == wNeededMajorVersion)
	{
		return wFileMinorVersion - wNeededMinorVersion;
	}
	else
	{
		return wFileMajorVersion - wNeededMajorVersion;
	}
}












