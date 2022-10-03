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

// DeckDataDialog.cpp : implementation file
//
#include "stdafx.h"
#include "decker.h"
#include "DeckDataDialog.h"
#include "Character.h"
#include "Program.h"
#include "Global.h"
#include "NameDlg.h" //FSO 6-20-01
#include "SoundEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// From WinUser.h
#define OBM_UPARROW         32753
#define OBM_DNARROW         32752
#define OBM_UPARROWD        32743
#define OBM_DNARROWD        32742
#define OBM_UPARROWI        32737
#define OBM_DNARROWI        32736

// The character data
extern CCharacter *g_pChar;

BOOL COEMBitmapButton::LoadBitmaps(UINT nIDBitmapResource,
			UINT nIDBitmapResourceSel,
			UINT nIDBitmapResourceFocus,
			UINT nIDBitmapResourceDisabled)
{
	// delete old bitmaps (if present)
	m_bitmap.DeleteObject();
	m_bitmapSel.DeleteObject();
	m_bitmapFocus.DeleteObject();
	m_bitmapDisabled.DeleteObject();

	if (!m_bitmap.LoadOEMBitmap(nIDBitmapResource))
	{
		TRACE0("Failed to load bitmap for normal image.\n");
		return FALSE;   // need this one image
	}
	BOOL bAllLoaded = TRUE;
	if (nIDBitmapResourceSel != 0)
	{
		if (!m_bitmapSel.LoadOEMBitmap(nIDBitmapResourceSel))
		{
			TRACE0("Failed to load bitmap for selected image.\n");
			bAllLoaded = FALSE;
		}
	}
	if (nIDBitmapResourceFocus != NULL)
	{
		if (!m_bitmapFocus.LoadOEMBitmap(nIDBitmapResourceFocus))
			bAllLoaded = FALSE;
	}
	if (nIDBitmapResourceDisabled != NULL)
	{
		if (!m_bitmapDisabled.LoadOEMBitmap(nIDBitmapResourceDisabled))
			bAllLoaded = FALSE;
	}
	return bAllLoaded;
}

IMPLEMENT_DYNAMIC(COEMBitmapButton, CBitmapButton);

/////////////////////////////////////////////////////////////////////////////
// CDeckDataDialog dialog


CDeckDataDialog::CDeckDataDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDeckDataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeckDataDialog)
	m_analysis = _T("");
	m_attack = _T("");
	m_cpu = _T("");
	m_current_load = _T("");
	m_defense = _T("");
	m_other_hw = _T("");
	m_stealth = _T("");
	m_cop = _T("");
	m_load_average = _T("");
	m_load_heavy = _T("");
	m_load_light = _T("");
	//}}AFX_DATA_INIT

	m_ilSoftware.Create(IDB_SOFTWARE_IL,16,0,WHITE);
}


void CDeckDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeckDataDialog)
	DDX_Control(pDX, IDC_DECK_SOUND, m_Sound);
	DDX_Control(pDX, IDC_DECK_RENAME, m_Rename);
	DDX_Control(pDX, IDC_DECK_TRASH, m_Trash);
	DDX_Control(pDX, IDC_DECK_DEFAULT, m_Default);
	DDX_Control(pDX, IDC_DECK_UP, m_Up);
	DDX_Control(pDX, IDC_DECK_DOWN, m_Down);
	DDX_Control(pDX, IDC_DECK_CLEAR, m_Clear);
	DDX_Control(pDX, IDC_DECK_SOFTWARE, m_Software);
	DDX_Control(pDX, IDC_DECK_LOAD, m_Load);
	DDX_Text(pDX, IDC_DECK_ANALYSIS, m_analysis);
	DDX_Text(pDX, IDC_DECK_ATTACK, m_attack);
	DDX_Text(pDX, IDC_DECK_CPU, m_cpu);
	DDX_Text(pDX, IDC_DECK_CURRENT_LOAD, m_current_load);
	DDX_Text(pDX, IDC_DECK_DEFENSE, m_defense);
	DDX_Text(pDX, IDC_DECK_OTHER_HW, m_other_hw);
	DDX_Text(pDX, IDC_DECK_STEALTH, m_stealth);
	DDX_Text(pDX, IDC_DECK_COP, m_cop);
	DDX_Text(pDX, IDC_DECK_LOAD_AVERAGE, m_load_average);
	DDX_Text(pDX, IDC_DECK_LOAD_HEAVY, m_load_heavy);
	DDX_Text(pDX, IDC_DECK_LOAD_LIGHT, m_load_light);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeckDataDialog, CDialog)
	//{{AFX_MSG_MAP(CDeckDataDialog)
	ON_BN_CLICKED(IDC_DECK_LOAD, OnDeckLoad)
	ON_BN_CLICKED(IDC_DECK_VIEW_CHAR, OnDeckViewChar)
	ON_BN_CLICKED(IDC_DECK_CLEAR, OnDeckClear)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DECK_SOFTWARE, OnItemchangedDeckSoftware)
	ON_BN_CLICKED(IDC_DECK_UP, OnDeckUp)
	ON_BN_CLICKED(IDC_DECK_DOWN, OnDeckDown)
	ON_BN_CLICKED(IDC_DECK_DEFAULT, OnDeckDefault)
	ON_BN_CLICKED(IDC_DECK_TRASH, OnDeckTrash)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_DECK_RENAME, OnDeckRename)
	ON_BN_CLICKED(IDC_DECK_SOUND, OnDeckSound)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_DECK_SOFTWARE, OnColumnclickDeckSoftware)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeckDataDialog message handlers

