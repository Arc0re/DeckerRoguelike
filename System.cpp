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

// System.cpp: implementation of the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "System.h"
#include "Area.h"
#include "Node.h"
#include "Ice.h"
#include "Character.h"
#include "Contract.h"
#include "DSFile.h"
#include "Global.h"
#include "MatrixView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

// Coordinate directions
CPoint g_ptDirCoord[4];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystem::CSystem()
{
	m_pSystemCPU = NULL;
	m_pSystemPortalOut = NULL;
	m_nAlert = ALERT_GREEN;
	m_nTurnsUntilCrash = 0;
	m_bExternalAlarmsDeactivated = FALSE;
	m_bBackdoor = FALSE;
	m_nIceReactivateTime = 0;
	m_bPasscodeAccessed = FALSE;

	g_ptDirCoord[0].x = 0;	// North (Coordinate plane is from upper left)
	g_ptDirCoord[0].y = -1;
	g_ptDirCoord[1].x = 1;	// East
	g_ptDirCoord[1].y = 0;
	g_ptDirCoord[2].x = 0;	// South
	g_ptDirCoord[2].y =  1;
	g_ptDirCoord[3].x = -1;	// West
	g_ptDirCoord[3].y = 0;
}

CSystem::~CSystem()
{
	CArea *pArea;
	CIce *pIce;
	
	while (!m_olAreaList.IsEmpty())
	{
		pArea = (CArea *)m_olAreaList.RemoveHead();

		delete pArea;
	}

	while (!m_olIceList.IsEmpty())
	{
		pIce = (CIce *)m_olIceList.RemoveHead();

		delete pIce;
	}

	while (!m_olDeadIceList.IsEmpty())
	{
		pIce = (CIce *)m_olDeadIceList.RemoveHead();

		delete pIce;
	}
	// Nodes will already have been deleted by areas
	m_olNodeList.RemoveAll();
}

