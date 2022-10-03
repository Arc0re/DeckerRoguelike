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

// Shop.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "Shop.h"
#include "Character.h"
#include "ShopItem.h"
#include "Program.h"
#include "Global.h"
#include "OrderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

// Shop names
static const char *g_szShopName[] = 
{
	"Thrifty Electronics",		//LS_STREET	
	"The Little Warez Shop",	//LS_LOW		
	"The Matrix Emporium",		//LS_MED		
	"Hacker's Haven",			//LS_HIGH		
	"Nullspace",				//LS_ELITE	
};

/////////////////////////////////////////////////////////////////////////////
// CShop dialog


CShop::CShop(CWnd* pParent /*=NULL*/)
	: CDialog(CShop::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShop)
	m_credits = _T("");
	//}}AFX_DATA_INIT

	m_ilItems.Create(IDB_ITEM_IL,16,0,WHITE);
}


void CShop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShop)
	DDX_Control(pDX, IDC_SHOP_PURCHASE, m_Purchase);
	DDX_Control(pDX, IDC_SHOP_ITEM_LIST, m_Item_List);
	DDX_Text(pDX, IDC_SHOP_CREDITS, m_credits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShop, CDialog)
	//{{AFX_MSG_MAP(CShop)
	ON_BN_CLICKED(IDC_SHOP_ORDER, OnShopOrder)
	ON_BN_CLICKED(IDC_SHOP_PURCHASE, OnShopPurchase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SHOP_ITEM_LIST, OnItemchangedShopItemList)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShop message handlers

