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

// Node.cpp: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Node.h"
#include "Character.h"
#include "Ice.h"
#include "Global.h"
#include "System.h"
#include "Contract.h"
#include "DSFile.h"
#include "Area.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

// Node abbreviations
static const char *g_szNodeAbbrev[NUM_NODES] = 
{
	"CPU","SPU","COP","DS","IO","JN","PI","PO"
};

//-----------------------
// Node Grid Information
//-----------------------
// Number of squares in the grid (hor+vert)
#define NODE_GRID_SIZE 9

// Node grid structure
typedef struct {
	BYTE nLocation[NODE_GRID_SIZE][NODE_GRID_SIZE];
} NODE_GRID;

// Node grid template data
const static BYTE g_nGridTemplateCPU[9][9] = 
{
	1,1,1,0,0,0,1,1,1,
	1,1,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,1,
	0,0,0,1,1,1,0,0,0,
	0,0,0,1,1,1,0,0,0,
	0,0,0,1,1,1,0,0,0,
	1,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,1,1,
	1,1,1,0,0,0,1,1,1,
};
const static BYTE g_nGridTemplateSPU[9][9] = 
{
	1,1,1,0,0,0,1,1,1,
	1,1,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,1,
	0,0,0,1,0,1,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,1,0,1,0,0,0,
	1,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,1,1,
	1,1,1,0,0,0,1,1,1,
};
const static BYTE g_nGridTemplateCOP[9][9] = 
{
	1,1,1,0,0,0,1,1,1,
	1,1,1,0,0,0,1,1,1,
	1,1,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,1,
	0,0,0,0,1,0,0,0,0,
	1,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,1,1,
	1,1,1,0,0,0,1,1,1,
	1,1,1,0,0,0,1,1,1,
};
const static BYTE g_nGridTemplatePortal[9][9] = 
{
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,1,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
};
const static BYTE g_nGridTemplateDS[9][9] = 
{
	1,1,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,0,
	0,1,0,1,0,0,1,0,0,
	0,0,0,1,1,0,0,0,0,
	0,0,0,0,1,0,1,0,0,
	0,0,0,1,0,0,0,0,0,
	1,0,0,0,0,1,0,0,0,
	0,0,0,0,0,0,0,1,1,
	0,1,0,0,0,0,0,1,1,
};
const static BYTE g_nGridTemplateIO[9][9] = 
{
	1,1,1,0,0,0,1,1,1,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,1,
	1,0,0,0,0,0,0,0,1,
	1,1,1,0,0,0,1,1,1,
};
const static BYTE g_nGridTemplateJunction[9][9] = 
{
	1,1,1,1,0,1,1,1,1,
	1,1,1,0,0,0,1,1,1,
	1,1,0,0,0,0,0,1,1,
	1,0,0,0,0,0,0,0,1,
	0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,0,1,
	1,1,0,0,0,0,0,1,1,
	1,1,1,0,0,0,1,1,1,
	1,1,1,1,0,1,1,1,1,
};

