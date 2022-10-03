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

// Character.cpp: implementation of the CCharacter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "Character.h"
#include "Program.h"
#include "Contract.h"
#include "System.h"
#include "Area.h"
#include "Node.h"
#include "Global.h"
#include "ShopItem.h"
#include "Source.h"
#include "Ice.h"
#include "DSFile.h"
#include "DeckerGraphics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const char *g_szHardwareName[NUM_HW] = 
{
	"Chip Burner",
	"Surge Suppressor",
	"Neural Damper",
	"Trace Monitor",
	"Bio Monitor",
	"High Bandwidth Bus",
	"Proximity Mapper",
	"Design Assistant", //FSO 6-20-01
	"AntiTrace Proxy",
};

static const BOOL g_bInitialProgramFlag[NUM_PROGRAMS] = 
{
	TRUE,	//PROGRAM_ATTACK	
	FALSE,	//PROGRAM_ATTACK_A
	FALSE,	//PROGRAM_ATTACK_P
	FALSE,	//PROGRAM_SLOW	
	FALSE,	//PROGRAM_VIRUS	
	TRUE,	//PROGRAM_SILENCE	
	FALSE,	//PROGRAM_CONFUSE	
	FALSE,	//PROGRAM_WEAKEN	
	FALSE,	//PROGRAM_SHIELD	
	TRUE,	//PROGRAM_SMOKE	
	FALSE,	//PROGRAM_DECOY	
	TRUE,	//PROGRAM_MEDIC	
	TRUE,	//PROGRAM_ARMOR	
	TRUE,	//PROGRAM_HIDE	
	TRUE,	//PROGRAM_DECEIVE	
	FALSE,	//PROGRAM_RELOCATE
	TRUE,	//PROGRAM_ANALYZE	
	TRUE,	//PROGRAM_SCAN	
	TRUE,	//PROGRAM_EVALUATE
	TRUE,	//PROGRAM_DECRYPT	
	FALSE,	//PROGRAM_REFLECT
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCharacter::CCharacter()
{
	int i;

	// Set initial values
	m_bIronMan = FALSE;
	m_bTooltips = FALSE;
	m_nCredits = 100;
	m_nRepLevel = 0;
	m_nRepPoints = 0;
	m_nLifestyle = LS_STREET;
	m_nAttackSkill = 1;
	m_nDefenseSkill = 1;
	m_nStealthSkill = 1;
	m_nAnalysisSkill = 1;
	m_nProgrammingSkill = 1;
	m_nChipDesignSkill = 1;
	m_nSkillPoints = 0;
	m_nImage = 0;
	memset(m_bBackdoor, 0, NUM_CORP);
	memset(m_bSystemAlerts, 0, NUM_CORP);

	m_nChip[CHIP_CPU] = 1;
	m_nChip[CHIP_ATTACK] = 1;
	m_nChip[CHIP_DEFENSE] = 1;
	m_nChip[CHIP_STEALTH] = 1;
	m_nChip[CHIP_ANALYSIS] = 1;
	m_nChip[CHIP_COPROCESSOR] = 1;

	//m_nHardware[HW_CHIP_BURNER	] = 0;
	//m_nHardware[HW_SURGE_SUPP	] = 0;
	//m_nHardware[HW_NEURAL_DAMPER] = 0;
	//m_nHardware[HW_TRACE_MONITOR] = 0;
	//m_nHardware[HW_BIO_MONITOR	] = 0;
	//m_nHardware[HW_HIGH_BW_BUS	] = 0;
	for (i = 0; i<NUM_HW_RESERVED; i++)
	{
		m_nHardware[i] = 0;
	}

	m_nDayOfMonth = 0;
	m_nMonth = 0;
	m_nYear = 2132;
	m_nDayOfWeek = 0;
	m_bOnRun = FALSE;
	m_nHealthPhysical = MAX_HEALTH;
	m_pCurrentContract = NULL;
	m_pSystem = NULL;
	m_nProjectType = -1;
	m_nProjectClass = 0;
	m_nProjectRating = 0;
	m_nProjectInitialTime = 0;
	m_nProjectTimeLeft = 0;
	m_pChipBurning = FALSE;
	m_nChipBurnTimeLeft = 0;
	m_pDefAttackProgram = NULL;
	m_pDefArmorProgram = NULL;
	m_pDefShieldProgram = NULL;
	m_pDefHideProgram = NULL;
	m_pDefReflectProgram = NULL;
	m_nDecoyCount = 0;
	m_nClientProgramStatus = 0;
	m_nOrderItemType = -1;
	m_nDamageMental = 0;
	m_nDamageDeck = 0;

	CalcCurrentLoad();

	// Don't bother with run information. It is reset when run starts
}

CCharacter::~CCharacter()
{
	// Clear out the programs
	CProgram *pProgram;
	while (!m_olSoftware.IsEmpty())
	{
		pProgram = (CProgram *)m_olSoftware.RemoveHead();
		delete pProgram;
	}

	// Clear out the available contracts
	ClearContracts();

	if (m_pCurrentContract!=NULL)
	{
		// Contract accepted, so kill it.
		delete m_pCurrentContract;
	}

	// Clear out the shop items
	ClearShopItems();

	// Delete the current system
	if (m_pSystem!=NULL)
	{
		delete m_pSystem;
	}

	// Remove the items from the current node ice list
	m_olCurrentIceList.RemoveAll();

	// Clear source
	CSource *pSource;
	while (!m_olSourceCode.IsEmpty())
	{
		pSource = (CSource *)m_olSourceCode.RemoveHead();
		delete pSource;
	}
}

void CCharacter::CreateNew(int nBonus)
{
	CProgram *pProgram;

	// Adjust for bonuses
	if (nBonus == BONUS_SKILLS)
	{
		m_nAttackSkill = 2;
		m_nDefenseSkill = 2;
		m_nStealthSkill = 2;
		m_nAnalysisSkill = 2;
		m_nProgrammingSkill = 2;
		m_nChipDesignSkill = 2;
	}
	else if (nBonus == BONUS_HARDWARE)
	{
		m_nChip[CHIP_CPU] = 2;
		m_nChip[CHIP_ATTACK] = 2;
		m_nChip[CHIP_DEFENSE] = 2;
		m_nChip[CHIP_STEALTH] = 2;
		m_nChip[CHIP_ANALYSIS] = 2;
		m_nChip[CHIP_COPROCESSOR] = 2;
	}
	else if (nBonus == BONUS_MONEY)
	{
		m_nCredits = 2000;
	}

	// Create inital programs
	for (int nType = 0; nType < NUM_PROGRAMS; nType++)
	{
		if (g_bInitialProgramFlag[nType])
		{
			pProgram = new CProgram;
			if (nBonus == BONUS_SOFTWARE)
			{
				pProgram->GetInitValues(nType,2);
			}
			else
			{
				pProgram->GetInitValues(nType,1);
			}
			pProgram->m_bLoadByDefault = TRUE;
			m_olSoftware.AddTail(pProgram);

			if (nType == PROGRAM_ATTACK)
				m_pDefAttackProgram = pProgram;
			else if (nType == PROGRAM_ARMOR)
				m_pDefArmorProgram = pProgram;
			else if (nType == PROGRAM_SHIELD)
				m_pDefShieldProgram = pProgram;
			else if (nType == PROGRAM_HIDE)
				m_pDefHideProgram = pProgram;
			else if (nType == PROGRAM_REFLECT)
				m_pDefReflectProgram = pProgram;
		}
	}

	// Generate initial contracts
	GenerateContracts();

	// Generate initial shop items
	GenerateShopItems();
}

const char * CCharacter::GetLifestyleString()
{
	return g_szLifestyleString[m_nLifestyle];
}

void CCharacter::CalcCurrentLoad()
{
	POSITION p;
	CProgram *pProgram;

	// Calculate the current load
	m_nCurrentLoad = 0;

	p = m_olSoftware.GetHeadPosition();
	while (p!=NULL)
	{
		pProgram = (CProgram *)m_olSoftware.GetNext(p);
		if (!m_bOnRun)
		{
			// Update loaded rating
			if (pProgram->m_bLoadByDefault)
			{
				pProgram->m_nLoadedRating = pProgram->m_nRating;
			}
			else
			{
				pProgram->m_nLoadedRating = 0;
			}
		}
		if (pProgram->m_nLoadedRating!=0)
		{
			m_nCurrentLoad += pProgram->GetSize();
		}
	}

	// Get the load modifier
	int nLight, nHeavy, nMax;
	GetLoadRatings(nLight,nHeavy,nMax);

	if (m_nCurrentLoad < nLight)
		m_nLoadStatus = LS_LIGHT;
	else if (m_nCurrentLoad > nHeavy)
		m_nLoadStatus = LS_HEAVY;
	else
		m_nLoadStatus = LS_NORMAL;
}

void CCharacter::GenerateContracts()
{
	int nMax;
	int nCount;
	CContract *pContract;

//FSO 7-25-01 Now, don't clear out old contracts - 
//			  They will expire on their own
//	// Clear out the old contracts
//	ClearContracts();

	// Generate 1..3+(Rep/5) contracts (1 = 1..3, 5 = 1..4, etc.)
	nMax = 3+(m_nRepLevel/5);

	nCount = 1+Random(nMax);

	// FSO 7-25-01 Count the number already there
	//nCount -= m_olContracts.GetCount();
	// FSO 3-30-04 Generate up to the maximum number
	if (nCount + m_olContracts.GetCount() > nMax)
	{
		nCount = nMax - m_olContracts.GetCount();
	}

	while (nCount-- > 0)
	{
		pContract = new CContract;
		pContract->Generate();
		m_olContracts.AddTail(pContract);
	}
}

void CCharacter::ClearContracts()
{
	CContract *pContract;

	while (!m_olContracts.IsEmpty())
	{
		pContract = (CContract *)m_olContracts.RemoveHead();
		delete pContract;
	}
}

void CCharacter::ClearCurrentContract()
{
	// If no contract, nothing to do
	if (m_pCurrentContract != NULL)
	{
		// If this is a 'run program' contract, delete the client program
		if (m_pCurrentContract->m_nType == CONT_RUN_PROGRAM)
		{
			POSITION p,p2;
			CProgram *pProgram;

			p = m_olSoftware.GetTailPosition();
			while (p != NULL)
			{
				p2 = p;
				pProgram = (CProgram *)m_olSoftware.GetPrev(p);

				if (pProgram->m_nClass == PROGRAM_CLIENT)
				{
					m_olSoftware.RemoveAt(p2);
					delete pProgram;
					// Should only have one, so we are done
					break;
				}
			}
			CalcCurrentLoad();
		}

		// Delete the contract
		delete m_pCurrentContract;
		m_pCurrentContract = NULL;
	}
}

void CCharacter::GenerateShopItems()
{
	CShopItem *pItem;
	int iCount;
	int i;

	// Clear out the old items
	ClearShopItems();

	//-------------------
	// Generate software
	//-------------------
	// Get number to have on sale (3..12)
	iCount = 3 + Random(4) + Random(4) + Random(4);

	// Create the items
	for (i=0; i<iCount; i++)
	{
		// Allocate a new item
		pItem = new CShopItem;

		// Generate until get a good one
		for (;;)
		{
			pItem->Generate(IT_SOFTWARE);

			if (!FindShopItem(pItem)) break;
		}

		// Add the item to the list
		m_olShopList.AddTail(pItem);
	}

	//-------------------
	// Generate hardware
	//-------------------
	// Get number to have on sale (1..4)
	iCount = 1 + Random(4);

	// Create the items
	for (i=0; i<iCount; i++)
	{
		// Allocate a new item
		pItem = new CShopItem;

		// Generate until get a good one
		for (;;)
		{
			pItem->Generate(IT_HARDWARE);

			if (!FindShopItem(pItem)) break;
		}

		// Add the item to the list
		m_olShopList.AddTail(pItem);
	}

	//-------------------
	// Generate chips
	//-------------------
	// Get number to have on sale (2..8)
	iCount = 2 + Random(4) + Random(4);

	// Create the items
	for (i=0; i<iCount; i++)
	{
		// Allocate a new item
		pItem = new CShopItem;

		// Generate until get a good one
		for (;;)
		{
			pItem->Generate(IT_CHIP);

			if (!FindShopItem(pItem)) break;
		}

		// Add the item to the list
		m_olShopList.AddTail(pItem);
	}
}

void CCharacter::ClearShopItems()
{
	CShopItem *pItem;
	
	while (!m_olShopList.IsEmpty())
	{
		pItem = (CShopItem *)m_olShopList.RemoveHead();
		delete pItem;
	}

}

BOOL CCharacter::FindShopItem(CShopItem *pItem)
{
	POSITION p;
	CShopItem *pListItem;

	p = m_olShopList.GetHeadPosition();
	while (p!=NULL)
	{
		pListItem = (CShopItem *)m_olShopList.GetNext(p);

		if ((pItem->m_nType == pListItem->m_nType) &&
			(pItem->m_nSubType == pListItem->m_nSubType) &&
			(pItem->m_nRating == pListItem->m_nRating))
		{
			return TRUE;
		}
	}

	return FALSE;
}


CProgram *CCharacter::FindProgram(int nType, BOOL bLoaded)
{
	CProgram *pProgram;
	POSITION p;
	CProgram *pHighestProgram = NULL;

	// Scan the software list
	p = m_olSoftware.GetHeadPosition();
	while (p!=NULL)
	{
		pProgram = (CProgram *)m_olSoftware.GetNext(p);

		// Look for the highest rated program of this type
		if (pProgram->m_nClass == nType)
		{
			if (bLoaded)
			{
				// Only consider loaded programs
				if (pProgram->m_nLoadedRating > 0)
				{
					if ((pHighestProgram==NULL) || (pProgram->m_nLoadedRating > pHighestProgram->m_nLoadedRating))
					{
						pHighestProgram = pProgram;
					}
				}
			}
			else
			{
				// Consider all programs
				if ((pHighestProgram==NULL) || (pProgram->m_nRating > pHighestProgram->m_nRating))
				{
					pHighestProgram = pProgram;
				}
			}
		}
	}

	return pHighestProgram;
}

int CCharacter::GetEffectiveRating(int nType)
{
	int nRating = 0;

	switch (nType)
	{
	case RATING_ATTACK:
		nRating = __min(m_nChip[CHIP_ATTACK], m_nChip[CHIP_CPU]) + m_nAttackSkill;
		if (m_pCurrentNode->m_nActiveSmokeLevel)
		{
			nRating-=4;
		}
		if ((m_pActiveBoost != NULL) && (m_pActiveBoost->m_nClass == PROGRAM_ATTACK_BOOST))
		{
			nRating += 3;
		}
		break;
	case RATING_DEFENSE:
		nRating = __min(m_nChip[CHIP_DEFENSE], m_nChip[CHIP_CPU]) + m_nDefenseSkill;
		if (m_pCurrentNode->m_nActiveSmokeLevel)
		{
			nRating+=4;
		}
		if ((m_pActiveBoost != NULL) && (m_pActiveBoost->m_nClass == PROGRAM_DEFENSE_BOOST))
		{
			nRating += 3;
		}
		break;
	case RATING_STEALTH:
		nRating = __min(m_nChip[CHIP_STEALTH], m_nChip[CHIP_CPU]) + m_nStealthSkill;
		if (m_pCurrentNode->m_nActiveSmokeLevel)
		{
			nRating+=4;
		}

		// If traced, reduce the rating
		if (m_bTraced) nRating -=4;

		if ((m_pActiveBoost != NULL) && (m_pActiveBoost->m_nClass == PROGRAM_STEALTH_BOOST))
		{
			nRating += 3;
		}
		break;
	case RATING_ANALYSIS:
		nRating = __min(m_nChip[CHIP_ANALYSIS], m_nChip[CHIP_CPU]) + m_nAnalysisSkill;
		if (m_pCurrentNode->m_nActiveSmokeLevel)
		{
			nRating-=4;
		}
		if ((m_pActiveBoost != NULL) && (m_pActiveBoost->m_nClass == PROGRAM_ANALYSIS_BOOST))
		{
			nRating += 3;
		}
		break;
	}

	// Modify by deck load
	if (m_nLoadStatus == LS_LIGHT)
		nRating += 1;
	else if (m_nLoadStatus == LS_HEAVY)
		nRating -= 2;

	// If running a client program, reduce all ratings
	if (m_nClientProgramStatus != 0)
	{
		nRating -= 3;
	}

	// Modify by deck condition - half normal
	nRating += (GetConditionModifier(m_nHealthDeck)/2);

	// Modify by physical condition (Use lower of stun/lethal)
	nRating += GetConditionModifier(__min(m_nHealthMental, m_nHealthPhysical));

	return nRating;
}

_inline int TotalPoints(int nSkill)
{
	return ((nSkill * (nSkill - 1)) / 2);
}

int CCharacter::GetEffectiveLevel()
{
	int nTotal;
	int nLevel;

	// Compute the total skill points earned
	nTotal = m_nSkillPoints;

	nTotal += TotalPoints(m_nAttackSkill);
	nTotal += TotalPoints(m_nDefenseSkill);
	nTotal += TotalPoints(m_nStealthSkill);
	nTotal += TotalPoints(m_nAnalysisSkill);
	nTotal += TotalPoints(m_nProgrammingSkill);
	nTotal += TotalPoints(m_nChipDesignSkill);

	// Divide the total by 6 to get average
	nTotal /= 6;

	// Now, compute the average skill level if we divided the skills evenly
	nLevel = 1;
	while (nTotal >= nLevel)
	{
		nTotal -= nLevel;
		nLevel++;
	}

	return nLevel;
}

// Returns TRUE if game over
BOOL CCharacter::PassTime(int nDays)
{
	CString szTxt;
	POSITION p;
	CContract *pContract;

	// Update the current game time
	m_nDayOfMonth+= nDays;
	while (m_nDayOfMonth >= GetDays(m_nMonth,m_nYear))
	{
		m_nDayOfMonth -= GetDays(m_nMonth,m_nYear);
		m_nMonth++;
		if (m_nMonth==12)
		{
			m_nMonth = 0;
			m_nYear++;
		}

		// Collect rent
		if (m_nCredits >= g_nLifestyleMonthlyCost[m_nLifestyle])
		{
			m_nCredits -= g_nLifestyleMonthlyCost[m_nLifestyle];

			szTxt.Format("Your monthly rent of %dcr has been collected.",g_nLifestyleMonthlyCost[m_nLifestyle]);
			DeckerMessageBox(szTxt);
		}
		else
		{
			while (m_nCredits < g_nLifestyleMonthlyCost[m_nLifestyle])
			{
				if (m_nLifestyle == 0)
				{
					DeckerMessageBox("You are broke. Some thugs come to 'reposess' all of your possessions, including your cyberdeck.\nGame over.");
					// FSO 7-25-01 Now go to intro prompt
					//PostQuitMessage(0);
					AfxGetMainWnd()->PostMessage(WM_NEW_GAME);

					return TRUE;
				}
				else
				{
					m_nLifestyle--;
				}
			}

			m_nCredits -= g_nLifestyleMonthlyCost[m_nLifestyle];
			szTxt.Format("You do not have enough money for rent. Your lifestyle has been down downgraded to %s.",
								GetLifestyleString());
			DeckerMessageBox(szTxt);
		}

		// Reduce system backdoor times
		for (int i = 0; i < NUM_CORP; i++)
		{
			if (m_bBackdoor[i] > 0)
			{
				m_bBackdoor[i]--;
			}
		}
	}

	// Update available contract times
	p = m_olContracts.GetHeadPosition();
	while (p != NULL)
	{
		POSITION p2 = p;
		pContract = (CContract *)m_olContracts.GetNext(p);
		if (pContract->m_nDaysLeft <= nDays)
		{
			// Contract expired. Delete it
			m_olContracts.RemoveAt(p2);
			delete pContract;

			// Possibly create a new one to replace it
			if (Random(2)==0)
			{
				pContract = new CContract;
				pContract->Generate();
				m_olContracts.InsertBefore(p, pContract);
			}
		}
		else
		{
			pContract->m_nDaysLeft -= nDays;

			// Increase pay by 10% per day
			pContract->m_nPay += (pContract->m_nPay * nDays / 10);
		}
	}

	// Update current contract deadline
	if (m_pCurrentContract != NULL)
	{
		if (m_pCurrentContract->m_nDaysLeft <= nDays)
		{
			// Contract failed
			DeckerMessageBox("The deadline for you contract has passed. Your reputation has suffered as a result.");

			m_nRepPoints -= m_pCurrentContract->m_nDifficulty;
			if (m_nRepPoints<0)
			{
				m_nRepPoints = 0;
			}

			if ((m_nRepLevel>0) &&
				(m_nRepPoints < CalcRepPointsForNextLevel(m_nRepLevel-1)))
			{
				m_nRepLevel--;
			}

			// Remove the contract
			ClearCurrentContract();
		}
		else
		{
			m_pCurrentContract->m_nDaysLeft -= nDays;
		}
	}

	// Update day of week
	m_nDayOfWeek += nDays;

	// If new week, generate new contracts and shop items
	if (m_nDayOfWeek>=7)
	{
		m_nDayOfWeek%=7;

		GenerateShopItems();
		GenerateContracts();
	
		DeckerMessageBox("New contracts and shop items available.");
	}

	// Update cooking chip
	if (m_pChipBurning != NULL)
	{
		m_nChipBurnTimeLeft -= (nDays * m_nHardware[HW_CHIP_BURNER]);
		if (m_nChipBurnTimeLeft <= 0)
		{
			// Chip burn complete
			szTxt.Format("Your new Rating %d %s chip is done. Do you wish to install it? (Old chip is rating %d)",
				m_pChipBurning->m_nRating, 
				GetChipName(m_pChipBurning->m_nClass),
				m_nChip[m_pChipBurning->m_nClass]);

			if (DeckerMessageBox(szTxt, MB_YESNO)==IDYES)
			{
				m_nChip[m_pChipBurning->m_nClass] = m_pChipBurning->m_nRating;
				DeckerMessageBox("Chip has been installed.");
			}
			else
			{
				DeckerMessageBox("Chip has been discarded.");
			}
			m_pChipBurning = NULL;
		}
	}

	// Update any shop orders
	if (m_nOrderItemType != -1)
	{
		m_nOrderTimeLeft -= nDays;

		if (m_nOrderTimeLeft<=0)
		{
			DeckerMessageBox("Your order has arrived.");

			if (m_nOrderItemType == IT_SOFTWARE)
			{
				// Create a new program
				CProgram *pProgram = new CProgram;
				pProgram->GetInitValues(m_nOrderItemClass, m_nOrderItemRating);

				m_olSoftware.AddTail(pProgram);
			}
			else if (m_nOrderItemType == IT_CHIP)
			{
				m_nChip[m_nOrderItemClass] = m_nOrderItemRating;
			}
			else //IT_HARDWARE
			{
				m_nHardware[m_nOrderItemClass] = m_nOrderItemRating;
			}

			// Clear the order
			m_nOrderItemType = -1;
		}
	}

	// Update system alerts
	for (int i = 0; i < NUM_CORP; i++)
	{
		if (m_bSystemAlerts[i] > nDays)
		{
			m_bSystemAlerts[i] -= nDays;
		}
		else
		{
			m_bSystemAlerts[i] = 0;
		}
	}

	return FALSE;
}

void CCharacter::GetLoadRatings(int &nLight, int &nHeavy, int &nMax)
{
	int nSum;

	// Get the square of the sum of the cpu and coprocessor
	nSum = m_nChip[CHIP_CPU] + min(m_nChip[CHIP_COPROCESSOR],m_nChip[CHIP_CPU]);

	// Set ratings
	nLight = 10*nSum;
	nHeavy = 20*nSum;
	nMax = 30*nSum;
}

BOOL CCharacter::SaveGame(CWnd *pWnd, BOOL bAskName)
{
	WORD wMajorVersion;
	WORD wMinorVersion;

	//---------------
	// Open the File
	//---------------
	// If the filename is empty, prompt for it
	if ((m_szFileName.IsEmpty()) || (bAskName))
	{
		CFileDialog dlgFile(FALSE,	// Open
							g_szExtension,	// Extension
							NULL,	// Default filename
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //Flags
							g_szFilter,		// Filter
							pWnd);	// Parent window
		if (dlgFile.DoModal()==IDCANCEL)
		{
			// User hit cancel
			return FALSE;
		}

		// Get the pathname
		m_szFileName = dlgFile.GetPathName();
	}

	// Open the file
	CFile File;

	if(!File.Open( m_szFileName, CFile::modeCreate | CFile::modeWrite ) ) 
	{
		DeckerMessageBox("Could not open the file for writing.");
		return FALSE;
	}
	CArchive ar( &File, CArchive::store);

	//-----------------------
	// Write the Information
	//-----------------------
	wMajorVersion = MAJOR_VERSION;
	wMinorVersion = MINOR_VERSION;
	ar << wMajorVersion;
	ar << wMinorVersion;
	Save(ar);

	return TRUE;
}

void CCharacter::Save(CArchive &ar)
{
	int i;
	POSITION p;
	WORD wCount;
	BYTE bThere;
	int nIndex;

	// Basic Attributes
	ar << m_szName;
	ar << m_bIronMan;
	ar << m_bTooltips;
	ar << m_nCredits;
	ar << m_nLifestyle;
	ar << m_nRepLevel;
	ar << m_nRepPoints;
	ar << m_nAttackSkill;
	ar << m_nDefenseSkill;
	ar << m_nStealthSkill;
	ar << m_nAnalysisSkill;
	ar << m_nProgrammingSkill;
	ar << m_nChipDesignSkill;
	ar << m_nSkillPoints;
	ar << m_nImage;
	ar.Write(m_bBackdoor,NUM_CORP);
	ar.Write(m_bSystemAlerts,NUM_CORP);

	// Chips
	for (i=0; i<NUM_CHIPS; i++)
	{
		ar << m_nChip[i];
	}

	// Hardware
	for (i=0; i<NUM_HW_RESERVED; i++)
	{
		ar << m_nHardware[i];
	}

	// Software
	wCount = (WORD)m_olSoftware.GetCount();
	ar << wCount;

	p = m_olSoftware.GetHeadPosition();
	while (p!=NULL)
	{
		CProgram *pProgram = (CProgram *)m_olSoftware.GetNext(p);

		// Write the program
		pProgram->Save(ar);
	}
	// No need to write current load or load status

	// Default Programs
	nIndex = FindListIndex(m_pDefAttackProgram, m_olSoftware);
	ar << nIndex;
	nIndex = FindListIndex(m_pDefArmorProgram, m_olSoftware);
	ar << nIndex;
	nIndex = FindListIndex(m_pDefShieldProgram, m_olSoftware);
	ar << nIndex;
	nIndex = FindListIndex(m_pDefHideProgram, m_olSoftware);
	ar << nIndex;
	nIndex = FindListIndex(m_pDefReflectProgram, m_olSoftware);
	ar << nIndex;

	// Contracts
	wCount = (WORD)m_olContracts.GetCount();
	ar << wCount;

	p = m_olContracts.GetHeadPosition();
	while (p!=NULL)
	{
		CContract *pContract = (CContract *)m_olContracts.GetNext(p);

		// Write the contract
		pContract->Save(ar);
	}

	if (m_pCurrentContract == NULL)
	{
		bThere = 0;
		ar << bThere;
	}
	else
	{
		bThere = 1;
		ar << bThere;
		m_pCurrentContract->Save(ar);
	}

	// Source Code
	wCount = (WORD)m_olSourceCode.GetCount();
	ar << wCount;

	p = m_olSourceCode.GetHeadPosition();
	while (p!=NULL)
	{
		CSource *pSource = (CSource *)m_olSourceCode.GetNext(p);

		// Write the source
		pSource->Save(ar);
	}
	
	// Current Project
	ar << m_nProjectType;
	ar << m_nProjectClass;
	ar << m_nProjectRating;
	ar << m_nProjectInitialTime;
	ar << m_nProjectTimeLeft;

	// Chip being burned
	nIndex = FindListIndex(m_pChipBurning, m_olSourceCode);
	ar << nIndex;
	ar << m_nChipBurnTimeLeft;

	// Shop items
	wCount = (WORD)m_olShopList.GetCount();
	ar << wCount;

	p = m_olShopList.GetHeadPosition();
	while (p!=NULL)
	{
		CShopItem *pItem = (CShopItem *)m_olShopList.GetNext(p);

		// Write the item
		pItem->Save(ar);
	}

	// Item on order
	ar << m_nOrderItemType;
	ar << m_nOrderItemClass;
	ar << m_nOrderItemRating;
	ar << m_nOrderTimeLeft;

	// Game State
	ar << m_nDayOfMonth;
	ar << m_nMonth;
	ar << m_nYear;
	ar << m_nDayOfWeek;

	ar << m_bOnRun;
	ar << m_nRunTime;

	ar << m_nHealthPhysical;
	ar << m_nHealthMental;
	ar << m_nHealthDeck;
	// Skip damage

	// System
	//FSO 7-24-01
	//if (m_pSystem == NULL)
	//{
	//	bThere = 0;
	//	ar << bThere;
	//}
	//else
	//{
	//	bThere = 1;
	//	ar << bThere;
	if (m_bOnRun)
	{
		m_pSystem->Save(ar);

		ar << m_dwRunFlags;

		// Current node
		nIndex = FindListIndex(m_pCurrentNode, m_pSystem->m_olNodeList);
		ar << nIndex;

		// Location in node
		ar << m_ptNodeLoc;
		// Skip screen location - can rebuild

		// Targetted Ice
		nIndex = FindListIndex(m_pTargettedIce, m_pSystem->m_olIceList);
		ar << nIndex;

		ar << m_nHighestDeceivedIce;

		// Skip current ice list - can rebuild

		// Active Programs
		nIndex = FindListIndex(m_pActiveArmor, m_olSoftware);
		ar << nIndex;
		nIndex = FindListIndex(m_pActiveShield, m_olSoftware);
		ar << nIndex;
		nIndex = FindListIndex(m_pActiveHide, m_olSoftware);
		ar << nIndex;
		nIndex = FindListIndex(m_pActiveScan, m_olSoftware);
		ar << nIndex;
		nIndex = FindListIndex(m_pActiveReflect, m_olSoftware);
		ar << nIndex;
		nIndex = FindListIndex(m_pActiveBoost, m_olSoftware);
		ar << nIndex;

		ar << m_nDecoyCount;
		for (i=0; i < m_nDecoyCount; i++)
		{
			ar << m_ptDecoyLocation[i];
		}

		ar << m_nClientProgramStatus;

		// Trace information
		nIndex = FindListIndex(m_pTraceIce, m_pSystem->m_olIceList);
		ar << nIndex;

		ar << m_nTraceInitialTime;
		ar << m_nTraceTurnsLeft;
		ar << m_nTraceType;
		ar << m_bTraced;

		// Transfer details
		nIndex = FindListIndex(m_pTransferProgram, m_olSoftware);
		ar << nIndex;

		nIndex = FindListIndex(m_pTransferFile, m_pCurrentNode->m_olFileList);
		ar << nIndex;

		ar << m_nTransferInitialTime;
		ar << m_nTransferTurnsLeft;
		ar << m_nFileTransferType;

		// Scan/Eval status
		ar << m_nCurrentScanMP;
		if ((m_pActiveScan == NULL) || (m_posScanFile == NULL))
		{
			nIndex = -1;
		}
		else
		{
			p = m_posScanFile;
			nIndex = 0;
			for (nIndex = 0;;nIndex++)
			{
				m_pCurrentNode->m_olFileList.GetPrev(p);
				if (p==NULL) break;
			}
		}
		ar << nIndex;
	}
}

BOOL CCharacter::LoadGame(CWnd *pWnd, const char *szFileName)
{
	BOOL bReturn;
	WORD wMajorVersion;
	WORD wMinorVersion;

	//---------------
	// Open the File
	//---------------
	if ((szFileName == NULL) || (*szFileName == NULL))
	{
		// Prompt for filename
		CFileDialog dlgFile(TRUE,			// Open
							g_szExtension,	// Extension
							NULL,			// Default filename
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, //Flags
							g_szFilter,		// Filter
							pWnd);			// Parent window
		if (dlgFile.DoModal()==IDCANCEL)
		{
			// User hit cancel
			return FALSE;
		}

		// Get the pathname
		m_szFileName = dlgFile.GetPathName();
	}
	else
	{
		m_szFileName = szFileName;
	}

	// Open the file
	CFile File;

	if(!File.Open( m_szFileName, CFile::modeRead ) ) 
	{
		DeckerMessageBox("Could not open the file for writing.");
		return FALSE;
	}
	CArchive ar( &File, CArchive::load);

	bReturn = TRUE;
	try
	{
		ar >> wMajorVersion;
		ar >> wMinorVersion;

		Load(ar, wMajorVersion, wMinorVersion);
	}
	catch( CArchiveException* e )
	{
		switch (e->m_cause)
		{
		case CArchiveException::endOfFile:
		case CArchiveException::badIndex:
			DeckerMessageBox("The file format is invalid.");
			break;
		//case CArchiveException::badClass:   Tried to read an object into an object of the wrong type.
		//case CArchiveException::badSchema:   Tried to read an object with a different version of the class. 
		//case CArchiveException::none:   No error occurred.
		//case CArchiveException::generic:
		//case CArchiveException::readOnly:	Tried to write into an archive opened for loading.
		//case CArchiveException::writeOnly:	Tried to read from an archive opened for storing.
		default:
				DeckerMessageBox("An unspecified error has occurred while reading the file.");
				break;
		}

		bReturn = FALSE;
	}

	return bReturn;
}

void CCharacter::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	int i;
	POSITION p;
	WORD wCount;
	BYTE bThere;
	int nIndex;

	// Basic Attributes
	ar >> m_szName;
	if ((wMajorVersion > 0) || (wMinorVersion >= 10))
	{
		ar >> m_bIronMan;
		ar >> m_bTooltips;
	}
	ar >> m_nCredits;
	ar >> m_nLifestyle;
	ar >> m_nRepLevel;
	ar >> m_nRepPoints;
	ar >> m_nAttackSkill;
	ar >> m_nDefenseSkill;
	ar >> m_nStealthSkill;
	ar >> m_nAnalysisSkill;
	ar >> m_nProgrammingSkill;
	ar >> m_nChipDesignSkill;
	ar >> m_nSkillPoints;
	ar >> m_nImage;
	// Make sure image is valid
	if (m_nImage >= g_nNumCharImages)
	{
		m_nImage = 0;
	}
	ar.Read(m_bBackdoor,NUM_CORP);
	if ((wMajorVersion != 0) || (wMinorVersion >= 6))
	{
		ar.Read(m_bSystemAlerts,NUM_CORP);
	}

	// Chips
	for (i=0; i<NUM_CHIPS; i++)
	{
		ar >> m_nChip[i];
	}

	// Hardware
	if ((wMajorVersion == 0) && (wMinorVersion == 1))
	{
		// Handle old version
		for (i=0; i<NUM_HW_0_1; i++)
		{
			ar >> m_nHardware[i];
		}
		while (i<NUM_HW_RESERVED)
		{
			m_nHardware[i++] = 0;
		}
	}
	else
	{
		for (i=0; i<NUM_HW_RESERVED; i++)
		{
			ar >> m_nHardware[i];
		}
	}

	// Software
	ar >> wCount;

	while (wCount--)
	{
		CProgram *pProgram = new CProgram();
		pProgram->Load(ar, wMajorVersion, wMinorVersion);
		m_olSoftware.AddTail(pProgram);
	}

	// Default Programs
	ar >> nIndex;
	m_pDefAttackProgram = (CProgram *)FindListObject(nIndex, m_olSoftware);
	ar >> nIndex;
	m_pDefArmorProgram = (CProgram *)FindListObject(nIndex, m_olSoftware);
	ar >> nIndex;
	m_pDefShieldProgram = (CProgram *)FindListObject(nIndex, m_olSoftware);
	ar >> nIndex;
	m_pDefHideProgram = (CProgram *)FindListObject(nIndex, m_olSoftware);
	if ((wMajorVersion == 0) && (wMinorVersion < 10))
	{
		m_pDefReflectProgram = NULL;
	}
	else
	{
		ar >> nIndex;
		m_pDefReflectProgram = (CProgram *)FindListObject(nIndex, m_olSoftware);
	}

	// Contracts
	ar >> wCount;

	while (wCount--)
	{
		CContract *pContract = new CContract();
		pContract->Load(ar, wMajorVersion, wMinorVersion);
		m_olContracts.AddTail(pContract);
	}

	ar >> bThere;
	if (bThere == 0)
	{
		m_pCurrentContract = NULL;
	}
	else
	{
		m_pCurrentContract = new CContract();
		m_pCurrentContract->Load(ar, wMajorVersion, wMinorVersion);
	}

	// Source Code
	ar >> wCount;

	while (wCount--)
	{
		CSource *pSource = new CSource();
		pSource->Load(ar, wMajorVersion, wMinorVersion);
		m_olSourceCode.AddTail(pSource);
	}

	// Current Project
	ar >> m_nProjectType;
	ar >> m_nProjectClass;
	ar >> m_nProjectRating;
	ar >> m_nProjectInitialTime;
	ar >> m_nProjectTimeLeft;

	// Chip being burned
	ar >> nIndex;
	m_pChipBurning = (CSource *)FindListObject(nIndex, m_olSourceCode);
	ar >> m_nChipBurnTimeLeft;

	// Shop items
	ar >> wCount;

	while (wCount--)
	{
		CShopItem *pItem = new CShopItem();
		pItem->Load(ar, wMajorVersion, wMinorVersion);
		m_olShopList.AddTail(pItem);
	}

	// Item on order
	ar >> m_nOrderItemType;
	ar >> m_nOrderItemClass;
	ar >> m_nOrderItemRating;
	ar >> m_nOrderTimeLeft;

	// Game State
	ar >> m_nDayOfMonth;
	ar >> m_nMonth;
	ar >> m_nYear;
	ar >> m_nDayOfWeek;

	ar >> m_bOnRun;
	ar >> m_nRunTime;

	ar >> m_nHealthPhysical;
	ar >> m_nHealthMental;
	ar >> m_nHealthDeck;
	// Skip damage

	// Check version - prior to 0.6, System was created at contract creation.
	//				On version 0.6 and later, only create when on run.
	// System
	if ((wMajorVersion == 0) && (wMinorVersion < 6))
	{
		ar >> bThere;
	}
	else
	{
		bThere = m_bOnRun;
	}

	if (bThere == NULL)
	{
		m_pSystem = NULL;
	}
	else
	{

		m_pSystem = new CSystem();
		m_pSystem->Load(ar, wMajorVersion, wMinorVersion);

		if (VersionCompare(wMajorVersion,wMinorVersion,1,6) >= 0)
		{
			ar >> m_dwRunFlags;
		}
		else
		{
			if (m_pSystem->m_nAlert == ALERT_RED)
			{
				// Set flag that alert was set
				m_dwRunFlags |= CRF_ALARMS_SET;
			}
		}

		// Current area/node
		ar >> nIndex;
		m_pCurrentNode = (CNode *)FindListObject(nIndex, m_pSystem->m_olNodeList);

		// Location in node
		ar >> m_ptNodeLoc;
		// Skip screen location - can rebuild

		// Targetted Ice
		ar >> nIndex;
		m_pTargettedIce = (CIce *)FindListObject(nIndex, m_pSystem->m_olIceList);

		// Rebuild current ice list
		p = m_pSystem->m_olIceList.GetHeadPosition();
		while (p!=NULL)
		{
			CIce *pIce = (CIce *)m_pSystem->m_olIceList.GetNext(p);

			if (pIce->m_pCurrentNode == m_pCurrentNode)
			{
				m_olCurrentIceList.AddTail(pIce);
			}
		}

		ar >> m_nHighestDeceivedIce;

		// Active Programs
		ar >> nIndex;
		m_pActiveArmor = (CProgram *)FindListObject(nIndex, m_olSoftware);
		ar >> nIndex;
		m_pActiveShield = (CProgram *)FindListObject(nIndex, m_olSoftware);
		ar >> nIndex;
		m_pActiveHide = (CProgram *)FindListObject(nIndex, m_olSoftware);
		ar >> nIndex;
		m_pActiveScan = (CProgram *)FindListObject(nIndex, m_olSoftware);
		if ((wMajorVersion == 0) && (wMinorVersion < 10))
		{
			m_pActiveReflect = NULL;
			m_pActiveBoost = NULL;
		}
		else
		{
			ar >> nIndex;
			m_pActiveReflect = (CProgram *)FindListObject(nIndex, m_olSoftware);
			ar >> nIndex;
			m_pActiveBoost = (CProgram *)FindListObject(nIndex, m_olSoftware);
		}

		ar >> m_nDecoyCount;
		for (i=0; i < m_nDecoyCount; i++)
		{
			ar >> m_ptDecoyLocation[i];
		}

		if ((wMajorVersion == 0) || ((wMajorVersion == 1) && (wMinorVersion < 10)))
		{
			m_nClientProgramStatus = 0;
		}
		else
		{
			ar >> m_nClientProgramStatus;
		}

		// Trace information
		ar >> nIndex;
		m_pTraceIce = (CIce *)FindListObject(nIndex, m_pSystem->m_olIceList);

		ar >> m_nTraceInitialTime;
		ar >> m_nTraceTurnsLeft;
		ar >> m_nTraceType;
		ar >> m_bTraced;

		// Transfer details
		ar >> nIndex;
		m_pTransferProgram = (CProgram *)FindListObject(nIndex, m_olSoftware);

		ar >> nIndex;
		m_pTransferFile = (CDSFile *)FindListObject(nIndex, m_pCurrentNode->m_olFileList);

		ar >> m_nTransferInitialTime;
		ar >> m_nTransferTurnsLeft;
		ar >> m_nFileTransferType;

		// Scan/Eval status
		ar >> m_nCurrentScanMP;
		ar >> nIndex;
		if (nIndex!=-1)
		{
			m_posScanFile = m_pCurrentNode->m_olFileList.FindIndex(nIndex);
		}

		// FSO 7-24-01 Fix for previous versions
		if ((wMajorVersion == 0) && (wMinorVersion < 6) && (!m_bOnRun))
		{
			// Delete the current system
			delete m_pSystem;
			m_pSystem = NULL;
		}
	}

	// Calculate the current load
	CalcCurrentLoad();
}