void CSystem::Generate(int nCorporation, int nSystemSize, DWORD dwFlags)
{
	int nAreaCount;
	int nAreaSize;
	int nAreaNumber;
	CArea *pArea;
	int nNodeNumber = 0;
	int nTarget;

	// Save the corperation
	m_nCorporation = nCorporation;

	// Save the difficulty rating
	m_nRating = nSystemSize;

	// Check for alert
	if (g_pChar->m_bSystemAlerts[g_pChar->m_pSystem->m_nCorporation])
	{
		// Set yellow alert
		m_nAlert = ALERT_YELLOW;
	}

	//-------------------------
	// Generate the actual map
	//-------------------------
	nAreaCount = ((nSystemSize+3)/4);
	// Old way
	//if (nSystemSize<5)
	//	nAreaSize = nSystemSize;
	//else
	//	nAreaSize = 4;
	// New way
	nAreaSize = ((nSystemSize-1)%4) + nAreaCount;

	nAreaNumber = 0;
	while (nAreaCount--)
	{
		// Create an area
		pArea = new CArea(this);

		pArea->GenerateArea(nAreaSize++,nAreaNumber++, nNodeNumber);

		m_olAreaList.AddTail(pArea);

		// Add this area's nodes to our list
		m_olNodeList.AddTail(&pArea->m_olNodeList);
	}

	// Save the system portal
	m_pSystemPortalOut = pArea->m_pAreaPortalOut;

	//--------------------
	// Add the Quest data
	//--------------------
	CDSFile *pFile;
	CObList olNodeList;
	CNode *pNode;
	POSITION p;
	int i;

	// Process according to quest type
	switch (g_pChar->m_pCurrentContract->m_nType)
	{
	// Data Store quests
	case CONT_STEAL:
	case CONT_STEAL_ERASE:
	case CONT_ERASE:
	case CONT_EDIT:
		for (nTarget = 0; nTarget < g_pChar->m_pCurrentContract->m_nNumTargets; nTarget++)
		{
			// Select a random area
			i = Random(m_olAreaList.GetCount());
			p = m_olAreaList.FindIndex(i);
			pArea = (CArea *)m_olAreaList.GetAt(p);

			// Create a list of DS nodes
			p = pArea->m_olNodeList.GetHeadPosition();
			while (p!=NULL)
			{
				pNode = (CNode *)pArea->m_olNodeList.GetNext(p);
				if (pNode->m_nType == NT_DS)
				{
					// Add the node to the list
					olNodeList.AddTail(pNode);
				}
			}

			// Choose a random node
			i = Random(olNodeList.GetCount());
			p = olNodeList.FindIndex(i);
			pNode = (CNode *)olNodeList.GetAt(p);
			olNodeList.RemoveAll();

			// Create a quest file
			pFile = new CDSFile;
			pFile->m_nType = FT_QUEST;
			pFile->Generate(pNode);

			// Add it to a random position in the file list for the node
			i = Random(pNode->m_olFileList.GetCount()+1);
			p = pNode->m_olFileList.FindIndex(i);
			if (p==NULL)
				pNode->m_olFileList.AddTail(pFile);
			else
				pNode->m_olFileList.InsertBefore(p, pFile);

			// Set the node subtype
			pNode->m_nSubType = NST_DS_QUEST_NODE; //FSO 6-20-01
		}
		break;

	// IO Quests
	case CONT_DEACTIVATE_IO:
	case CONT_ACTIVATE_IO:
	case CONT_SABOTAGE_IO:
		for (nTarget = 0; nTarget < g_pChar->m_pCurrentContract->m_nNumTargets; nTarget++)
		{
			// Select a random area
			i = Random(m_olAreaList.GetCount());
			p = m_olAreaList.FindIndex(i);
			pArea = (CArea *)m_olAreaList.GetAt(p);

			// Create a list of IO nodes
			p = pArea->m_olNodeList.GetHeadPosition();
			while (p!=NULL)
			{
				pNode = (CNode *)pArea->m_olNodeList.GetNext(p);
				if ((pNode->m_nType == NT_IO) &&
					(pNode->m_nSubType == NST_IO_USELESS))
				{
					// Add the node to the list
					olNodeList.AddTail(pNode);
				}
			}
			// If no nodes left, we have a problem
			if (olNodeList.IsEmpty())
			{
				// Create one
				int nNodeNumber = pArea->m_olNodeList.GetCount();
				pNode = pArea->AddNode(pArea->m_pAreaSPU, NT_IO, nNodeNumber);
				//FSO 5-28-02 Bug fix
				m_olNodeList.AddTail(pNode);
				// FSO 5-31-04
				pNode->GenerateName();
			}
			else
			{
				// Choose a random node
				i = Random(olNodeList.GetCount());
				p = olNodeList.FindIndex(i);
				pNode = (CNode *)olNodeList.GetAt(p);
				olNodeList.RemoveAll();
			}

			// Set the quest flag for this node
			pNode->m_nSubType = NST_IO_QUEST_NODE;
		}
		break;

	// CPU Quests
	case CONT_CRASH_SYS:
	case CONT_BACKDOOR:
		// The CPU is the node
		// No need to mark anything
		break;

	case CONT_RUN_PROGRAM:
		if (g_pChar->m_pCurrentContract->m_nTargetObject != CT_NODE_CPU)
		{
			int nNodeType;
			if (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_IO)
				nNodeType = NT_IO;
			else
				nNodeType = NT_DS;

			for (nTarget = 0; nTarget < g_pChar->m_pCurrentContract->m_nNumTargets; nTarget++)
			{
				// Select a random area
				i = Random(m_olAreaList.GetCount());
				p = m_olAreaList.FindIndex(i);
				pArea = (CArea *)m_olAreaList.GetAt(p);

				// Create a list of IO nodes
				p = pArea->m_olNodeList.GetHeadPosition();
				while (p!=NULL)
				{
					pNode = (CNode *)pArea->m_olNodeList.GetNext(p);
					if ((pNode->m_nType == nNodeType) &&
						(pNode->m_nSubType == 0))
					{
						// Add the node to the list
						olNodeList.AddTail(pNode);
					}
				}
				// If no nodes left, we have a problem
				if (olNodeList.IsEmpty())
				{
					// Create one
					int nNodeNumber = pArea->m_olNodeList.GetCount();
					pNode = pArea->AddNode(pArea->m_pAreaSPU, nNodeType, nNodeNumber);
					//FSO 5-28-02 Bug fix
					m_olNodeList.AddTail(pNode);
					// FSO 5-31-04
					pNode->GenerateName();
				}
				else
				{
					// Choose a random node
					i = Random(olNodeList.GetCount());
					p = olNodeList.FindIndex(i);
					pNode = (CNode *)olNodeList.GetAt(p);
					olNodeList.RemoveAll();
				}

				// Set the quest flag for this node
				pNode->m_nSubType = 1; //NST_IO_QUEST_NODE or NST_DS_QUEST_NODE
			}
		}
		break;
	}

	//--------------
	// Generate ICE
	//--------------
	p = m_olNodeList.GetHeadPosition();
	while (p != NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);

		// Add ICE
		pNode->GenerateICE();
	}

}

