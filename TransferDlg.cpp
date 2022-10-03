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

// TransferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "TransferDlg.h"
#include "Program.h"
#include "Character.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg dialog

CTransferDlg::CTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferDlg)
	m_after = _T("");
	m_average = _T("");
	m_heavy = _T("");
	m_light = _T("");
	m_load = _T("");
	//}}AFX_DATA_INIT
}


void CTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferDlg)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDC_TRANSFER_ITEM_LIST, m_Item_List);
	DDX_Text(pDX, IDC_TRANSFER_AFTER, m_after);
	DDX_Text(pDX, IDC_TRANSFER_AVERAGE, m_average);
	DDX_Text(pDX, IDC_TRANSFER_HEAVY, m_heavy);
	DDX_Text(pDX, IDC_TRANSFER_LIGHT, m_light);
	DDX_Text(pDX, IDC_TRANSFER_LOAD, m_load);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferDlg, CDialog)
	//{{AFX_MSG_MAP(CTransferDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TRANSFER_ITEM_LIST, OnItemchangedTransferItemList)
	ON_NOTIFY(NM_DBLCLK, IDC_TRANSFER_ITEM_LIST, OnDblclkVcTransferItemList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferDlg message handlers

BOOL CTransferDlg::OnInitDialog() 
{
	POSITION p;
	CProgram *pProgram;
	CString szTmp;
	int i;

	CDialog::OnInitDialog();

	m_Ok.EnableWindow(FALSE);

	SetWindowText("Select program to load");

	// Get the load ratings
	g_pChar->GetLoadRatings(m_nLightLoad, m_nHeavyLoad, m_nMaxLoad);
	m_light.Format("0 - %d MP", m_nLightLoad-1);
	m_average.Format("%d - %d MP", m_nLightLoad, m_nHeavyLoad);
	m_heavy.Format("%d - %d MP", m_nHeavyLoad+1, m_nMaxLoad);

	GetLoadText(m_load, g_pChar->m_nCurrentLoad);
	m_after.Empty();

	// Create the column headers
	m_Item_List.InsertColumn(0,"Name",		LVCFMT_LEFT,160);
	m_Item_List.InsertColumn(1,"Class",		LVCFMT_LEFT,80);
	m_Item_List.InsertColumn(2,"Rating",	LVCFMT_LEFT,50);
	m_Item_List.InsertColumn(3,"Size",		LVCFMT_LEFT,50);
	m_Item_List.InsertColumn(4,"Time",		LVCFMT_LEFT,50);

	// Insert the programs - Only ones not fully loaded
	i = 0;
	p = g_pChar->m_olSoftware.GetHeadPosition();
	while (p!=NULL)
	{
		pProgram = (CProgram *)g_pChar->m_olSoftware.GetNext(p);

		// if already fully loaded, skip
		if (pProgram->m_nLoadedRating == pProgram->m_nRating) continue;

		// Name
		m_Item_List.InsertItem(i,pProgram->m_szName);

		// Class
		m_Item_List.SetItemText(i,1,GetProgramClassName(pProgram->m_nClass));

		// Rating
		szTmp.Format("%d",pProgram->m_nRating);
		m_Item_List.SetItemText(i,2,szTmp);

		// Size
		szTmp.Format("%d MP",pProgram->GetSize());
		m_Item_List.SetItemText(i,3,szTmp);

		// Time
		szTmp.Format("%d", pProgram->GetLoadTime());
		m_Item_List.SetItemText(i,4,szTmp);

		// Save a pointer to the program
		m_Item_List.SetItemData(i,(DWORD)pProgram);

		i++;
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTransferDlg::GetLoadText(CString &szStr, int nLoad)
{
	szStr.Format("%d ", nLoad);
	if (nLoad < m_nLightLoad)
		szStr += "(Light)";
	else if (nLoad > m_nHeavyLoad)
		szStr += "(Heavy)";
	else if (nLoad > m_nMaxLoad)
		szStr += "(Overloaded)";
	else
		szStr += "(Average)";
}

void CTransferDlg::OnItemchangedTransferItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex;

	nIndex = m_Item_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex != -1)
	{
		m_Ok.EnableWindow(TRUE);

		CProgram *pProgram = (CProgram *)m_Item_List.GetItemData(nIndex);

		GetLoadText(m_after, g_pChar->m_nCurrentLoad + pProgram->GetSize());
		UpdateData(FALSE);
	}
	
	*pResult = 0;
}

void CTransferDlg::OnOK() 
{
	int nIndex;

	// See what is selected
	nIndex = m_Item_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex == -1) return;
	CProgram *pProgram = (CProgram *)m_Item_List.GetItemData(nIndex);

	// Make sure this will not overload the deck
	if ((g_pChar->m_nCurrentLoad + pProgram->GetSize()) > m_nMaxLoad)
	{
		DeckerMessageBox("This would overload the deck.");
		return;
	}

	// See if something else is being loaded
	if ((g_pChar->m_pTransferProgram != NULL) ||
		(g_pChar->m_pTransferFile != NULL))
	{
		if (DeckerMessageBox("There is currently a transfer in progress. Do you wish to cancel it?",MB_YESNO)==IDNO)
		{
			return;
		}

		g_pChar->m_pTransferFile = NULL;
	}

	g_pChar->m_pTransferProgram = (CProgram *)m_Item_List.GetItemData(nIndex);
	g_pChar->m_nTransferInitialTime = g_pChar->m_pTransferProgram->GetLoadTime();
	g_pChar->m_nTransferTurnsLeft = 1 + g_pChar->m_nTransferInitialTime;
	// NOTE: We use +1 because it will decrement immediately
	
	CDialog::OnOK();
}

void CTransferDlg::OnDblclkVcTransferItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK();
	
	*pResult = 0;
}
