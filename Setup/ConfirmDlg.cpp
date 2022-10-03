// ConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "setup.h"
#include "ConfirmDlg.h"
#include <setupapi.h>	//Setup API
#include <shlobj.h>	//Shell API
#include <objbase.h>	//COM

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString g_szDestination;
extern CString g_szFolder;
extern CString g_szPath;

BOOL g_bSetupSuccessful;

static BOOL DoInstall(HWND hParentWin);
static BOOL AddItemsToStartMenu();
static BOOL AddItemsToRegistry(const char *szKey, const char *szValue); //FSO 11-30-99
static BOOL CreateLink(const char *szSource, const char *szTarget,
		       const char *szDesc, const char *szGroup,
		       const char *szWorkingDirectory);
static LRESULT WINAPI DriverQueueCallback(PVOID Context, UINT Notification, UINT Param1, UINT Param2);

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg property page

IMPLEMENT_DYNCREATE(CConfirmDlg, CPropertyPage)

CConfirmDlg::CConfirmDlg() : CPropertyPage(CConfirmDlg::IDD)
{
	//{{AFX_DATA_INIT(CConfirmDlg)
	m_setup_info = _T("");
	//}}AFX_DATA_INIT
}

CConfirmDlg::~CConfirmDlg()
{
}

void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfirmDlg)
	DDX_Text(pDX, IDC_SETUP_INFO, m_setup_info);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfirmDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CConfirmDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg message handlers

BOOL CConfirmDlg::OnSetActive() 
{
	((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);

	/* Fill in the information */
	m_setup_info.Format("Destination:\t%s\r\nFolder:\t\t%s",
		g_szDestination, g_szFolder);
	UpdateData(FALSE);
	
	return CPropertyPage::OnSetActive();
}

LRESULT CConfirmDlg::OnWizardNext() 
{
	/* Start installation */
	g_bSetupSuccessful=DoInstall(GetParent()->m_hWnd);
	
	return CPropertyPage::OnWizardNext();
}


static BOOL DoInstall(HWND hParentWin)
{
	HINF		hInf;
	CString		szTmp;
	BOOL		bResult;
	PVOID		pContext;

	/* Open the INF file */
	szTmp=g_szPath;
	szTmp+="Decker.inf";
	hInf = SetupOpenInfFile(szTmp, NULL, INF_STYLE_WIN4, NULL);
	if (hInf == INVALID_HANDLE_VALUE) 
	{
		AfxMessageBox("Error: Could not open the INF file.");
		return FALSE;
	}

	/* Get a handle to the destination directory */
	bResult = SetupSetDirectoryId(hInf, 32768, g_szDestination);
	if (!bResult) 
	{
		AfxMessageBox("Error: Could not associate a directory ID with the destination directory.");
		SetupCloseInfFile(hInf);
		return FALSE;
	}

	/* Give setup a window handle to use as dialog box parent */
	pContext=SetupInitDefaultQueueCallback(hParentWin);

	/* Perform the install steps */
	bResult=SetupInstallFromInfSection(
		hParentWin,		// optional, handle of a parent window 
		hInf,			// handle to the INF file 
		"Install",		// section of the INF file to install 
		SPINST_ALL,		// which lines to install from section 
		NULL,			// optional, key for registry installs 
		g_szPath,		// optional, path for source files 
		0,			// optional, specifies copy behavior 
		(PSP_FILE_CALLBACK)DriverQueueCallback, // optional, specifies callback routine 
		pContext,		// optional, callback routine context 
		NULL,			// optional, device information set 
		NULL			// optional, device info structure 
		);
	if (!bResult)
	{
		SetupTermDefaultQueueCallback(pContext);
		SetupCloseInfFile(hInf);
		return FALSE;
	}
	//HSPFILEQ hFileQueue;
	//hFileQueue = SetupOpenFileQueue();
	//
	//bResult=SetupInstallFilesFromInfSection(
	//	hInf,			// handle to the INF file
	//	NULL,			// optional, layout INF handle
	//	hFileQueue,		// handle to the file queue
	//	TEXT("Install"),	// name of the Install section
	//	g_szPath,		// optional, root path to source files
	//	0);			// optional, specifies copy behavior
	//if (!bResult)
	//{
	//	SetupTermDefaultQueueCallback(pContext);
	//	SetupCloseInfFile(hInf);
	//	SetupCloseFileQueue(hFileQueue);
	//	return FALSE;
	//}
	//bResult = SetupCommitFileQueue(
	//		hParentWin,
	//		hFileQueue,
	//		(PSP_FILE_CALLBACK)DriverQueueCallback, 
	//		pContext);
	//if (!bResult)
	//{
	//	SetupTermDefaultQueueCallback(pContext);
	//	SetupCloseInfFile(hInf);
	//	SetupCloseFileQueue(hFileQueue);
	//	return FALSE;
	//}
	//SetupCloseFileQueue(hFileQueue);

	/* Add the items to the start menu */
	if (!AddItemsToStartMenu())
	{
		AfxMessageBox("Warning: Program items could not be created.");
	}
	
	SetupTermDefaultQueueCallback(pContext);
	SetupCloseInfFile(hInf);

	return TRUE;
}


static LRESULT WINAPI DriverQueueCallback(PVOID Context, UINT Notification, UINT Param1, UINT Param2)
{	
	return(SetupDefaultQueueCallback(Context, Notification, Param1, Param2));
}


static BOOL AddItemsToStartMenu()
{
	char	szProgramsPath[MAX_PATH];
	CString szSource;
	CString szTarget;
	CString szGroup;
	LPITEMIDLIST pidlPrograms;
	BOOL    bRet = TRUE;
	HRESULT hres;

	/* Initialize COM */
	hres = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (!SUCCEEDED(hres)) return FALSE;

	/* Get the program folders location */
	SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &pidlPrograms);

	/* Get the program folders path */
	SHGetPathFromIDList(pidlPrograms, szProgramsPath);

	/* Save the information in the registry */
	AddItemsToRegistry("Directory",g_szDestination);
	AddItemsToRegistry("Program Folder",g_szFolder);

	/* Create the program file link */
	szSource.Format("%s\\Decker.exe", g_szDestination);
	szGroup.Format("%s\\%s", szProgramsPath, g_szFolder);
	szTarget.Format("%s\\Decker.lnk", szGroup);

	if (!CreateLink(szSource, szTarget, "Decker", szGroup, g_szDestination))
	{
		MessageBox(NULL,"CreateLink failed", "Setup", MB_OK);
		CoUninitialize();       
		return FALSE;
	}

	/* Create the help file link */
	szSource.Format("%s\\Decker.hlp", g_szDestination);
	szGroup.Format("%s\\%s", szProgramsPath, g_szFolder);
	szTarget.Format("%s\\Decker Help.lnk", szGroup);

	if (!CreateLink(szSource, szTarget, "Decker Help", szGroup, g_szDestination))
	{
		MessageBox(NULL,"CreateLink failed", "Setup", MB_OK);
		CoUninitialize();       
		return FALSE;
	}


	/* Uninitialize COM */
	CoUninitialize();       

	return(TRUE);
}

