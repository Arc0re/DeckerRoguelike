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

// Contract.cpp: implementation of the CContract class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "Contract.h"
#include "Global.h"
#include "Character.h"
#include "Node.h"
#include "System.h"
#include "DSFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

//---------------------------
// Contract type information
//---------------------------
// Contract type strings
const char *g_szContractTypeText[] = 
{
	"Steal Files",
	"Steal & Erase Files",
	"Erase Files",
	"Alter Files",
	"Deactivate I/O",
	"Activate I/O",
	"Sabotage",
	"Crash System",
	"Create Backdoor",
	"Run Program",
};

// Target Set Limits
#define CT_MIN_DS		CT_RESEARCH_DATA
#define CT_MAX_DS		CT_INVENTORY_DATA
//#define CT_MIN_IO		CT_DOORLOCKS
//#define CT_MAX_IO		CT_CHEM_PRODUCTION
#define CT_MIN_DEACT	CT_DOORLOCKS
#define CT_MAX_DEACT	CT_ALARM_SYSTEMS
#define CT_MIN_ACT		CT_FIRE_ALARMS
#define CT_MAX_ACT		CT_CHEM_ALARMS
#define CT_MIN_SAB		CT_DOORLOCKS			//FSO Changed this so that we can sabotage anything in IO node
#define CT_MAX_SAB		CT_VAULT_CONTROLS
#define CT_MIN_RUNPROG	CT_NODE_IO
#define CT_MAX_RUNPROG	CT_NODE_CPU

// Contract Target Object Strings
const char *g_szTargetObjectText[] = 
{
	"valuable research data",
	"chemical formulae",
	"personnel files",
	"financial data",
	"grade reports",
	"security files",
	"security camera recordings",
	"blueprints",
	"employee evaluation records",
	"product information",
	"medical records",
	"records of illegal activity",
	"test results",
	"inventory records",
	"electronic doorlocks",
	"security cameras",
	"alarm systems",
	"fire alarms",
	"security alarms",
	"radiation leak alarms",
	"biohazard alarms",
	"hazardous chemical alarms",
	"manufacturing controls",
	"chemical production",
	"vault controls",
	"CPU",
	"an I/O node",
	"a datastore",
	"the CPU",
};

// Flags for the above
 // Datastore targets
#define CTF_RESEARCH_DATA		0x00000001
#define CTF_CHEM_FORMULA		0x00000002
#define CTF_PERSONNEL_FILES		0x00000004	// Common
#define CTF_FINANCIAL_DATA		0x00000008	// Common
#define CTF_GRADE_REPORTS		0x00000010
#define CTF_SECURITY_FILES		0x00000020	// Common
#define CTF_SECURITY_CAM_REC	0x00000040	// Common
#define CTF_BLUEPRINTS			0x00000080
#define CTF_EMPLOYEE_EVAL		0x00000100	// Common
#define CTF_PRODUCT_INFO		0x00000200
#define CTF_MEDICAL_RECORDS		0x00000400
#define CTF_ILLEGAL_ACTIVITY	0x00000800	// Common
#define CTF_TEST_RESULTS		0x00001000
#define CTF_INVENTORY_DATA		0x00002000
 // Deactive IO targets
#define CTF_DOORLOCKS			0x00004000	// Common
#define CTF_SECURITY_CAMERAS	0x00008000	// Common
#define CTF_ALARM_SYSTEMS		0x00010000	// Common
 // Activate IO targets
#define CTF_FIRE_ALARMS			0x00020000	// Common
#define CTF_SECURITY_ALARMS		0x00040000	// Common
#define CTF_RADIATION_ALARMS	0x00080000
#define CTF_BIOHAZARD_ALARMS	0x00100000
#define	CTF_CHEM_ALARMS			0x00200000
 // Sabotage (only) targets
#define CTF_MANUF_CONTROLS		0x00400000
#define CTF_CHEM_PRODUCTION		0x00800000
#define CTF_VAULT_CONTROLS		0x01000000
 // All other mission types are common to all corperations

//--------------------------
// Corporation Information
//--------------------------
// Corporation types (determines missions available)
#define CT_MEGACORP		0	// Mega-corporation. Anything possible.
#define CT_ELECTRONICS	1	// Electronics design/production
#define CT_BANK			2	// Banking
#define CT_LAB			3	// Laboratories
#define CT_CONSUMER		4	// Consumer goods
#define CT_MEDICAL		5	// Any medical
#define CT_OIL			6	// Petrolium
#define CT_MFG			7	// Manufacturing
#define CT_SCHOOL		8	// College/University
#define CT_ENVIRON		9	// Environmental

#define NUM_CORP_TYPES	10

// Missions available to all
#define CTF_COMMON (CTF_PERSONNEL_FILES|CTF_FINANCIAL_DATA|CTF_SECURITY_FILES|CTF_SECURITY_CAM_REC|CTF_EMPLOYEE_EVAL|CTF_ILLEGAL_ACTIVITY|CTF_DOORLOCKS|CTF_SECURITY_CAMERAS|CTF_ALARM_SYSTEMS|CTF_FIRE_ALARMS|CTF_SECURITY_ALARMS)