BOOL CShop::OnInitDialog() 
{
	CString szTmp;
	CDialog::OnInitDialog();
	int nImage;
	int nCurrent;

	this->SetWindowText(g_szShopName[g_pChar->m_nLifestyle]);

	m_Purchase.EnableWindow(FALSE);

	// Initialize the item list
	m_Item_List.SetImageList(&m_ilItems,LVSIL_SMALL   );
	m_Item_List.InsertColumn(0,"Item",		LVCFMT_LEFT,200);
	m_Item_List.InsertColumn(1,"Cost",		LVCFMT_LEFT,60);
	m_Item_List.InsertColumn(2,"Current",	LVCFMT_LEFT,60);
	
	POSITION p;
	int i = 0;
	CShopItem *pItem;
	p = g_pChar->m_olShopList.GetHeadPosition();
	while (p!=NULL)
	{
		pItem = (CShopItem *)g_pChar->m_olShopList.GetNext(p);

		// Get the image
		nImage = pItem->m_nType;

		// Text
		m_Item_List.InsertItem(i,pItem->m_szText, nImage);

		// Pay
		szTmp.Format("%dcr",pItem->m_nPrice);
		m_Item_List.SetItemText(i,1,szTmp);

		// Current
		nCurrent = g_pChar->GetCurrentItemLevel(pItem->m_nType, pItem->m_nSubType);
		if (nCurrent == 0)
			szTmp = "-";
		else
			szTmp.Format("%d",nCurrent);
		m_Item_List.SetItemText(i,2,szTmp);

		i++;
	}

	// Update variables
	m_credits.Format("%d", g_pChar->m_nCredits);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShop::OnShopPurchase() 
{
	int nIndex;
	POSITION p;
	CShopItem *pItem;
	BOOL bFoundGreater = FALSE;
	CProgram *pProgram;
	CString szTmp;

	// Find out what is selected
    nIndex=m_Item_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
    if (nIndex==-1)
	{
		return;
	}

	// Get a pointer to this item
	p = g_pChar->m_olShopList.FindIndex(nIndex);
	pItem = (CShopItem *)g_pChar->m_olShopList.GetAt(p);

	// See if we have enough money
	if (g_pChar->m_nCredits < pItem->m_nPrice)
	{
		DeckerMessageBox("You cannot afford this.");
		return;
	}

	// Process depending on type
	switch (pItem->m_nType)
	{
	case IT_SOFTWARE:
		// Look for program of equivalent level or greater
		p = g_pChar->m_olSoftware.GetHeadPosition();
		while (p!=NULL)
		{
			pProgram = (CProgram *)g_pChar->m_olSoftware.GetNext(p);

			if (pProgram->m_nClass == pItem->m_nSubType)
			{
				if (pProgram->m_nRating==pItem->m_nRating)
				{
					DeckerMessageBox("You already own this type of program");
					return;
				}
				else if (pProgram->m_nRating>pItem->m_nRating)
				{
					bFoundGreater = TRUE;
				}
			}
		}

		if (bFoundGreater)
		{
			if (DeckerMessageBox("You already own a stronger version of this program. Do you still wish to buy it?",MB_YESNO)==IDNO)
			{
				return;
			}
		}

		// Get the money
		g_pChar->m_nCredits -= pItem->m_nPrice;

		// Buy the program
		pProgram = new CProgram;
		pProgram->GetInitValues(pItem->m_nSubType, pItem->m_nRating);

		// Add it to the user's software list (sorted)
		p = g_pChar->m_olSoftware.GetHeadPosition();
		while (p!=NULL)
		{
			CProgram *pListProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);

			if (pListProgram->m_nClass > pProgram->m_nClass) break;

			if ((pListProgram->m_nClass == pProgram->m_nClass) &&
				(pListProgram->m_nRating < pProgram->m_nRating)) break;

			g_pChar->m_olSoftware.GetNext(p);
		}

		g_pChar->m_olSoftware.InsertBefore(p, pProgram);

		DeckerMessageBox("Program purchased.");

		break;

	case IT_HARDWARE:
		// Look for hardware of equivalent level or greater
		if (g_pChar->m_nHardware[pItem->m_nSubType] >= pItem->m_nRating)
		{
			DeckerMessageBox("You already own this type of hardware.");
			return;
		}

		// Check for chip burner and chips active
		if ((pItem->m_nSubType == HW_CHIP_BURNER) && (g_pChar->m_pChipBurning != NULL))
		{
			if (DeckerMessageBox("Your currently have chips cooking. If you upgrade, these will be destroyed. Continue?", MB_YESNO)==IDNO)
			{
				return;
			}
			g_pChar->m_pChipBurning = NULL;
		}

		// Check for refund
		if (g_pChar->m_nHardware[pItem->m_nSubType]!=0)
		{
			int iRefund;

			iRefund = GetHardwarePrice(pItem->m_nSubType,g_pChar->m_nHardware[pItem->m_nSubType]) / 2;
			szTmp.Format("Hardware purchased. You sold your old hardware for %dcr.", iRefund);
			g_pChar->m_nCredits += iRefund;
		}
		else
		{
			DeckerMessageBox("Hardware purchased.");
		}

		// Set the hardware item
		g_pChar->m_nHardware[pItem->m_nSubType] = pItem->m_nRating;

		// Get the money
		g_pChar->m_nCredits -= pItem->m_nPrice;

		// Remove the item
		g_pChar->m_olShopList.RemoveAt(p);
		delete pItem;
		m_Item_List.DeleteItem(nIndex);
		break;
	case IT_CHIP:
		// Look for hardware of equivalent level or greater
		if (g_pChar->m_nChip[pItem->m_nSubType] >= pItem->m_nRating)
		{
			DeckerMessageBox("You already own a chip of this rating or higher.");
			return;
		}

		DeckerMessageBox("Chip purchased.");

		// Save the chip
		g_pChar->m_nChip[pItem->m_nSubType] = pItem->m_nRating;

		// Get the money
		g_pChar->m_nCredits -= pItem->m_nPrice;

		// Remove the item
		g_pChar->m_olShopList.RemoveAt(p);
		delete pItem;
		m_Item_List.DeleteItem(nIndex);
	}

	// Update the screen
	m_credits.Format("%d", g_pChar->m_nCredits);
	UpdateData(FALSE);

	// Update all ratings
	int nCurrent;
	int i = 0;
	p = g_pChar->m_olShopList.GetHeadPosition();
	while (p!=NULL)
	{
		pItem = (CShopItem *)g_pChar->m_olShopList.GetNext(p);

		// Current
		nCurrent = g_pChar->GetCurrentItemLevel(pItem->m_nType, pItem->m_nSubType);
		if (nCurrent == 0)
			szTmp = "-";
		else
			szTmp.Format("%d",nCurrent);
		m_Item_List.SetItemText(i,2,szTmp);

		i++;
	}

    nIndex=m_Item_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
    m_Purchase.EnableWindow(nIndex!=-1);
}

void CShop::OnShopOrder() 
{
	// Is an order in progress
	if (g_pChar->m_nOrderItemType != -1)
	{
		CString szTmp;
		szTmp.Format("Your order will arrive in %d days.", g_pChar->m_nOrderTimeLeft);
		DeckerMessageBox(szTmp);
	}
	else
	{
		COrderDlg dlgOrder;
		dlgOrder.DoModal();
	}
}

void CShop::OnItemchangedShopItemList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (m_Item_List.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED)!=-1)
	{
		m_Purchase.EnableWindow(TRUE);
	}
	
	*pResult = 0;
}


BOOL CShop::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_SHOP);
	return TRUE;
}
