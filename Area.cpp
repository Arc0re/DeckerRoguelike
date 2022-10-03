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

// Area.cpp: implementation of the CArea class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "System.h"
#include "Area.h"
#include "Node.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CPoint g_ptDirCoord[4];


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CArea::CArea(CSystem *pParentSystem)
{
	m_pSystem = pParentSystem;
	m_nAreaNumber = 0;
	m_pAreaSPU = NULL;
	m_pAreaPortalIn = NULL;
	m_pAreaPortalOut = NULL;
	m_pIceIONode = NULL;
	m_pHighSpeedIONode = NULL;
}

CArea::~CArea()
{
	CNode *pNode;
	
	while (!m_olNodeList.IsEmpty())
	{
		pNode = (CNode *)m_olNodeList.RemoveHead();

		delete pNode;
	}
}

void CArea::GenerateArea(int nAreaSize, int nAreaNumber, int &nNodeNumber)
{
	int nNodes;
	CNode *pNode;
	POSITION p;
	int nBasicNodes = 0;

	// Save our area number
	m_nAreaNumber = nAreaNumber;

	//------------------
	// Create the Nodes
	//------------------
	// Create the CPU/SPU
	if (nAreaNumber==0)
	{
		m_pAreaSPU = new CNode(this, NT_CPU, nNodeNumber++);
		m_pSystem->m_pSystemCPU = m_pAreaSPU;
	}
	else
	{
		m_pAreaSPU = new CNode(this, NT_SPU, nNodeNumber++);
	}

	m_olNodeList.AddTail(m_pAreaSPU);

	// FSO 12-17-01 
	// Add security node
	pNode = AddNode(m_pAreaSPU, NT_COP, nNodeNumber);
	pNode->m_nSubType = NST_COP_SECURITY;

	// Add Datastores (1 .. 1+(2*Size))
	nNodes = 1 + Random2(2,nAreaSize+1);

	while (nNodes--)
	{
		pNode = AddNode(m_pAreaSPU, NT_DS, nNodeNumber);

		// Add a coprocessor every fouth node
		nBasicNodes++;
		if ((nBasicNodes&0x03) == 0)
		{
			//FSO 12-17-01 Add COP's to current node
			//AddNode(m_pAreaSPU, NT_COP, nNodeNumber);
			AddNode(pNode, NT_COP, nNodeNumber);
		}
	}
	
	// Add I/O (1 .. 1+2*Size)
	nNodes = 1+Random2(2,nAreaSize+1);

	while (nNodes--)
	{
		pNode = AddNode(m_pAreaSPU, NT_IO, nNodeNumber);

		// Add a coprocessor every fourth node
		nBasicNodes++;
		if ((nBasicNodes&0x03) == 0)
		{
			//FSO 12-17-01 Add COP's to current node
			//AddNode(m_pAreaSPU, NT_COP, nNodeNumber);
			AddNode(pNode, NT_COP, nNodeNumber);
		}
	}

	// Add the ICE port I/O node
	m_pIceIONode = AddNode(m_pAreaSPU, NT_IO, nNodeNumber);
	m_pIceIONode->m_nSubType = NST_IO_ICE_PORT;

	// Add a coprocessor every fouth node
	nBasicNodes++;
	if ((nBasicNodes&0x03) == 0)
	{
		//FSO 12-17-01 Add COP's to current node
		//AddNode(m_pAreaSPU, NT_COP, nNodeNumber);
		AddNode(m_pIceIONode, NT_COP, nNodeNumber);
	}

	// Add the external alarm I/O node if this is the innermost area.
	if (nAreaNumber == 0)
	{
		pNode = AddNode(m_pAreaSPU, NT_IO, nNodeNumber);
		pNode->m_nSubType = NST_IO_ALARM;

		// Add a coprocessor every fouth node
		nBasicNodes++;
		if ((nBasicNodes&0x03) == 0)
		{
			//FSO 12-17-01 Add COP's to current node
			//AddNode(m_pAreaSPU, NT_COP, nNodeNumber);
			AddNode(pNode, NT_COP, nNodeNumber);
		}
	}

	// Add high-speed i/o node, maybe
	if (Random(30)<m_pSystem->m_nRating)
	{
		m_pHighSpeedIONode = AddNode(m_pAreaSPU, NT_IO, nNodeNumber);
		m_pHighSpeedIONode->m_nSubType = NST_IO_MATRIX;

		// Add a coprocessor every fouth node
		nBasicNodes++;
		if ((nBasicNodes&0x03) == 0)
		{
			//FSO 12-17-01 Add COP's to current node
			//AddNode(m_pAreaSPU, NT_COP, nNodeNumber);
			AddNode(m_pHighSpeedIONode, NT_COP, nNodeNumber);
		}
	}

//	// Add Co-processors (Basic Nodes/4)
//	nNodes = ((nBasicNodes+3)/4);
//
//	while (nNodes--)
//	{
//		AddNode(m_pAreaSPU, NT_COP, nNodeNumber);
//	}

	// Add the portal in
	if (m_nAreaNumber!=0)
	{
		m_pAreaPortalIn = AddNode(m_pAreaSPU, NT_PORTAL_IN, nNodeNumber);
	}

	// Add the portal out
	m_pAreaPortalOut = AddNode(m_pAreaSPU, NT_PORTAL_OUT, nNodeNumber);

	//-------------------------------
	// Divide the area into clusters
	//-------------------------------
	int nDir;
	int nClusterNumber = 1;
	int nSubNodes;
	BOOL bHasCOP;
	for (nDir = 0; nDir<4; nDir++)
	{
		if (m_pAreaSPU->m_pAdjNode[nDir]!=NULL)
		{
			nSubNodes = 0;
			bHasCOP = FALSE;
			m_pAreaSPU->m_pAdjNode[nDir]->CreateClusters(nClusterNumber, OppDir(nDir), nSubNodes,bHasCOP);
		}
	}

	//----------------------------
	// Make auxiliary connections
	//----------------------------
	// Go through and make connections to DS's and COP's
	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);

		if ((pNode->m_nType == NT_DS) || (pNode->m_nType == NT_COP))
		{
			int nDir;
			CPoint ptLoc;
			CNode *pOtherNode;

			for (nDir = 0; nDir<4; nDir++)
			{
				// If already a node connected there, ignore
				if (pNode->m_pAdjNode[nDir]!=NULL) continue;

				// See if there is an unceonnected node there
				ptLoc = pNode->m_ptLocation + g_ptDirCoord[nDir];

				pOtherNode = FindNode(ptLoc);

				if ((pOtherNode != NULL) &&								// Node is present
					(pOtherNode->m_nType != NT_PORTAL_IN) &&			// Auxiliary connections not allowed with portals
					(pOtherNode->m_nType != NT_PORTAL_OUT) &&			// Auxiliary connections not allowed with portals
					(Random(2) == 1))									// 50% chance of connection
				{
					// Link the two nodes
					pNode->m_pAdjNode[nDir] = pOtherNode;
					pOtherNode->m_pAdjNode[OppDir(nDir)] = pNode;
				}
			}
		}
	}

	//-------------------
	// Assign node names,
	// Create Files,
	//-------------------
	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);

		// Assign node names,
		pNode->GenerateName();

		// Create Files,
		if (pNode->m_nType == NT_DS)
		{
			pNode->CreateFileList();
		}
	}

	//-----------------------------
	// Adjust the coordinate space
	//-----------------------------
	// Now, adjust the coordinates and get the min/max
	int iMinX = 0, iMaxX = 0, iMinY = 0, iMaxY = 0;

	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);

		if (pNode->m_ptLocation.x < iMinX) 
		{
			iMinX = pNode->m_ptLocation.x;
		}
		else if (pNode->m_ptLocation.x > iMaxX) 
		{
			iMaxX = pNode->m_ptLocation.x;
		}
		if (pNode->m_ptLocation.y < iMinY) 
		{
			iMinY = pNode->m_ptLocation.y;
		}
		else if (pNode->m_ptLocation.y > iMaxY) 
		{
			iMaxY = pNode->m_ptLocation.y;
		}
	}

	// Adjust all to positive range
	int iAdjustX = (-iMinX);
	int iAdjustY = (-iMinY);

	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);

		pNode->m_ptLocation.x += iAdjustX;
		pNode->m_ptLocation.y += iAdjustY;
	}
	
	m_nSizeX = (iMaxX + iAdjustX)+1;
	m_nSizeY = (iMaxY + iAdjustY)+1;
}


