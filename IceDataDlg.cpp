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

// IceDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "IceDataDlg.h"
#include "Character.h"
#include "Ice.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// CIceDataDlg dialog


CIceDataDlg::CIceDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIceDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIceDataDlg)
	m_name = _T("");
	m_notes = _T("");
	m_rating = _T("");
	m_status = _T("");
	m_type = _T("");
	//}}AFX_DATA_INIT
}


void CIceDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIceDataDlg)
	DDX_Text(pDX, IDC_ICE_NAME, m_name);
	DDX_Text(pDX, IDC_ICE_NOTES, m_notes);
	DDX_Text(pDX, IDC_ICE_RATING, m_rating);
	DDX_Text(pDX, IDC_ICE_STATUS, m_status);
	DDX_Text(pDX, IDC_ICE_TYPE, m_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIceDataDlg, CDialog)
	//{{AFX_MSG_MAP(CIceDataDlg)
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIceDataDlg message handlers

BOOL CIceDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Get the targetted ice
	CIce *pIce = g_pChar->m_pTargettedIce;

	// Set the name
	m_name = pIce->m_szName;

	// Get the type - this will vary according to scan status
	m_type = pIce->GetTypeString();

	// Most information is not available unless the ice has been scanned
	if (pIce->m_nAnalyzed)
	{
		//Get rating
		m_rating.Format("%d", pIce->m_nRating);
	}
	else
	{
		m_rating = "?";
	}

	// Get status
	m_status.Format("%d%% damaged", (MAX_HEALTH - pIce->m_nHealth) * HEALTH_INCREMENT);
	if (pIce->m_nSlowLevel) m_status += "\nSlowed";
	if (pIce->m_nConfusionLevel) m_status += "\nConfused";
	if (pIce->m_nWeakenLevel) m_status += "\nWeakened";
	if (pIce->m_nVirusLevel) m_status += "\nInfected";
	
	// Get notes
	pIce->GetNotes(m_notes);

	// Show the data
	UpdateData(FALSE);

	// Re-size the picture
	GetDlgItem(IDC_ICE_PIC)->SetWindowPos(NULL,0,0,24,24,SWP_NOMOVE|SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CIceDataDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (lpDrawItemStruct->CtlID == IDC_ICE_PIC)
	{
		CRect rect(&lpDrawItemStruct->rcItem);

		//m_pIceIL->DrawIndirect(	CDC::FromHandle(lpDrawItemStruct->hDC), 
		//						g_pChar->m_pTargettedIce->m_nImage,
		//						rect.TopLeft(),
		//						rect.Size(),
		//						CPoint(0,0));
		m_pIceIA->DrawImage(	CDC::FromHandle(lpDrawItemStruct->hDC), 
								rect.left,
								rect.top,
								g_pChar->m_pTargettedIce->m_nImage);

		return;
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
