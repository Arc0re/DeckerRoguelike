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

// NodeView.cpp: implementation of the CNodeView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "NodeView.h"
#include "Character.h"
#include "System.h"
#include "Node.h"
#include "MatrixView.h"
#include "Global.h"
#include "Ice.h"
#include "DeckerGraphics.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

// Node grid starting coordinate
#define NODE_GRID_OFFSET	12	// Offset of grid
#define NODE_GRID_SIZE		24	// Size of node grid square

// Connection starting coordinates
#define LINE_N_X	107
#define LINE_N_Y	0
#define LINE_E_X	227
#define LINE_E_Y	107
#define LINE_S_X	107
#define LINE_S_Y	227
#define LINE_W_X	0
#define LINE_W_Y	107

// Size of line bitmaps
#define LINE_WIDTH	26	// cy for NS, cx for EW
#define LINE_LENGTH	13	// cx for NS, cy for EW

// Miscellaneous images
#define MISC_TARGET	0
#define MISC_DAMAGE	1
#define MISC_QUERY	2
#define MISC_ALERT	3
#define MISC_BYPASS	4

#define NUM_MISC_IMAGES	5

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeView::CNodeView()
{
	HINSTANCE hInst = AfxGetInstanceHandle();

	// Load the node bitmaps
	m_hNodeBitMap[NT_CPU		] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_CPU));
	m_hNodeBitMap[NT_SPU		] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_SPU));
	m_hNodeBitMap[NT_COP		] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_COP));
	m_hNodeBitMap[NT_DS			] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_DS));
	m_hNodeBitMap[NT_IO			] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_IO));
	m_hNodeBitMap[NT_JUNC		] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_JUNCTION));
	m_hNodeBitMap[NT_PORTAL_IN	] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_PORTAL));
	m_hNodeBitMap[NT_PORTAL_OUT	] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_PORTAL));

	// Load the line bitmaps
	m_hLineBitMap[DIR_NORTH] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_LINE_NS));
	m_hLineBitMap[DIR_EAST ] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NODE_LINE_EW));
	m_hLineBitMap[DIR_SOUTH] = m_hLineBitMap[DIR_NORTH];
	m_hLineBitMap[DIR_WEST ] = m_hLineBitMap[DIR_EAST ];

	// Initialize the line coordinates
	m_ptLineLocation[DIR_NORTH].x = LINE_N_X;
	m_ptLineLocation[DIR_NORTH].y = LINE_N_Y;
	m_ptLineLocation[DIR_EAST].x = LINE_E_X;
	m_ptLineLocation[DIR_EAST].y = LINE_E_Y;
	m_ptLineLocation[DIR_SOUTH].x = LINE_S_X;
	m_ptLineLocation[DIR_SOUTH].y = LINE_S_Y;
	m_ptLineLocation[DIR_WEST].x = LINE_W_X;
	m_ptLineLocation[DIR_WEST].y = LINE_W_Y;

	// Initialize the line sizes
	m_szLineSize[DIR_NORTH].cx = LINE_WIDTH;
	m_szLineSize[DIR_NORTH].cy = LINE_LENGTH;
	m_szLineSize[DIR_EAST].cx = LINE_LENGTH;
	m_szLineSize[DIR_EAST].cy = LINE_WIDTH;
	m_szLineSize[DIR_SOUTH].cx = LINE_WIDTH;
	m_szLineSize[DIR_SOUTH].cy = LINE_LENGTH;
	m_szLineSize[DIR_WEST].cx = LINE_LENGTH;
	m_szLineSize[DIR_WEST].cy = LINE_WIDTH;

	// Create the ICE and character image lists
	// NOTE: Cannot use the Create functions below because
	// these cause the image list to default to 16 colors.
	//m_ilChar.Create(IDB_IL_CHAR,24,0,BLACK);
	//m_ilICE.Create(IDB_IL_ICE,24,0,BLACK);
	m_ilChar.Create(NODE_GRID_SIZE,NODE_GRID_SIZE,ILC_COLOR24|ILC_MASK,0,g_nNumCharImages * 2);
	// I have discovered that using a bitmap in an image list screws up the background color.
	// Therefore, we have to create a temporary bitmap to hold the colors.
	HBITMAP hTmpBM;
	hTmpBM = (HBITMAP)CopyImage(g_hBitmaps[GR_MISC_CHARACTERS],IMAGE_BITMAP,0,0,LR_COPYRETURNORG);
	m_ilChar.Add(CBitmap::FromHandle(hTmpBM),PURPLE);
	DeleteObject(hTmpBM);

	CBitmap bmBitmap;
	m_ilMisc.Create(NODE_GRID_SIZE,NODE_GRID_SIZE,ILC_COLOR24|ILC_MASK,0,NUM_MISC_IMAGES * 2);
	bmBitmap.LoadBitmap(IDB_IL_MISC);
	m_ilMisc.Add(&bmBitmap,BLACK);
	bmBitmap.DeleteObject();
}

