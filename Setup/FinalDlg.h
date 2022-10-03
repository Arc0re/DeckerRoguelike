#if !defined(AFX_FINALDLG_H__F7628283_5470_11D2_8059_00104B751D91__INCLUDED_)
#define AFX_FINALDLG_H__F7628283_5470_11D2_8059_00104B751D91__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FinalDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFinalDlg dialog

class CFinalDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CFinalDlg)

// Construction
public:
	CFinalDlg();
	~CFinalDlg();

// Dialog Data
	//{{AFX_DATA(CFinalDlg)
	enum { IDD = IDD_FINAL_DLG };
	CStatic	m_Final_Text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFinalDlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFinalDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINALDLG_H__F7628283_5470_11D2_8059_00104B751D91__INCLUDED_)
