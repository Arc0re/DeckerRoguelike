// FinalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "setup.h"
#include "FinalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern BOOL g_bSetupSuccessful;

/////////////////////////////////////////////////////////////////////////////
// CFinalDlg property page

IMPLEMENT_DYNCREATE(CFinalDlg, CPropertyPage)

CFinalDlg::CFinalDlg() : CPropertyPage(CFinalDlg::IDD)
{
	//{{AFX_DATA_INIT(CFinalDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFinalDlg::~CFinalDlg()
{
}

void CFinalDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFinalDlg)
	DDX_Control(pDX, IDC_FINAL_TEXT, m_Final_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFinalDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CFinalDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinalDlg message handlers

BOOL CFinalDlg::OnSetActive() 
{
	((CPropertySheet *)GetParent())->SetWizardButtons(PSWIZB_FINISH);

	if (!g_bSetupSuccessful)
	{
		m_Final_Text.SetWindowText("Setup has terminated unsuccessfully.");
	}
	
	return CPropertyPage::OnSetActive();
}
