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

// FileAccessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "FileAccessDlg.h"
#include "Character.h"
#include "DSFile.h"
#include "System.h"
#include "Node.h"
#include "Ice.h"
#include "Global.h"
#include "Program.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// CFileAccessDlg dialog


CFileAccessDlg::CFileAccessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileAccessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileAccessDlg)
	//}}AFX_DATA_INIT
}


void CFileAccessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileAccessDlg)
	DDX_Control(pDX, IDC_TRANSFER_AFTER_TEXT, m_After_Text);
	DDX_Control(pDX, IDC_TRANSFER_LOAD_TEXT, m_Load_Text);
	DDX_Control(pDX, IDC_TRANSFER_LOAD, m_Load);
	DDX_Control(pDX, IDC_TRANSFER_LIGHT_TEXT, m_Light_Text);
	DDX_Control(pDX, IDC_TRANSFER_LIGHT, m_Light);
	DDX_Control(pDX, IDC_TRANSFER_HEAVY_TEXT, m_Heavy_Text);
	DDX_Control(pDX, IDC_TRANSFER_HEAVY, m_Heavy);
	DDX_Control(pDX, IDC_TRANSFER_AVERAGE_TEXT, m_Average_Text);
	DDX_Control(pDX, IDC_TRANSFER_AVERAGE, m_Average);
	DDX_Control(pDX, IDC_TRANSFER_AFTER, m_After);
	DDX_Control(pDX, IDC_TRANSFER_ITEM_LIST, m_Item_List);
	DDX_Control(pDX, IDOK, m_Ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileAccessDlg, CDialog)
	//{{AFX_MSG_MAP(CFileAccessDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TRANSFER_ITEM_LIST, OnItemchangedTransferItemList)
	ON_NOTIFY(NM_DBLCLK, IDC_TRANSFER_ITEM_LIST, OnDblclkVcTransferItemList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileAccessDlg message handlers

BOOL CFileAccessDlg::OnInitDialog() 
{
	POSITION p;
	CDSFile *pFile;
	CString szTmp;
	int i;

	CDialog::OnInitDialog();

	m_Ok.EnableWindow(FALSE);

	// Hide unused windows
	m_Load_Text.ShowWindow(SW_HIDE);
	m_Load.ShowWindow(SW_HIDE);
	m_Light_Text.ShowWindow(SW_HIDE);
	m_Light.ShowWindow(SW_HIDE);
	m_Heavy_Text.ShowWindow(SW_HIDE);
	m_Heavy.ShowWindow(SW_HIDE);
	m_Average_Text.ShowWindow(SW_HIDE);
	m_Average.ShowWindow(SW_HIDE);
	m_After.ShowWindow(SW_HIDE);
	m_After_Text.ShowWindow(SW_HIDE);

	switch (m_nOperationType)
	{
	case FO_GET:
		szTmp.Format("Select a file to download from %s", g_pChar->m_pCurrentNode->m_szName);
		break;
	case FO_EDIT:
		szTmp.Format("Select a file to edit on %s", g_pChar->m_pCurrentNode->m_szName);
		break;
	case FO_ERASE:
		szTmp.Format("Select a file to erase from %s", g_pChar->m_pCurrentNode->m_szName);
		break;
	//case FO_VIEW:
	default:
		szTmp.Format("Files on %s", g_pChar->m_pCurrentNode->m_szName);
		break;
	}
	SetWindowText(szTmp);

	// Create the column headers
	m_Item_List.InsertColumn(0,"Name",		LVCFMT_LEFT,100);
	m_Item_List.InsertColumn(1,"Contents",	LVCFMT_LEFT,160);
	m_Item_List.InsertColumn(2,"Value",		LVCFMT_LEFT,60);
	m_Item_List.InsertColumn(3,"Size",		LVCFMT_LEFT,50);
	m_Item_List.InsertColumn(4,"Time",		LVCFMT_LEFT,50);
	m_Item_List.InsertColumn(5,"Guard",		LVCFMT_LEFT,100);

	// Insert the files - Only ones that are in the node
	i = 0;
	p = g_pChar->m_pCurrentNode->m_olFileList.GetHeadPosition();
	while (p!=NULL)
	{
		pFile = (CDSFile *)g_pChar->m_pCurrentNode->m_olFileList.GetNext(p);

		// if not in node, ignore
		if (!(pFile->m_nState & STATE_IN_NODE)) continue;

		// Name
		m_Item_List.InsertItem(i,pFile->m_szName);

		// Contents - only if scanned
		if (pFile->m_nState & STATE_SCAN)
		{
			szTmp = pFile->GetContentString();
			if (pFile->m_nState & STATE_EDITED_N) szTmp += " (Edited)";
		}
		else
		{
			szTmp = "?";
		}
		m_Item_List.SetItemText(i,1,szTmp);

		// Value (quest+worthless visible on scan, other on eval)
		if (pFile->m_nState & STATE_SCAN)
		{
			if (pFile->m_nType == FT_QUEST)
			{
				szTmp = "*Contract*";
			}
			else if (pFile->m_nType == FT_USELESS)
			{
				szTmp = "-";
			}
			else if (pFile->m_nState & STATE_EVAL)
			{
				if (pFile->m_nType == FT_VALUABLE)
				{
					// If it has been edited, mark it as 0
					if (pFile->m_nState & STATE_EDITED_N)
						szTmp = "0";
					// If has been downloaded, or is worthless, mark as none
					else if (pFile->m_nState & STATE_IN_DECK)
						szTmp = "-";
					else
						szTmp.Format("%d", pFile->m_nValue);
				}
				else if ((pFile->m_nType == FT_PROGRAM) || (pFile->m_nType == FT_P_SOURCE))
				{
					GetSoftwareText(szTmp, pFile->m_nContents, pFile->m_nValue);
				}
				else if (pFile->m_nType == FT_C_SOURCE)
				{
					szTmp.Format("%s %d",GetChipName(pFile->m_nContents), pFile->m_nValue);
				}
				else //if ((pFile->m_nType == FT_CLUE)||(pFile->m_nType == FT_PASSCODE)
				{
					szTmp = "-";
				}
			}
			else
			{
				szTmp = "?";
			}
		}
		else
		{
			szTmp = "?";
		}
		m_Item_List.SetItemText(i,2,szTmp);

		// Size (always visible)
		szTmp.Format("%d", pFile->m_nSize);
		m_Item_List.SetItemText(i,3,szTmp);

		// Time (always visible)
		if (pFile->m_nState & STATE_IN_DECK)
		{
			szTmp = '-';
		}
		else
		{
			szTmp.Format("%d", pFile->GetLoadTime());
		}
		m_Item_List.SetItemText(i,4,szTmp);

		// Guard (always visible)
		if (pFile->m_pTapeworm != NULL)
		{
			m_Item_List.SetItemText(i,5,pFile->m_pTapeworm->m_szName);
		}

		// Save a pointer to the program
		m_Item_List.SetItemData(i,(DWORD)pFile);

		i++;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileAccessDlg::OnItemchangedTransferItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex;

	nIndex = m_Item_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);

	if (nIndex==-1)
	{
		m_Ok.EnableWindow(FALSE);
	}
	else
	{
		CDSFile *pFile;

		pFile = (CDSFile *)m_Item_List.GetItemData(nIndex);

		// If tapeworm, no access possible
		if (pFile->m_pTapeworm != NULL) //&& (!pFile->m_pTapeworm->m_bBypassed))
		{
			m_Ok.EnableWindow(FALSE);
		}
		else
		{
			// Activate ok based on type
			switch (m_nOperationType)
			{
			case FO_GET:
				// Activate if not downloaded
				m_Ok.EnableWindow((pFile->m_nState & STATE_IN_DECK)==0);
				break;
			case FO_EDIT:
				// Activate if not edited
				m_Ok.EnableWindow((pFile->m_nState & STATE_EDITED_N)==0);
				break;
			case FO_ERASE:
				// Can always erase
				m_Ok.EnableWindow(TRUE);
				break;
			//case FO_VIEW:
				// Leave inactive
			}
		}
	}
	
	*pResult = 0;
}

void CFileAccessDlg::OnOK() 
{
	int nIndex;
	CDSFile *pFile;

	// See what is selected
	nIndex = m_Item_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	if (nIndex == -1) return;

	// Get a pointer to the file
	pFile = (CDSFile *)m_Item_List.GetItemData(nIndex);

	// Process based on type
	if (m_nOperationType == FO_ERASE)
	{
		// Erasing is instant
		pFile->m_nState &= (~STATE_IN_NODE);
	}
	else
	{
		// See if something else is being loaded
		if ((g_pChar->m_pTransferProgram != NULL) ||
			(g_pChar->m_pTransferFile != NULL))
		{
			if (DeckerMessageBox("There is currently a transfer in progress. Do you wish to cancel it?",MB_YESNO)==IDNO)
			{
				return;
			}

			g_pChar->m_pTransferProgram = NULL;
		}

		g_pChar->m_nFileTransferType = m_nOperationType;
		g_pChar->m_pTransferFile = pFile;
		g_pChar->m_nTransferInitialTime = pFile->GetLoadTime();

		// If this is an edit, it goes at double speed
		if (m_nOperationType == FO_EDIT)
		{
			g_pChar->m_nTransferInitialTime = (g_pChar->m_nTransferInitialTime+1)/2;
		}
		if (g_pChar->m_nTransferInitialTime<1)
		{
			g_pChar->m_nTransferInitialTime = 1;
		}

		g_pChar->m_nTransferTurnsLeft = 1 + g_pChar->m_nTransferInitialTime;
		// NOTE: We use +1 because it will decrement immediately
	}
	
	CDialog::OnOK();
}

void CFileAccessDlg::OnDblclkVcTransferItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (::IsWindowEnabled(m_Ok.m_hWnd))
	{
		OnOK();
	}
	
	*pResult = 0;
}