BOOL CDeckDataDialog::OnInitDialog() 
{
	CString szTmp;
	int i;
	CDialog::OnInitDialog();

	// Set the image list
	m_Software.SetImageList(&m_ilSoftware,LVSIL_SMALL);

	// Set the up/down button images
	m_Up.LoadBitmaps(OBM_UPARROW,OBM_UPARROWD,NULL,OBM_UPARROWI);
	m_Up.SizeToContent();
	m_Down.LoadBitmaps(OBM_DNARROW,OBM_DNARROWD,NULL,OBM_DNARROWI);
	m_Down.SizeToContent();

	// Get the load ratings
	g_pChar->GetLoadRatings(m_nLightLoad, m_nHeavyLoad, m_nMaxLoad);
	m_load_light.Format("0 - %d MP", m_nLightLoad-1);
	m_load_average.Format("%d - %d MP", m_nLightLoad, m_nHeavyLoad);
	m_load_heavy.Format("%d - %d MP", m_nHeavyLoad+1, m_nMaxLoad);
	
	// Set up the software list
	m_Software.InsertColumn(0,"Name",		LVCFMT_LEFT,100);
	m_Software.InsertColumn(1,"Class",		LVCFMT_LEFT,100);
	m_Software.InsertColumn(2,"Rating",		LVCFMT_LEFT,50);
	m_Software.InsertColumn(3,"Size",		LVCFMT_LEFT,50);
	if (g_pChar->m_bOnRun)
	{
		m_Software.InsertColumn(4,"Loaded",		LVCFMT_LEFT,60);
	}
	else
	{
		m_Software.InsertColumn(4,"Load",		LVCFMT_LEFT,40);
		m_Software.InsertColumn(5,"Default",	LVCFMT_LEFT,50);
	}

	// Set the initial values
	m_cpu.Format("%d",g_pChar->m_nChip[CHIP_CPU]);
	m_cop.Format("%d",g_pChar->m_nChip[CHIP_COPROCESSOR]);
	m_attack.Format("%d",g_pChar->m_nChip[CHIP_ATTACK]);
	m_defense.Format("%d",g_pChar->m_nChip[CHIP_DEFENSE]);
	m_stealth.Format("%d",g_pChar->m_nChip[CHIP_STEALTH]);
	m_analysis.Format("%d",g_pChar->m_nChip[CHIP_ANALYSIS]);
	m_other_hw.Empty();
	for (i = 0; i < NUM_HW; i++)
	{
		if (g_pChar->m_nHardware[i]!=0)
		{
			GetHardwareString(szTmp,i,g_pChar->m_nHardware[i]);
			m_other_hw += szTmp;
			m_other_hw += '\n';
		}
	}

	// Show the software
	POSITION p;
	i = 0;
	CProgram *pProgram;
	m_nDefaultLoad = 0;
	p = g_pChar->m_olSoftware.GetHeadPosition();
	while (p!=NULL)
	{
		pProgram = (CProgram *)g_pChar->m_olSoftware.GetNext(p);

		InsertProgram(i, pProgram);

		if (g_pChar->m_bOnRun)
		{
			if (pProgram->m_nLoadedRating)
			{
				m_nDefaultLoad += pProgram->GetSize();
			}
		}
		else
		{
			if (pProgram->m_bLoadByDefault)
			{
				m_nDefaultLoad += pProgram->GetSize();
			}
		}

		i++;
	}

	UpdateLoad();

	UpdateData(FALSE);

	UpdateControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeckDataDialog::UpdateLoad()
{
	if (m_nDefaultLoad < m_nLightLoad)
		m_current_load.Format("%d MP (Light)", m_nDefaultLoad);
	else if (m_nDefaultLoad > m_nHeavyLoad)
		m_current_load.Format("%d MP (Heavy)", m_nDefaultLoad);
	else
		m_current_load.Format("%d MP (Average)", m_nDefaultLoad);
}

void CDeckDataDialog::InsertProgram(int nIndex, CProgram *pProgram)
{
	CString szTxt;

	// Name
	m_Software.InsertItem(nIndex,pProgram->m_szName, pProgram->m_nClass);

	// Class
	m_Software.SetItemText(nIndex,1,GetProgramClassName(pProgram->m_nClass));

	// Rating
	szTxt.Format("%d",pProgram->m_nRating);
	m_Software.SetItemText(nIndex,2,szTxt);

	// Size
	szTxt.Format("%d MP",pProgram->GetSize());
	m_Software.SetItemText(nIndex,3,szTxt);

	if (g_pChar->m_bOnRun)
	{
		// Loaded
		if (pProgram->m_nLoadedRating)
		{
			m_Software.SetItemText(nIndex,4,"Yes");
		}
		else
		{
			m_Software.SetItemText(nIndex,4,"No");
		}
	}
	else
	{
		// Load
		if (pProgram->m_bLoadByDefault)
		{
			m_Software.SetItemText(nIndex,4,"Yes");
		}
		else
		{
			m_Software.SetItemText(nIndex,4,"No");
		}

		// Default
		if ((pProgram == g_pChar->m_pDefAttackProgram) ||
			(pProgram == g_pChar->m_pDefArmorProgram) ||
			(pProgram == g_pChar->m_pDefShieldProgram) ||
			(pProgram == g_pChar->m_pDefHideProgram) ||
			(pProgram == g_pChar->m_pDefReflectProgram))
		{
			m_Software.SetItemText(nIndex,5,"Yes");
		}
		else
		{
			m_Software.SetItemText(nIndex,5,"No");
		}
	}

	// Set the data
	m_Software.SetItemData(nIndex, (LPARAM)pProgram);
}

void CDeckDataDialog::OnDeckLoad() 
{
	int nIndex;
	CProgram *pProgram;

	// Find out what is selected
    nIndex=m_Software.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
    if (nIndex==-1)
	{
		return;
	}

	// Get a pointer to the program object
	pProgram = (CProgram *)m_Software.GetItemData(nIndex);

	// Is this a load or unload?
	if (pProgram->m_bLoadByDefault)
	{
		//--------
		// Unload
		//--------
		// Clear loaded rating
		pProgram->m_bLoadByDefault = FALSE;
		m_nDefaultLoad -= pProgram->GetSize();

		// Update Copies Loaded
		m_Software.SetItemText(nIndex,4,"No");

		// Also update default
		m_Software.SetItemText(nIndex,5,"No");
		if (pProgram == g_pChar->m_pDefAttackProgram)
		{
			g_pChar->m_pDefAttackProgram = NULL;
		}
		else if (pProgram == g_pChar->m_pDefArmorProgram)
		{
			g_pChar->m_pDefArmorProgram = NULL;
		}
		else if (pProgram == g_pChar->m_pDefShieldProgram)
		{
			g_pChar->m_pDefShieldProgram = NULL;
		}
		else if (pProgram == g_pChar->m_pDefHideProgram)
		{
			g_pChar->m_pDefHideProgram = NULL;
		}
		else if (pProgram == g_pChar->m_pDefReflectProgram)
		{
			g_pChar->m_pDefReflectProgram = NULL;
		}
	}
	else
	{
		//------
		// Load
		//------
		// Check max load
		if ((m_nDefaultLoad + pProgram->GetSize()) > m_nMaxLoad)
		{
			DeckerMessageBox("This would overload your deck.");
			return;
		}

		// Set default load
		pProgram->m_bLoadByDefault = TRUE;
		m_nDefaultLoad += pProgram->GetSize();

		// Update Copies Loaded
		m_Software.SetItemText(nIndex,4,"Yes");
	}

	// Update current load
	UpdateLoad();

	UpdateData(FALSE);
	UpdateControls();
}

void CDeckDataDialog::OnDeckViewChar() 
{
	EndDialog(IDC_DECK_VIEW_CHAR);
}

void CDeckDataDialog::OnDeckClear() 
{
	POSITION p;
	CProgram *pProgram;
	int nIndex = 0;

	p = g_pChar->m_olSoftware.GetHeadPosition();
	while (p!=NULL)
	{
		pProgram = (CProgram *)g_pChar->m_olSoftware.GetNext(p);

		pProgram->m_bLoadByDefault = FALSE;

		// Update Copies Loaded
		m_Software.SetItemText(nIndex++,4,"No");

		// Also update default
		m_Software.SetItemText(nIndex,5,"No");
		if (pProgram == g_pChar->m_pDefAttackProgram)
		{
			g_pChar->m_pDefAttackProgram = NULL;
		}
		else if (pProgram == g_pChar->m_pDefArmorProgram)
		{
			g_pChar->m_pDefArmorProgram = NULL;
		}
		else if (pProgram == g_pChar->m_pDefShieldProgram)
		{
			g_pChar->m_pDefShieldProgram = NULL;
		}
		else if (pProgram == g_pChar->m_pDefHideProgram)
		{
			g_pChar->m_pDefHideProgram = NULL;
		}
		else if (pProgram == g_pChar->m_pDefReflectProgram)
		{
			g_pChar->m_pDefReflectProgram = NULL;
		}
	}

	m_nDefaultLoad = 0;

	// Update current load
	UpdateLoad();

	UpdateData(FALSE);

	UpdateControls();
}

void CDeckDataDialog::UpdateControls()
{
	BOOL bEnableLoad = FALSE;
	BOOL bEnableUp = FALSE;
	BOOL bEnableDown = FALSE;
	BOOL bEnableDefault = FALSE;
	BOOL bEnableClear = FALSE;
	BOOL bEnableTrash = FALSE;
	BOOL bEnableRename = FALSE;
	BOOL bEnableSound = FALSE;

	// If on run, everything disabled
	if (!g_pChar->m_bOnRun)
	{
		int nIndex;
		POSITION p;
		CProgram *pProgram;

		// Not on run, so clear always enabled
		bEnableClear = TRUE;

		// Find out what is selected. If nothing, all controls (except clear) disabled
		nIndex=m_Software.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
		if (nIndex != -1)
		{
			// Get a pointer to the program object
			p = g_pChar->m_olSoftware.FindIndex(nIndex);
			pProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);

			// Enable the up/down arrows
			if (p == g_pChar->m_olSoftware.GetHeadPosition())
				bEnableUp = FALSE;
			else
				bEnableUp = TRUE;

			if (p == g_pChar->m_olSoftware.GetTailPosition())
				bEnableDown = FALSE;
			else
				bEnableDown = TRUE;

			// Always enable the load button, and set text
			bEnableLoad = TRUE;
			if (pProgram->m_bLoadByDefault)
				m_Load.SetWindowText("Unload");
			else
				m_Load.SetWindowText("Load");

			// Always enable trash and rename buttons
			bEnableTrash = TRUE;
			bEnableRename = TRUE;
			bEnableSound = TRUE;

			// See if we should enable the Default button
			if ((pProgram->m_nClass == PROGRAM_ATTACK) ||
				(pProgram->m_nClass == PROGRAM_ATTACK_A) ||
				(pProgram->m_nClass == PROGRAM_ATTACK_P) ||
				(pProgram->m_nClass == PROGRAM_SHIELD) ||
				(pProgram->m_nClass == PROGRAM_ARMOR) ||
				(pProgram->m_nClass == PROGRAM_HIDE) ||
				(pProgram->m_nClass == PROGRAM_REFLECT))
			{
				bEnableDefault = TRUE;
			}
		}

	}

	m_Clear.EnableWindow(bEnableClear);
	m_Load.EnableWindow(bEnableLoad);
	m_Default.EnableWindow(bEnableDefault);
	m_Up.EnableWindow(bEnableUp);
	m_Down.EnableWindow(bEnableDown);
	m_Rename.EnableWindow(bEnableRename);
	m_Trash.EnableWindow(bEnableTrash); //FSO 7-2-01
	m_Sound.EnableWindow(bEnableSound);
}