static const NODE_GRID *g_NodeGridTemplate[NUM_NODES] =
{
	(NODE_GRID *)g_nGridTemplateCPU,
	(NODE_GRID *)g_nGridTemplateSPU,
	(NODE_GRID *)g_nGridTemplateCOP,
	(NODE_GRID *)g_nGridTemplateDS,
	(NODE_GRID *)g_nGridTemplateIO,
	(NODE_GRID *)g_nGridTemplateJunction,
	(NODE_GRID *)g_nGridTemplatePortal,
	(NODE_GRID *)g_nGridTemplatePortal,
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNode::CNode(CArea *pParentArea, int nNodeType, int nNodeNumber)
{
	m_pParentArea = pParentArea;
	m_nType = nNodeType;
	m_nNodeNumber = nNodeNumber;
	m_bMapped = FALSE;
	m_nSubType = 0;
	m_bActivated = FALSE;
	m_nSpecialImage = -1; //FSO 6-20-01

	for (int i=0; i<4; i++)
	{
		m_pAdjNode[i] = NULL;
	}
	m_ptLocation.x = 0;
	m_ptLocation.y = 0;
	m_nCluster = 0;
	m_nActiveSmokeLevel = 0;
	m_nActiveSilenceLevel = 0;
}

CNode::~CNode()
{
	CDSFile *pFile;

	// Clean up the file list
	while (!m_olFileList.IsEmpty())
	{
		pFile = (CDSFile *)m_olFileList.RemoveHead();
		delete pFile;
	}
}

BOOL CNode::ChooseLocation(CPoint &ptLoc, int nDir)
{
	CPoint ptTmp;

	// Choose a location dependant on desired direction
	switch (nDir)
	{
	case DIR_NORTH	:
		ptTmp.x = 4;
		ptTmp.y = 0;
		break;
	case DIR_EAST	:
		ptTmp.x = 8;
		ptTmp.y = 4;
		break;
	case DIR_SOUTH	:
		ptTmp.x = 4;
		ptTmp.y = 8;
		break;
	case DIR_WEST	:
		ptTmp.x = 0;
		ptTmp.y = 4;
		break;
	case DIR_CENTER:
		ptTmp.x = 4;
		ptTmp.y = 4;
		break;
	//case DIR_NONE:
	default:
		// Choose any location
		if (!ChooseAnyLocation(ptTmp))
		{
			// Should never happen
			return FALSE;
		}

		ptLoc = ptTmp;
		return TRUE;
	}

	if (!ChooseClosestLocation(ptTmp))
	{
		// Should never happen
		return FALSE;
	}

	ptLoc = ptTmp;
	return TRUE;
}

BOOL CNode::ChooseClosestLocation(CPoint &ptLoc)
{
	int nTries;
	int nMinX, nMaxX, nMinY, nMaxY;

	// Get a pointer to the background map for this node
	const NODE_GRID *pGrid = g_NodeGridTemplate[m_nType];

	// Try for the target location
	if ((pGrid->nLocation[ptLoc.y][ptLoc.x]==0) &&
	    (IsEmptyLocation(ptLoc)))
	{
		return TRUE;
	}

	// Target is not empty, so try expanding radius until one is found
	nMinX = ptLoc.x;
	nMaxX = ptLoc.x;
	nMinY = ptLoc.y;
	nMaxY = ptLoc.y;
	while ((nMinX!=0)||(nMaxX!=8)||(nMinY!=0)||(nMaxY!=8))
	{
		// Create the range of locations
		if (nMinX!=0) nMinX--;
		if (nMaxX!=8) nMaxX++;
		if (nMinY!=0) nMinY--;
		if (nMaxY!=8) nMaxY++;

		// Set the tries
		nTries = 100;

		int nRangeX = 1 + nMaxX - nMinX;
		int nRangeY = 1 + nMaxY - nMinY;

		while (nTries--)
		{
			// Choose a random point in range
			ptLoc.x = Random(nRangeX) + nMinX;
			ptLoc.y = Random(nRangeY) + nMinY;

			if ((pGrid->nLocation[ptLoc.y][ptLoc.x]==0) &&
				(IsEmptyLocation(ptLoc)))
			{
				return TRUE;
			}
		}
	}

	// Node is full
	return FALSE;
}

BOOL CNode::ChooseAnyLocation(CPoint &ptLoc)
{
	int nTries;

	// Get a pointer to the background map for this node
	const NODE_GRID *pGrid = g_NodeGridTemplate[m_nType];

	// Set the tries
	nTries = 100;

	while (nTries--)
	{
		// Choose a random point in range
		ptLoc.x = Random(8);
		ptLoc.y = Random(8);

		if ((pGrid->nLocation[ptLoc.y][ptLoc.x]==0) &&
			(IsEmptyLocation(ptLoc)))
		{
			return TRUE;
		}
	}

	// Node is full
	return FALSE;
}

BOOL CNode::IsEmptyLocation(CPoint &ptLoc)
{
	int i;

	// Is the character at this location
	if (g_pChar->m_pCurrentNode==this)
	{
		// Is character at this spot?
		if (g_pChar->m_ptNodeLoc == ptLoc)
		{
			return FALSE;
		}

		// Is a decoy at this location
		for (i = 0; i < g_pChar->m_nDecoyCount; i++)
		{
			if (ptLoc == g_pChar->m_ptDecoyLocation[i])
			{
				return FALSE;
			}
		}
	}

	// Look for ICE at this location
	POSITION p;
	CIce *pIce;
	p = g_pChar->m_pSystem->m_olIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);

		if ((pIce->m_pCurrentNode==this) &&
			(pIce->m_ptNodeLoc==ptLoc))
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CNode::CreateClusters(int &nClusterNumber, int nDirFrom, int &nSubNodes, BOOL &bHasCOP)
{
	int nDir;

	// If this is a junction, process the sub nodes, and decide on gateway.
	// The is because at this point, only junctions and the CPU/SPU have multiple connections.
	if (m_nType == NT_JUNC)
	{
		for (nDir = 0; nDir < 4; nDir++)
		{
			// Ignore the direction we just came from
			if (nDir == nDirFrom) continue;

			// Process if there is a connection
			if (m_pAdjNode[nDir]!=NULL)
			{
				m_pAdjNode[nDir]->CreateClusters(	nClusterNumber,
													OppDir(nDir),
													nSubNodes, 
													bHasCOP);
			}
		}

		// Decide if we need to create a cluster.
		// Basically, we must have at least 1 coprocessor and 4 other nodes total
		if ((bHasCOP) && (nSubNodes>=4))
		{
			// Set the cluster numbers for all the the nodes
			MarkNewCluster(nClusterNumber, nDirFrom);
			nClusterNumber++;

			// Reset the pertinent information
			bHasCOP = FALSE;
			nSubNodes = 0;
		}
	}
	// Is this a portal?
	else if ((m_nType==NT_PORTAL_IN) || (m_nType==NT_PORTAL_OUT))
	{
		// Portal automatically qualifies as a cluster unto itself
		m_nCluster = nClusterNumber++;
	}
	// Is this a coprocessor?
	else if (m_nType==NT_COP)
	{
		bHasCOP = TRUE;
	}
	// Must be an IO/DS/Portal IN
	{
		nSubNodes++;
	}
}


void CNode::MarkNewCluster(int nClusterNumber, int nDirFrom)
{
	int nDir;

	// Set the sub-nodes cluster number
	for (nDir = 0; nDir < 4; nDir++)
	{
		// Ignore parent direction
		if (nDir==nDirFrom) continue;

		// Mark if there is a connection and the connected node is in the same cluster as us
		if ((m_pAdjNode[nDir] != NULL) && (m_pAdjNode[nDir]->m_nCluster == m_nCluster))
		{

			m_pAdjNode[nDir]->MarkNewCluster(nClusterNumber, OppDir(nDir));
		}
	}

	// Set our cluster number;
	m_nCluster = nClusterNumber;
}

void CNode::GenerateName()
{
	m_szName.Format("%s-%01X%01X%03X",
		g_szNodeAbbrev[m_nType],
		m_pParentArea->m_nAreaNumber,
		m_nCluster,
		m_nNodeNumber);
}

void CNode::GenerateICE()
{
	int nDifficulty;
	CIce *pIce;
	int nRoll;
	int nNumIce;
	int nDir;

	// Get the difficulty level of this mission. This will be the base ICE difficulty
	nDifficulty = g_pChar->m_pSystem->m_nRating;

	//--------------------------
	// Generate the GATEWAY ice
	//--------------------------
	// Gateway is generated at any entry which crosses a cluster boundary
	for (nDir = 0; nDir<4; nDir++)
	{
		if ((m_pAdjNode[nDir]!=NULL) && (m_pAdjNode[nDir]->m_nCluster != m_nCluster))
		{
			// Generate a gateway ice here
			pIce = new CIce;
			pIce->m_nType = ICE_GATEWAY;
			pIce->m_nSubType = nDir;
			pIce->m_nRating = nDifficulty;
			pIce->m_pHomeNode = this;
			pIce->m_nState = STATE_GUARDING;

			// Adjust the rating if this is a CPU/SPU, or if we are connecting to one.
			nRoll = Random(20);
			if ((m_nType == NT_CPU) || 
				(m_nType == NT_SPU) || 
				(m_pAdjNode[nDir]->m_nType == NT_CPU) ||
				(m_pAdjNode[nDir]->m_nType == NT_SPU))
			{
				pIce->m_nRating++;
			}

			// Now, do a random adjustment to rating. (10% +1, 5% +2)
			nRoll = Random(20);
			if (nRoll==0)
			{
				// Must be at least Lv 2
				if (nDifficulty >= 2)
				{
					pIce->m_nRating++;
				}
			}
			else if (nRoll<=2)
			{
				// Must be at least Lv 4
				if (nDifficulty >= 4)
				{
					pIce->m_nRating += 2;
				}
			}

			// Load the ICE data
			pIce->LoadInitData();

			// Put the ICE in the node. Since gateway is the first ICE to go into a node,
			// we can automatically assign a location at the doorway.
			pIce->m_pCurrentNode = this;
			if (nDir==DIR_NORTH)
			{
				pIce->m_ptNodeLoc.x = 4;
				pIce->m_ptNodeLoc.y = 0;
			}
			else if (nDir==DIR_EAST)
			{
				pIce->m_ptNodeLoc.x = 8;
				pIce->m_ptNodeLoc.y = 4;
			}
			else if (nDir==DIR_WEST)
			{
				pIce->m_ptNodeLoc.x = 0;
				pIce->m_ptNodeLoc.y = 4;
			}
			else //if (nDir==DIR_SOUTH)
			{
				pIce->m_ptNodeLoc.x = 4;
				pIce->m_ptNodeLoc.y = 8;
			}

			// Add the ICE to the system's ICE list
			g_pChar->m_pSystem->m_olIceList.AddTail(pIce);
		}
	}

	//-------------------------------------
	// Generate the other ICE
	//-------------------------------------
	// No ICE in a junction
	if (m_nType == NT_JUNC)
	{
		return;
	}

	// Get the number of ICE.
	// This is a roll 0..4, +1 for SPU/Portal IN, +2 for CPU/COP/Security
	// 0 = no ice. 1 = 1 ice. 2,3 = 2 ice. 4,5 = 3 ice
	nRoll = Random(4);
	if ((m_nType == NT_SPU) || (m_nType == NT_PORTAL_IN) || (m_nType==NT_PORTAL_OUT))
	{
		nRoll++;
	}
	else if ((m_nType == NT_CPU) || (m_nType == NT_COP) || 		
			 ((m_nType == NT_COP) && (m_nSubType == NST_COP_SECURITY))) //FSO 12-17-01
	{
		nRoll += 2;
	}

	if (nRoll==0)
	{
		nNumIce = 0;
	}
	else if (nRoll == 1)
	{
		nNumIce = 1;
	}
	//FSO 3-29-04 Fixing bug that missed case 5
	else if ((nRoll == 2) || (nRoll = 3))
	{
		nNumIce = 2;
	}
	else
	{
		nNumIce = 3;
	}

	// If this is the system portal, max at 1 ice to make it a little easier to get in
	if ((m_pParentArea->m_pSystem->m_pSystemPortalOut == this) &&
		(nNumIce>1))
	{
		nNumIce = 1;
	}

	while (nNumIce--)
	{
		// Create a new ICE
		pIce = new CIce;

		nDir = DIR_NONE;

		// Set the type. 
		if (nNumIce==0)
		{
			// First ice is always white. For COP, it is probe. For all others, it is guardian.
			if ((m_nType == NT_COP) && (m_nSubType != NST_COP_SECURITY))
			{
				pIce->m_nType = ICE_PROBE;
				pIce->m_nState = STATE_SEARCHING;
			}
			else if (this == m_pParentArea->m_pAreaPortalOut)
			{
				// This is the system portal. Guardian is is useless.
				// Make this a response attack ice.
				pIce->m_nType = ICE_ATTACK;
				pIce->m_nState = STATE_INACTIVE;
				pIce->m_bResponse = TRUE;
			}
			else
			{
				// For all other nodes, first ICE is guardian
				pIce->m_nType = ICE_GUARDIAN;
				pIce->m_nState = STATE_GUARDING;
				nDir = DIR_CENTER;
			}
		}
		else if (nNumIce==1)
		{
			// Second ice is random black ice. (75% attack, 25% trace)
			// No Trace ice on L1 and L2
			nRoll = Random(4);
			if ((nDifficulty>2) && (nRoll == 0))
			{
				pIce->m_nType = ICE_TRACE;
			}
			else
			{
				pIce->m_nType = ICE_ATTACK;
			}

			// Ice is response if it is in a coprocessor
			if ((m_nType == NT_COP) && (m_nSubType != NST_COP_SECURITY))
			{
				pIce->m_bResponse = TRUE;
			}
			pIce->m_nState = STATE_INACTIVE;
		}
		else
		{
			// Third ice is always attack
			pIce->m_nType = ICE_ATTACK;

			// Ice is response if it is in a coprocessor
			if ((m_nType == NT_COP) && (m_nSubType != NST_COP_SECURITY))
			{
				pIce->m_bResponse = TRUE;
			}
			pIce->m_nState = STATE_INACTIVE;
		}

		// If this is attack/trace, we have to determine a sub-type
		if (pIce->m_nType == ICE_ATTACK)
		{
			// Determine if normal/lethal
			nRoll = Random(100)+1;
			if (nRoll < ((nDifficulty-10)*5))
			{
				pIce->m_nSubType = AST_KILLER;
			}
			else
			{
				pIce->m_nSubType = AST_NORMAL;
			}

			// Determine if hardened/phasing/c+b (3% per level over 5)
			nRoll = Random(100)+1;
			if (nRoll < ((nDifficulty-5)*3))
			{
				// Ice has options. Choose which
				switch (Random(5))
				{
				case 0:
				case 1:
					// Hardened
					pIce->m_nSubType |= AST_HARDENED;
					break;
				case 2:
				case 3:
					// Phasing
					pIce->m_nSubType |= AST_PHASING;
					break;
				case 4:
					// Crash & Burn
					pIce->m_nSubType |= AST_CRASH;
					break;
				}
			}
		}
		else if (pIce->m_nType == ICE_TRACE)
		{
			// Distribution is as follows:
			// Normal: 100% up to L5, -5% each, 25% @20
			// Fry: 0% up to L10, 2% each, 20% @ 20
			// Dump: Non linear. Whatever is left.
			int nChanceNormal = 5*(25 - nDifficulty);	
			int nChanceFry = 2*(nDifficulty-10);		

			nRoll = Random(100);
			if (nRoll<nChanceNormal)
			{
				// Most common at lower levels
				pIce->m_nSubType = TST_NORMAL;
			}
			else if (nRoll < (nChanceFry+nChanceNormal))
			{
				// Only at upper levels
				pIce->m_nSubType = TST_FRY;
			}
			else
			{
				// Most common at mid to upper levels
				pIce->m_nSubType = TST_DUMP;
			}
		}

		pIce->m_pHomeNode = this;
		pIce->m_nRating = nDifficulty;
		
		// Adjust the rating if this is a CPU/SPU/Security
		nRoll = Random(20);
		if ((m_nType == NT_CPU) || (m_nType == NT_SPU) ||
			((m_nType == NT_COP) && (m_nSubType == NST_COP_SECURITY)))
		{
			pIce->m_nRating++;
		}

		// Now, do a random adjustment to rating. (10% +1, 5% +2)
		nRoll = Random(20);
		if (nRoll==0)
		{
			// Must be at least Lv 2
			if (nDifficulty >= 2)
			{
				pIce->m_nRating++;
			}
		}
		else if (nRoll<=2)
		{
			// Must be at least Lv 4
			if (nDifficulty >= 4)
			{
				pIce->m_nRating += 2;
			}
		}

		// Load the ICE data
		pIce->LoadInitData();

		// Place the ice in the node
		if (!ChooseLocation(pIce->m_ptNodeLoc, nDir))
		{
			// This should never happen
			DeckerMessageBox("Error placing initial ice!");
			delete pIce;
			break;
		}

		pIce->m_pCurrentNode = this;

		// Add the ICE to the system's ICE list
		g_pChar->m_pSystem->m_olIceList.AddTail(pIce);
	}

	//------------------
	// Create Tapeworms
	//------------------
	if (m_nType == NT_DS)
	{
		POSITION p;
		CDSFile *pFile;
		int iTarget;

		// Go through the list of files
		p = m_olFileList.GetHeadPosition();
		while (p!=NULL)
		{
			pFile = (CDSFile *)m_olFileList.GetNext(p);

			// Get the chance of a tapeworm based on contents
			switch (pFile->m_nType)
			{
			case FT_USELESS:
				// 5%
				iTarget = 1;
				break;
			case FT_VALUABLE:
				// 20%
				iTarget = 4;
				break;
			case FT_QUEST:
				// 30%
				iTarget = 6;
				break;
			case FT_CLUE:
				// 75%
				iTarget = 15;
				break;
			case FT_PASSCODE:
				// 100%
				iTarget = 20;
				break;
			}

			if (Random(20) < iTarget)
			{
				// Create an ICE to guard this file
				pFile->m_pTapeworm = new CIce;
				pFile->m_pTapeworm->m_nType = ICE_TAPEWORM;

				nRoll = Random(100)+1;
				if (nRoll < ((nDifficulty-10)*5))
				{
					pFile->m_pTapeworm->m_nSubType = TWST_DATABOMB;
				}
				else
				{
					pFile->m_pTapeworm->m_nSubType = TWST_NORMAL;
				}
				pFile->m_pTapeworm->m_nRating = nDifficulty;
				pFile->m_pTapeworm->m_pHomeNode = this;
				pFile->m_pTapeworm->m_nState = STATE_GUARDING;
				pFile->m_pTapeworm->m_pFile = pFile;

				// Now, do a random adjustment to rating. (10% +1, 5% +2)
				nRoll = Random(20);
				if (nRoll==0)
				{
					// Must be at least Lv 2
					if (nDifficulty >= 2)
					{
						pFile->m_pTapeworm->m_nRating++;
					}
				}
				else if (nRoll<=2)
				{
					// Must be at least Lv 4
					if (nDifficulty >= 4)
					{
						pFile->m_pTapeworm->m_nRating += 2;
					}
				}

				// Load the ICE data
				pFile->m_pTapeworm->LoadInitData();

				// Place the ice in the node
				if (!ChooseLocation(pFile->m_pTapeworm->m_ptNodeLoc, DIR_NONE))
				{
					// This should never happen
					DeckerMessageBox("Error placing initial ice!");
					delete pFile->m_pTapeworm;
					continue;
				}
				pFile->m_pTapeworm->m_pCurrentNode = this;

				// Add the ICE to the system's ICE list
				g_pChar->m_pSystem->m_olIceList.AddTail(pFile->m_pTapeworm);
			}
		}
	}
}

void CNode::CreateFileList()
{
	CDSFile *pFile;
	int nNumFiles;
	int nFile;
	int nChance;
	//int nAreas;
	int nRoll; //FSO 12-17-01

	// Generate the number of files (5d4)
	nNumFiles = 5+Random2(5,4);

	//FSO 12-17-01 Change chance of valuable file. This will probably screw up all of the
	//				economic information that I hade done, but in larger systems there were
	//				many DS without any valuable files. Player doesn't want to search every
	//				node in the system for valuable files just to pay rent.
	//// Compute the chances of a valuable file (10% -2% per area above 1)
	//nAreas = ((m_pParentArea->m_pSystem->m_nRating+3)/4);
	//nChance = 12 - (nAreas*2);
	nChance = 10;

	// Add each file
	for (nFile = 0; nFile < nNumFiles; nFile++)
	{
		// Create a file
		pFile = new CDSFile;

		// Generate the type (Chance of valuable computed above)
		//FSO 12-17-01 Do one die roll
		nRoll = Random(100);
		if (nRoll < nChance)
		{
			// Valuable file
			pFile->m_nType = FT_VALUABLE;
		}
		// 1% chance of hint file //FSO 6-20-01
		else if ((g_pChar->m_pCurrentContract->HintAvailable()) && 
				 (nRoll == 99))
		{
			pFile->m_nType = FT_CLUE;
		}
		// 1% chance of a passcode file
		else if (nRoll == 98)
		{
			pFile->m_nType = FT_PASSCODE;
		}
		// 1% chance of a program/source
		else if (nRoll == 97)
		{
			// Roll again to determine type
			nRoll = Random(100);
			if (nRoll < 70)
			{
				pFile->m_nType = FT_PROGRAM;
			}
			else if (nRoll < 90)
			{
				pFile->m_nType = FT_P_SOURCE;
			}
			else
			{
				pFile->m_nType = FT_C_SOURCE;
			}
		}


		// Generate the file contents based on type
		pFile->Generate(this);

		// Add the file to the list
		m_olFileList.AddTail(pFile);
	}
}

void CNode::MarkPaths(int nDirFrom, int iIndex)
{
	int nDir;

	// Mark this node
	m_bProcessed[iIndex] = m_pAdjNode[nDirFrom]->m_bProcessed[iIndex]+1;

	// Mark adjacent nodes
	for (nDir = 0; nDir<4; nDir++)
	{
		// Ignore parent node
		if (nDir==nDirFrom) continue;

		// If no node, ignore
		if (m_pAdjNode[nDir]==NULL) continue;

		// If sibling node value is higher, process it
		if (m_pAdjNode[nDir]->m_bProcessed[iIndex] > (m_bProcessed[iIndex]+1))
		{
			m_pAdjNode[nDir]->MarkPaths(OppDir(nDir), iIndex);
		}
	}
}

void CNode::Save(CArchive &ar)
{
	int nIndex;
	int i;
	WORD wCount;
	POSITION p;
	CDSFile *pFile;

	// Skip parent area
	// Write basic info
	ar << m_szName;
	ar << m_nType;
	ar << m_nSubType;
	ar << m_nCluster;
	ar << m_nNodeNumber;

	// Write adjacent node list
	for (i = 0; i < 4; i++)
	{
		nIndex = FindListIndex(m_pAdjNode[i], m_pParentArea->m_olNodeList);
		ar << nIndex;
	}

	// Write location
	ar << m_ptLocation;

	// Write file list
	wCount = (WORD)m_olFileList.GetCount();
	ar << wCount;

	p = m_olFileList.GetHeadPosition();
	while (p!=NULL)
	{
		pFile = (CDSFile *)m_olFileList.GetNext(p);

		// Write the item
		pFile->Save(ar);
	}

	// Write misc
	ar << m_nActiveSmokeLevel;
	ar << m_nActiveSilenceLevel;
	ar << m_bActivated;
	ar << m_bMapped;
	ar << m_nSpecialImage;
}

void CNode::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	int nIndex;
	int i;
	WORD wCount;
	CDSFile *pFile;

	// Skip parent area
	// Read basic info
	ar >> m_szName;
	ar >> m_nType;
	ar >> m_nSubType;
	ar >> m_nCluster;
	ar >> m_nNodeNumber;

	// Read adjacent node list
	for (i = 0; i < 4; i++)
	{
		ar >> nIndex;

		m_pAdjNode[i] = (CNode *)FindListObject(nIndex, m_pParentArea->m_olNodeList);
	}

	// Read location
	ar >> m_ptLocation;

	// Read file list
	ar >> wCount;

	while (wCount--)
	{
		pFile = new CDSFile;
		pFile->Load(ar, wMajorVersion, wMinorVersion);
		m_olFileList.AddTail(pFile);
	}

	// Read misc
	ar >> m_nActiveSmokeLevel;
	ar >> m_nActiveSilenceLevel;
	ar >> m_bActivated;
	ar >> m_bMapped;
	if ((wMajorVersion > 0) || (wMinorVersion >=4))
	{
		ar >> m_nSpecialImage;
	}
}

