// ProgramFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "setup.h"
#include "ProgramFolderDlg.h"
#include <shlobj.h> //For shell functions

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Globals
CString g_szFolder;

/////////////////////////////////////////////////////////////////////////////
// CProgramFolderDlg property page

IMPLEMENT_DYNCREATE(CProgramFolderDlg, CPropertyPage)

CProgramFolderDlg::CProgramFolderDlg() : CPropertyPage(CProgramFolderDlg::IDD)
{
	//{{AFX_DATA_INIT(CProgramFolderDlg)
	m_folder = _T("");
	//}}AFX_DATA_INIT
}

CProgramFolderDlg::~CProgramFolderDlg()
{
}

void CProgramFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgramFolderDlg)
	DDX_Text(pDX, IDC_PROGAM_FOLDER_FLD, m_folder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgramFolderDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CProgramFolderDlg)
	ON_BN_CLICKED(IDC_FOLDER_BROWSE_BTN, OnFolderBrowseBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgramFolderDlg message handlers

BOOL CProgramFolderDlg::OnSetActive() 
{
	UpdateData(FALSE);

	((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);
	
	return CPropertyPage::OnSetActive();
}

void CProgramFolderDlg::OnFolderBrowseBtn() 
{
	LPITEMIDLIST pidlFolder;
	BROWSEINFO BrInfo;
	char szFolderName[MAX_PATH];

	/* Get the StartMenu/Common programs folder information */
	//SHGetSpecialFolderLocation(m_hWnd, CSIDL_COMMON_PROGRAMS, &pidlFolder);
	SHGetSpecialFolderLocation(m_hWnd, CSIDL_PROGRAMS, &pidlFolder);
	

	ZeroMemory(&BrInfo, sizeof(BrInfo));
	BrInfo.hwndOwner        = m_hWnd;
	BrInfo.pidlRoot         = pidlFolder;
	BrInfo.pszDisplayName   = szFolderName;
	BrInfo.lpszTitle        = "Select the folder...";


	//use the shell's folder browser
	pidlFolder = SHBrowseForFolder(&BrInfo);

	//did the user select the cancel button
	if (pidlFolder!=NULL)
	{
		m_folder=szFolderName;
		UpdateData(FALSE);
	}
}

LRESULT CProgramFolderDlg::OnWizardNext() 
{
	UpdateData(TRUE);

	g_szFolder=m_folder;

	return CPropertyPage::OnWizardNext();
}
