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

// ContractListDialog.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "ContractListDialog.h"
#include "Character.h"
#include "Contract.h"
#include "ContractDataDialog.h"
#include "System.h"
#include "Program.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// CContractListDialog dialog


CContractListDialog::CContractListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CContractListDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContractListDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CContractListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContractListDialog)
	DDX_Control(pDX, IDC_VC_VIEW, m_View);
	DDX_Control(pDX, IDC_VC_CONTRACT_LIST, m_Contract_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContractListDialog, CDialog)
	//{{AFX_MSG_MAP(CContractListDialog)
	ON_BN_CLICKED(IDC_VC_VIEW, OnVcView)
	ON_NOTIFY(NM_DBLCLK, IDC_VC_CONTRACT_LIST, OnDblclkVcContractList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_VC_CONTRACT_LIST, OnItemchangedVcContractList)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContractListDialog message handlers

BOOL CContractListDialog::OnInitDialog() 
{
	CString szTmp;

	CDialog::OnInitDialog();
	
	m_View.EnableWindow(FALSE);

	// Initialize the columns	
	m_Contract_List.InsertColumn(0,"Type",		LVCFMT_LEFT,120);
	m_Contract_List.InsertColumn(1,"Difficulty",LVCFMT_LEFT,60);
	m_Contract_List.InsertColumn(2,"Target",	LVCFMT_LEFT,160);
	m_Contract_List.InsertColumn(3,"Deadline",	LVCFMT_LEFT,60);
	m_Contract_List.InsertColumn(4,"Pay",		LVCFMT_LEFT,60);

	POSITION p;
	int i = 0;
	CContract *pContract;
	p = g_pChar->m_olContracts.GetHeadPosition();
	while (p!=NULL)
	{
		pContract = (CContract *)g_pChar->m_olContracts.GetNext(p);

		// Type
		pContract->GetTypeText(szTmp);
		m_Contract_List.InsertItem(i,szTmp);

		// Difficulty
		szTmp.Format("%d%%", pContract->m_nDifficulty*5);
		m_Contract_List.SetItemText(i,1,szTmp);

		// Target
		szTmp.Format("%s (%d)", pContract->m_szSystemName, pContract->m_nSystemSize);
		m_Contract_List.SetItemText(i,2, szTmp);

		// Deadline
		szTmp.Format("%d days", pContract->m_nDaysLeft);
		m_Contract_List.SetItemText(i,3, szTmp);

		// Pay
		szTmp.Format("%dcr",pContract->m_nPay);
		m_Contract_List.SetItemText(i,4,szTmp);

		i++;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContractListDialog::OnVcView() 
{
	int nIndex;
	POSITION p;
	CContract *pContract;
	CString szTmp;

	// Find out what is selected
    nIndex=m_Contract_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
    if (nIndex==-1)
	{
		return;
	}

	// Get a pointer to this contract
	p = g_pChar->m_olContracts.FindIndex(nIndex);
	pContract = (CContract *)g_pChar->m_olContracts.GetAt(p);

	// Show this contract
	CContractDataDialog dlgContract;
	dlgContract.m_pContract = pContract;

	if (dlgContract.DoModal()==IDOK)
	{
		// User has accepted contract
		g_pChar->m_pCurrentContract = pContract;

		g_pChar->m_olContracts.RemoveAt(p);

		// Keep the contracts until they regenerate at the end of the week
		//g_pChar->ClearContracts();

// FSO 7-24-01 Don't generate until enter matrix
//		// Generate the system for this contract
//		g_pChar->m_pSystem = new CSystem();
//		g_pChar->m_pSystem->Generate(pContract->m_nSystemSize,0);

		// If this is a Run Program contract, add the program to the user's deck
		if (pContract->m_nType == CONT_RUN_PROGRAM)
		{
			// Add the program to run
			CProgram *pProgram = new CProgram();
			pProgram->GetInitValues(PROGRAM_CLIENT, (pContract->m_nCorporation+4)/5);

			g_pChar->m_olSoftware.AddTail(pProgram);
		}

		OnOK();
	}
}

void CContractListDialog::OnDblclkVcContractList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnVcView();
	
	*pResult = 0;
}

void CContractListDialog::OnItemchangedVcContractList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (m_Contract_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED)!=-1)
	{
		m_View.EnableWindow(TRUE);
	}
	
	*pResult = 0;
}

BOOL CContractListDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_CONTRACT);
	return TRUE;
}
