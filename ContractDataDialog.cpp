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

// ContractDataDialog.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "ContractDataDialog.h"
#include "Character.h"
#include "Contract.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// CContractDataDialog dialog


CContractDataDialog::CContractDataDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CContractDataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContractDataDialog)
	m_description = _T("");
	m_difficulty = _T("");
	m_pay = _T("");
	m_target_corp = _T("");
	m_type = _T("");
	m_deadline = _T("");
	m_status1 = _T("");
	m_status2 = _T("");
	//}}AFX_DATA_INIT
}


void CContractDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContractDataDialog)
	DDX_Control(pDX, IDC_CONT_STATUS2, m_Status2);
	DDX_Control(pDX, IDC_CONT_STATUS1, m_Status1);
	DDX_Control(pDX, IDOK, m_Accept);
	DDX_Text(pDX, IDC_CONT_DESC, m_description);
	DDX_Text(pDX, IDC_CONT_DIFFICULTY, m_difficulty);
	DDX_Text(pDX, IDC_CONT_PAY, m_pay);
	DDX_Text(pDX, IDC_CONT_TARGET_CORP, m_target_corp);
	DDX_Text(pDX, IDC_CONT_TYPE, m_type);
	DDX_Text(pDX, IDC_CONT_DEADLINE, m_deadline);
	DDX_Text(pDX, IDC_CONT_STATUS1, m_status1);
	DDX_Text(pDX, IDC_CONT_STATUS2, m_status2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContractDataDialog, CDialog)
	//{{AFX_MSG_MAP(CContractDataDialog)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContractDataDialog message handlers

BOOL CContractDataDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pContract->GetDescriptiveText(m_description);
	m_pay.Format("%d",m_pContract->m_nPay);
	m_target_corp.Format("%s (%d)", m_pContract->m_szSystemName, m_pContract->m_nSystemSize);
	m_pContract->GetTypeText(m_type);
	m_deadline.Format("%d days", m_pContract->m_nDaysLeft);
	m_difficulty.Format("%d%%", (m_pContract->m_nDifficulty * 5));

	//FSO 4-8-02
	if (g_pChar->m_bOnRun)
	{
		CString szTmp;
		// Show the current status
		GetContractStatus(szTmp, m_status2, TRUE, FALSE);
		m_status1.Format("Status: %s", szTmp);
	}
	else
	{
		// Hide status
		m_status1.Empty();
		m_status2.Empty();
	}

	UpdateData(FALSE);

	m_Accept.EnableWindow(g_pChar->m_pCurrentContract==NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CContractDataDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_CONTRACT);
	return TRUE;
}
