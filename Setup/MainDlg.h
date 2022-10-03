#if !defined(AFX_MAINDLG_H__FC4CF6C5_53B8_11D2_8058_00104B751D91__INCLUDED_)
#define AFX_MAINDLG_H__FC4CF6C5_53B8_11D2_8058_00104B751D91__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog

class CMainDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CMainDlg)

// Construction
public:
	CMainDlg();
	~CMainDlg();

// Dialog Data
	//{{AFX_DATA(CMainDlg)
	enum { IDD = IDD_MAIN_DLG };
	CStatic	m_WelcomeText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMainDlg)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMainDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__FC4CF6C5_53B8_11D2_8058_00104B751D91__INCLUDED_)