CNode *CArea::AddNode(CNode *pParentNode, int nNodeType, int &nNodeNumber)
{
	CNode *pNewNode;
	int nDir;
	int iCount;
	BOOL bDone;
	int nRot;

	// Allocate a new node
	pNewNode = new CNode(this, nNodeType, nNodeNumber++);

	// Add it to the list
	m_olNodeList.AddTail(pNewNode);

	// Find a place to put it
	nDir = Random(4);
	nRot = ChooseRot();

	bDone = FALSE;
	for (iCount = 0; iCount<4; iCount++)
	{
		if (AddNodeDir(pParentNode, pNewNode, nDir, nNodeNumber))
		{
			bDone = TRUE;
			break;
		}
		nDir += nRot;
		nDir &= 3;
	}

	if (!bDone)
	{
		//NOTE: This should not be possible
		DeckerMessageBox("Error - Could not add node");
	}

	return pNewNode;
}

BOOL CArea::AddNodeDir(CNode *pParentNode, CNode *pNewNode, int nDir, int &nNodeNumber)
{
	int nOppositeDir = OppDir(nDir); // Opposite direction
	int nNewDir;
	int nCount;
	CPoint ptNewLoc;
	int nRot;

	// See if this direction is empty
	if (pParentNode->m_pAdjNode[nDir]==NULL)
	{
		// See if there is a node in this location
		ptNewLoc = pParentNode->m_ptLocation + g_ptDirCoord[nDir];
		if (FindNode(ptNewLoc)!=NULL)
		{
			return FALSE;
		}

		// Put it here
		pParentNode->m_pAdjNode[nDir] = pNewNode;
		pNewNode->m_pAdjNode[nOppositeDir] = pParentNode;
		pNewNode->m_ptLocation = ptNewLoc;

		return TRUE;
	}

	// Opposite direction is occupied. See if it is a junction.
	if (pParentNode->m_pAdjNode[nDir]->m_nType == NT_JUNC)
	{
		// Try to add it to the junction
		nNewDir = Random(4);
		nRot = ChooseRot();

		for (nCount = 0; nCount<4; nCount++)
		{
			if (nNewDir != nOppositeDir)
			{
				if (AddNodeDir(pParentNode->m_pAdjNode[nDir], pNewNode, nNewDir, nNodeNumber)==TRUE)
				{
					return TRUE;
				}
			}
			nNewDir += nRot;
			nNewDir &= 3;
		}
		return FALSE;
	}
	else
	{
		// We will have to try to create a junction at this location
		// See if there are at least two open locations
		BOOL bOpen[4];
		CPoint ptLoc[4];
		CNode *pJunction;
		CNode *pOldNode;

		// Get the old node
		pOldNode = pParentNode->m_pAdjNode[nDir];
		ptNewLoc = pOldNode->m_ptLocation;

		nCount = 0;
		for (nNewDir = 0; nNewDir<4; nNewDir++)
		{
			// If opposite direction, closed
			if (nNewDir == nOppositeDir)
			{
				bOpen[nNewDir] = FALSE;
				continue;
			}
			
			// Get location
			ptLoc[nNewDir] = ptNewLoc + g_ptDirCoord[nNewDir];

			// If node here, closed
			if (FindNode(ptLoc[nNewDir])!=NULL)
			{
				bOpen[nNewDir] = FALSE;
				continue;
			}

			// This is open
			bOpen[nNewDir] = TRUE;
			nCount++;
		}

		if (nCount<2)
		{
			// No room to add
			return FALSE;
		}

		// Create the junction and link it to the parent node in the place of the old node
		pJunction = new CNode(this, NT_JUNC, nNodeNumber++);
		m_olNodeList.AddTail(pJunction);
		pJunction->m_ptLocation = ptNewLoc;

		for (nCount = 0; nCount<4; nCount++)
		{
			pJunction->m_pAdjNode[nCount] = pOldNode->m_pAdjNode[nCount];
			if (pOldNode->m_pAdjNode[nCount] != NULL)
			{
				pOldNode->m_pAdjNode[nCount]->m_pAdjNode[OppDir(nCount)] = pJunction;
				pOldNode->m_pAdjNode[nCount] = NULL;
			}
		}		

		// Link the old node to a random direction
		nDir = Random(4);
		nRot = ChooseRot();
		while (!bOpen[nDir])
		{
			nDir += nRot;
			nDir &= 3;
		}
		nOppositeDir = OppDir(nDir);

		pOldNode->m_ptLocation = ptLoc[nDir];
		pJunction->m_pAdjNode[nDir] = pOldNode;
		pOldNode->m_pAdjNode[nOppositeDir] = pJunction;

		// Set this location to not available for next one
		bOpen[nDir] = FALSE;
		
		// Link the new node
		nDir = Random(4);
		nRot = ChooseRot();
		while (!bOpen[nDir])
		{
			nDir += nRot;
			nDir &= 3;
		}
		nOppositeDir = OppDir(nDir);

		pNewNode->m_ptLocation = ptLoc[nDir];
		pJunction->m_pAdjNode[nDir] = pNewNode;
		pNewNode->m_pAdjNode[nOppositeDir] = pJunction;
	}

	return TRUE;
}