static BOOL AddItemsToRegistry(const char *szKey, const char *szValue)
{
	HKEY hKeyValue;

	DWORD dwReturnCode,
		  dwDisposition,
		  dwSize;

	dwReturnCode = RegCreateKeyEx(HKEY_LOCAL_MACHINE, 
				      "Software\\Decker", 				      0, 
				      NULL,
				      REG_OPTION_NON_VOLATILE, 
				      KEY_WRITE,
				      NULL, 
				      &hKeyValue, 
				      &dwDisposition);

	if (ERROR_SUCCESS == dwReturnCode)
	{
		dwSize = strlen(szValue);
	
		RegSetValueEx(hKeyValue,
			     szKey, 
			     0,
			     REG_SZ,
			     (CONST BYTE *)szValue,
			     dwSize);
		
		RegCloseKey(hKeyValue);

		return(TRUE);
	}

	return(FALSE);
}



static BOOL CreateLink(const char *szSource, const char *szTarget,
		       const char *szDesc, const char *szGroup,
		       const char *szWorkingDirectory) 
{ 
	BOOL    bReturn = TRUE;
	HRESULT hres; 
	IShellLink *pShellLink; 

	if (NULL != szGroup)
	{
		//
		//      Make sure the group exists
		//
		CreateDirectory(szGroup, NULL);

		//
		//      Notify the shell that we made a change
		//
		SHChangeNotify(SHCNE_MKDIR, SHCNF_PATH, szGroup, 0);
	}


	//
	//      Get a pointer to the IShellLink interface. 
	//
	hres = CoCreateInstance(CLSID_ShellLink, 
				    NULL, 
					CLSCTX_INPROC_SERVER, 
					    IID_IShellLink, 
						(LPVOID *)&pShellLink); 
	if (SUCCEEDED(hres)) 
	{ 
		IPersistFile *pPersistFile; 

		// 
		//      Set the path to the shortcut target, and add the description. 
		//
		pShellLink->SetPath(szSource); 
		pShellLink->SetDescription(szDesc); 
		pShellLink->SetWorkingDirectory(szWorkingDirectory);
		
		// 
		//      Query IShellLink for the IPersistFile interface for saving the 
		//  shortcut in persistent storage. 
		//
		hres = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPersistFile); 
		if (SUCCEEDED(hres)) 
		{ 
			WCHAR wsz[MAX_PATH]; 
		
			// 
			//      Ensure that the string is ANSI. 
			//
			MultiByteToWideChar(CP_ACP, 0, szTarget, -1, wsz, MAX_PATH); 
			
			
			// 
			//      Save the link by calling IPersistFile::Save. 
			//
			hres = pPersistFile->Save(wsz, TRUE); 
			if (FAILED(hres))
			{
				bReturn = FALSE;
			}

			pPersistFile->Release(); 
		}
		else
		{
			bReturn = FALSE;
		}

		pShellLink->Release(); 
	} 
	else
	{
		bReturn = FALSE;
	}

	return(bReturn); 
}
