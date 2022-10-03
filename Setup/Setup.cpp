// Setup.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Setup.h"

#include "MainDlg.h"
#include "DestinationDlg.h"
#include "ProgramFolderDlg.h"
#include "ConfirmDlg.h"
#include "FinalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Globals
CString g_szPath;

//Static functions
static void GetSourceDirectory();
static BOOL ReadRegistry(const char *szKey, CString &szValue);


/////////////////////////////////////////////////////////////////////////////
// CSetupApp

BEGIN_MESSAGE_MAP(CSetupApp, CWinApp)
	//{{AFX_MSG_MAP(CSetupApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupApp construction

CSetupApp::CSetupApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSetupApp object

CSetupApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSetupApp initialization

BOOL CSetupApp::InitInstance()
{
	CPropertySheet psPropSheet("Decker Setup");
	
	CMainDlg dlgMain;
	CDestinationDlg dlgDestination;
	CProgramFolderDlg dlgProgramFolder;
	CConfirmDlg dlgConfirm;
	CFinalDlg dlgFinal;

	//TCHAR szDefaultFolderName[MAX_PATH],


	// Standard initialization
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	/* Put the property sheet in wizard (install) mode */
	psPropSheet.SetWizardMode();

	/* Add the property pages */
	psPropSheet.AddPage(&dlgMain);
	psPropSheet.AddPage(&dlgDestination);
	psPropSheet.AddPage(&dlgProgramFolder);
	psPropSheet.AddPage(&dlgConfirm);
	psPropSheet.AddPage(&dlgFinal);

	/* Get the source directory */
	GetSourceDirectory();

	/* Initialize the pages */
	dlgDestination.m_destination="C:\\Program Files\\Decker";
	dlgProgramFolder.m_folder="Decker";

	/* Get values from the registry, if they exist */
	ReadRegistry("Directory", dlgDestination.m_destination);
	ReadRegistry("Program Folder", dlgProgramFolder.m_folder);

	/* Run the sheet */
	psPropSheet.DoModal();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

static void GetSourceDirectory()
{
	char szPath[MAX_PATH];
	int i;

	GetModuleFileName(NULL,szPath,MAX_PATH);

	i=strlen(szPath);
	while ((i>0)&&(szPath[i-1]!='\\'))
	{
		szPath[--i]=0;
	}
	g_szPath=szPath;
}

static BOOL ReadRegistry(const char *szKey, CString &szValue)
{
	HKEY hKeyValue;
	DWORD dwRC, dwType, dwSize;
	CHAR szBuffer[256];

	/* Open the registry key */
	dwRC = RegOpenKeyEx(	HKEY_LOCAL_MACHINE, 
					"Software\\Decker",
					0,
					KEY_READ,
  					&hKeyValue);

	if (dwRC == ERROR_SUCCESS)
	{
		/* Read the value */
		dwSize = sizeof(szBuffer);
	
		dwRC = RegQueryValueEx(	hKeyValue,
					szKey,
					NULL,
  					&dwType,
  					(BYTE *)szBuffer,
  					&dwSize);
		RegCloseKey(hKeyValue);

		if ((dwRC == ERROR_SUCCESS) && (dwType==REG_SZ))
		{
			szValue = szBuffer;
			return(TRUE);
		}
	}

	return(FALSE);
}