void CDeckDataDialog::OnItemchangedDeckSoftware(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	UpdateControls();

	*pResult = 0;
}

void CDeckDataDialog::OnDeckUp() 
{
	int nIndex;
	POSITION p,p2;
	CProgram *pProgram;

	// Find out what is selected
	nIndex=m_Software.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	p = g_pChar->m_olSoftware.FindIndex(nIndex);
	pProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);

	// Change the position in the internal program list
	p2 = p;
	g_pChar->m_olSoftware.GetPrev(p2);
	g_pChar->m_olSoftware.RemoveAt(p);
	g_pChar->m_olSoftware.InsertBefore(p2, pProgram);

	// Change the program in the software list
	m_Software.DeleteItem(nIndex);
	InsertProgram(--nIndex, pProgram);
	m_Software.SetItemState(nIndex, LVIS_SELECTED , LVIS_SELECTED );
	m_Software.EnsureVisible(nIndex,FALSE);

	UpdateControls();
}

void CDeckDataDialog::OnDeckDown() 
{
	int nIndex;
	POSITION p,p2;
	CProgram *pProgram;

	// Find out what is selected
	nIndex=m_Software.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	p = g_pChar->m_olSoftware.FindIndex(nIndex);
	pProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);

	// Change the position in the internal program list
	p2 = p;
	g_pChar->m_olSoftware.GetNext(p2);
	g_pChar->m_olSoftware.RemoveAt(p);
	g_pChar->m_olSoftware.InsertAfter(p2, pProgram);

	// Change the program in the software list
	m_Software.DeleteItem(nIndex);
	InsertProgram(++nIndex, pProgram);
	m_Software.SetItemState(nIndex, LVIS_SELECTED , LVIS_SELECTED );
	m_Software.EnsureVisible(nIndex,FALSE);

	UpdateControls();
}