CNodeView::~CNodeView()
{
	int i;

	for (i=0; i<NUM_NODES; i++)
	{
		DeleteObject(m_hNodeBitMap[i]);
	}
}

BEGIN_MESSAGE_MAP(CNodeView,CWnd )
	//{{AFX_MSG_MAP(CNodeView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CNodeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create a background DC
	CDC *pDC = GetDC();
	m_dcBackground.CreateCompatibleDC(pDC);
	m_iaIce.LoadImages(pDC, MAKEINTRESOURCE(IDB_IL_ICE), NODE_GRID_SIZE, NODE_GRID_SIZE, PURPLE);
	ReleaseDC(pDC);

	return 0;
}

void CNodeView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int nDir;
	CNode *pNode;
	POSITION p;
	CIce *pIce;
	int i;
	
	// Get a pointer to the current node
	pNode = g_pChar->m_pCurrentNode;

	// Black out the background
	dc.FillSolidRect(0,0,NODE_SIZE,NODE_SIZE,(COLORREF)0);

	//-----------------------
	// Draw the current node
	//-----------------------
	CPoint ptPoint;
	CSize szSize;

	ptPoint.x = 0;
	ptPoint.y = 0;
	szSize.cx = NODE_SIZE;
	szSize.cy = NODE_SIZE;
	//dc.DrawState(ptPoint,szSize,m_hNodeBitMap[pNode->m_nType],DST_BITMAP,DSS_NORMAL);
	m_dcBackground.SelectObject(m_hNodeBitMap[pNode->m_nType]);
	dc.BitBlt(0,0,NODE_SIZE,NODE_SIZE,&m_dcBackground,0,0,SRCCOPY);

	//---------------------------
	// Draw the node connections
	//---------------------------
	for (nDir=0; nDir<4; nDir++)
	{
		if (pNode->m_pAdjNode[nDir]!=NULL)
		{
			dc.DrawState(	m_ptLineLocation[nDir],
							m_szLineSize[nDir],
							m_hLineBitMap[nDir],
							DST_BITMAP,
							DSS_NORMAL);
		}
	}

	//--------------------
	// Draw the character
	//--------------------
	g_pChar->m_ptScreenLoc.x = (g_pChar->m_ptNodeLoc.x * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
	g_pChar->m_ptScreenLoc.y = (g_pChar->m_ptNodeLoc.y * NODE_GRID_SIZE) + NODE_GRID_OFFSET;

	m_ilChar.DrawIndirect(&dc, g_pChar->m_nImage, g_pChar->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);

	// Draw the decoys
	for (i=0; i < g_pChar->m_nDecoyCount; i++)
	{
		ptPoint.x = (g_pChar->m_ptDecoyLocation[i].x * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
		ptPoint.y = (g_pChar->m_ptDecoyLocation[i].y * NODE_GRID_SIZE) + NODE_GRID_OFFSET;

		// Redraw the ICE
		m_ilChar.DrawIndirect(&dc, g_pChar->m_nImage, ptPoint, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
	}

	//------------------------------
	// Draw any objects in the node
	//------------------------------
	p = g_pChar->m_pSystem->m_olIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_pSystem->m_olIceList.GetNext(p);

		// Is the ice in this node?
		if (pIce->m_pCurrentNode==g_pChar->m_pCurrentNode)
		{
			// Draw the ICE
			pIce->m_ptScreenLoc.x = (pIce->m_ptNodeLoc.x * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
			pIce->m_ptScreenLoc.y = (pIce->m_ptNodeLoc.y * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
			//m_ilICE.DrawIndirect(&dc, pIce->m_nImage, pIce->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
			m_iaIce.DrawImage(&dc, pIce->m_ptScreenLoc.x, pIce->m_ptScreenLoc.y, pIce->m_nImage);

			// If this ICE is targetted, draw the crosshairs
			if (pIce == g_pChar->m_pTargettedIce)
			{
				m_ilMisc.DrawIndirect(&dc, MISC_TARGET, pIce->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
			}
		}
	}
}

void CNodeView::DoCharMove()
{
	CWindowDC dc(this); // device context for painting

	// Erase the old character
	dc.BitBlt(	g_pChar->m_ptScreenLoc.x,g_pChar->m_ptScreenLoc.y,
				NODE_GRID_SIZE,NODE_GRID_SIZE,
				&m_dcBackground,
				g_pChar->m_ptScreenLoc.x,g_pChar->m_ptScreenLoc.y,
				SRCCOPY);

	// Draw the new character
	g_pChar->m_ptScreenLoc.x = (g_pChar->m_ptNodeLoc.x * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
	g_pChar->m_ptScreenLoc.y = (g_pChar->m_ptNodeLoc.y * NODE_GRID_SIZE) + NODE_GRID_OFFSET;

	//m_ilChar.Draw(&dc, g_pChar->m_nImage, ptPoint, ILD_TRANSPARENT);
	m_ilChar.DrawIndirect(&dc, g_pChar->m_nImage, g_pChar->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
}

// Code to move char
//	// If already there, ignore
//	if (ptGridLoc == g_pChar->m_ptNodeLoc)
//	{
//		return;
//	}
//
//	// Try to move the char
//	if (!g_pChar->m_pCurrentNode->ChooseClosestLocation(ptGridLoc))
//	{
//		// Can't do
//		return;
//	}
//
//	// Move the char
//	g_pChar->m_ptNodeLoc = ptGridLoc;
//	
//	// Redraw
//	DoCharMove();


void CNodeView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// Find out what square this area is in
	CPoint ptGridLoc;

	ptGridLoc.x = ((point.x-NODE_GRID_OFFSET)/NODE_GRID_SIZE);
	ptGridLoc.y = ((point.y-NODE_GRID_OFFSET)/NODE_GRID_SIZE);

	// See if there is an ICE here
	POSITION p;
	CIce *pIce;
	p = g_pChar->m_olCurrentIceList.GetHeadPosition();
	while (p!=NULL)
	{
		pIce = (CIce *)g_pChar->m_olCurrentIceList.GetNext(p);

		// See if this ICE is in this location
		if (pIce->m_ptNodeLoc == ptGridLoc)
		{
			// If this ICE is not already targetted, target it
			if (pIce != g_pChar->m_pTargettedIce)
			{
				CWindowDC dc(this);

				// Restore the old target
				if (g_pChar->m_pTargettedIce!=NULL)
				{
					// Restore the background
					dc.BitBlt(	g_pChar->m_pTargettedIce->m_ptScreenLoc.x,
								g_pChar->m_pTargettedIce->m_ptScreenLoc.y,
								NODE_GRID_SIZE,NODE_GRID_SIZE,
								&m_dcBackground,
								g_pChar->m_pTargettedIce->m_ptScreenLoc.x,
								g_pChar->m_pTargettedIce->m_ptScreenLoc.y,
								SRCCOPY);

					// Redraw the ICE
					//m_ilICE.DrawIndirect(&dc, g_pChar->m_pTargettedIce->m_nImage, g_pChar->m_pTargettedIce->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
					m_iaIce.DrawImage(&dc, g_pChar->m_pTargettedIce->m_ptScreenLoc.x, g_pChar->m_pTargettedIce->m_ptScreenLoc.y, g_pChar->m_pTargettedIce->m_nImage);
				}

				// Select the ICE as the target
				g_pChar->m_pTargettedIce = pIce;

				// Draw the target
				m_ilMisc.DrawIndirect(&dc, MISC_TARGET, pIce->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);

				// Tell the view that the target has changed
				((CMatrixView *)this->GetParent())->UpdateTarget();
			}
			// We are done
			break;
		}
	}

	// Not necessary - calls Default()
	// CWnd ::OnLButtonUp(nFlags, point);
}

void CNodeView::EraseIce(CIce *pIce)
{
	// Make sure this ice is in the current node
	if (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode)
	{
		CClientDC dc(this);

		// Recalculate position, just in case
		pIce->m_ptScreenLoc.x = (pIce->m_ptNodeLoc.x * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
		pIce->m_ptScreenLoc.y = (pIce->m_ptNodeLoc.y * NODE_GRID_SIZE) + NODE_GRID_OFFSET;

		// Erase this ice
		dc.BitBlt(	pIce->m_ptScreenLoc.x,
					pIce->m_ptScreenLoc.y,
					NODE_GRID_SIZE,NODE_GRID_SIZE,
					&m_dcBackground,
					pIce->m_ptScreenLoc.x,
					pIce->m_ptScreenLoc.y,
					SRCCOPY);
	}
}

void CNodeView::DrawIce(CIce *pIce)
{
	// Make sure this ice is in the current node
	if (pIce->m_pCurrentNode == g_pChar->m_pCurrentNode)
	{
		CClientDC dc(this);

		//// Recalculate position, just in case
		pIce->m_ptScreenLoc.x = (pIce->m_ptNodeLoc.x * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
		pIce->m_ptScreenLoc.y = (pIce->m_ptNodeLoc.y * NODE_GRID_SIZE) + NODE_GRID_OFFSET;

		// Redraw the ICE
		//m_ilICE.DrawIndirect(&dc, pIce->m_nImage, pIce->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
		m_iaIce.DrawImage(&dc, pIce->m_ptScreenLoc.x, pIce->m_ptScreenLoc.y, pIce->m_nImage);
	}
}

void CNodeView::DrawDecoy(CPoint &ptLoc)
{
	CPoint ptScreenLoc;
	CClientDC dc(this);

	// Recalculate position, just in case
	ptScreenLoc.x = (ptLoc.x * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
	ptScreenLoc.y = (ptLoc.y * NODE_GRID_SIZE) + NODE_GRID_OFFSET;

	// Redraw the ICE
	m_ilChar.DrawIndirect(&dc, g_pChar->m_nImage, ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
}

void CNodeView::EraseGrid(CPoint &ptLoc)
{
	CPoint ptScreenLoc;
	CClientDC dc(this);

	// Calculate position
	ptScreenLoc.x = (ptLoc.x * NODE_GRID_SIZE) + NODE_GRID_OFFSET;
	ptScreenLoc.y = (ptLoc.y * NODE_GRID_SIZE) + NODE_GRID_OFFSET;

	// Redraw the square
	dc.BitBlt(	ptScreenLoc.x,
				ptScreenLoc.y,
				NODE_GRID_SIZE,NODE_GRID_SIZE,
				&m_dcBackground,
				ptScreenLoc.x,
				ptScreenLoc.y,
				SRCCOPY);
}

void CNodeView::DrawDamage(CIce *pIce)
{
	DrawMisc(pIce, MISC_DAMAGE);
}

void CNodeView::DrawQuery(CIce *pIce)
{
	DrawMisc(pIce, MISC_QUERY);
}

void CNodeView::DrawAlert(CIce *pIce)
{
	DrawMisc(pIce, MISC_ALERT);
}

void CNodeView::DrawBypass(CIce *pIce)
{
	DrawMisc(pIce, MISC_BYPASS);
}

void CNodeView::DrawMisc(CIce *pIce, int nImage)
{
	CPoint ptScreenLoc;
	CClientDC dc(this);

	// Calculate position
	if (pIce==NULL)
	{
		ptScreenLoc = g_pChar->m_ptScreenLoc;
	}
	else
	{
		// Make sure the ICE is in the current node
		if (pIce->m_pCurrentNode != g_pChar->m_pCurrentNode)
		{
			return;
		}
		ptScreenLoc = pIce->m_ptScreenLoc;
	}

	// Draw the damage
	m_ilMisc.DrawIndirect(&dc, nImage, ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);

	Sleep(500);

	// Redraw the square
	dc.BitBlt(	ptScreenLoc.x,
				ptScreenLoc.y,
				NODE_GRID_SIZE,NODE_GRID_SIZE,
				&m_dcBackground,
				ptScreenLoc.x,
				ptScreenLoc.y,
				SRCCOPY);
	if (pIce==NULL)
	{
		m_ilChar.DrawIndirect(&dc, g_pChar->m_nImage, g_pChar->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
	}
	else
	{
		m_iaIce.DrawImage(&dc, pIce->m_ptScreenLoc.x, pIce->m_ptScreenLoc.y, pIce->m_nImage);

		// Is this the target?
		if (g_pChar->m_pTargettedIce == pIce)
		{
			// Draw the target
			m_ilMisc.DrawIndirect(&dc, MISC_TARGET, pIce->m_ptScreenLoc, CSize(NODE_GRID_SIZE,NODE_GRID_SIZE), CPoint(0,0), ILD_TRANSPARENT);
		}
	}
}

