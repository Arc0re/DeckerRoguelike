#if !defined(AFX_DESTINATIONDLG_H__FC4CF6C3_53B8_11D2_8058_00104B751D91__INCLUDED_)
#define AFX_DESTINATIONDLG_H__FC4CF6C3_53B8_11D2_8058_00104B751D91__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DestinationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDestinationDlg dialog

class CDestinationDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CDestinationDlg)

// Construction
public:
	CDestinationDlg();
	~CDestinationDlg();

// Dialog Data
	//{{AFX_DATA(CDestinationDlg)
	enum { IDD = IDD_DESTINATION_DLG };
	CString	m_destination;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDestinationDlg)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDestinationDlg)
	afx_msg void OnBrowseButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESTINATIONDLG_H__FC4CF6C3_53B8_11D2_8058_00104B751D91__INCLUDED_)