void CDeckDataDialog::OnDeckDefault() 
{
	int nIndex;
	CProgram *pProgram;
	LVFINDINFO lvFindInfo;
	CProgram **ppDefProgram;
	int nDefIndex;

	// Find out what is selected
	nIndex=m_Software.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	pProgram = (CProgram *)m_Software.GetItemData(nIndex);

	if ((!pProgram->m_bLoadByDefault) && ((m_nDefaultLoad + pProgram->GetSize()) > m_nMaxLoad))
	{
		DeckerMessageBox("This would overload your deck.");
		return;
	}

	// Get pointer to the default value according to type
	if ((pProgram->m_nClass == PROGRAM_ATTACK) ||
		(pProgram->m_nClass == PROGRAM_ATTACK_A) ||
		(pProgram->m_nClass == PROGRAM_ATTACK_P))
	{
		ppDefProgram = &(g_pChar->m_pDefAttackProgram);
	}
	else if (pProgram->m_nClass == PROGRAM_SHIELD)
	{
		ppDefProgram = &(g_pChar->m_pDefShieldProgram);
	}
	else if (pProgram->m_nClass == PROGRAM_ARMOR)
	{
		ppDefProgram = &(g_pChar->m_pDefArmorProgram);
	}
	else if (pProgram->m_nClass == PROGRAM_HIDE)
	{
		ppDefProgram = &(g_pChar->m_pDefHideProgram);
	}
	else if (pProgram->m_nClass == PROGRAM_REFLECT)
	{
		ppDefProgram = &(g_pChar->m_pDefReflectProgram);
	}
	else return;

	// See if this is already the default
	if ((*ppDefProgram) == pProgram) return;

	// If there is a default program, clear it
	if ((*ppDefProgram) != NULL)
	{
		memset(&lvFindInfo, 0, sizeof(LVFINDINFO));

		lvFindInfo.flags = LVFI_PARAM;
		lvFindInfo.lParam = (LPARAM)(*ppDefProgram);

		nDefIndex = m_Software.FindItem(&lvFindInfo);
		if (nDefIndex!=-1)
		{
			m_Software.SetItemText(nDefIndex,5,"No");
		}
	}

	// Set us as default
	(*ppDefProgram) = pProgram;
	m_Software.SetItemText(nIndex,5,"Yes");

	// Set us to load if not already done
	if (!pProgram->m_bLoadByDefault)
	{
		pProgram->m_bLoadByDefault = TRUE;
		m_Software.SetItemText(nIndex,4,"Yes");
		m_nDefaultLoad += pProgram->GetSize();
		UpdateLoad();
		UpdateData(FALSE);
	}

	UpdateControls();
}

