//////////////////////////////////////////////////////////////////////
//Decker - A Cyberspace RPG
//Copyright (C) 2001  Shawn Overcash
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

// MapView.cpp: implementation of the CMapView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "MapView.h"
#include "Character.h"
#include "System.h"
#include "Area.h"
#include "Node.h"
#include "MatrixView.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

IMPLEMENT_DYNCREATE(CMapView, CScrollView)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapView::CMapView()
{
	m_bUseBig = FALSE;
	m_nGridSize = 16;
	m_ilNodesLarge.Create(IDB_NODE_IL_32,32,0,BLACK);
	m_ilNodesSmall.Create(IDB_NODE_IL_16,16,0,BLACK);
	m_ilSpecialNodesLarge.Create(IDB_IL_SPECIAL_NODES_32,32,0,BLACK);
	m_ilSpecialNodesSmall.Create(IDB_IL_SPECIAL_NODES_16,16,0,BLACK);
}

CMapView::~CMapView()
{
}

BEGIN_MESSAGE_MAP(CMapView,CScrollView )
	//{{AFX_MSG_MAP(CMapView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CMapView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView ::OnCreate(lpCreateStruct) == -1)
		return -1;

	RedoSize();

	ScrollToCurrentNode();

	return 0;
}

void CMapView::OnDraw(CDC* pDC) 
{
	POSITION p;
	CNode *pNode;
	CImageList *pImageList;
	CImageList *pSpecialImageList;
	int iCenterOffset;
	int x,y;
	POINT ptPoint;
	CArea *pArea = g_pChar->m_pCurrentNode->m_pParentArea;

	pDC->SetMapMode(MM_TEXT);
	
	// Black out the background
	pDC->FillSolidRect(0,0,m_sizeTotal.cx-1,m_sizeTotal.cy-1,BLACK);

	// Get the image list and size
	if (m_bUseBig)
	{
		pImageList = &m_ilNodesLarge;
		pSpecialImageList = &m_ilSpecialNodesLarge;
	}
	else
	{
		pImageList = &m_ilNodesSmall;
		pSpecialImageList = &m_ilSpecialNodesSmall;
	}
	iCenterOffset = (m_nGridSize/2);

	// Set all nodes to 'not processed'
	pArea->ResetProcessedFlags();

	// Go through and draw everything
	p = pArea->m_olNodeList.GetHeadPosition();
	while (p!=NULL)
	{
		// Get the node
		pNode = (CNode *)(pArea->m_olNodeList.GetNext(p));

		// If the node has not been mapped, skip it
		if (!pNode->m_bMapped) continue;

		// Get the coordinates of the center
		// Note that we add half of the view size because we
		//  want this much of a border in order to be able
		//  to center the map on the node.
		ptPoint.x = (m_szBorderOffset.cx) + (pNode->m_ptLocation.x*m_nGridSize);
		ptPoint.y = (m_szBorderOffset.cy) + (pNode->m_ptLocation.y*m_nGridSize);
		x = ptPoint.x + iCenterOffset;
		y = ptPoint.y + iCenterOffset;

		// Process north line
		if ((pNode->m_pAdjNode[0]!=NULL)&&(!pNode->m_bProcessed[0]))
		{
			// Mark both nodes as processed
			pNode->m_bProcessed[0] = 1;
			pNode->m_pAdjNode[0]->m_bProcessed[2] = 1;

			// We must go up a notch (North) to get the start point
			int y2 = y - m_nGridSize;

			// Draw the outer rectangle
			pDC->FillSolidRect(x-2,y2,4,m_nGridSize,BLUE);

			// Draw the inside rectangle
			pDC->FillSolidRect(x-1,y2,2,m_nGridSize,DK_AQUA);
		}

		// Process east line
		if ((pNode->m_pAdjNode[1]!=NULL)&&(!pNode->m_bProcessed[1]))
		{
			// Mark both nodes as processed
			pNode->m_bProcessed[1] = 1;
			pNode->m_pAdjNode[1]->m_bProcessed[3] = 1;

			// Draw the outer rectangle
			pDC->FillSolidRect(x,y-2,m_nGridSize,4,BLUE);

			// Draw the inside rectangle
			pDC->FillSolidRect(x,y-1,m_nGridSize,2,DK_AQUA);
		}

		// Process south line
		if ((pNode->m_pAdjNode[2]!=NULL)&&(!pNode->m_bProcessed[2]))
		{
			// Mark both nodes as processed
			pNode->m_bProcessed[2] = 1;
			pNode->m_pAdjNode[2]->m_bProcessed[0] = 1;

			// Draw the outer rectangle
			pDC->FillSolidRect(x-2,y,4,m_nGridSize,BLUE);

			// Draw the inside rectangle
			pDC->FillSolidRect(x-1,y,2,m_nGridSize,DK_AQUA);
		}

		// Process west line
		if ((pNode->m_pAdjNode[3]!=NULL)&&(!pNode->m_bProcessed[3]))
		{
			// Mark both nodes as processed
			pNode->m_bProcessed[3] = 1;
			pNode->m_pAdjNode[3]->m_bProcessed[1] = 1;

			// We must go left a notch (West) to get the start point
			int x2 = x - m_nGridSize;

			// Draw the outer rectangle
			pDC->FillSolidRect(x2,y-2,m_nGridSize,4,BLUE);

			// Draw the inside rectangle
			pDC->FillSolidRect(x2,y-1,m_nGridSize,2,DK_AQUA);
		}

		// Now, draw the icon
		if (pNode->m_nSpecialImage == -1) //FSO 6-20-01
		{
			// Normal node
			pImageList->Draw(pDC, pNode->m_nType, ptPoint, ILD_NORMAL);
		}
		else
		{
			pSpecialImageList->Draw(pDC, pNode->m_nSpecialImage, ptPoint, ILD_NORMAL);
		}

		// Draw the cursor
		if (pNode == g_pChar->m_pCurrentNode)
		{
			CPoint ptTmpPoint;

			pDC->SelectStockObject(WHITE_PEN);

			// Draw the vertical line
			ptTmpPoint.x = x;
			ptTmpPoint.y = y-3;
			pDC->MoveTo(ptTmpPoint);

			ptTmpPoint.y+=7;
			pDC->LineTo(ptTmpPoint);

			// Draw the horizontal line
			ptTmpPoint.x = x-3;
			ptTmpPoint.y = y;
			pDC->MoveTo(ptTmpPoint);

			ptTmpPoint.x+=7;
			pDC->LineTo(ptTmpPoint);

		}
	}
}

