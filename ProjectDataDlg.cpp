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

// ProjectDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "ProjectDataDlg.h"
#include "Character.h"
#include "Program.h"
#include "Source.h"
#include "NewProjectDlg.h"
#include "Global.h"
#include "BuildDialog.h"

// The character data
extern CCharacter *g_pChar;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectDataDlg dialog


CProjectDataDlg::CProjectDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProjectDataDlg)
	m_date = _T("");
	m_time = _T("");
	m_type = _T("");
	m_chip_time = _T("");
	m_chip_type = _T("");
	//}}AFX_DATA_INIT

	m_ilItems.Create(IDB_ITEM_IL,16,0,WHITE);
}


void CProjectDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectDataDlg)
	DDX_Control(pDX, IDC_PROJ_TRASH, m_Trash);
	DDX_Control(pDX, IDC_PROJ_WORK_WEEK, m_Work_Week);
	DDX_Control(pDX, IDC_PROJ_WORK_DAY, m_Work_Day);
	DDX_Control(pDX, IDC_PROJ_SOURCE, m_Source);
	DDX_Control(pDX, IDC_PROJ_NEW, m_New);
	DDX_Control(pDX, IDC_PROJ_FINISH, m_Finish);
	DDX_Control(pDX, IDC_PROJ_BUILD, m_Build);
	DDX_Text(pDX, IDC_PROJ_DATE, m_date);
	DDX_Text(pDX, IDC_PROJ_TIME, m_time);
	DDX_Text(pDX, IDC_PROJ_TYPE, m_type);
	DDX_Text(pDX, IDC_PROJ_CHIP_TIME, m_chip_time);
	DDX_Text(pDX, IDC_PROJ_CHIP_TYPE, m_chip_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectDataDlg, CDialog)
	//{{AFX_MSG_MAP(CProjectDataDlg)
	ON_BN_CLICKED(IDC_PROJ_BUILD, OnProjBuild)
	ON_BN_CLICKED(IDC_PROJ_FINISH, OnProjFinish)
	ON_BN_CLICKED(IDC_PROJ_NEW, OnProjNew)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROJ_SOURCE, OnItemchangedProjSource)
	ON_BN_CLICKED(IDC_PROJ_WORK_DAY, OnProjWorkDay)
	ON_BN_CLICKED(IDC_PROJ_WORK_WEEK, OnProjWorkWeek)
	ON_BN_CLICKED(IDC_PROJ_TRASH, OnProjTrash)
	ON_WM_HELPINFO()
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PROJ_SOURCE, OnColumnclickProjSource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectDataDlg message handlers

