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

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

// Color definitions
#define BLACK (COLORREF)0
#define WHITE (COLORREF)0x00FFFFFF
#define DK_GRAY (COLORREF) 0x00808080
#define LT_GRAY (COLORREF) 0x00C0C0C0
#define BLUE (COLORREF)0x00FF0000
#define GREEN (COLORREF)0x0000FF00
#define RED (COLORREF) 0x000000FF
#define YELLOW (COLORREF) 0x0000FFFF
#define DK_BLUE (COLORREF)0x00800000
#define DK_GREEN (COLORREF)0x00008000
#define ORANGE (COLORREF) 0x000080FF
#define PURPLE (COLORREF) 0x00FF00FF
#define AQUA (COLORREF) 0x00FFFF00
#define DK_AQUA (COLORREF)0x00808000

// Reputation level factor - # points to gain level (relative)
#define REP_LEVEL_FACTOR	5
#define LIFESTYLE_UPGRADE_FACTOR	3

// Maximum health
#define MAX_HEALTH 20
#define HEALTH_INCREMENT 5

// Global Variables
extern const char *g_szFilter;
extern const char *g_szExtension;
extern const char *g_szMonthNames[12];
extern const char *g_szLifestyleString[];
extern const int g_nLifestyleMonthlyCost[];
extern const char *g_szRepLevelString[];

// Global functions
int Random(int nMax);
int Random2(int nCount, int nMax);
int ChooseRot();
int DoDieRoll(int iTargetNumber);
int GetConditionModifier(int nCondition);
int CalcRepPointsAfterLoss(int nNewLevel);
int CalcRepPointsForNextLevel(int nCurrentLevel);
int GetDays(int nMonth, int nYear);
int FindListIndex(CObject *pObj, CObList &olList);
CObject *FindListObject(int nIndex, CObList &olList);
int DeckerMessageBox( LPCTSTR lpszText, UINT nType = MB_OK);
int ComputeDamage(int iSuccess);
int VersionCompare(WORD wFileMajorVersion, WORD wFileMinorVersion,
				   WORD wNeededMajorVersion, WORD wNeededMinorVersion);

//HPALETTE LoadPalette(const char *szFileName, int iNumColors);

#endif //__GLOBAL_H__
