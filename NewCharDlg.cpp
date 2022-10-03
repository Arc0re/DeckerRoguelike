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

// NewCharDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "NewCharDlg.h"
#include "Global.h"
#include "DeckerGraphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewCharDlg dialog


CNewCharDlg::CNewCharDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewCharDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewCharDlg)
	m_name = _T("Hacker X");
	m_bonus = 0;
	m_ironman = FALSE;
	m_tooltips = FALSE;
	//}}AFX_DATA_INIT

	// I have discovered that using a bitmap in an image list screws up the background color.
	// Therefore, we have to create a temporary bitmap to hold the colors.
	HBITMAP hTmpBM;
	hTmpBM = (HBITMAP)CopyImage(g_hBitmaps[GR_MISC_CHARACTERS],IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

	m_ilChar.Create(24,24,ILC_COLOR24|ILC_MASK,0,g_nNumCharImages*2);
	m_ilChar.Add(CBitmap::FromHandle(hTmpBM),PURPLE);

	DeleteObject(hTmpBM);

	m_nImage = 0;
}


void CNewCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewCharDlg)
	DDX_Control(pDX, IDC_NEW_CHAR_IMAGE, m_Image);
	DDX_Control(pDX, IDC_NEW_CHAR_IMAGE_SPIN, m_Image_Spin);
	DDX_Text(pDX, IDC_NEW_CHAR_NAME, m_name);
	DDX_Radio(pDX, IDC_NEW_CHAR_BONUS, m_bonus);
	DDX_Check(pDX, IDC_NEW_CHAR_IRONMAN, m_ironman);
	DDX_Check(pDX, IDC_NEW_CHAR_TOOLTIPS, m_tooltips);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewCharDlg, CDialog)
	//{{AFX_MSG_MAP(CNewCharDlg)
	ON_EN_KILLFOCUS(IDC_NEW_CHAR_NAME, OnKillfocusNewCharName)
	ON_WM_HELPINFO()
	ON_NOTIFY(UDN_DELTAPOS, IDC_NEW_CHAR_IMAGE_SPIN, OnDeltaposNewCharImageSpin)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_NEW_CHAR_IRONMAN, OnNewCharIronman)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewCharDlg message handlers

void CNewCharDlg::OnKillfocusNewCharName() 
{
}

void CNewCharDlg::OnOK() 
{
	UpdateData(TRUE);

	if (m_name.IsEmpty())
	{
		DeckerMessageBox("You need to enter a name.");
		return;
	}
	
	CDialog::OnOK();
}

BOOL CNewCharDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_INTRO);
	return TRUE;
}

void CNewCharDlg::OnDeltaposNewCharImageSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	m_nImage = ((pNMUpDown->iPos + pNMUpDown->iDelta + g_nNumCharImages) % g_nNumCharImages);
	m_Image.RedrawWindow();

	*pResult = 0;
}

void CNewCharDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (lpDrawItemStruct->CtlID == IDC_NEW_CHAR_IMAGE)
	{
		CRect rect(&lpDrawItemStruct->rcItem);

		CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

		//pDC->FillSolidRect(&rect,LT_GRAY);
		m_ilChar.DrawIndirect(	pDC, 
								m_nImage,
								rect.TopLeft(),
								rect.Size(),
								CPoint(0,0));
		return;
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CNewCharDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Image_Spin.SetRange(0, g_nNumCharImages - 1);

	// Re-size the picture
	GetDlgItem(IDC_NEW_CHAR_IMAGE)->SetWindowPos(NULL,0,0,24,24,SWP_NOMOVE|SWP_NOZORDER);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewCharDlg::OnNewCharIronman() 
{
	
}