BOOL CProjectDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//------------------------------
	// Fill in the chip information
	//------------------------------
	UpdateChip();
	
	//---------------------------------
	// Fill in the project information
	//---------------------------------
	UpdateProject();

	//--------------------------------
	// Fill in the source information
	//---------------------------------
	POSITION p;
	CSource *pSource;
	int i = 0;
	CString szTxt;

	// Set the image list
	m_Source.SetImageList(&m_ilItems,LVSIL_SMALL   );

	// Create the column headers
	m_Source.InsertColumn(0,"Type",		LVCFMT_LEFT,150);
	m_Source.InsertColumn(1,"Rating",	LVCFMT_LEFT,50);
	m_Source.InsertColumn(2,"Current",	LVCFMT_LEFT,50);

	p = g_pChar->m_olSourceCode.GetHeadPosition();
	while (p!=NULL)
	{
		pSource = (CSource *)g_pChar->m_olSourceCode.GetNext(p);

		// Type
		if (pSource->m_nType == IT_SOFTWARE)
		{
			szTxt.Format("%s", GetProgramClassName(pSource->m_nClass));
		}
		else
		{
			szTxt.Format("%s", GetChipName(pSource->m_nClass));
		}

		m_Source.InsertItem(i, szTxt, pSource->m_nType);

		// Rating
		szTxt.Format("%d", pSource->m_nRating);
		m_Source.SetItemText(i, 1, szTxt);

		// Current
		if (pSource->m_nType == IT_SOFTWARE)
		{
			CProgram *pProgram;
			pProgram = g_pChar->FindProgram(pSource->m_nClass, FALSE);
			if (pProgram == NULL)
			{
				szTxt = '-';
			}
			else
			{
				szTxt.Format("%d", pProgram->m_nRating);
			}
		}
		else
		{
			szTxt.Format("%d", g_pChar->m_nChip[pSource->m_nClass]);
		}
		m_Source.SetItemText(i, 2, szTxt);

		// Save the data
		m_Source.SetItemData(i, (DWORD)pSource);

		i++;
	}

	m_Build.EnableWindow(FALSE);
	m_Trash.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProjectDataDlg::UpdateProject()
{
	// Fill in the date
	m_date.Format("%s %d, %d",
					g_szMonthNames[g_pChar->m_nMonth],
					g_pChar->m_nDayOfMonth+1,
					g_pChar->m_nYear);

	// Fill in the project information
	if (g_pChar->m_nProjectType == -1)
	{
		m_type = "None";
		m_time.Empty();
		m_Finish.EnableWindow(FALSE);
		m_Work_Week.EnableWindow(FALSE);
		m_Work_Day.EnableWindow(FALSE);
	}
	else
	{
		if (g_pChar->m_nProjectType == IT_SOFTWARE)
		{
			m_type.Format("%s - %d", GetProgramClassName(g_pChar->m_nProjectClass), g_pChar->m_nProjectRating);
			m_nSkill = g_pChar->m_nProgrammingSkill;
		}
		else
		{
			m_type.Format("%s - %d", GetChipName(g_pChar->m_nProjectClass), g_pChar->m_nProjectRating);
			m_nSkill = g_pChar->m_nChipDesignSkill;
		}
		int nSkill = m_nSkill * (1 + g_pChar->m_nHardware[HW_DESIGN_ASSIST]);
		m_time.Format("%d", ((g_pChar->m_nProjectTimeLeft + nSkill - 1) / nSkill));
		m_Finish.EnableWindow(TRUE);
		m_Work_Week.EnableWindow(TRUE);
		m_Work_Day.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

void CProjectDataDlg::UpdateChip(BOOL bUpdateSource)
{
	if (g_pChar->m_pChipBurning == NULL)
	{
		m_chip_type = "None";
		m_chip_time.Empty();

		// If we have passed time and finished a chip, update source
		if (bUpdateSource)
		{
			int nIndex;
			int nCount;
			CSource *pSource;
			CString szTmp;

			nCount = m_Source.GetItemCount();
			for (nIndex = 0; nIndex < nCount; nIndex++)
			{
				pSource = (CSource *)m_Source.GetItemData(nIndex);
				if (pSource->m_nType == IT_CHIP)
				{
					szTmp.Format("%d", g_pChar->m_nChip[pSource->m_nClass]);
					m_Source.SetItemText(nIndex,2,szTmp);
				}
			}
		}
	}
	else
	{
		m_chip_type.Format("%s Level %d", GetChipName(g_pChar->m_pChipBurning->m_nClass), g_pChar->m_pChipBurning->m_nRating);
		m_chip_time.Format("%d", (g_pChar->m_nChipBurnTimeLeft + g_pChar->m_nHardware[HW_CHIP_BURNER] - 1)/g_pChar->m_nHardware[HW_CHIP_BURNER]);
	}
}

void CProjectDataDlg::OnItemchangedProjSource(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex = m_Source.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	m_Build.EnableWindow(nIndex != -1);
	m_Trash.EnableWindow(nIndex != -1);
	
	*pResult = 0;
}

void CProjectDataDlg::OnProjWorkDay() 
{
	if (g_pChar->PassTime(1))
	{
		SendMessage(WM_CLOSE);
		return;
	}

	int nSkill = m_nSkill * (1 + g_pChar->m_nHardware[HW_DESIGN_ASSIST]);

	g_pChar->m_nProjectTimeLeft -= nSkill;

	if (g_pChar->m_nProjectTimeLeft <= 0)
	{
		DoCompleteProject();
	}
	UpdateChip(TRUE);
	UpdateProject();
}

void CProjectDataDlg::OnProjWorkWeek() 
{
	int nTime;
	int nDays;
	int nSkill = m_nSkill * (1 + g_pChar->m_nHardware[HW_DESIGN_ASSIST]);

	// Calculate days left
	nDays = ((g_pChar->m_nProjectTimeLeft + nSkill - 1) / nSkill);

	// Calculate time until end of week or end of project
	nTime = (7 - g_pChar->m_nDayOfWeek);
	if (nTime > nDays)
	{
		nTime = nDays;
	}
	if (g_pChar->PassTime(nTime))
	{
		SendMessage(WM_CLOSE);
		return;
	}

	g_pChar->m_nProjectTimeLeft -= (nTime * nSkill);

	if (g_pChar->m_nProjectTimeLeft <= 0)
	{
		DoCompleteProject();
	}
	UpdateChip(TRUE);
	UpdateProject();
}

void CProjectDataDlg::OnProjFinish() 
{
	int nDays;
	int nSkill = m_nSkill * (1 + g_pChar->m_nHardware[HW_DESIGN_ASSIST]);

	// Calculate days left
	nDays = ((g_pChar->m_nProjectTimeLeft + nSkill - 1) / nSkill);

	if (g_pChar->PassTime(nDays))
	{
		SendMessage(WM_CLOSE);
		return;
	}

	g_pChar->m_nProjectTimeLeft = 0;

	DoCompleteProject();

	UpdateChip(TRUE);
	UpdateProject();
}

void CProjectDataDlg::OnProjNew() 
{
	CNewProjectDlg dlgNew;

	if (dlgNew.DoModal() == IDOK)
	{
		UpdateProject();
	}
}

void CProjectDataDlg::DoCompleteProject()
{
	int nTarget;
	CString szTxt;
	int nIndex;

	// Make a roll to see if the project was successful
	nTarget = 10 + g_pChar->m_nProjectRating - (m_nSkill + g_pChar->m_nHardware[HW_DESIGN_ASSIST]);

	if (DoDieRoll(nTarget)>0)
	{
		// Project is complete
		DeckerMessageBox("Your project is now complete.");

		// Create a new source 
		CSource *pSource = new CSource;
		pSource->m_nType = g_pChar->m_nProjectType;
		pSource->m_nClass = g_pChar->m_nProjectClass;
		pSource->m_nRating = g_pChar->m_nProjectRating;

		// Add it to the list
		g_pChar->m_olSourceCode.AddTail(pSource);

		// Type
		if (pSource->m_nType == IT_SOFTWARE)
		{
			szTxt.Format("%s", GetProgramClassName(pSource->m_nClass));
		}
		else
		{
			szTxt.Format("%s", GetChipName(pSource->m_nClass));
		}
		nIndex = m_Source.InsertItem(m_Source.GetItemCount(), szTxt, pSource->m_nType);

		// Rating
		szTxt.Format("%d", pSource->m_nRating);
		m_Source.SetItemText(nIndex, 1, szTxt);

		// Current
		if (pSource->m_nType == IT_SOFTWARE)
		{
			CProgram *pProgram;
			pProgram = g_pChar->FindProgram(pSource->m_nClass, FALSE);
			if (pProgram == NULL)
			{
				szTxt = '-';
			}
			else
			{
				szTxt.Format("%d", pProgram->m_nRating);
			}
		}
		else
		{
			szTxt.Format("%d", g_pChar->m_nChip[pSource->m_nClass]);
		}
		m_Source.SetItemText(nIndex, 2, szTxt);

		m_Source.SetItemData(nIndex, (DWORD)pSource);

		// Clear the project
		g_pChar->m_nProjectType = -1;
	}
	else
	{
		// Design flaw
		DeckerMessageBox("You have discovered a flaw in your code. Additional time will be required.");

		g_pChar->m_nProjectInitialTime = ((g_pChar->m_nProjectInitialTime+3)/4);
		g_pChar->m_nProjectTimeLeft = g_pChar->m_nProjectInitialTime;
	}
}

void CProjectDataDlg::OnProjBuild() 
{
	int nIndex;
	CSource *pSource;
	CString szTmp;

	// Get the selected source
	nIndex = m_Source.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex==-1) return;

	pSource = (CSource *)m_Source.GetItemData(nIndex);

	// Process depending on program/chip
	if (pSource->m_nType == IT_SOFTWARE)
	{
		CBuildDialog dlgBuild;
		int nLightLoad, nHeavyLoad, nMaxLoad;

		g_pChar->GetLoadRatings(nLightLoad, nHeavyLoad, nMaxLoad);

		// Make sure we can load this program without overloading the deck
		if ((g_pChar->m_nCurrentLoad + GetProgramSize(pSource->m_nClass, pSource->m_nRating)) > nMaxLoad)
		{
			dlgBuild.m_bAllowLoad = FALSE;
		}

		// Prompt for the name
		dlgBuild.m_text.Format("%s %d", GetProgramClassName(pSource->m_nClass), pSource->m_nRating);
		if (dlgBuild.DoModal()==IDOK)
		{
			// Create a new program and add it to our list
			CProgram *pProgram = new CProgram;
			CProgram *pTmpProgram;
			pProgram->m_nClass = pSource->m_nClass;
			pProgram->m_nRating = pSource->m_nRating;
			pProgram->m_bLoadByDefault = dlgBuild.m_load;
			pProgram->m_szName = dlgBuild.m_text;
			pProgram->m_nSound = pProgram->m_nClass;

			// Add it to the list - sorted
			POSITION p = g_pChar->m_olSoftware.GetHeadPosition();
			for (;;)
			{
				if (p==NULL)
				{
					// Just add it to the tail
					g_pChar->m_olSoftware.AddTail(pProgram);
					break;
				}
				else
				{
					// Get the program at this position
					pTmpProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);

					if (pTmpProgram->m_nClass == pProgram->m_nClass)
					{
						// Add it here
						g_pChar->m_olSoftware.InsertBefore(p, pProgram);

						// Do something with the old ones?
						if (dlgBuild.m_trash)
						{
							// Trash all programs 
							while (p!=NULL)
							{
								// Get the program at this point
								pTmpProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);
								if ((pTmpProgram->m_nClass == pProgram->m_nClass) &&
									(pTmpProgram->m_nRating <= pProgram->m_nRating))
								{
									// Replace any defaults
									if (pTmpProgram == g_pChar->m_pDefArmorProgram)
									{
										g_pChar->m_pDefArmorProgram = pProgram;
									}
									else if (pTmpProgram == g_pChar->m_pDefAttackProgram)
									{
										g_pChar->m_pDefAttackProgram = pProgram;
									}
									else if (pTmpProgram == g_pChar->m_pDefHideProgram)
									{
										g_pChar->m_pDefHideProgram = pProgram;
									}
									else if (pTmpProgram == g_pChar->m_pDefShieldProgram)
									{
										g_pChar->m_pDefShieldProgram = pProgram;
									}
									else if (pTmpProgram == g_pChar->m_pDefReflectProgram)
									{
										g_pChar->m_pDefReflectProgram = pProgram;
									}

									// Delete the program
									POSITION p2 = p;
									g_pChar->m_olSoftware.GetNext(p);

									delete pTmpProgram;
									g_pChar->m_olSoftware.RemoveAt(p2);
								}
								else
								{
									g_pChar->m_olSoftware.GetNext(p);
								}
							}
						}
						else if (dlgBuild.m_unload)
						{
							// Unload all programs
							while (p!=NULL)
							{
								if ((pTmpProgram->m_nClass == pProgram->m_nClass) &&
									(pTmpProgram->m_nRating <= pProgram->m_nRating))
								{
									// Replace any defaults
									if (pTmpProgram == g_pChar->m_pDefArmorProgram)
									{
										g_pChar->m_pDefArmorProgram = pProgram;
									}
									else if (pTmpProgram == g_pChar->m_pDefAttackProgram)
									{
										g_pChar->m_pDefAttackProgram = pProgram;
									}
									else if (pTmpProgram == g_pChar->m_pDefHideProgram)
									{
										g_pChar->m_pDefHideProgram = pProgram;
									}
									else if (pTmpProgram == g_pChar->m_pDefShieldProgram)
									{
										g_pChar->m_pDefShieldProgram = pProgram;
									}
									else if (pTmpProgram == g_pChar->m_pDefReflectProgram)
									{
										g_pChar->m_pDefReflectProgram = pProgram;
									}

									// Unload the program
									pTmpProgram->m_bLoadByDefault = FALSE;
								}

								pTmpProgram = (CProgram *)g_pChar->m_olSoftware.GetNext(p);
							}
						}
						break;
					}

					// Go to the next item
					g_pChar->m_olSoftware.GetNext(p);
				}
			}
			
			DeckerMessageBox("Program has been compiled and is now available for use.");

			// Recalculate load rating
			g_pChar->CalcCurrentLoad();

			// Update the current ratings
			pTmpProgram = g_pChar->FindProgram(pProgram->m_nClass, FALSE);
			if (pTmpProgram == pProgram)
			{
				// Update all items of same type
				int nTmpIndex;
				int nCount;

				szTmp.Format("%d", pProgram->m_nRating);

				nCount = m_Source.GetItemCount();
				for (nTmpIndex = 0; nTmpIndex < nCount; nTmpIndex++)
				{
					CSource *pSrc;
					pSrc = (CSource *)m_Source.GetItemData(nTmpIndex);
					if ((pSrc->m_nType == IT_SOFTWARE) && (pSrc->m_nClass == pProgram->m_nClass))
					{
						m_Source.SetItemText(nTmpIndex,2,szTmp);
					}
				}
			}

		}
	}
	else
	{
		// Make sure we have a chip burner
		if (g_pChar->m_nHardware[HW_CHIP_BURNER] == 0)
		{
			DeckerMessageBox("You do not own a chip burner.");
		}
		else if (g_pChar->m_nChip[pSource->m_nClass] >= pSource->m_nRating)
		{
			DeckerMessageBox("This chip is lower than what is already installed.");
		}
		else
		{
			// Is there a chip already burning?
			if (g_pChar->m_pChipBurning != NULL)
			{
				if (DeckerMessageBox("You already have a chip in production. Do you wish to cancel it?", MB_YESNO)==IDNO)
				{
					return;
				}
			}

			// Burn the chip
			g_pChar->m_pChipBurning = pSource;
			g_pChar->m_nChipBurnTimeLeft = GetChipComplexity(pSource->m_nClass) * pSource->m_nRating;
			UpdateChip();
			UpdateData(FALSE);

			DeckerMessageBox("The chip is now cooking.");
		}
	}
}


