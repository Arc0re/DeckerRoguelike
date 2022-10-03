// Setup.h : main header file for the SETUP application
//

#if !defined(AFX_SETUP_H__FC4CF694_53B8_11D2_8058_00104B751D91__INCLUDED_)
#define AFX_SETUP_H__FC4CF694_53B8_11D2_8058_00104B751D91__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSetupApp:
// See Setup.cpp for the implementation of this class
//

class CSetupApp : public CWinApp
{
public:
	CSetupApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSetupApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUP_H__FC4CF694_53B8_11D2_8058_00104B751D91__INCLUDED_)