CNode *CArea::FindNode(CPoint &ptLoc)
{
	POSITION p;
	CNode *pNode;

	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);

		if (pNode->m_ptLocation == ptLoc)
		{
			return pNode;
		}
	}

	return NULL;
}

void CArea::ResetProcessedFlags()
{
	POSITION p;
	CNode *pNode;

	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);
		pNode->m_dwProcessed = 0;
	}
}

void CArea::CalculatePathsToNode(CNode *pTargetNode, int iIndex)
{
	POSITION p;
	CNode *pNode;
	int nDir;

	// Reset the processed flags
	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);
		pNode->m_bProcessed[iIndex] = 255;
	}

	// Mark the target node as processed
	pTargetNode->m_bProcessed[iIndex] = 0;

	for (nDir = 0; nDir<4; nDir++)
	{
		// If no node, ignore
		if (pTargetNode->m_pAdjNode[nDir]!=NULL)
		{
			pTargetNode->m_pAdjNode[nDir]->MarkPaths(OppDir(nDir), iIndex);
		}
	}
}

void CArea::Save(CArchive &ar)
{
	WORD wCount;
	POSITION p;
	CNode *pNode;
	int nIndex;

	// Write the node list
	wCount = (WORD)m_olNodeList.GetCount();
	ar << wCount;

	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);

		// Write the item
		pNode->Save(ar);
	}

	// Write the area number
	ar << m_nAreaNumber;

	// Write the are node pointers
	nIndex = FindListIndex(m_pAreaSPU, m_olNodeList);
	ar << nIndex;
	nIndex = FindListIndex(m_pAreaPortalIn, m_olNodeList);
	ar << nIndex;
	nIndex = FindListIndex(m_pAreaPortalOut, m_olNodeList);
	ar << nIndex;
	nIndex = FindListIndex(m_pIceIONode, m_olNodeList);
	ar << nIndex;
	nIndex = FindListIndex(m_pHighSpeedIONode, m_olNodeList);
	ar << nIndex;

	// Write the size
	ar << m_nSizeX;
	ar << m_nSizeY;
}