// Valid contract targets per corporation type
static const DWORD g_dwValidTarget[NUM_CORP_TYPES] =
{
/* CT_MEGACORP */	CTF_COMMON|CTF_RESEARCH_DATA|CTF_CHEM_FORMULA|CTF_BLUEPRINTS|CTF_PRODUCT_INFO|CTF_MEDICAL_RECORDS|CTF_TEST_RESULTS|CTF_INVENTORY_DATA,
/* CT_ELECT */		CTF_COMMON|CTF_RESEARCH_DATA|CTF_BLUEPRINTS|CTF_PRODUCT_INFO|CTF_TEST_RESULTS|CTF_INVENTORY_DATA|CTF_CHEM_ALARMS|CTF_MANUF_CONTROLS,
/* CT_BANK */		CTF_COMMON,
/* CT_LAB */		CTF_COMMON|CTF_RESEARCH_DATA|CTF_CHEM_FORMULA|CTF_PRODUCT_INFO|CTF_MEDICAL_RECORDS|CTF_TEST_RESULTS|CTF_INVENTORY_DATA|CTF_RADIATION_ALARMS|CTF_BIOHAZARD_ALARMS|CTF_CHEM_ALARMS|CTF_CHEM_PRODUCTION,
/* CT_CONSUMER */	CTF_COMMON|CTF_PRODUCT_INFO|CTF_INVENTORY_DATA,
/* CT_MEDICAL */	CTF_COMMON|CTF_RESEARCH_DATA|CTF_CHEM_FORMULA|CTF_MEDICAL_RECORDS|CTF_TEST_RESULTS|CTF_RADIATION_ALARMS|CTF_BIOHAZARD_ALARMS|CTF_CHEM_PRODUCTION,
/* CT_OIL */		CTF_COMMON|CTF_CHEM_FORMULA|CTF_TEST_RESULTS|CTF_CHEM_ALARMS|CTF_CHEM_PRODUCTION,
/* CT_MFG */		CTF_COMMON|CTF_BLUEPRINTS|CTF_PRODUCT_INFO|CTF_INVENTORY_DATA|CTF_MANUF_CONTROLS,
/* CT_SCHOOL */		CTF_COMMON|CTF_RESEARCH_DATA|CTF_CHEM_FORMULA|CTF_GRADE_REPORTS|CTF_BLUEPRINTS|CTF_MEDICAL_RECORDS|CTF_TEST_RESULTS|CTF_RADIATION_ALARMS|CTF_BIOHAZARD_ALARMS|CTF_CHEM_ALARMS,
/* CT_ENVIRON */	CTF_COMMON|CTF_TEST_RESULTS|CTF_BIOHAZARD_ALARMS|CTF_CHEM_ALARMS,
};

// Default contract targets per corporation type
static const int g_nDefaultTarget[NUM_CORP_TYPES] =
{
/* CT_MEGACORP */	CT_FINANCIAL_DATA,
/* CT_ELECT */		CT_BLUEPRINTS,
/* CT_BANK */		CT_FINANCIAL_DATA,
/* CT_LAB */		CT_RESEARCH_DATA,
/* CT_CONSUMER */	CT_INVENTORY_DATA,
/* CT_MEDICAL */	CT_MEDICAL_RECORDS,
/* CT_OIL */		CT_CHEM_PRODUCTION,
/* CT_MFG */		CT_MANUF_CONTROLS,
/* CT_SCHOOL */		CT_GRADE_REPORTS,
/* CT_ENVIRON */	CT_TEST_RESULTS,
};


// Corporation Information Structure
typedef struct {
	char *szName;
	int nCorpType;
} COORP_INFO;