void CProjectDataDlg::OnProjTrash() 
{
	int nIndex;
	POSITION p;
	CSource *pSource;

	// Get the selected source
	nIndex = m_Source.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex==-1) return;
	p = g_pChar->m_olSourceCode.FindIndex(nIndex);
	pSource = (CSource *)g_pChar->m_olSourceCode.GetAt(p);

	// See if this is the chip being burnt
	if (pSource == g_pChar->m_pChipBurning)
	{
		DeckerMessageBox("That source is currently being used to burn a chip.");
		return;
	}

	// Ask for confirmation
	if (DeckerMessageBox("Are you sure you want to erase this source code?", MB_YESNO)==IDYES)
	{
		// Delete the item
		m_Source.DeleteItem(nIndex);
		g_pChar->m_olSourceCode.RemoveAt(p);
		delete pSource;
	}
}

BOOL CProjectDataDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_PROGRAM);
	return TRUE;
}

static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSource *pSource1;
	CSource *pSource2;

	pSource1 = (CSource *)lParam1;
	pSource2 = (CSource *)lParam2;

	switch (lParamSort)
	{
	case 0:	// Type & Class
		if (pSource1->m_nType != pSource2->m_nType)
		{
			return pSource1->m_nType - pSource2->m_nType;
		}

		if (pSource1->m_nClass != pSource2->m_nClass)
		{
			return pSource1->m_nClass - pSource2->m_nClass;
		}
		// Fall through
	case 1:	// Rating
		return pSource1->m_nRating - pSource2->m_nRating;
	case 2:	// Current
		// Ignore for now
		//return pSource1->m_nRating - pSource2->m_nRating;
		return 0;
	}

	return 0;
}

void CProjectDataDlg::OnColumnclickProjSource(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;
	
	// No sort on current
	if (pNMListView->iSubItem == 2)
	{
		return;
	}

	// Sort the list
	m_Source.SortItems(SortFunc, pNMListView->iSubItem);

	// Now rebuild the source list
	g_pChar->m_olSourceCode.RemoveAll();

	int nIndex;
	int nCount = m_Source.GetItemCount();
	CSource *pSource;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		pSource = (CSource *)m_Source.GetItemData(nIndex);
		g_pChar->m_olSourceCode.AddTail(pSource);
	}
}