void CArea::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	WORD wCount;
	POSITION p;
	CNode *pNode;
	int nIndex;

	// Read the node list
	ar >> wCount;

	// Create the nodes first so we can fill in the adjacent node list
	while (wCount--)
	{
		pNode = new CNode(this);
		m_olNodeList.AddTail(pNode);
	}

	// Now, actually read the nodes in
	p = m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		pNode = (CNode *)m_olNodeList.GetNext(p);

		// Read the item
		pNode->Load(ar, wMajorVersion, wMinorVersion);
	}

	// Read the area number
	ar >> m_nAreaNumber;

	// Read the are node pointers
	ar >> nIndex;
	m_pAreaSPU = (CNode *)FindListObject(nIndex, m_olNodeList);
	ar >> nIndex;
	m_pAreaPortalIn = (CNode *)FindListObject(nIndex, m_olNodeList);
	ar >> nIndex;
	m_pAreaPortalOut = (CNode *)FindListObject(nIndex, m_olNodeList);
	ar >> nIndex;
	m_pIceIONode = (CNode *)FindListObject(nIndex, m_olNodeList);
	ar >> nIndex;
	m_pHighSpeedIONode = (CNode *)FindListObject(nIndex, m_olNodeList);

	// Read the size
	ar >> m_nSizeX;
	ar >> m_nSizeY;
}