// The corporation database (In order of size)
static const COORP_INFO g_CorpDB[NUM_CORP] = 
{
	{"Generic Genetics",					CT_MEDICAL},
	{"Radio Shack",							CT_CONSUMER},
	{"Digital Systems",						CT_ELECTRONICS},
	{"Ron's Repos",							CT_CONSUMER},
	{"Bubba's Discount Bait & Tackle",		CT_CONSUMER},
	{"North Carolina State University",		CT_SCHOOL},
	{"Sperm & Ova Superstore",				CT_CONSUMER},
	{"Ahab's Quickie Mart",					CT_CONSUMER},
	{"Kellog-Post",							CT_CONSUMER},
	{"Smith & Wesson",						CT_MFG},
	{"Meg-Lo-Mart",							CT_CONSUMER},
	{"ACME Industries",						CT_MFG},
	{"PETA's Flaky Granolas",				CT_CONSUMER},
	{"SAAB",								CT_MFG},
	{"Winchester",							CT_MFG},
	{"Limbs-R-Us",							CT_MEDICAL},
	{"Oxford University",					CT_SCHOOL},
	{"Little Robot Factory",				CT_ELECTRONICS},
	{"Henson's Toxic Waste Disposal",		CT_ENVIRON},
	{"Kraft",								CT_CONSUMER},
	{"Nike-Adidas",							CT_MFG},
	{"Me, Myself, & I Cloning",				CT_LAB},
	{"Luke's 'Danger Boy' RV's",			CT_MFG},
	{"Kentucky Fried Rodents",				CT_CONSUMER},
	{"Remington",							CT_MFG},
	{"Antarctica Bank & Trust",				CT_BANK},
	{"CalTech",								CT_SCHOOL},
	{"Noah's Livestock Engineering",		CT_MEDICAL},
	{"Mueller-Schluter Infotech",			CT_ELECTRONICS},
	{"Microdeck Technologies",				CT_ELECTRONICS},
	{"McWendys",							CT_CONSUMER},
	{"Multiplicity Cloning",				CT_MEDICAL},
	{"Spacely's Sprockets",					CT_MFG},
	{"Cross Applied Technologies",			CT_ELECTRONICS},
	{"SAAB",								CT_MFG},
	{"Walt Disney",							CT_CONSUMER},
	{"Watson Electronics",					CT_ELECTRONICS},
	{"Transys Neuronet",					CT_ELECTRONICS},
	{"Chevron",								CT_OIL},
	{"Harley-Davidson",						CT_MFG},
	{"Texaco",								CT_OIL},
	{"Shockwave Behavior Management",		CT_MEDICAL},
	{"Black Mesa Research Facilities",		CT_LAB},
	{"Shiawise",							CT_ELECTRONICS},
	{"Kurita Labs",							CT_LAB},
	{"Intel-TI",							CT_ELECTRONICS},
	{"Cybernetic Prosthetics",				CT_MEDICAL},
	{"Envirocon",							CT_ENVIRON},
	{"Steiner-Davion Robotics",				CT_MFG},
	{"Yamatetsu",							CT_ELECTRONICS},
	{"Yamaha",								CT_MFG},
	{"Warren Pharmaceuticals",				CT_LAB},
	{"Sara Lee",							CT_MEGACORP},
	{"Heckler & Koch",						CT_MFG},
	{"Consolidated Asteroid Mining",		CT_MFG},
	{"Exxon",								CT_OIL},
	{"NASA Spaceyachts",					CT_MFG},
	{"Soilant Products",					CT_LAB},
	{"Corning",								CT_MFG},
	{"Lear",								CT_MFG},
	{"Ewing Oil",							CT_OIL},
	{"New Earth Terraforming",				CT_ENVIRON},
	{"Procter & Gamble",					CT_LAB},
	{"Cessna",								CT_MFG},
	{"Sendanku",							CT_MFG},
	{"Sony",								CT_ELECTRONICS},
	{"Bayer Pharmaceuticals",				CT_LAB},
	{"Lunar Waste Disposal",				CT_ENVIRON},
	{"Fisher-Rosemont",						CT_ELECTRONICS},
	{"Motorola",							CT_ELECTRONICS},
	{"Bank of AmeriCanada",					CT_BANK},
	{"CytoTech Labs",						CT_LAB},
	{"Siemens",								CT_ELECTRONICS},
	{"Fuchi",								CT_MEGACORP},
	{"Federated Boeing",					CT_MFG},
	{"Ford",								CT_MFG},
	{"Massachussets Institute of Technology",CT_SCHOOL},
	{"Johnson & Johnson",					CT_MEGACORP},
	{"Phillip-Morris Pharmaceuticals",		CT_MEDICAL},
	{"Integrated Matrix Systems",			CT_ELECTRONICS},
	{"Cyberdine",							CT_ELECTRONICS},
	{"IBM",									CT_ELECTRONICS},
	{"European Financial Conglomeration",	CT_BANK},
	{"Aztechnology",						CT_ELECTRONICS},
	{"Chrysler-Nissan",						CT_MFG},
	{"Toyota",								CT_MFG},
	{"Gaeatronics",							CT_ELECTRONICS},
	{"Pepsi Cola",							CT_MFG},
	{"CryoTech",							CT_MEDICAL},
	{"Ares Macrotechnology",				CT_ELECTRONICS},
	{"Weyland-Yutani",						CT_MEGACORP},
	{"ChildVision Embryonic Design",		CT_MEDICAL},
	{"Coca Cola",							CT_CONSUMER},
	{"Zurich Orbital Gemeinschaft Bank",	CT_BANK},
	{"Renraku",								CT_MEGACORP},
	{"Novatech",							CT_MEGACORP},
	{"Mitsuhama",							CT_MEGACORP},
	{"Bell Labs",							CT_LAB},
	{"World Banking Consortium",			CT_BANK},
	{"Saeder-Krupp",						CT_MEGACORP},
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContract::CContract()
{
	m_nCorporation = 0;
	m_nSystemSize = 0;
	m_nType = 0;
	m_nTargetObject = 0;
	m_nDifficulty = 0;
	m_nPay = 0;
	m_dwFlags = 0;
	m_nDaysLeft = 0;
	m_nNumTargets = 1;
}

CContract::~CContract()
{

}

void CContract::GetDescriptiveText(CString &szText)
{
	CString szText2;

	// Each mission type has its own text
	switch (m_nType)
	{
	case CONT_STEAL:
		szText.Format("A client needs access to %s from the %s system. You must recover a copy of this data.",
			g_szTargetObjectText[m_nTargetObject], m_szSystemName);
		break;
	case CONT_STEAL_ERASE:
		szText.Format("A client desires sole access to %s from the %s system. You must recover a copy of this data and erase it from the system.",
			g_szTargetObjectText[m_nTargetObject], m_szSystemName);
		break;
	case CONT_ERASE:
		szText.Format("A client wishes to destroy %s located on the %s system. You must locate this data and erase it from the system.",
			g_szTargetObjectText[m_nTargetObject], m_szSystemName);
		break;
	case CONT_EDIT:
		szText.Format("A client wishes to alter %s located on the %s system. You must locate this data in the system and edit it to the client's specifications.",
			g_szTargetObjectText[m_nTargetObject], m_szSystemName);
		break;
	case CONT_DEACTIVATE_IO:
		szText.Format("A client wishes to deactivate %s from the %s system. You must locate the I/O node controlling this and deactivate it.",
			g_szTargetObjectText[m_nTargetObject], m_szSystemName);
		break;
	case CONT_ACTIVATE_IO:
		szText.Format("A client wishes to activate %s from the %s system. You must locate the I/O node controlling this and activate it.",
			g_szTargetObjectText[m_nTargetObject], m_szSystemName);
		break;
	case CONT_SABOTAGE_IO:
		szText.Format("A client wishes to sabotage %s at the %s system. You must locate the I/O node controlling this and sabotage it.",
			g_szTargetObjectText[m_nTargetObject], m_szSystemName);
		break;
	case CONT_CRASH_SYS:
		szText.Format("A client wishes to crash the %s system. You must locate the CPU for the system and initiate a system failure.",
			m_szSystemName);
		break;
	case CONT_BACKDOOR:
		szText.Format("A client wishes for a backdoor to be created into the %s system. You must locate the CPU for the system and create this back door. The client's decker will take care of the rest.",
			m_szSystemName);
		break;
	case CONT_RUN_PROGRAM:
		szText.Format("A client needs a special program activated on the %s system. Locate the target node(s) by running the program, then activate the program within the node. You must remain in the node until the program finishes running.",
			m_szSystemName);
		break;
	}

	// Add additional lines for various mission options
	//FSO 4-8-02
	if (m_nNumTargets > 1)
	{
		szText2.Format("(%d targets)", m_nNumTargets);
		szText += szText2;
	}
	if (m_dwFlags&CF_NO_ALARMS)
	{
		szText += " The target system must not become aware of any tampering.";
	}
	if (m_dwFlags&CF_TIMED)
	{
		szText2.Format(" The mission must be completed within %d minutes of system entry",m_dwTime);
		szText += szText2;
	}
}

void CContract::Generate()
{
	int i;
	int nBaseDifficulty;

	// Get the base difficulty
	if (g_pChar->m_nRepLevel > ((g_pChar->m_nLifestyle + 1) * 4))
	{
		nBaseDifficulty = ((g_pChar->m_nLifestyle + 1) * 4);
	}
	else
	{
		nBaseDifficulty = g_pChar->m_nRepLevel;
	}

	// Set minimum difficulty to 1
	if (nBaseDifficulty == 0)
	{
		nBaseDifficulty = 1;
	}

	// Set the actual difficulty to the base difficulty.
	// This will be adjusted as we go
	m_nDifficulty = nBaseDifficulty;

	// Choose a system size around the target difficulty
	// 10% -2, 20% -1, 40% 0, 20% +1, 10% +2
	i = Random(10);
	switch (i)
	{
	case 0:	// -2
		if (m_nDifficulty!=1)
		{
			m_nDifficulty--;
		}
		// Fall through
	case 1:	//-1
	case 2:
		if (m_nDifficulty!=1)
		{
			m_nDifficulty--;
		}
		break;
	case 3:	// +2
		if (m_nDifficulty!=20)
		{
			m_nDifficulty++;
		}
		// Fall through
	case 4:	// +1
	case 5:
		if (m_nDifficulty!=20)
		{
			m_nDifficulty++;
		}
		break;
	//default:
	}

	//------------------------------------
	// Choose the system to attack
	//------------------------------------
	// Set the initial system size to be the difficulty
	m_nSystemSize = m_nDifficulty;

	// Choose one of the five corps at this size
	m_nCorporation = ((m_nSystemSize-1)*5) + Random(5);

	// Get the name
	m_szSystemName = g_CorpDB[m_nCorporation].szName;

	//FSO 3-30-04 Changing the way contract targets are generated. Previously, we chose a target and got the
	//			  mission type from the target type (backwards). Now, we are going to choose a mission type first
	//			  and then choose a target.
	//
	//	Breakdown:
	//		Steal files - 15%
	//		Steal and erase - 10%
	//		Erase - 10%
	//		Edit - 10%						DataStore: 45%
	//		Deactivate - 15%
	//		Activate - 15%
	//		Sabotage - 10%					I/O Node: 40
	//		Crash - 5%
	//		Backdoor - 5%					CPU: 10%
	//		Run Program - 5%				Steal Files: 5%
	//
	// Choose a mission type
	i = Random(100);
	if (i < 15)
	{
		// Steal files - 15%
		m_nType = CONT_STEAL;
	}
	else if (i < 25)
	{
		// Steal and erase - 10%
		m_nType = CONT_STEAL_ERASE;
	}
	else if (i < 35)
	{
		// Erase - 10%
		m_nType = CONT_ERASE;
	}
	else if (i < 45)
	{
		// Edit - 10%
		m_nType = CONT_EDIT;
	}
	else if (i < 60)
	{
		// Deactivate - 15%
		m_nType = CONT_DEACTIVATE_IO;
	}
	else if (i < 75)
	{
		// Activate - 15%
		m_nType = CONT_ACTIVATE_IO;
	}
	else if (i < 85)
	{
		// Sabotage - 10%
		m_nType = CONT_SABOTAGE_IO;
	}
	else if (i < 90)
	{
		// Crash - 5%
		m_nType = CONT_CRASH_SYS;
	}
	else if (i < 95)
	{
		// Backdoor - 5%
		m_nType = CONT_BACKDOOR;
	}
	else //if (i < 100)
	{
		// Run Program - 5%
		m_nType = CONT_RUN_PROGRAM;
	}

	// Generate the valid target flags for the corperation. Note that this is used later for
	// generating file types for datastores in the matrix.
	m_dwTargetFlags = g_dwValidTarget[g_CorpDB[m_nCorporation].nCorpType];

	// Choose a target base on mission type
	int nMinType;
	int nMaxType;
	BOOL bDone = FALSE;

	switch (m_nType)
	{
	case CONT_STEAL			:
	case CONT_STEAL_ERASE	:
	case CONT_ERASE			:
	case CONT_EDIT			:
		nMinType = CT_MIN_DS;
		nMaxType = CT_MAX_DS;
		break;
	case CONT_DEACTIVATE_IO	:
		nMinType = CT_MIN_DEACT;
		nMaxType = CT_MAX_DEACT;
		break;
	case CONT_ACTIVATE_IO	:
		nMinType = CT_MIN_ACT;
		nMaxType = CT_MAX_ACT;
		break;
	case CONT_SABOTAGE_IO	:
		nMinType = CT_MIN_SAB;
		nMaxType = CT_MAX_SAB;
		break;
	case CONT_CRASH_SYS		:
	case CONT_BACKDOOR		:
		m_nTargetObject = CT_CPU;
		bDone = TRUE;
		break;
	case CONT_RUN_PROGRAM	:
		m_nTargetObject = CT_MIN_RUNPROG + Random(1 + CT_MAX_RUNPROG - CT_MIN_RUNPROG);
		bDone = TRUE;
		break;
	}

	// Choose until we get a valid one
	while (!bDone)
	{
		m_nTargetObject = nMinType + Random(1 + nMaxType - nMinType);

		if (m_dwTargetFlags & (1<<m_nTargetObject))
		{
			bDone = TRUE;
		}
	}

	// Check for mission enhancers
	// Check for no alarms (20% chance, 40% for edit, 50% for backdoor, no chance for crash)
	int nChance;
	switch (m_nType)
	{
	case CONT_CRASH_SYS:
		nChance = 0;
		break;
	case CONT_EDIT:
		nChance = 4;
		break;
	case CONT_BACKDOOR:
		nChance = 5;
		break;
	default:
		nChance = 2;
		break;
	}
	if (Random(10) < nChance)
	{
		m_dwFlags |= CF_NO_ALARMS;
		m_nDifficulty++;
	}

	// Check for timed mission (10% chance)
	if (nBaseDifficulty>4) // Not until base level 5
	{
		i = Random(10);
		if (i==0)
		{
			// Time is three minutes per area (3..15)
			m_dwFlags |= CF_TIMED;
			m_dwTime = 3 * ((m_nSystemSize+3)/4);
			m_nDifficulty += 2;
		}
	}

	// FSO 4-8-02
	// Check for multiple targets
	if ((m_nTargetObject != CT_CPU) && (m_nTargetObject != CT_NODE_CPU))
	{
		// 2.5% at level 1. 50% chance at level 20.
		if (Random(40) < nBaseDifficulty)
		{
			// 2 to 5 targets
			m_nNumTargets = 2 + Random(4);
			m_nDifficulty += (m_nNumTargets/2);
		}
	}

	// FSO 7-24-01
	// Generate the days allowed
	if (m_dwFlags & CF_TIMED)
	{
		// Timed missions happen immediately
		m_nDaysLeft = 1;
	}
	else
	{
		// Others have 1 to 5 days
		m_nDaysLeft = Random(5) + 1;
	}

	// Generate the pay
	// Base pay is 100 * (difficulty + lifestyle))
	m_nPay = 100 * (m_nDifficulty + g_pChar->m_nLifestyle);

	// Modify for length of time allowed, with 3 being average (-20%..+20%)
	m_nPay += ((m_nPay * (3-m_nDaysLeft))/10);

	// Modify by a random factor (+/- 1..15%), which represents client's cheapness/generosity
	i = Random(31) - 15; // -15..15%
	m_nPay += ((m_nPay * i)/100);
}

void CContract::GetTypeText(CString &szTypeText)
{
	CString szTmp;

	// Get the basic text
	szTypeText = g_szContractTypeText[m_nType];

	// Add qualifiers
	if (m_nNumTargets != 1)
	{
		szTmp.Format("%d", m_nNumTargets);
	}
	if (m_dwFlags & CF_NO_ALARMS)
	{
		szTmp += 'N';
	}
	if (m_dwFlags & CF_TIMED)
	{
		szTmp += 'T';
	}

	if (!szTmp.IsEmpty())
	{
		szTypeText += " (";
		szTypeText += szTmp;
		szTypeText += ")";
	}
}

const char *CContract::GetTargetObjectText()
{
	return g_szTargetObjectText[m_nTargetObject];
}

void CContract::Save(CArchive &ar)
{
	ar << m_nCorporation;
	ar << m_szSystemName;
	ar << m_nSystemSize;
	ar << m_dwTargetFlags;
	ar << m_nType;
	ar << m_nTargetObject;
	ar << m_nDifficulty;
	ar << m_nPay;
	ar << m_dwFlags;
	ar << m_dwTime;
	ar << m_nCompletionTime; // Not used
	ar << m_nDaysLeft;
	ar << m_nNumTargets;
}

void CContract::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	ar >> m_nCorporation;
	ar >> m_szSystemName;
	ar >> m_nSystemSize;
	ar >> m_dwTargetFlags;
	ar >> m_nType;
	ar >> m_nTargetObject;
	ar >> m_nDifficulty;
	ar >> m_nPay;
	ar >> m_dwFlags;
	ar >> m_dwTime;
	ar >> m_nCompletionTime; // Not used
	if ((wMajorVersion == 0) && (wMinorVersion < 6))
	{
		// Use the default of 3 days
		m_nDaysLeft = 3;
	}
	else
	{
		ar >> m_nDaysLeft;
	}
	if (VersionCompare(wMajorVersion,wMinorVersion,1,6) >= 0)
	{
		ar >> m_nNumTargets;
	}
}

