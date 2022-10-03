#if !defined(AFX_BROWSEDLG_H__FC4CF6C7_53B8_11D2_8058_00104B751D91__INCLUDED_)
#define AFX_BROWSEDLG_H__FC4CF6C7_53B8_11D2_8058_00104B751D91__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BrowseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrowseDlg dialog

class CBrowseDlg : public CDialog
{
// Construction
public:
	CBrowseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBrowseDlg)
	enum { IDD = IDD_BROWSE_DLG };
	CTreeCtrl	m_DirectoryTree;
	CString	m_path;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrowseDlg)
	afx_msg void OnSelchangedDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandingDirectoryTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	CImageList m_imglTreeImages;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSEDLG_H__FC4CF6C7_53B8_11D2_8058_00104B751D91__INCLUDED_)
