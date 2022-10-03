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

// NewProjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "NewProjectDlg.h"
#include "Character.h"
#include "Program.h"
#include "Source.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// The character data
extern CCharacter *g_pChar;

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg dialog


CNewProjectDlg::CNewProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewProjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewProjectDlg)
	m_class = 0;
	m_rating = 0;
	m_skill = _T("");
	m_source = _T("");
	m_time = _T("");
	m_type = 0;
	//}}AFX_DATA_INIT
}


void CNewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewProjectDlg)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDC_PROJ_TYPE, m_Type);
	DDX_Control(pDX, IDC_PROJ_RATING, m_Rating);
	DDX_Control(pDX, IDC_PROJ_CLASS, m_Class);
	DDX_CBIndex(pDX, IDC_PROJ_CLASS, m_class);
	DDX_CBIndex(pDX, IDC_PROJ_RATING, m_rating);
	DDX_Text(pDX, IDC_PROJ_SKILL, m_skill);
	DDX_Text(pDX, IDC_PROJ_SOURCE, m_source);
	DDX_Text(pDX, IDC_PROJ_TIME, m_time);
	DDX_CBIndex(pDX, IDC_PROJ_TYPE, m_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewProjectDlg, CDialog)
	//{{AFX_MSG_MAP(CNewProjectDlg)
	ON_CBN_SELCHANGE(IDC_PROJ_TYPE, OnSelchangeProjType)
	ON_CBN_SELCHANGE(IDC_PROJ_RATING, OnSelchangeProjRating)
	ON_CBN_SELCHANGE(IDC_PROJ_CLASS, OnSelchangeProjClass)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewProjectDlg message handlers

BOOL CNewProjectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Fill in the class list box. This will take care of everything else.
	FillInClassList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewProjectDlg::FillInClassList()
{
	int i;
	int nIndex;

	m_Class.ResetContent();

	if (m_type == IT_SOFTWARE)
	{
		// Program
		for (i=0; i<NUM_PROGRAMS; i++)
		{
			if (i != PROGRAM_CLIENT)
			{
				nIndex = m_Class.AddString(GetProgramClassName(i));
				m_Class.SetItemData(nIndex, (DWORD)i);
			}
		}
	}
	else
	{
		// Chip
		for (i=0; i<NUM_CHIPS; i++)
		{
			nIndex = m_Class.AddString(GetChipName(i));
			m_Class.SetItemData(nIndex, (DWORD)i);
		}
	}
	m_class = 0;

	FillInRatingList();
}

void CNewProjectDlg::FillInRatingList()
{
	int i;
	CString szTmp;
	int nClass = m_Class.GetItemData(m_class);

	// Find the maximum source level
	CSource *pSource;
	POSITION p;

	m_nMaxRating = 0;
	p = g_pChar->m_olSourceCode.GetHeadPosition();
	while (p != NULL)
	{
		pSource = (CSource *)g_pChar->m_olSourceCode.GetNext(p);

		if ((pSource->m_nType == m_type) &&
			(pSource->m_nClass == nClass))
		{
			if (pSource->m_nRating > m_nMaxRating)
			{
				m_nMaxRating = pSource->m_nRating;
			}
		}
	}

	// Fill in the source available
	m_source.Format("%d", m_nMaxRating);

	// Fill in the list box
	m_Rating.ResetContent();

	if (m_type == IT_SOFTWARE)
		m_nSkill = g_pChar->m_nProgrammingSkill;
	else
		m_nSkill = g_pChar->m_nChipDesignSkill;
	m_skill.Format("%d", m_nSkill);

	for (i = 0; i < m_nSkill; i++)
	{
		szTmp.Format("%d", i+1);
		m_Rating.AddString(szTmp);
	}

	// Select an initial rating of the highest level
	m_rating = m_nSkill - 1;

	// Update source and time
	FillInMisc();
}

void CNewProjectDlg::FillInMisc()
{
	int nComplexity;
	int nNewTime;
	int nOldTime;
	int nRating;
	int nClass = m_Class.GetItemData(m_class);

	// Get the complexity
	if (m_type == IT_SOFTWARE)
	{
		// Get time to write program
		nComplexity = GetProgramComplexity(nClass);
	}
	else
	{
		// Get time to design chip
		nComplexity = GetChipComplexity(nClass);
	}

	// Get the estimated time
	nRating = (m_rating + 1);
	if (nRating > m_nMaxRating)
	{
		// Doing an upgrade - use difference
		nNewTime = nComplexity * nRating * nRating;
		nOldTime = nComplexity * m_nMaxRating * m_nMaxRating;
		m_nBaseTime = nNewTime - nOldTime;
	}
	else
	{
		CSource *pSource;
		POSITION p;
		int nBestFit;

		// Doing a downgrade - use lowest that is higher than desired
		nBestFit = m_nMaxRating;
		p = g_pChar->m_olSourceCode.GetHeadPosition();
		while (p != NULL)
		{
			pSource = (CSource *)g_pChar->m_olSourceCode.GetNext(p);

			// Find program/chip of the the same type
			if ((pSource->m_nType == m_type) &&
				(pSource->m_nClass == nClass))
			{
				if ((pSource->m_nRating >= nRating) &&
					(pSource->m_nRating < m_nMaxRating))
				{
					nBestFit = pSource->m_nRating;
				}
			}
		}

		// Use difference of ratings for complexity time calculation
		m_nBaseTime = nComplexity * (nBestFit - nRating);

		// Set minimum time to 1
		if (m_nBaseTime <= 0)
		{
			m_nBaseTime = 1;
		}
	}

	// Base time is now computed. Divide base time by skill (including design assistant)
	int nSkill = m_nSkill * (1 + g_pChar->m_nHardware[HW_DESIGN_ASSIST]);
	m_time.Format("%d", ((m_nBaseTime + nSkill - 1) / nSkill));

	UpdateData(FALSE);
}

void CNewProjectDlg::OnOK() 
{
	int nClass = m_Class.GetItemData(m_class);

	// If another project is active, confirm its cancellation
	if (g_pChar->m_nProjectType != -1)
	{
		if (DeckerMessageBox("You already have a project. Do you wish to cancel it to start this one?",MB_YESNO)==IDNO)
		{
			return;
		}
	}

	// Start the new project
	g_pChar->m_nProjectType = m_type;
	g_pChar->m_nProjectClass = nClass;
	g_pChar->m_nProjectRating = m_rating + 1;
	g_pChar->m_nProjectInitialTime = m_nBaseTime;
	g_pChar->m_nProjectTimeLeft = m_nBaseTime;
	
	CDialog::OnOK();
}

void CNewProjectDlg::OnSelchangeProjType() 
{
	UpdateData(TRUE);
	FillInClassList();
}

void CNewProjectDlg::OnSelchangeProjClass() 
{
	UpdateData(TRUE);
	FillInRatingList();
}

void CNewProjectDlg::OnSelchangeProjRating() 
{
	UpdateData(TRUE);
	FillInMisc();
}


BOOL CNewProjectDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	//return CDialog::OnHelpInfo(pHelpInfo);
	WinHelp(HID_PROGRAM);
	return TRUE;
}
