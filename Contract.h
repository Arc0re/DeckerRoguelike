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

// Contract.h: interface for the CContract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTRACT_H__08BBE2A1_82A6_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_CONTRACT_H__08BBE2A1_82A6_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Number of corporation defined in the database
#define NUM_CORP	100

// Contract Types
#define CONT_STEAL			0
#define CONT_STEAL_ERASE	1
#define CONT_ERASE			2
#define CONT_EDIT			3
#define CONT_DEACTIVATE_IO	4
#define CONT_ACTIVATE_IO	5
#define CONT_SABOTAGE_IO	6
#define CONT_CRASH_SYS		7
#define CONT_BACKDOOR		8
#define CONT_RUN_PROGRAM	9

// Contract Targets
 // Datastore targets
#define CT_RESEARCH_DATA	0
#define CT_CHEM_FORMULA		1
#define CT_PERSONNEL_FILES	2
#define CT_FINANCIAL_DATA	3
#define CT_GRADE_REPORTS	4
#define CT_SECURITY_FILES	5
#define CT_SECURITY_CAM_REC	6
#define CT_BLUEPRINTS		7
#define CT_EMPLOYEE_EVAL	8
#define CT_PRODUCT_INFO		9
#define CT_MEDICAL_RECORDS	10
#define CT_ILLEGAL_ACTIVITY	11
#define CT_TEST_RESULTS		12
#define CT_INVENTORY_DATA	13
//Special note: If you add another file type, add it to the list in DSFile.cpp
 // Deactive IO targets
#define CT_DOORLOCKS		14
#define CT_SECURITY_CAMERAS	15
#define CT_ALARM_SYSTEMS	16
 // Activate IO targets
#define CT_FIRE_ALARMS		17
#define CT_SECURITY_ALARMS	18
#define CT_RADIATION_ALARMS	19
#define CT_BIOHAZARD_ALARMS	20
#define	CT_CHEM_ALARMS		21
 // Sabotage targets
#define CT_MANUF_CONTROLS	22
#define CT_CHEM_PRODUCTION	23
#define CT_VAULT_CONTROLS	24
 // Crash system, backdoor target
#define CT_CPU				25
 // Run Program targets
#define CT_NODE_IO			26
#define CT_NODE_DS			27
#define CT_NODE_CPU			28

// Contract flags
#define	CF_NO_ALARMS		0x00000001	// Can't set off an alarm
#define CF_TIMED			0x00000002	// Limited time to complete

class CContract : public CObject  
{
public:
	// Construction
	CContract();
	virtual ~CContract();

	// Member Fuctions
	void GetDescriptiveText(CString &szText);
	void Generate();
	void GetTypeText(CString &szTypeText);
	const char *GetTargetObjectText();
	void Save(CArchive &ar);
	void Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion);
	BOOL HintAvailable();

	// Data Members
	int	m_nCorporation;		// Target corporation
	CString m_szSystemName;	// Target corporation name
	int m_nSystemSize;		// Size of system
	DWORD m_dwTargetFlags;	// Flags (for file types)
	int m_nType;			// Type of contract
	int m_nTargetObject;	// Target object
	int m_nDifficulty;		// Difficulty level
	int m_nPay;				// Completion pay
	DWORD m_dwFlags;		// Miscellaneous flags
	DWORD m_dwTime;			// Time variable for timed missions (minutes)
	int m_nDaysLeft;		// Days left to complete
	int m_nNumTargets;		// Number of target objects

	int m_nCompletionTime;	// *Not currently used*
};

#endif // !defined(AFX_CONTRACT_H__08BBE2A1_82A6_11D4_B835_0080C8C1DB04__INCLUDED_)
