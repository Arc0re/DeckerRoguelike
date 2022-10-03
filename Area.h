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

// Area.h: interface for the CArea class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AREA_H__6C1BC782_7CE2_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_AREA_H__6C1BC782_7CE2_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSystem;
class CNode;

class CArea : public CObject  
{
public:
	CArea(CSystem *pParentSystem);
	virtual ~CArea();

	void GenerateArea(int nAreaSize, int nAreaNumber, int &nNodeNumber);
	CNode *AddNode(CNode *pParentNode, int nNodeType, int &nNodeNumber);
	BOOL AddNodeDir(CNode *pParentNode, CNode *pNewNode, int nDir, int &nNodeNumber);
	CNode *FindNode(CPoint &ptLoc);
	void ResetProcessedFlags();
	void CalculatePathsToNode(CNode *pTargetNode, int iIndex = 0);
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion);

	CSystem *m_pSystem;
	CObList m_olNodeList;
	int m_nAreaNumber;
	CNode *m_pAreaSPU;
	CNode *m_pAreaPortalIn;
	CNode *m_pAreaPortalOut;
	CNode *m_pIceIONode;
	CNode *m_pHighSpeedIONode;
	int m_nSizeX;
	int m_nSizeY;
};

#endif // !defined(AFX_AREA_H__6C1BC782_7CE2_11D4_B835_0080C8C1DB04__INCLUDED_)