BOOL CContract::HintAvailable()
{
	switch(m_nType)
	{
	case CONT_STEAL:
	case CONT_STEAL_ERASE:
	case CONT_ERASE:
	case CONT_EDIT:
	case CONT_DEACTIVATE_IO:
	case CONT_ACTIVATE_IO:
	case CONT_SABOTAGE_IO:
		return TRUE;
	//case CONT_CRASH_SYS:
	//case CONT_BACKDOOR:
	//case CONT_RUN_PROGRAM:
	default:
		return FALSE;
	}
}

// Global functions
int GetFileTypePerCorp()
{
	int nFileType;

	// Generate until we gat a valid one
	for (;;)
	{
		// Generate a random file type
		nFileType = Random(1 + CT_MAX_DS);

		// And see if it is valid
		
		if (g_pChar->m_pCurrentContract->m_dwTargetFlags & (1<<nFileType)) break;
	}

	return nFileType;
}

int GetContractStatus(CString &szOverall, CString &szBreakdown, BOOL bInProgress, BOOL bCrash)
{
	BOOL bFailed = FALSE;
	BOOL bIncomplete = FALSE;
	CDSFile *pFile;
	CObList olFiles;
	CNode *pNode;
	CObList olNodes;
	CString szTxt;

	switch (g_pChar->m_pCurrentContract->m_nType)
	{
	case CONT_STEAL			:
		//szBreakdown += "Steal Files:\n";

		// Built a list of all contract files
		g_pChar->m_pSystem->BuildFileList(olFiles, TRUE);

		// Find the quest files
		while (!olFiles.IsEmpty())
		{
			pFile = (CDSFile *)olFiles.RemoveHead();
			
			// Is it in the deck?
			if (pFile->m_nState & STATE_IN_DECK)
			{
				// Is it edited?
				if (pFile->m_nState & STATE_EDITED_D)
				{
					bFailed = TRUE;
					szTxt.Format("(F) File %s was edited before download, making it useless.\n",pFile->m_szName);
				}
				else
				{
					szTxt.Format("(C) File %s was downloaded.\n",pFile->m_szName);
				}
			}
			else
			{
				bIncomplete = TRUE;
				if (bInProgress)
				{
					if (pFile->m_nState & STATE_SCAN)
					{
						szTxt.Format("(I) File %s has not been downloaded.\n",pFile->m_szName);
					}
					else
					{
						szTxt.Format("(I) File ??? has not been downloaded.\n");
					}
				}
				else
				{
					szTxt.Format("(I) File %s was not downloaded.\n",pFile->m_szName);
				}
			}

			szBreakdown += szTxt;
		}
		break;

	case CONT_STEAL_ERASE	:
		//szBreakdown += "Steal & Erase Files:\n";

		// Built a list of all contract files
		g_pChar->m_pSystem->BuildFileList(olFiles, TRUE);

		// Find the quest files
		while (!olFiles.IsEmpty())
		{
			pFile = (CDSFile *)olFiles.RemoveHead();
			
			// Is it in the deck?
			if (pFile->m_nState & STATE_IN_DECK)
			{
				// Is it edited?
				if (pFile->m_nState & STATE_EDITED_D)
				{
					bFailed = TRUE;
					szTxt.Format("(F) File %s was edited before download, making it useless.\n",pFile->m_szName);
				}
				else
				{
					szTxt.Format("(C) File %s was successfully downloaded.\n",pFile->m_szName);
				}
			}
			else
			{
				bIncomplete = TRUE;
				if (bInProgress)
				{
					if (pFile->m_nState & STATE_SCAN)
					{
						szTxt.Format("(I) File %s has not been downloaded.\n",pFile->m_szName);
					}
					else
					{
						szTxt.Format("(I) File ??? has not been downloaded.\n");
					}
				}
				else
				{
					szTxt.Format("(I) File %s was not downloaded.\n",pFile->m_szName);
				}
			}
			szBreakdown += szTxt;

			// Is it erased?
			if (pFile->m_nState & STATE_IN_NODE)
			{
				bIncomplete = TRUE;
				if (bInProgress)
				{
					if (pFile->m_nState & STATE_SCAN)
					{
						szTxt.Format("(I) File %s has not been erased.\n",pFile->m_szName);
					}
					else
					{
						szTxt.Format("(I) File ??? has not been erased.\n");
					}
				}
				else
				{
					szTxt.Format("(I) File %s was not erased.\n",pFile->m_szName);
				}
			}
			else
			{
				szTxt.Format("(C) Mission file %s was erased.\n",pFile->m_szName);
			}

			szBreakdown += szTxt;
		}
		break;

	case CONT_ERASE			:
		//szBreakdown += "Erase Files:\n";

		// Built a list of all contract files
		g_pChar->m_pSystem->BuildFileList(olFiles, TRUE);

		// Find the quest files
		while (!olFiles.IsEmpty())
		{
			pFile = (CDSFile *)olFiles.RemoveHead();
			
			// Is it erased?
			if (pFile->m_nState & STATE_IN_NODE)
			{
				bIncomplete = TRUE;
				if (bInProgress)
				{
					if (pFile->m_nState & STATE_SCAN)
					{
						szTxt.Format("(I) File %s has not been erased.\n",pFile->m_szName);
					}
					else
					{
						szTxt.Format("(I) File ??? has not been erased.\n");
					}
				}
				else
				{
					szTxt.Format("(I) File %s was not erased.\n",pFile->m_szName);
				}
			}
			else
			{
				szTxt.Format("(C) Mission file %s was erased.\n",pFile->m_szName);
			}

			szBreakdown += szTxt;
		}
		break;

	case CONT_EDIT			:
		//szBreakdown += "Edit Files:\n";

		// Built a list of all contract files
		g_pChar->m_pSystem->BuildFileList(olFiles, TRUE);

		// Find the quest files
		while (!olFiles.IsEmpty())
		{
			pFile = (CDSFile *)olFiles.RemoveHead();
			
			// Is it erased?
			if (pFile->m_nState & STATE_IN_NODE)
			{
				// Was it edited
				if (pFile->m_nState & STATE_EDITED_N)
				{
					szTxt.Format("(C) File %s was edited.\n",pFile->m_szName);
				}
				else
				{
					bIncomplete = TRUE;
					if (bInProgress)
					{
						if (pFile->m_nState & STATE_SCAN)
						{
							szTxt.Format("(I) File %s has not been edited.\n",pFile->m_szName);
						}
						else
						{
							szTxt.Format("(I) File ??? has not been edited.\n");
						}
					}
					else
					{
						szTxt.Format("(I) File %s was not edited.\n",pFile->m_szName);
					}
				}
			}
			else
			{
				bFailed = TRUE;
				szTxt.Format("(F)File %s was erased.\n",pFile->m_szName);
			}

			szBreakdown += szTxt;
		}
		break;

	case CONT_DEACTIVATE_IO	:
		//szBreakdown += "Deactivate IO:\n";

		// Build a list of IO nodes
		g_pChar->m_pSystem->BuildNodeList(olNodes, NT_IO, TRUE);

		// Go through the list of nodes, looking for the IO node
		while (!olNodes.IsEmpty())
		{
			pNode = (CNode *)olNodes.RemoveHead();

			if (pNode->m_bActivated)
			{
				szTxt.Format("(C) IO node %s was deactivated\n", pNode->m_szName);
			}
			else
			{
				bIncomplete = TRUE;
				if (bInProgress)
				{
					if (pNode->m_nSpecialImage == -1)
					{
						szTxt.Format("(I) IO node ??? has not been deactivated\n");
					}
					else
					{
						szTxt.Format("(I) IO node %s has not been deactivated\n", pNode->m_szName);
					}
				}
				else
				{
					szTxt.Format("(I) IO node %s was not deactivated\n", pNode->m_szName);
				}
			}

			szBreakdown += szTxt;
		}
		break;

	case CONT_ACTIVATE_IO	:
		//szBreakdown += "Activate IO:\n";

		// Build a list of IO nodes
		g_pChar->m_pSystem->BuildNodeList(olNodes, NT_IO, TRUE);

		// Go through the list of nodes, looking for the IO node
		while (!olNodes.IsEmpty())
		{
			pNode = (CNode *)olNodes.RemoveHead();

			if (pNode->m_bActivated)
			{
				szTxt.Format("(C) IO node %s was activated\n", pNode->m_szName);
			}
			else
			{
				bIncomplete = TRUE;
				if (bInProgress)
				{
					if (pNode->m_nSpecialImage == -1)
					{
						szTxt.Format("(I) IO node ??? has not been activated\n");
					}
					else
					{
						szTxt.Format("(I) IO node %s has not been activated\n", pNode->m_szName);
					}
				}
				else
				{
					szTxt.Format("(I) IO node %s was not activated\n", pNode->m_szName);
				}
			}

			szBreakdown += szTxt;
		}
		break;

	case CONT_SABOTAGE_IO	:
		//szBreakdown += "Sabotage IO:\n";

		// Build a list of IO nodes
		g_pChar->m_pSystem->BuildNodeList(olNodes, NT_IO, TRUE);

		// Go through the list of nodes, looking for the IO node
		while (!olNodes.IsEmpty())
		{
			pNode = (CNode *)olNodes.RemoveHead();

			if (pNode->m_bActivated)
			{
				szTxt.Format("(C) IO node %s was sabotaged\n", pNode->m_szName);
			}
			else
			{
				bIncomplete = TRUE;
				if (bInProgress)
				{
					if (pNode->m_nSpecialImage == -1)
					{
						szTxt.Format("(I) IO node ??? has not been sabotaged\n");
					}
					else
					{
						szTxt.Format("(I) IO node %s has not been sabotaged\n", pNode->m_szName);
					}
				}
				else
				{
					szTxt.Format("(I) IO node %s was not sabotaged\n", pNode->m_szName);
				}
			}

			szBreakdown += szTxt;
		}
		break;

	case CONT_CRASH_SYS		:
		if (bCrash)
		{
			szBreakdown += "(C) System was crashed.\n";
		}
		else
		{
			bIncomplete = TRUE;
			if (bInProgress)
			{
				szBreakdown += "(I) System has not been crashed.\n";
			}
			else
			{
				szBreakdown += "(I) System was not crashed.\n";
			}
		}
		break;

	case CONT_BACKDOOR		:
		if (g_pChar->m_pSystem->m_bBackdoor)
		{
			szBreakdown += "(C) Backdoor was created.\n";
		}
		else
		{
			bIncomplete = TRUE;
			if (bInProgress)
			{
				szBreakdown += "(I) Backdoor has not been created.\n";
			}
			else
			{
				szBreakdown += "(I) Backdoor was not created.\n";
			}
		}
		break;

	case CONT_RUN_PROGRAM		:
		// Build a list of quest nodes
		if (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_IO)
		{
			g_pChar->m_pSystem->BuildNodeList(olNodes, NT_IO, TRUE);
		}
		else if (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_DS)
		{
			g_pChar->m_pSystem->BuildNodeList(olNodes, NT_DS, TRUE);
		}
		else //if (g_pChar->m_pCurrentContract->m_nTargetObject == CT_NODE_CPU)
		{
			olNodes.AddTail(g_pChar->m_pSystem->m_pSystemCPU);
		}

		// Go through the list of nodes, looking for the IO node
		while (!olNodes.IsEmpty())
		{
			pNode = (CNode *)olNodes.RemoveHead();

			if (pNode->m_bActivated)
			{
				szTxt.Format("(C) Program was successfully run in node %s\n", pNode->m_szName);
			}
			else
			{
				bIncomplete = TRUE;
				if (bInProgress)
				{
					if (pNode->m_nSpecialImage == -1)
					{
						szTxt.Format("(I) Program has not been run in node ???\n");
					}
					else
					{
						szTxt.Format("(I) Program has not been run in node %s\n", pNode->m_szName);
					}
				}
				else
				{
					szTxt.Format("(I) Program was not run in node %s\n", pNode->m_szName);
				}
			}

			szBreakdown += szTxt;
		}
		break;
	}

	// Check for no alarms
	if (g_pChar->m_pCurrentContract->m_dwFlags & CF_NO_ALARMS)
	{
		// See if alarms were set
		if (g_pChar->m_dwRunFlags & CRF_ALARMS_SET)
		{
			bFailed = TRUE;
			szBreakdown += "(F) A red alert was triggered.\n";
		}
		else
		{
			if (bInProgress)
			{
				szBreakdown += "(C) No red alerts have been detected.\n";
			}
			else
			{
				szBreakdown += "(C) No red alerts were detected.\n";
			}
		}
	}

	// Check timer
	if (g_pChar->m_pCurrentContract->m_dwFlags & CF_TIMED)
	{
		// See if the time was ok (Contract time is in minutes)
		if (g_pChar->m_nRunTime <= (int)(g_pChar->m_pCurrentContract->m_dwTime*60))
		{
			if (bInProgress)
			{
				szBreakdown += "(I) Time limit has not expired.\n";
			}
			else
			{
				szBreakdown += "(C) Time limit did not expire.\n";
			}
		}
		else
		{
			bFailed = TRUE;
			szBreakdown += "(F) Time limit expired.\n";
		}
	}

	// Check deadline
	if ((!bInProgress) && (!bFailed) && (bIncomplete))
	{
		if (g_pChar->m_pCurrentContract->m_nDaysLeft == 1)
		{
			bFailed = TRUE;
			szBreakdown += "Mission was not completed before deadline.\n";
		}
		else
		{
			szTxt.Format("You have %d days left before deadline.\n", 
				(g_pChar->m_pCurrentContract->m_nDaysLeft - 1));
			szBreakdown += szTxt;
		}
	}

	// Now, compute overall success/failure
	if (bFailed)
	{
		szOverall = "Failed";
		return -1;
	}
	else if (bIncomplete)
	{
		szOverall = "Incomplete";
		return 0;
	}
	else
	{
		szOverall = "Complete";
		return 1;
	}
}
