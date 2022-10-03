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

#include "stdafx.h"
#include "decker.h"
#include "DeckerGraphics.h"
#include "Global.h"


// Graphics names
static const char *g_szGraphicsNames[GR_COUNT] =
{
	"HOME_MAIN",
	"HOME_CHAR0",
	"HOME_CHAR1",
	"HOME_DECK0",
	"HOME_DECK1",
	"HOME_CONTRACT0",
	"HOME_CONTRACT1",
	"HOME_SHOP0",
	"HOME_SHOP1",
	"HOME_PROJECT1",
	"HOME_PROJECT0",
	"HOME_REST0",
	"HOME_REST1",
	"HOME_MATRIX0",
	"HOME_MATRIX1",
	"HOME_OPTIONS0",
	"HOME_OPTIONS1",
	"MATRIX_MAIN",
	"MATRIX_ARROW_N0",
	"MATRIX_ARROW_N1",
	"MATRIX_ARROW_N2",
	"MATRIX_ARROW_E0",
	"MATRIX_ARROW_E1",
	"MATRIX_ARROW_E2",
	"MATRIX_ARROW_S0",
	"MATRIX_ARROW_S1",
	"MATRIX_ARROW_S2",
	"MATRIX_ARROW_W0",
	"MATRIX_ARROW_W1",
	"MATRIX_ARROW_W2",
	"MATRIX_WAIT0",
	"MATRIX_WAIT1",
	"MATRIX_ATTACK0",
	"MATRIX_ATTACK1",
	"MATRIX_ATTACK2",
	"MATRIX_DECEIVE0",
	"MATRIX_DECEIVE1",
	"MATRIX_DECEIVE2",
	"MATRIX_VIEW_ICE0",
	"MATRIX_VIEW_ICE1",
	"MATRIX_VIEW_ICE2",
	"MATRIX_SCAN0",
	"MATRIX_SCAN1",
	"MATRIX_SCAN2",
	"MATRIX_CHAR0",
	"MATRIX_CHAR1",
	"MATRIX_DECK0",
	"MATRIX_DECK1",
	"MATRIX_CONTRACT0",
	"MATRIX_CONTRACT1",
	"MATRIX_DISCONNECT0",
	"MATRIX_DISCONNECT1",
	"MATRIX_OPTIONS0",
	"MATRIX_OPTIONS1",
	"MATRIX_RUN_PROGRAM0",
	"MATRIX_RUN_PROGRAM1",
	"MATRIX_RUN_PROGRAM2",
	"MATRIX_SET_DEF0",
	"MATRIX_SET_DEF1",
	"MATRIX_SET_DEF2",
	"MATRIX_LOAD0",
	"MATRIX_LOAD1",
	"MATRIX_LOAD2",
	"MATRIX_UNLOAD0",
	"MATRIX_UNLOAD1",
	"MATRIX_UNLOAD2",
	"MATRIX_GETFILE0",
	"MATRIX_GETFILE1",
	"MATRIX_GETFILE2",
	"MATRIX_EDIT0",
	"MATRIX_EDIT1",
	"MATRIX_EDIT2",
	"MATRIX_ERASE0",
	"MATRIX_ERASE1",
	"MATRIX_ERASE2",
	"MATRIX_CRASH0",
	"MATRIX_CRASH1",
	"MATRIX_CRASH2",
	"MATRIX_USE_IO0",
	"MATRIX_USE_IO1",
	"MATRIX_USE_IO2",
	"MATRIX_GETMAP0",
	"MATRIX_GETMAP1",
	"MATRIX_GETMAP2",
	"MATRIX_BACKDOOR0",
	"MATRIX_BACKDOOR1",
	"MATRIX_BACKDOOR2",
	"MATRIX_KILL_ALARM0",
	"MATRIX_KILL_ALARM1",
	"MATRIX_KILL_ALARM2",
	"MATRIX_ENTER_PORTAL0",
	"MATRIX_ENTER_PORTAL1",
	"MATRIX_ENTER_PORTAL2",
	"MATRIX_KILL_TRACE0",
	"MATRIX_KILL_TRACE1",
	"MATRIX_KILL_TRACE2",
	"MATRIX_KILL_SHUTDOWN0",
	"MATRIX_KILL_SHUTDOWN1",
	"MATRIX_KILL_SHUTDOWN2",
	"MATRIX_MAP_ZOOM0",
	"MATRIX_MAP_ZOOM1",
	"MATRIX_BIO_COVER",
	"MATRIX_TRACE_COVER",
	"MISC_CHARACTERS",
};

// Customizable bitmap handles
HBITMAP g_hBitmaps[GR_COUNT];

// Number of character images available
int g_nNumCharImages;

//-----------
// Functions
//-----------
BOOL LoadGraphics(const char *szDirectory)
{
	int nIndex;
	DWORD dwRC;
	char szGraphicsDir[MAX_PATH];
	CString szFileName;
	CString szMessage;

	// Initialize
	for (nIndex = 0; nIndex < GR_COUNT; nIndex++)
	{
		g_hBitmaps[nIndex] = NULL;
	}

	// Read the INI file string to see if this item has been overridden
	dwRC = GetPrivateProfileString(
						"Graphics",				// points to section name
						"Directory",			// points to key name
						"DefaultGraphics",		// points to default string
						szGraphicsDir,			// points to destination buffer
						MAX_PATH,				// size of destination buffer
						".\\Decker.ini");		// points to initialization filename

	// Graphic processing loop
	for (nIndex = 0; nIndex < GR_COUNT; nIndex++)
	{
		// Build the graphics file name
		szFileName.Format("%s\\%s\\%s.bmp", szDirectory, szGraphicsDir, g_szGraphicsNames[nIndex]);

		// Load the bitmap
		g_hBitmaps[nIndex] = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (g_hBitmaps[nIndex] == NULL)
		{
			szMessage.Format("Error: Could not load graphics file %s.", szFileName);
			DeckerMessageBox(szMessage);
			return FALSE;
		}
	}

	// Set the number of character images available
	BITMAP bm;
	GetObject(g_hBitmaps[GR_MISC_CHARACTERS], sizeof(bm), &bm);
	g_nNumCharImages = (bm.bmWidth / CHAR_IMAGE_SIZE);

	if (g_nNumCharImages < 1)
	{
		szMessage.Format("Error: No character images available.");
		DeckerMessageBox(szMessage);
		return FALSE;
	}

	return TRUE;
}

void UnloadGraphics()
{
	int nIndex;

	for (nIndex = 0; nIndex < GR_COUNT; nIndex++)
	{
		if (g_hBitmaps[nIndex] != NULL)
		{
			DeleteObject(g_hBitmaps[nIndex]);
		}
	}
}