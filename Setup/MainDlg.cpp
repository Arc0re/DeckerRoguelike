// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "setup.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDlg property page

IMPLEMENT_DYNCREATE(CMainDlg, CPropertyPage)

CMainDlg::CMainDlg() : CPropertyPage(CMainDlg::IDD)
{
	//{{AFX_DATA_INIT(CMainDlg)
	//}}AFX_DATA_INIT
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_WELCOME_TEXT, m_WelcomeText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CMainDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

BOOL CMainDlg::OnSetActive() 
{
	((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_NEXT);

	return CPropertyPage::OnSetActive();
}

LRESULT CMainDlg::OnWizardNext() 
{

	return CPropertyPage::OnWizardNext();
}