void CSystem::RouteResponseIC(CNode *pNode)
{
	POSITION p;
	CIce *pIce;

	p = m_olIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)m_olIceList.GetNext(p);

		// Is this response IC?
		if (pIce->m_bResponse)
		{
			// If this ice is hostile, it has already found the player
			if (pIce->m_nState & STATE_MASK_HOSTILE) continue;

			// Is it in the same area
			if (pIce->m_pCurrentNode->m_pParentArea == pNode->m_pParentArea)
			{
				// Is it already in this node?
				if (pIce->m_pCurrentNode != pNode)
				{
					// Send it to the target node
					pIce->m_nState = STATE_MOVING;
					pIce->m_pTargetNode = pNode;
				}
				else
				{
					// Ice is in the same node. If player is here, attack
					if (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode)
					{
						pIce->m_nState = STATE_ATTACKING;
					}
					else
					{
						// Not here, so set to wander
						pIce->m_nState = STATE_SEARCHING;
					}
				}
			}
			else
			{
				// Activate, but set to wander
				pIce->m_nState = STATE_SEARCHING;
			}
		}
	}
}

void CSystem::SetReactivateTime(BOOL bAlreadyActive)
{
	// Set the time that it takes to actually rebuild and activate the ice
	m_nIceReactivateTime = (10 - (m_nRating>>2)); // 10..5 turns

	// If we are not currently activating ICE, the system is assumed
	// to be scanning for crashed ice only occasionally, so add a random
	// amount to the total.
	if (!bAlreadyActive)
	{
		int nScanTime;

		// First, compute the dead ice scan time according to alert status.
		if (m_nAlert == ALERT_RED)
		{
			// For red alert, check for dead ice often
			nScanTime = (21 - m_nRating); // 20..1 turns
		}
		else
		{
			// For green/yellow alert, check for dead ice occasionally
			nScanTime = (31 - m_nRating); // 30..11 turns
		}

		// Now, assume that a random amount of the scan time is already gone.
		nScanTime -= Random(nScanTime);

		// Add the scan time to the reactivation time
		m_nIceReactivateTime += nScanTime;
	}
}

