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

// System.h: interface for the CSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEM_H__6C1BC781_7CE2_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_SYSTEM_H__6C1BC781_7CE2_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArea;
class CNode;
class CMatrixView;

// Alert Levels
#define ALERT_GREEN		0
#define ALERT_YELLOW	1
#define ALERT_RED		2

class CSystem : public CObject  
{
public:
	CSystem();
	virtual ~CSystem();

	void Generate(int nCorporation, int nSystemSize, DWORD dwFlags = 0);
	void RouteResponseIC(CNode *pNode);
	void SetReactivateTime(BOOL bAlreadyActive);
	void BuildFileList(CObList &olFiles, BOOL bContractOnly);
	void BuildNodeList(CObList &olNodes, int nType, BOOL bContractOnly);
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion);
	void MarkContractNodes(CMatrixView *pView);

	CObList m_olAreaList;
	CNode *m_pSystemCPU;
	CNode *m_pSystemPortalOut;
	CObList m_olIceList;
	CObList m_olDeadIceList;
	CObList m_olNodeList; // Compilation of area's node lists
	int m_nAlert;
	int m_nRating; // Security rating
	int m_nTurnsUntilCrash;
	BOOL m_bExternalAlarmsDeactivated;
	BOOL m_bBackdoor;
	int m_nIceReactivateTime;
	int m_nCorporation; //FSO 7-24-01
	BOOL m_bPasscodeAccessed; //FSO 11-26-01
};

#endif // !defined(AFX_SYSTEM_H__6C1BC781_7CE2_11D4_B835_0080C8C1DB04__INCLUDED_)
