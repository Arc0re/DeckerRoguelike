// MessageView.cpp : implementation file
//

#include "stdafx.h"
#include "decker.h"
#include "MessageView.h"
#include "Global.h"
#include "MatrixView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_MESSAGES	100
#define SCREEN_WIDTH	40

class CMessage : public CObject
{
public:
	CMessage() {};
	~CMessage() {};
	DECLARE_DYNCREATE(CMessage)

	CString m_szText;
	COLORREF m_crColor;
};

IMPLEMENT_DYNCREATE(CMessage, CObject)

/////////////////////////////////////////////////////////////////////////////
// CMessageView

IMPLEMENT_DYNCREATE(CMessageView, CScrollView)

CMessageView::CMessageView()
{
	m_popup_menu.LoadMenu(IDR_MESSAGEVIEW_POPUP);
	m_dx = 10;
	m_dy = 10;
}

CMessageView::~CMessageView()
{
	CMessage *pMsg;

	while (!m_olMessageList.IsEmpty())
	{
		pMsg = (CMessage *)m_olMessageList.RemoveHead();
		delete pMsg;
	}
}


BEGIN_MESSAGE_MAP(CMessageView, CScrollView)
	//{{AFX_MSG_MAP(CMessageView)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_COMMAND(ID_MSGVIEWMENU_CLEAR, OnClear)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageView drawing

void CMessageView::OnDraw(CDC* pDC) 
{
	POSITION p;
	CMessage *pMsg;
	RECT ClipRect;
	int iLine;
	int iLastLine;
	CMatrixView *pView = (CMatrixView *)GetParent();
	POINT pt;

	/* Figure out which area needs to be redrawn */
	pDC->GetClipBox(&ClipRect);
	iLine=(int)ClipRect.top/m_dy;
	iLastLine=(int)(ClipRect.bottom+m_dy-1)/m_dy;

	// Blank out the area
//	pDC->FillSolidRect(	ClipRect.left,
//						ClipRect.top,
//						1 + ClipRect.right - ClipRect.left,
//						1 + ClipRect.bottom - ClipRect.top,
//						WHITE);
	pt.x = 0;
	pt.y = 0;
	MapWindowPoints(pView, &pt, 1);
	pDC->BitBlt(ClipRect.left,
				ClipRect.top,
				ClipRect.right - ClipRect.left,
				ClipRect.bottom - ClipRect.top,
				&pView->m_dcBackground,pt.x,pt.y,SRCCOPY);

	// Write out the strings
	p = m_olMessageList.FindIndex(iLine);
	while ((iLine<=iLastLine) && (p!=NULL))
	{
		// Get the message
		pMsg = (CMessage *)m_olMessageList.GetNext(p);

		//// Blank out the area
		//pDC->FillSolidRect(0,iLine*m_dy,m_sizeTotal.cx,m_dy,WHITE);

		// Set the color
		pDC->SetTextColor(pMsg->m_crColor);

		// Draw the message
		pDC->TextOut(0,iLine*m_dy,pMsg->m_szText);

		// Go to the next line
		iLine++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMessageView message handlers

int CMessageView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Get the text sizes
	CDC *pDC;
	TEXTMETRIC tm;

	pDC=GetDC();
	pDC->GetTextMetrics(&tm);
	m_dy=(int)(tm.tmHeight+tm.tmExternalLeading);
	m_dx=(int)tm.tmAveCharWidth;

	// Set the scrolling sizes
	RedoSize();

	return 0;
}

void CMessageView::AddMessage(const char *szStr, COLORREF crColor)
{
	CMessage *pMessage;
	BOOL bResize;

	// See if we are at the message limit
	if (m_olMessageList.GetCount() == MAX_MESSAGES)
	{
		// Reuse the first message
		pMessage = (CMessage *)m_olMessageList.RemoveHead();
		bResize = FALSE;
	}
	else
	{
		// Allocate a new message
		pMessage = new CMessage;
		bResize = TRUE;
	}

	// Set the information
	pMessage->m_szText = szStr;
	pMessage->m_crColor = crColor;

	// Add the message to the end of the list
	m_olMessageList.AddTail(pMessage);

	// Resize the display
	if (bResize)
	{
		RedoSize();
	}

	// Scroll to the end
	ScrollToBottom();

	RedrawWindow();
}

void CMessageView::RedoSize()
{
	int iLines;
	int nMaxSize;
	POSITION p;
	CMessage *pMessage;
	CDC *pDC;
	CSize sSize;

	// Get the DC
	pDC = GetDC();

	// Get the number of lines
	iLines = m_olMessageList.GetCount();
	if (iLines<1) iLines = 1;

	// FSO 12-17-01 
	// Find max message size
	nMaxSize = 1;
	p = m_olMessageList.GetHeadPosition();
	while (p != NULL)
	{
		pMessage = (CMessage *)m_olMessageList.GetNext(p);

		sSize = pDC->GetTextExtent(pMessage->m_szText);
		if (sSize.cx > nMaxSize)
		{
			nMaxSize = sSize.cx;
		}
	}

	// Set vertical to scroll to number of lines,
	//FSO 12-17-01
	// Set horizontal to max width
	//m_sizeTotal.cx = m_dx * SCREEN_WIDTH;
	m_sizeTotal.cx = nMaxSize;

	m_sizeTotal.cy = m_dy * iLines;

	SetScrollSizes(MM_TEXT, m_sizeTotal);
}

void CMessageView::ScrollToBottom()
{
	POINT pt;
	RECT ClientRect;

	GetClientRect(&ClientRect);
	if (ClientRect.bottom < m_sizeTotal.cy)
	{
		pt.x = 0;
		pt.y = (m_sizeTotal.cy - ClientRect.bottom);
		ScrollToPosition(pt);
	}
}

void CMessageView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CMenu* pPopup;
	CPoint Point;

	/* Get the position of the cursor */
	GetCursorPos(&Point);

	/* Load the popup menu */
	pPopup = m_popup_menu.GetSubMenu(0);

	/* Display the menu */
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this);

	//CScrollView::OnRButtonUp(nFlags, point);
}

void CMessageView::OnClear()
{
	CMessage *pMsg;

	while (!m_olMessageList.IsEmpty())
	{
		pMsg = (CMessage *)m_olMessageList.RemoveHead();
		delete pMsg;
	}

	RedoSize();
	RedrawWindow();
}

BOOL CMessageView::OnEraseBkgnd(CDC* pDC) 
{
////	return CScrollView::OnEraseBkgnd(pDC);
	CMatrixView *pView = (CMatrixView *)GetParent();
	POINT pt, pt2;
	RECT rectClient;
	GetClientRect(&rectClient);

	/* Figure out which area needs to be redrawn */
	pt.x = 0;
	pt.y = 0;
	MapWindowPoints(pView, &pt, 1);
	pt2 = GetDeviceScrollPosition();

	pDC->BitBlt(pt2.x,
				pt2.y,
				1 + pt2.x - (rectClient.right - rectClient.left),
				1 + pt2.y - (rectClient.bottom - rectClient.top),
				&pView->m_dcBackground,pt.x,pt.y,SRCCOPY);

	RedrawWindow();
	return TRUE;
}