int CCharacter::GetCurrentItemLevel(int nType, int nSubType)
{
	POSITION p;
	CProgram *pProgram;
	int nMax;

	if (nType == IT_SOFTWARE)
	{
		// Look for the highest rated program of this type
		nMax = 0;
		p = m_olSoftware.GetHeadPosition();
		while (p != NULL)
		{
			pProgram = (CProgram *)m_olSoftware.GetNext(p);

			if ((pProgram->m_nClass == nSubType) &&
				(pProgram->m_nRating > nMax))
			{
				nMax = pProgram->m_nRating;
			}
		}

		return nMax;
	}
	else if (nType == IT_HARDWARE)
	{
		return m_nHardware[nSubType];
	}
	else //if (nType == IT_CHIP)
	{
		return m_nChip[nSubType];
	}
}

// Global function
void GetHardwareString(CString &szStr, int nHardware, int nRating)
{
	switch (nHardware)
	{
	case HW_CHIP_BURNER	:
		szStr = "Chip Burner";
		if (nRating==2)
			szStr += " (Double Speed)";
		else if (nRating==3)
			szStr += " (Triple Speed)";
		else if (nRating==4)
			szStr += " (Quad Speed)";
		break;
	case HW_SURGE_SUPP	:
		szStr.Format("Surge Suppressor Level %d", nRating);
		break;
	case HW_NEURAL_DAMPER:
		szStr.Format("Neural Damper Level %d", nRating);
		break;
	case HW_TRACE_MONITOR:
		szStr.Format("Trace Monitor Level %d", nRating);
		break;
	case HW_BIO_MONITOR	:
		szStr = "Bio Monitor";
		if (nRating == 2)
		{
			szStr += " w/ AutoDump";
		}
		break;
	case HW_HIGH_BW_BUS	:
		szStr.Format("High Bandwidth Bus Level %d", nRating);
		break;
	case HW_MAPPER	:
		szStr = "Proximity Mapper";
		break;
	case HW_DESIGN_ASSIST:
		szStr.Format("Design Assistant Level %d", nRating);
		break;
	case HW_PROXY:
		szStr = "AntiTrace Proxy";
		break;
	}
}

const char * GetHardwareName(int nType)
{
	return g_szHardwareName[nType];
}

int GetChipComplexity(int nClass)
{
	int nComplexity;

	switch (nClass)
	{
	case CHIP_CPU:
	case CHIP_COPROCESSOR:
		nComplexity = 5;
		break;
	case CHIP_ATTACK:
	case CHIP_DEFENSE:
	case CHIP_STEALTH:
	case CHIP_ANALYSIS:
		nComplexity = 4;
		break;
	}

	return nComplexity;
}