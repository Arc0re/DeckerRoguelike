// DestinationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "setup.h"
#include "DestinationDlg.h"
#include "BrowseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Globals
CString g_szDestination;

//Static functions
static BOOL CreateDirectoryLayered(CString szDirectory); //FSO 11-30-99

/////////////////////////////////////////////////////////////////////////////
// CDestinationDlg property page

IMPLEMENT_DYNCREATE(CDestinationDlg, CPropertyPage)

CDestinationDlg::CDestinationDlg() : CPropertyPage(CDestinationDlg::IDD)
{
	//{{AFX_DATA_INIT(CDestinationDlg)
	m_destination = _T("");
	//}}AFX_DATA_INIT
}

CDestinationDlg::~CDestinationDlg()
{
}

void CDestinationDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDestinationDlg)
	DDX_Text(pDX, IDC_DESTINATION, m_destination);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDestinationDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CDestinationDlg)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON, OnBrowseButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDestinationDlg message handlers

BOOL CDestinationDlg::OnSetActive() 
{
	UpdateData(FALSE);

	((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);

	return CPropertyPage::OnSetActive();
}

void CDestinationDlg::OnBrowseButton() 
{
	CBrowseDlg dlgBrowse;
	dlgBrowse.m_path=m_destination;
	if (dlgBrowse.DoModal()==IDOK)
	{
		m_destination=dlgBrowse.m_path;
		UpdateData(FALSE);
	}
}

LRESULT CDestinationDlg::OnWizardNext() 
{
	/* Make sure the path is valid */
	CFileFind ffSearch;
	int iLength;

	/* Make sure the path is not empty */
	if (m_destination.IsEmpty())
	{
		AfxMessageBox("You must enter a destination directory.");
		return -1;
	}

	/* Remove a slash from the path */
	iLength=m_destination.GetLength();
	if (m_destination[iLength-1]=='\\')
	{
		m_destination=m_destination.Left(--iLength);
	}

	/* See if this is a drive letter */
	if ((iLength==2)&&(m_destination[1]==':'))
	{
		/* Have no way to know if this drive exists. */
		/* so assume it is ok.			     */
		return CPropertyPage::OnWizardNext();
	}

	/* Attempt to find the directory */
	if (!ffSearch.FindFile(m_destination, 0))
	{
		/* Directory does not exist */
		/* See if the user wants to try to create it */
		if (AfxMessageBox("Directory does not exist. Create it?",MB_YESNO|MB_ICONQUESTION)!=IDYES)
			return -1;

		/* Create the directory */
		//FSO 11-30-99
		//if (!CreateDirectory(m_destination,NULL))
		if (!CreateDirectoryLayered(m_destination))
		{
			AfxMessageBox("Could not create the directory.");
			return -1;
		}
	}
	else
	{
		/* Initial find was successful */
		/* Do second find, but it will fail if is directory */
		ffSearch.FindNextFile();

		/* Make sure it is a directory */
		if (!ffSearch.IsDirectory())
		{
			AfxMessageBox("This is not a valid directory.");
			return -1;
		}
		if (ffSearch.IsReadOnly())
		{
			AfxMessageBox("This directory is Read-Only.");
			return -1;
		}
	}

	g_szDestination=m_destination;
	
	return CPropertyPage::OnWizardNext();
}

static BOOL CreateDirectoryLayered(CString szDirectory)
{
	CFileFind ffSearch;
	CString szPartialDirectory;
	int nIndex;

	/* Process each subdirectory individually */
	nIndex = szDirectory.ReverseFind('\\');
	if ((nIndex!=-1)&&(szDirectory[nIndex-1]!=':'))
	{
		/* Make sure the parent deirectory exists */
		szPartialDirectory = szDirectory.Left(nIndex);
		if (!CreateDirectoryLayered(szPartialDirectory))
			return FALSE;
	}

	/* See if this directory exists */
	if (!ffSearch.FindFile(szDirectory, 0))
	{
		/* Create the directory */
		if (!CreateDirectory(szDirectory,NULL))
		{
			return FALSE;
		}
	}

	return TRUE;
}





