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

// Program.h: interface for the CProgram class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROGRAM_H__3AA7B782_8233_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_PROGRAM_H__3AA7B782_8233_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Program Types
#define PROGRAM_ATTACK			0
#define PROGRAM_ATTACK_A		1
#define PROGRAM_ATTACK_P		2
#define PROGRAM_SLOW			3
#define PROGRAM_VIRUS			4
#define PROGRAM_SILENCE			5
#define PROGRAM_CONFUSE			6
#define PROGRAM_WEAKEN			7
#define PROGRAM_SHIELD			8
#define PROGRAM_SMOKE			9
#define PROGRAM_DECOY			10
#define PROGRAM_MEDIC			11
#define PROGRAM_ARMOR			12
#define PROGRAM_HIDE			13
#define PROGRAM_DECEIVE			14
#define PROGRAM_RELOCATE		15
#define PROGRAM_ANALYZE			16
#define PROGRAM_SCAN			17
#define PROGRAM_EVALUATE		18
#define PROGRAM_DECRYPT			19
#define PROGRAM_REFLECT			20
#define PROGRAM_ATTACK_BOOST	21
#define PROGRAM_DEFENSE_BOOST	22
#define PROGRAM_STEALTH_BOOST	23
#define PROGRAM_ANALYSIS_BOOST	24
#define PROGRAM_CLIENT			25	// Special program for missions only

#define NUM_PROGRAMS			26

class CProgram : public CObject  
{
public:
	// Construction/Destruction
	CProgram();
	virtual ~CProgram();

	// Member Fuctions
	void GetInitValues(int nClass, int nRating);
	int GetLoadTime();
	int GetSize();
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion);

	// Data Members
	CString m_szName;
	int m_nClass;
	int m_nRating;
	BOOL m_bLoadByDefault;
	int m_nLoadedRating;
	int m_nSound;
};

// Global functions
int GetSoftwarePrice(int nClass, int nRating);
void GetSoftwareText(CString &szStr, int nClass, int nRating);
const char *GetProgramClassName(int nClass);
int GetProgramComplexity(int nClass);
int GetProgramSize(int nClass, int nRating);

#endif // !defined(AFX_PROGRAM_H__3AA7B782_8233_11D4_B835_0080C8C1DB04__INCLUDED_)
