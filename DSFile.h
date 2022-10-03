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

// DSFile.h: interface for the CDSFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSFILE_H__44080461_88BD_11D4_82AF_00104B751D91__INCLUDED_)
#define AFX_DSFILE_H__44080461_88BD_11D4_82AF_00104B751D91__INCLUDED_

class CIce;
class CNode;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// File types
#define FT_USELESS	0
#define FT_VALUABLE	1
#define FT_PASSCODE	2
#define FT_CLUE		3	//FSO 6-20-01
#define FT_QUEST	4
#define FT_PROGRAM	5	//FSO 12-17-01
#define FT_P_SOURCE	6
#define FT_C_SOURCE	7

// States (flags)
#define STATE_IN_NODE	0x01	// It is in the node
#define STATE_IN_DECK	0x02	// It is in the deck (downloaded)
#define STATE_EDITED_N	0x04	// It has been edited in the node
#define STATE_EDITED_D	0x08	// It has been edited in the deck (useless)
#define STATE_SCAN		0x10	// It has been scanned (contents known)
#define STATE_EVAL		0x20	// It has been evaluated (value known)

class CDSFile : public CObject  
{
public:
	CDSFile();
	virtual ~CDSFile();

	CString m_szName;	// File name
	int m_nType;		// Type (Valuable, quest, useless, etc.)
	int m_nValue;		// Value of file (or type for programs)
	int m_nContents;	// Contents
	int m_nState;		// Current state
	CIce *m_pTapeworm;	// Guardian tapeworm
	int m_nSize;		// Size of file (MP)
	
	const char * GetContentString();
	void Generate(CNode *pParentNode);
	int GetLoadTime();
	BOOL IsValidForScanEval(int nProgramClass);
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion);
	const char * GetScanString();
};

#endif // !defined(AFX_DSFILE_H__44080461_88BD_11D4_82AF_00104B751D91__INCLUDED_)