void CMapView::RedoSize()
{
	CArea *pArea = g_pChar->m_pCurrentNode->m_pParentArea;

	// Note that we add the view size to the total because we
	//  want a border in order to be able
	//  to center the map on the node.
	m_sizeTotal.cx = (pArea->m_nSizeX * m_nGridSize) + MAP_SIZE_X;
	m_sizeTotal.cy = (pArea->m_nSizeY * m_nGridSize) + MAP_SIZE_Y;
	SetScrollSizes(MM_TEXT, m_sizeTotal);

	// Recalculate the viewable window size
	CSize szScroll;
	GetScrollBarSizes(szScroll);

	m_szBorderOffset.cx = ((MAP_SIZE_X - GetSystemMetrics(SM_CXVSCROLL)) - m_nGridSize)/2;
	m_szBorderOffset.cy = ((MAP_SIZE_Y - GetSystemMetrics(SM_CYHSCROLL)) - m_nGridSize)/2;
}

void CMapView::ScrollToCurrentNode()
{
	CNode *pNode = g_pChar->m_pCurrentNode;
	CPoint ptPoint;

	// Get the location of this node
	ptPoint.x = (pNode->m_ptLocation.x*m_nGridSize);
	ptPoint.y = (pNode->m_ptLocation.y*m_nGridSize);

	// Try to center this on the screen
	ScrollToPosition(ptPoint);
}

void CMapView::DoMove()
{
	ScrollToCurrentNode();
	RedrawWindow();
}

void CMapView::DoZoom()
{
	// Toggle the zoom modes
	if (m_bUseBig)
	{
		m_bUseBig = FALSE;
		m_nGridSize = 16;
	}
	else
	{
		m_bUseBig = TRUE;
		m_nGridSize = 32;
	}

	// Resize the window
	RedoSize();
	ScrollToCurrentNode();
	RedrawWindow();
}