void CSystem::BuildFileList(CObList &olFiles, BOOL bContractOnly)
{
	POSITION posArea;
	CArea *pArea;
	POSITION posNode;
	CNode *pNode;
	POSITION posFile;
	CDSFile *pFile;

	posArea = m_olAreaList.GetHeadPosition();
	while (posArea!=NULL)
	{
		pArea = (CArea *)m_olAreaList.GetNext(posArea);

		posNode = pArea->m_olNodeList.GetHeadPosition();
		while (posNode!=NULL)
		{
			pNode = (CNode *)pArea->m_olNodeList.GetNext(posNode);

			if (bContractOnly)
			{
				posFile = pNode->m_olFileList.GetHeadPosition();
				while (posFile != NULL)
				{
					pFile = (CDSFile *)pNode->m_olFileList.GetNext(posFile);
					if (pFile->m_nType == FT_QUEST)
					{
						olFiles.AddTail(pFile);
					}
				}
			}
			else
			{
				olFiles.AddTail(&pNode->m_olFileList);
			}
		}
	}
}

void CSystem::BuildNodeList(CObList &olNodes, int nType, BOOL bContractOnly)
{
	POSITION posArea;
	CArea *pArea;
	POSITION posNode;
	CNode *pNode;

	posArea = m_olAreaList.GetHeadPosition();
	while (posArea!=NULL)
	{
		pArea = (CArea *)m_olAreaList.GetNext(posArea);

		posNode = pArea->m_olNodeList.GetHeadPosition();
		while (posNode!=NULL)
		{
			pNode = (CNode *)pArea->m_olNodeList.GetNext(posNode);

			if (pNode->m_nType == nType)
			{
				if (bContractOnly)
				{
					if ((nType == NT_IO) && (pNode->m_nSubType == NST_IO_QUEST_NODE))
					{
						olNodes.AddTail(pNode);
					}
					else if ((nType == NT_DS) && (pNode->m_nSubType == NST_DS_QUEST_NODE))
					{
						olNodes.AddTail(pNode);
					}
					// Add other nodes as needed
				}
				else
				{
					olNodes.AddTail(pNode);
				}
			}
		}
	}
}

void CSystem::Save(CArchive &ar)
{
	WORD wCount;
	int nIndex;
	CArea *pArea;
	CIce *pIce;
	POSITION p;

	// Write the corperation
	ar << m_nCorporation;

	// Write the area list
	wCount = (WORD)m_olAreaList.GetCount();
	ar << wCount;

	p = m_olAreaList.GetHeadPosition();
	while (p!=NULL)
	{
		pArea = (CArea *)m_olAreaList.GetNext(p);

		// Write the item
		pArea->Save(ar);
	}

	// Skip the CPU. It is always the first node

	// Write the system portal (it will be in the last area
	nIndex = FindListIndex(m_pSystemPortalOut, pArea->m_olNodeList);
	ar << nIndex;

	// Write the ice list
	wCount = (WORD)m_olIceList.GetCount();
	ar << wCount;

	p = m_olIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)m_olIceList.GetNext(p);

		// Write the item
		pIce->Save(ar);
	}

	// Write the dead ice list
	wCount = (WORD)m_olDeadIceList.GetCount();
	ar << wCount;

	p = m_olDeadIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)m_olDeadIceList.GetNext(p);

		// Write the item
		pIce->Save(ar);
	}

	// Write the basic properties
	ar << m_nAlert;
	ar << m_nRating;
	ar << m_nTurnsUntilCrash;
	ar << m_bExternalAlarmsDeactivated;
	ar << m_bBackdoor;
	ar << m_nIceReactivateTime;
	ar << m_bPasscodeAccessed;
}

