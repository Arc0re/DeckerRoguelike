#if !defined(AFX_CONFIRMDLG_H__F7628282_5470_11D2_8059_00104B751D91__INCLUDED_)
#define AFX_CONFIRMDLG_H__F7628282_5470_11D2_8059_00104B751D91__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ConfirmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfirmDlg dialog

class CConfirmDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfirmDlg)

// Construction
public:
	CConfirmDlg();
	~CConfirmDlg();

// Dialog Data
	//{{AFX_DATA(CConfirmDlg)
	enum { IDD = IDD_CONFIRM_DLG };
	CString	m_setup_info;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfirmDlg)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfirmDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIRMDLG_H__F7628282_5470_11D2_8059_00104B751D91__INCLUDED_)
