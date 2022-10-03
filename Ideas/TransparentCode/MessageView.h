#if !defined(AFX_MESSAGEVIEW_H__FF21F1E1_8821_11D4_B835_0080C8C1DB04__INCLUDED_)
#define AFX_MESSAGEVIEW_H__FF21F1E1_8821_11D4_B835_0080C8C1DB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageView view

class CMessageView : public CScrollView
{
public:
	CMessageView();           // protected constructor used by dynamic creation
	virtual ~CMessageView();
	DECLARE_DYNCREATE(CMessageView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void RedoSize();
	void ScrollToBottom();

	CObList m_olMessageList;
	int m_dx;
	int m_dy;
	CSize m_sizeTotal;
	CMenu m_popup_menu;

public:
	void OnClear();
	void AddMessage(const char *szStr, COLORREF crColor);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEVIEW_H__FF21F1E1_8821_11D4_B835_0080C8C1DB04__INCLUDED_)