void CSystem::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	WORD wCount;
	int nIndex;
	CArea *pArea;
	CIce *pIce;

	// Read the corperation
	if ((wMajorVersion == 0) && (wMinorVersion < 6))
	{
		m_nCorporation = g_pChar->m_pCurrentContract->m_nCorporation;
	}
	else
	{
		ar >> m_nCorporation;
	}

	// Read the area list
	ar >> wCount;

	while (wCount--)
	{
		pArea = new CArea(this);
		pArea->Load(ar, wMajorVersion, wMinorVersion);
		m_olAreaList.AddTail(pArea);

		// Add this area's nodes to our list
		m_olNodeList.AddTail(&pArea->m_olNodeList);
	}

	// Get the CPU. It is always the first node
	pArea = (CArea *)m_olAreaList.GetHead();
	m_pSystemCPU = (CNode *)pArea->m_olNodeList.GetHead();

	// Read the system portal (it will be in the last area)
	pArea = (CArea *)m_olAreaList.GetTail();
	ar >> nIndex;
	m_pSystemPortalOut = (CNode *)FindListObject(nIndex, pArea->m_olNodeList);

	// Read the ice list
	ar >> wCount;

	while (wCount--)
	{
		pIce = new CIce;
		pIce->Load(ar, wMajorVersion, wMinorVersion, this);
		m_olIceList.AddTail(pIce);
	}

	// Read the dead ice list
	ar >> wCount;

	while (wCount--)
	{
		pIce = new CIce;
		pIce->Load(ar, wMajorVersion, wMinorVersion, this);
		m_olDeadIceList.AddTail(pIce);
	}

	// Read the basic properties
	ar >> m_nAlert;
	ar >> m_nRating;
	ar >> m_nTurnsUntilCrash;
	ar >> m_bExternalAlarmsDeactivated;
	ar >> m_bBackdoor;
	ar >> m_nIceReactivateTime;
	if ((wMajorVersion != 0) || (wMinorVersion >= 10))
	{
		ar >> m_bPasscodeAccessed;
	}
}

void CSystem::MarkContractNodes(CMatrixView *pView)
{
	CNode *pNode;
	POSITION p;
	CString szStr;

	if ((g_pChar->m_pCurrentContract->m_nType == CONT_STEAL) ||
		(g_pChar->m_pCurrentContract->m_nType == CONT_STEAL_ERASE) ||
		(g_pChar->m_pCurrentContract->m_nType == CONT_ERASE) ||
		(g_pChar->m_pCurrentContract->m_nType == CONT_EDIT) ||
		((g_pChar->m_pCurrentContract->m_nType == CONT_RUN_PROGRAM) && (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_DS)))
	{
		p = m_olNodeList.GetHeadPosition();
		while (p != NULL)
		{
			pNode = (CNode *)m_olNodeList.GetNext(p);

			if ((pNode->m_nType == NT_DS) && (pNode->m_nSubType == NST_DS_QUEST_NODE))
			{
				// Mark this node
				pNode->m_bMapped = TRUE;
				pNode->m_nSpecialImage = NSI_DS_QUEST;

				szStr.Format("Contract Node: Area %d Node %s", pNode->m_pParentArea->m_nAreaNumber, pNode->m_szName);
				pView->m_MessageView.AddMessage(szStr, BLUE);
			}
		}
	}
	else if	((g_pChar->m_pCurrentContract->m_nType == CONT_DEACTIVATE_IO) ||
			 (g_pChar->m_pCurrentContract->m_nType == CONT_ACTIVATE_IO) ||
			 (g_pChar->m_pCurrentContract->m_nType == CONT_SABOTAGE_IO) ||
			 ((g_pChar->m_pCurrentContract->m_nType == CONT_RUN_PROGRAM) && (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_IO)))
	{
		p = m_olNodeList.GetHeadPosition();
		while (p != NULL)
		{
			pNode = (CNode *)m_olNodeList.GetNext(p);

			if ((pNode->m_nType == NT_IO) && (pNode->m_nSubType == NST_IO_QUEST_NODE))
			{
				// Mark this node
				pNode->m_bMapped = TRUE;
				pNode->m_nSpecialImage = NSI_IO_QUEST;

				szStr.Format("Contract Node: Area %d Node %s", pNode->m_pParentArea->m_nAreaNumber, pNode->m_szName);
				pView->m_MessageView.AddMessage(szStr, BLUE);
			}
		}
	}
}