void CDeckDataDialog::OnDeckTrash() 
{
	int nIndex;
	POSITION p;
	CProgram *pProgram;

	// Find out what is selected
	nIndex=m_Software.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	p = g_pChar->m_olSoftware.FindIndex(nIndex);
	pProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);

	// Query for confirmation
	if (DeckerMessageBox("Are you sure you wish to delete this program?", MB_YESNO)==IDYES)
	{
		// Unload the program
		if (pProgram->m_bLoadByDefault)
		{
			m_nDefaultLoad -= pProgram->GetSize();

			// Also update default
			if (pProgram == g_pChar->m_pDefAttackProgram)
			{
				g_pChar->m_pDefAttackProgram = NULL;
			}
			else if (pProgram == g_pChar->m_pDefArmorProgram)
			{
				g_pChar->m_pDefArmorProgram = NULL;
			}
			else if (pProgram == g_pChar->m_pDefShieldProgram)
			{
				g_pChar->m_pDefShieldProgram = NULL;
			}
			else if (pProgram == g_pChar->m_pDefHideProgram)
			{
				g_pChar->m_pDefHideProgram = NULL;
			}
			else if (pProgram == g_pChar->m_pDefReflectProgram)
			{
				g_pChar->m_pDefReflectProgram = NULL;
			}
		}

		// Update screen
		UpdateLoad();
		UpdateData(FALSE);

		// Delete the program
		g_pChar->m_olSoftware.RemoveAt(p);
		delete pProgram;
		m_Software.DeleteItem(nIndex);

		UpdateControls();
	}
}

