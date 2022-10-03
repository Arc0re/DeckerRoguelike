#if !defined(AFX_PROGRAMFOLDERDLG_H__FC4CF6C6_53B8_11D2_8058_00104B751D91__INCLUDED_)
#define AFX_PROGRAMFOLDERDLG_H__FC4CF6C6_53B8_11D2_8058_00104B751D91__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProgramFolderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgramFolderDlg dialog

class CProgramFolderDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CProgramFolderDlg)

// Construction
public:
	CProgramFolderDlg();
	~CProgramFolderDlg();

// Dialog Data
	//{{AFX_DATA(CProgramFolderDlg)
	enum { IDD = IDD_PROGRAM_FOLDER_DLG };
	CString	m_folder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProgramFolderDlg)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProgramFolderDlg)
	afx_msg void OnFolderBrowseBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRAMFOLDERDLG_H__FC4CF6C6_53B8_11D2_8058_00104B751D91__INCLUDED_)
