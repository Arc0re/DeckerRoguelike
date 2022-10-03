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
#include <mmsystem.h>
#include "decker.h"
#include "DeckerSound.h"
#include "Global.h"


//------------------
// Global Variables
//------------------
BOOL g_bSound = TRUE;

//------------------
// Static Variables
//------------------
// The array of WAV files
static CStringArray g_SoundWavs;

// INI keys
static const char *g_szSoundKey[NUM_SOUNDS] = 
{
	"DefaultAttack",
	"DefaultAreaAttack",
	"DefaultPiercingAttack",
	"DefaultSlow",
	"DefaultVirus",
	"DefaultSilence",
	"DefaultConfuse",
	"DefaultWeaken",
	"DefaultShield",
	"DefaultSmoke",
	"DefaultDecoy",
	"DefaultMedic",
	"DefaultArmor",
	"DefaultHide",
	"DefaultDeceive",
	"DefaultRelocate",
	"DefaultAnalyze",
	"DefaultScan",
	"DefaultEvaluate",
	"DefaultDecrypt",
	"DefaultReflect",
	"DefaultAttackBoost",
	"DefaultDefenseBoost",
	"DefaultStealthBoost",
	"DefaultAnalysisBoost",
	"DownloadFile",
	"EditFile",
	"EraseFile",
	"CrashSystem",
	"UseIO",
	"GetMap",
	"CreateBackdoor",
	"CancelAlert",
	"EnterPortal",
	"CancelTrace",
	"CancelShutdown",
	"Move",
	"UnloadProgram",
	"EnterMatrix",
	"DumpDisconnect",
	"DumpDeckDamage",
	"DumpUnconscious",
	"DumpDeath",
	"DumpTrace",
	"DumpSysCrash",
	"DumpSysOffline",
	"Queried",
	"Attacked",
	"YellowAlert",
	"RedAlert",
	"Shutdown",
	"TraceStart",
	"ICECrash",
	"DownloadComplete",
	"EditComplete",
	"ICEEnterNode",
	"ICELeaveNode",
	"ProgramLoaded",
	"ScanComplete",
	"EvalComplete",
	"FileDestroyed",
	"CustomSound1",
	"CustomSound2",
	"CustomSound3",
	"CustomSound4",
	"CustomSound5",
	"CustomSound6",
	"CustomSound7",
	"CustomSound8",
	"CustomSound9",
	"CustomSound10",
	"CustomSound11",
	"CustomSound12",
	"CustomSound13",
	"CustomSound14",
	"CustomSound15",
	"CustomSound16",
	"CustomSound17",
	"CustomSound18",
	"CustomSound19",
	"CustomSound20",
	"DefaultClient",
};

// Sound directory
static char g_szSoundDirectory[MAX_PATH] = ".";

void LoadSounds(const char *szDirectory)
{
	DWORD dwRC;
	int nIndex;
	char szSound[MAX_PATH];
	//char *pTmp;

	// Set the size of the sound array
	g_SoundWavs.SetSize(NUM_SOUNDS);

	// Read the INI file
	for (nIndex = 0; nIndex < NUM_SOUNDS; nIndex++)
	{
		// Read the sound
		dwRC = GetPrivateProfileString(
							"Sounds",				// points to section name
							g_szSoundKey[nIndex],	// points to key name
							"",						// points to default string
							szSound,				// points to destination buffer
							MAX_PATH,				// size of destination buffer
							".\\Decker.ini");		// points to initialization filename

		// Store the sound
		g_SoundWavs.SetAt(nIndex, szSound);
	}

	// Set the current directory
	sprintf(g_szSoundDirectory, "%s\\Sound\\", szDirectory);
}

void PlayGameSound(int nSound)
{
	CString szSound;

	// Check for sound off
	if (!g_bSound)
	{
		return;
	}

	// Check for valid sound effect
	if ((nSound < 0) || (nSound >=NUM_SOUNDS) || (g_SoundWavs.GetAt(nSound).IsEmpty()))
	{
		return;
	}

	// Play the sound file
	szSound.Format("%s%s",g_szSoundDirectory,g_SoundWavs.GetAt(nSound));
	PlaySound(szSound, NULL,SND_ASYNC|SND_FILENAME/*|SND_NOWAIT*/);
}

void UnloadSounds()
{
	g_SoundWavs.RemoveAll();
}