BOOL CDeckDataDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_DECK);
	return TRUE;
}

void CDeckDataDialog::OnDeckRename() 
{
	CNameDlg dlgName;
	int nIndex;
	POSITION p;
	CProgram *pProgram;

	// Find out what is selected
	nIndex = m_Software.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	p = g_pChar->m_olSoftware.FindIndex(nIndex);
	pProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);

	dlgName.m_text = pProgram->m_szName;
	if (dlgName.DoModal() == IDOK)
	{
		pProgram->m_szName = dlgName.m_text;
		m_Software.SetItemText(nIndex, 0, dlgName.m_text);
	}

}

void CDeckDataDialog::OnDeckSound() 
{
	CSoundEditDlg dlgSoundEdit;
	int nIndex;
	POSITION p;
	CProgram *pProgram;

	// Find out what is selected
	nIndex = m_Software.GetNextItem(-1,LVNI_ALL|LVNI_SELECTED);
	p = g_pChar->m_olSoftware.FindIndex(nIndex);
	pProgram = (CProgram *)g_pChar->m_olSoftware.GetAt(p);

	dlgSoundEdit.m_nSound = pProgram->m_nSound;
	dlgSoundEdit.m_nDefault = pProgram->m_nClass;

	if (dlgSoundEdit.DoModal() == IDOK)
	{
		pProgram->m_nSound = dlgSoundEdit.m_nSound;
	}
}

static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CProgram *pProgram1;
	CProgram *pProgram2;

	pProgram1 = (CProgram *)lParam1;
	pProgram2 = (CProgram *)lParam2;

	switch (lParamSort)
	{
	case 0:	// Name
		return strcmp(pProgram1->m_szName, pProgram2->m_szName);
	case 1:	// Class
		return pProgram1->m_nClass - pProgram2->m_nClass;
	case 2:	// Rating
		return pProgram1->m_nRating - pProgram2->m_nRating;
	case 3:	// Size
		return pProgram1->GetSize() - pProgram2->GetSize();
	case 4:	// Load
		return pProgram1->m_nLoadedRating - pProgram2->m_nLoadedRating;
	case 5:	// Default
		return pProgram1->m_bLoadByDefault - pProgram2->m_bLoadByDefault;
	}

	return 0;
}

void CDeckDataDialog::OnColumnclickDeckSoftware(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Sort the list
	m_Software.SortItems(SortFunc, pNMListView->iSubItem);

	// Now rebuild the software list
	g_pChar->m_olSoftware.RemoveAll();

	int nIndex;
	int nCount = m_Software.GetItemCount();
	CProgram *pProgram;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		pProgram = (CProgram *)m_Software.GetItemData(nIndex);
		g_pChar->m_olSoftware.AddTail(pProgram);
	}
	
	*pResult = 0;
}

