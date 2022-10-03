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

// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__E6A87294_7B31_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_NODE_H__E6A87294_7B31_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArea;

// Node Types
#define NT_CPU			0	// Central Processing Unit
#define	NT_SPU			1	// Sub Processing Unit
#define NT_COP			2	// Coprocessor
#define	NT_DS			3	// Data store
#define	NT_IO			4	// I/O Controller
#define NT_JUNC			5	// Junction
#define NT_PORTAL_IN	6	// Portal
#define NT_PORTAL_OUT	7	// Portal

#define NUM_NODES	8

// Directions
#define DIR_NORTH	0
#define DIR_EAST	1
#define DIR_SOUTH	2
#define DIR_WEST	3

#define DIR_NONE	-1
#define DIR_CENTER	-2

// Macro to get the opposite direction
#define OppDir(n) ((n+2)%4)

// Node subtypes
#define NST_IO_USELESS		0	// Useless IO node
#define NST_IO_QUEST_NODE	1	// This is the quest node
#define NST_IO_ALARM		2	// IO Node - alarm
#define NST_IO_ICE_PORT		3	// IO Node - ICE port - ice respawn here (not impl.)
#define NST_IO_MATRIX		4	// High-speed matrix port

#define NST_DS_NORMAL		0
#define NST_DS_QUEST_NODE	1

#define NST_COP_NORMAL		0
#define NST_COP_SECURITY	1	// FSO 12-17-01

// Node special images
#define NSI_DS_QUEST		0
#define NSI_IO_QUEST		1
#define NSI_IO_ALARM		2
#define NSI_IO_ICE_PORT		3
#define NSI_IO_MATRIX		4
#define NSI_COP_SECURITY	5

class CNode : public CObject  
{
public:
	CNode(CArea *pParentArea, int nNodeType = 0, int nNodeNumber = 0);
	virtual ~CNode();

	CArea	*m_pParentArea;			// Parent area
	CString	m_szName;				// Node name
	int		m_nType;				// Node Type
	int		m_nSubType;				// Sub Type (Currently only for IO nodes)
	int		m_nCluster;				// Cluster #
	int		m_nNodeNumber;			// Node number
	CNode	*m_pAdjNode[4];			// Adjacent nodes
	CPoint	m_ptLocation;			// Current location
	CObList	m_olFileList;			// File list for datastores
	int		m_nActiveSmokeLevel;	// Level of active smoke program
	int		m_nActiveSilenceLevel;	// Level of active silence program
	BOOL	m_bActivated;			// Has it been activated?
	BOOL	m_bMapped;				// Has this node been mapped?
	BOOL	m_nSpecialImage;		// Special image on the map //FSO 6-20-01

	// Temporary data
	union {
	BYTE	m_bProcessed[4];	// Processing flags used in various situations
	DWORD	m_dwProcessed;
	};

	// Functions
	BOOL ChooseLocation(CPoint &ptLoc, int nDir);
	BOOL ChooseClosestLocation(CPoint &ptLoc);
	BOOL ChooseAnyLocation(CPoint &ptLoc);
	BOOL IsEmptyLocation(CPoint &ptLoc);
	void CreateClusters(int &nClusterNumber, int nDirFrom, int &nSubNodes, BOOL &bHasCOP);
	void MarkNewCluster(int nClusterNumber, int nDirFrom);
	void GenerateName();
	void GenerateICE();
	void CreateFileList();
	void MarkPaths(int nDirFrom, int iIndex);
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion);
};

#endif // !defined(AFX_NODE_H__E6A87294_7B31_11D4_B835_0080C8C1DB04__INCLUDED_)
