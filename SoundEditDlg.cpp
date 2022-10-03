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

// SoundEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "SoundEditDlg.h"
#include "DeckerSound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundEditDlg dialog


CSoundEditDlg::CSoundEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundEditDlg)
	m_selected_sound = -1;
	//}}AFX_DATA_INIT
}


void CSoundEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundEditDlg)
	DDX_Control(pDX, IDC_SOUND_PLAY, m_Sound_Play);
	DDX_Control(pDX, IDC_SOUND_LIST, m_Sound_List);
	DDX_LBIndex(pDX, IDC_SOUND_LIST, m_selected_sound);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSoundEditDlg)
	ON_LBN_SELCHANGE(IDC_SOUND_LIST, OnSelchangeSoundList)
	ON_BN_CLICKED(IDC_SOUND_PLAY, OnSoundPlay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundEditDlg message handlers

BOOL CSoundEditDlg::OnInitDialog() 
{
	CString szTmp;

	CDialog::OnInitDialog();

	// Fill in the sound list
	m_Sound_List.AddString("None");
	m_Sound_List.AddString("Default");
	for (int i = 1; i <= 20; i++)
	{
		szTmp.Format("Custom Sound %d", i);
		m_Sound_List.AddString(szTmp);
	}

	// Select the sound
	if (m_nSound == -1)
	{
		m_selected_sound = 0;
	}
	else if (m_nSound == m_nDefault)
	{
		m_selected_sound = 1;
	}
	else
	{
		m_selected_sound = 2 + (m_nSound - SOUND_CUSTOM1);
	}

	UpdateData(FALSE);

	m_Sound_Play.EnableWindow(m_selected_sound);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundEditDlg::OnSelchangeSoundList() 
{
	UpdateData(TRUE);

	m_Sound_Play.EnableWindow(m_selected_sound);	
}

void CSoundEditDlg::OnSoundPlay() 
{
	if (m_selected_sound == 1)
	{
		PlayGameSound(m_nDefault);
	}
	else
	{
		PlayGameSound(SOUND_CUSTOM1 + (m_selected_sound-2));
	}
}

void CSoundEditDlg::OnOK() 
{
	UpdateData(TRUE);

	if (m_selected_sound == 0)
	{
		m_nSound = -1;
	}
	else if (m_selected_sound == 1)
	{
		m_nSound = m_nDefault;
	}
	else
	{
		m_nSound = SOUND_CUSTOM1 + (m_selected_sound-2);
	}
	
	CDialog::OnOK();
}

void CSoundEditDlg::OnCancel() 
{
	CDialog::OnCancel();
}
