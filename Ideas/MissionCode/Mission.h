// Mission.h: interface for the CMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISSION_H__A2E3D8A8_AC2D_11D5_83F9_00104B751D91__INCLUDED_)
#define AFX_MISSION_H__A2E3D8A8_AC2D_11D5_83F9_00104B751D91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	int		nMissionID;			// ID number of mission - Used to locate mission texts
	int		nWhenAvailable;		// Rep level to become available
	int		nContractType;		// Type of contract (CONT_xxx)
	DWORD	dwFlags;			// Contract flags (CF_xxx)
	int		nTargets;			// Number of targets
	int		nDifficulty;		// Difficulty / System Rating
	int		nRepPoints;			// Number of rep points awarded
	int		nPay;				// Amount of money received
	int		nNextMission;		// ID of next mission
} MISSION;

#endif // !defined(AFX_MISSION_H__A2E3D8A8_AC2D_11D5_83F9_00104B751D91__INCLUDED_)
