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

// Program.cpp: implementation of the CProgram class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "decker.h"
#include "Program.h"
#include "Character.h"
#include "Node.h"
#include "Area.h"
#include "DeckerSound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The character data
extern CCharacter *g_pChar;

static const int g_nProgramComplexity[] = 
{
	2,	// PROGRAM_ATTACK
	3,	// PROGRAM_ATTACK_A
	3,	// PROGRAM_ATTACK_P
	2,	// PROGRAM_SLOW
	3,	// PROGRAM_VIRUS
	3,	// PROGRAM_SILENCE
	4,	// PROGRAM_CONFUSE
	2,	// PROGRAM_WEAKEN
	3,	// PROGRAM_SHIELD
	1,	// PROGRAM_SMOKE
	4,	// PROGRAM_DECOY
	4,	// PROGRAM_MEDIC
	3,	// PROGRAM_ARMOR
	3,	// PROGRAM_HIDE
	2,	// PROGRAM_DECEIVE
	2,	// PROGRAM_RELOCATE
	1,	// PROGRAM_ANALYZE
	1,	// PROGRAM_SCAN
	1,	// PROGRAM_EVALUATE
	2,	// PROGRAM_DECRYPT
	4,	// PROGRAM_REFLECT
	3,	// PROGRAM_ATTACK_BOOST
	3,	// PROGRAM_DEFENSE_BOOST
	3,	// PROGRAM_STEALTH_BOOST
	3,	// PROGRAM_ANALYSIS_BOOST
	4,	// PROGRAM_CLIENT
};

static const char *g_szProgramClassName[NUM_PROGRAMS] = 
{
	"Attack",
	"Area Attack",
	"Piercing Attack",
	"Slow",
	"Virus",
	"Silence",
	"Confuse",
	"Weaken",
	"Shield",
	"Smoke",
	"Decoy",
	"Medic",
	"Armor",
	"Hide",
	"Deceive",
	"Relocate",
	"Analyze",
	"Scan",
	"Evaluate",
	"Decrypt",
	"Reflect",
	"Attack Boost",
	"Defense Boost",
	"Stealth Boost",
	"Analysis Boost",
	"Client Program",
};

// Predefined Programs Tables
const char *g_szAttackPrograms[20] = 
{
	"Zap 1.0",
	"Zap 2.1",
	"IceBreaker Mk1",
	"Magnum",
	"AK 4.7",
	"Blaster",
	"IceBreaker Mk2",
	"Bazooka",
	"Magnum II",
	"Zap 4.2",
	"Bazooka",
	"CyberDagger",
	"SuperBlaster",
	"Zap 5.0",
	"CyberSword",
	"MegaBlaster",
	"DigiUzi",
	"CyberKatana",
	"IceBreaker Mk3",
	"GigaBlaster",
};

const char *g_szAttackAPrograms[20] = 
{
	"Grenade 1.0",
	"Logic Bomb I",
	"Grenade 1.5",
	"BugSwarm",
	"Shrapnel 1.0",
	"Fireball 1.2",
	"Scattergun",
	"Grenade 2.0",
	"BugSwarm II",
	"Logic Bomb II",
	"Shrapnel 3.0",
	"Grenade 3.0",
	"Fireball 3.1",
	"Logic Bomb III",
	"BugSwarm III",
	"Grenade 4.0",
	"Logic Bomb IV",
	"EMP",
	"Logic Bomb V",
	"Nuke",
};

const char *g_szAttackPPrograms[20] = 
{
	"Spear 1.0a",
	"Crossbow",
	"Laser 1.1",
	"Javelin 1.0",
	"Scalpel",
	"Drill 2.2",
	"IcePick 1.3",
	"FMJ",
	"Teflon",
	"Stiletto 1.1.0",
	"Awl 1.0",
	"Drill 3.1",
	"Scalpel II",
	"IcePick 2.0",
	"Laser 4.0",
	"IcePick 2.3",
	"Drill 4.0",
	"Laser 5.1",
	"IcePick 3.0",
	"Shredder",
};

const char *g_szSlowPrograms[20] = 
{
	"Slow",
	"Bind 1.0",
	"Goo 1.2",
	"Limpets 1.0",
	"Quicksand 2.3",
	"Glue",
	"Flypaper 1.7a",
	"Goo 2.2",
	"Limpets 2.0",
	"Goo 3.0",
	"Quicksand 3.0",
	"Flypaper 2.2b",
	"SuperGlue",
	"Freeze 1.0",
	"Quicksand",
	"Bind 3.1",
	"Limpets 3.0",
	"KrazyGlue",
	"Bind 4.1",
	"TimeStop",
};

const char *g_szVirusPrograms[20] = 
{
	"Flu 1.0",
	"Flu 2.0",
	"Pneumonia 1.2",
	"Arsenic",
	"Strep 1.0",
	"BrainBugs 1.2",
	"RotWorms Mk1",
	"Cancer 2.3",
	"BedBugs",
	"Flu 10.0",
	"Pneumonia 3.1",
	"RotWorms Mk2",
	"Cancer 3.0",
	"More Bedbugs",
	"Cyanide",
	"Pneumonia 4.0",
	"RotWorms Mk2",
	"Cancer 4.0",
	"BrainBugs 3.1",
	"Ebola",
};

const char *g_szSilencePrograms[20] = 
{
	"Silence",
	"QuietYou",
	"Gag 3.3",
	"ZipIt 1.0",
	"Muffler 2.1",
	"Shhhh!",
	"Laryngitis 2.3",
	"MouthClamp 2.1",
	"Hush 1.0",
	"QuietYou 2.0",
	"Muffler 3.0",
	"Laryngitis 3.3a",
	"QuietYou 3.0",
	"Hush 2.0",
	"Shhhh! II",
	"Muffler 4.0",
	"QuietYou 4.1",
	"Laryngitis 4.02",
	"ZipIt 2.1",
	"MegaMute",
};

const char *g_szConfusionPrograms[20] = 
{
	"Confusion",
	"Duh? 12.3",
	"Gremlins",
	"Gremlins II",
	"LSD 4.1",
	"Duh? 192.334",
	"Lobotomy 1.0",
	"Duh? 3.14159",
	"LSD 5.0",
	"Fermat's Theorem",
	"Lobotomy 2.0",
	"Gump 2.3",
	"BrainFry 1.0",
	"Gremlins III",
	"Lobotomy 3.0",
	"Gump 3.1",
	"BrainFry 2.1",
	"Psychadelicious",
	"Lobotomy 4.0",
	"DanQuayle",
};

const char *g_szWeakenPrograms[20] = 
{
	"Weaken",
	"WussyBoy 2.0",
	"Shrink 1.0",
	"Hamstring 1.2",
	"WussyBoy 2.3a",
	"Decrepify Mk1",
	"Soften",
	"Shrink 2.0",
	"Weinee 1.0",
	"GirlyMan 1.0",
	"YouPansy 1.0",
	"Nausea 3.2",
	"Decrepify Mk2",
	"Tenderize",
	"Hamstring 2.2",
	"Decrepify Mk3",
	"GirlyMan 3.2",
	"Weinee 2.0",
	"Sap",
	"Impotence",
};

const char *g_szShieldPrograms[20] = 
{
	"Shield",
	"Buckler 1.1a",
	"Umbrella 1.0",
	"Shield Mk2",
	"Blocker 1.0",
	"Bumper",
	"Airbag 1.0",
	"Blocker 2.0",
	"Shield Mk3",
	"Buckler 2.3",
	"Airbag 2.0",
	"Umbrella 3.0",
	"ForceField 1.0",
	"Buckler 3.0",
	"Shield Mk4",
	"Airbag 3.0",
	"Buckler 3.2c",
	"ForceField 2.0",
	"Blocker 7.0",
	"Aegis",
};

const char *g_szSmokePrograms[20] = 
{
	"Smoke",
	"Blind 1.0",
	"Darkness 1.1",
	"Distraction 1.1",
	"Escape! 1.2",
	"Fog",
	"Smog",
	"Blind 2.1",
	"Sandstorm",
	"Distraction 2.0",
	"ECM 1.0",
	"Flashbang 1.0",
	"Blind 3.2",
	"Distraction 3.0",
	"WhereDidHeGo?",
	"Blind 3.7",
	"Flashbang 2.0",
	"Distraction 4.1",
	"Blind 4.0a",
	"Houdini",
};

const char *g_szDecoyPrograms[20] = 
{
	"Decoy",
	"MirrorImage 1.0",
	"MyBuddy 1.0",
	"StandIn 1.0",
	"Twins 2.0",
	"BodyDouble 1.3",
	"MirrorImage 2.0",
	"Mitosis 1.02",
	"StandIn 2.0",
	"Clone 1.2",
	"MyBuddy 2.0",
	"BodyDouble 2.1",
	"MirrorImage 3.0",
	"Clone 2.0",
	"Mitosis 1.3",
	"Clone 2.21",
	"MirrorImage 4.0",
	"BodyDouble 3.2",
	"StandIn 4.1",
	"Simulacrum",
};

const char *g_szMedicPrograms[20] = 
{
	"Medic",
	"FirstAid 1.0",
	"VirtualEMT",
	"Bandage 1.0",
	"Tourniquet 2.2",
	"VirtualNurse",
	"FirstAid 2.4d",
	"MedKit 1.0",
	"Restoration",
	"Succor 1.0",
	"Bandage 2.30",
	"VirtualDoctor",
	"Restoration II",
	"Succor 2.01",
	"Bandage 4.1",
	"Restoration III",
	"Succor 3.2",
	"Restoration IV",
	"VirtualSurgeon",
	"M.A.S.H",
};

const char *g_szArmorPrograms[20] = 
{
	"Armor",
	"StoneSkin 1.0",
	"ChainMail",
	"SteelPlate 1.2",
	"Protector 1.2",
	"Kevlar 2.0",
	"Protector 2.3a",
	"SteelPlate 2.1",
	"Kevlar 3.0",
	"StoneSkin 2.0",
	"PlateMail",
	"Kevlar 4.1",
	"Mithril",
	"SteelPlate 3.1",
	"StoneSkin 3.0",
	"Titanium",
	"Mithril II",
	"Titanium Mk2",
	"StoneSkin 4.0",
	"Adamantium",
};

const char *g_szHidePrograms[20] = 
{
	"Hide",
	"IgnoreMe 1.0",
	"Cloak",
	"Chameleon 1.0",
	"Hide Mk2",
	"Camoflauge 2.1",
	"IgnoreMe 2.0",
	"Inviso",
	"IgnoreMe 2.2a",
	"Camoflauge 3.0",
	"Inviso II",
	"Chameleon 2.1",
	"IgnoreMe 3.02",
	"Camoflauge 4.1",
	"Inviso III",
	"Enhanced Cloak",
	"IgnoreMe 4.1",
	"Hide Mk5",
	"SuperCloak",
	"HollowMan",
};

const char *g_szDeceivePrograms[20] = 
{
	"Deceive",
	"PassGen 2.0",
	"LiarLiar 1.02",
	"FakeOut 3.1",
	"MistakenID 1.2",
	"Masquerade",
	"Costume 2.1",
	"Passport 3.1",
	"Masquerade III",
	"PassGen 3.0",
	"FakeOut 3.2",
	"Masquerade IV",
	"LiarLiar 2.11",
	"Forge 1.0",
	"Costume 3.2",
	"PassGen 4.0",
	"Masquerade VI",
	"Forge 2.0",
	"Forge 2.3a",
	"Politician",
};

const char *g_szRelocatePrograms[20] = 
{
	"Relocate",
	"ImGone 1.1",
	"Misdirect 1.0a",
	"WildGooseChase 1.31",
	"TraceBuster 1.0",
	"WrongNumber 1.3",
	"Mislead 1.0",
	"ImGone 2.0",
	"LineSwitch 9.0",
	"Loopback 10.0",
	"WildGooseChase 2.03",
	"Misdirect 2.3b",
	"Mislead 2.0",
	"TraceBuster 2.0",
	"WrongNumber 2.1",
	"RedHerring",
	"Misdirect 3.1a",
	"RedHerring II",
	"TraceBuster 3.0",
	"Trail-B-Gone",
};

const char *g_szAnalyzePrograms[20] = 
{
	"Analyze",
	"WhatzIt 1.0",
	"Encyclopedia",
	"Identify 1.0.1",
	"Classify 1.0",
	"Taxonomy 3.0",
	"Autopsy",
	"Classify 2.0",
	"WhatzIt 2.0",
	"Identify 2.1.1",
	"Microscope 1.0",
	"Enhanced Analyze",
	"Taxonomy 5.0",
	"Identify 2.2.0",
	"WhatzIt 3.0",
	"Microscope 3.0",
	"Taxonomy 7.0",
	"WhatzIt 3.2",
	"Identify 3.0.3",
	"Forensics",
};

const char *g_szScanPrograms[20] = 
{
	"Scan",
	"FindIt 1.0",
	"NodeSearch 1.2",
	"FindIt 2.0",
	"Detective 1.3",
	"Sherlock 1.1",
	"Flashlight Mk1",
	"FindIt 3.0",
	"NodeSearch 2.0",
	"FindIt 4.0",
	"Snoopy 1.0",
	"Detective 3.1",
	"Flashlight Mk2",
	"NodeSearch 3.1",
	"Snoopy 2.0",
	"Detective 3.5",
	"Sherlock 3.1",
	"Flashlight Mk3",
	"Snoopy 3.0",
	"SuperScan",
};

const char *g_szEvaluatePrograms[20] = 
{
	"Evaluate",
	"Priceless 1.0",
	"Divine",
	"BlueBook 1.0",
	"ValueSoft 1.0",
	"Evaluate Mk2",
	"GoldDigger",
	"Priceless 2.0",
	"BlueBook 2.1",
	"Priceless 2.1",
	"Peruse 1.0",
	"Appraise 1.0",
	"Evaluate Mk3",
	"BlueBook 3.0",
	"Priceless 3.0",
	"ValueSoft 7.0",
	"GoldDigger II",
	"Evaluate Mk4",
	"BlueBook 4.0a",
	"ShowMeTheMoney",
};

const char *g_szDecryptPrograms[20] = 
{
	"Decrypt",
	"SolveIt 2.0",
	"CodeBreaker 1.1",
	"Descramble",
	"WormKiller 1.2",
	"Untangle",
	"SolveIt 3.0",
	"Decrypt II",
	"CodeBreaker 2.2",
	"WormKiller 1.7",
	"Descramble 95",
	"SolveIt 4.0",
	"Untangle Mk2",
	"WormKiller 2.1",
	"Decrypt III",
	"Descramble 98",
	"CodeBreaker 3.4",
	"SolveIt 6.0",
	"Decrypt IV",
	"SuperCracker",
};

const char *g_szReflectPrograms[20] = 
{
	"Reflect",
	"ImRubber 1.1",
	"Reflect Mk2",
	"BounceBack",
	"Reflect Mk3",
	"ImRubber 2.1",
	"Reflect Mk4",
	"ImRubber 3.0",
	"BounceBackEx",
	"Deflector I",
	"Reflect Mk5",
	"BounceBackDeluxe",
	"ImRubber 3.4",
	"Deflector II",
	"ImRubber 4.2",
	"Deflector III",
	"BounceBackPremium",
	"Deflector IV",
	"BounceBackSupreme",
	"Trampoline",
};

const char *g_szAttackBoostPrograms[20] = 
{
	"Attack Boost 1.0",
	"Attack Boost 1.1",
	"Attack Boost 1.2",
	"Attack Boost 1.3",
	"Attack Boost 1.4",
	"Attack Boost 1.5",
	"Attack Boost 2.0",
	"Attack Boost 2.1",
	"Attack Boost 2.2",
	"Attack Boost 2.3",
	"Attack Boost 3.1",
	"Attack Boost 3.2",
	"Attack Boost 3.3",
	"Attack Boost 3.4",
	"Attack Boost 4.1",
	"Attack Boost 4.2",
	"Attack Boost 4.3",
	"Attack Boost 5.0",
	"Attack Boost 5.1",
	"Attack Boost 6.0",
};

const char *g_szDefenseBoostPrograms[20] = 
{
	"Defense Boost 1.0",
	"Defense Boost 1.1",
	"Defense Boost 1.2",
	"Defense Boost 1.3",
	"Defense Boost 1.4",
	"Defense Boost 1.5",
	"Defense Boost 2.0",
	"Defense Boost 2.1",
	"Defense Boost 2.2",
	"Defense Boost 2.3",
	"Defense Boost 3.1",
	"Defense Boost 3.2",
	"Defense Boost 3.3",
	"Defense Boost 3.4",
	"Defense Boost 4.1",
	"Defense Boost 4.2",
	"Defense Boost 4.3",
	"Defense Boost 5.0",
	"Defense Boost 5.1",
	"Defense Boost 6.0",
};

const char *g_szStealthBoostPrograms[20] = 
{
	"Stealth Boost 1.0",
	"Stealth Boost 1.1",
	"Stealth Boost 1.2",
	"Stealth Boost 1.3",
	"Stealth Boost 1.4",
	"Stealth Boost 1.5",
	"Stealth Boost 2.0",
	"Stealth Boost 2.1",
	"Stealth Boost 2.2",
	"Stealth Boost 2.3",
	"Stealth Boost 3.1",
	"Stealth Boost 3.2",
	"Stealth Boost 3.3",
	"Stealth Boost 3.4",
	"Stealth Boost 4.1",
	"Stealth Boost 4.2",
	"Stealth Boost 4.3",
	"Stealth Boost 5.0",
	"Stealth Boost 5.1",
	"Stealth Boost 6.0",
};

const char *g_szAnalysisBoostPrograms[20] = 
{
	"Analysis Boost 1.0",
	"Analysis Boost 1.1",
	"Analysis Boost 1.2",
	"Analysis Boost 1.3",
	"Analysis Boost 1.4",
	"Analysis Boost 1.5",
	"Analysis Boost 2.0",
	"Analysis Boost 2.1",
	"Analysis Boost 2.2",
	"Analysis Boost 2.3",
	"Analysis Boost 3.1",
	"Analysis Boost 3.2",
	"Analysis Boost 3.3",
	"Analysis Boost 3.4",
	"Analysis Boost 4.1",
	"Analysis Boost 4.2",
	"Analysis Boost 4.3",
	"Analysis Boost 5.0",
	"Analysis Boost 5.1",
	"Analysis Boost 6.0",
};

const char **g_ProgramNames[] =
{
	g_szAttackPrograms,
	g_szAttackAPrograms,
	g_szAttackPPrograms,
	g_szSlowPrograms,
	g_szVirusPrograms,
	g_szSilencePrograms,
	g_szConfusionPrograms,
	g_szWeakenPrograms,
	g_szShieldPrograms,
	g_szSmokePrograms,
	g_szDecoyPrograms,
	g_szMedicPrograms,
	g_szArmorPrograms,
	g_szHidePrograms,
	g_szDeceivePrograms,
	g_szRelocatePrograms,
	g_szAnalyzePrograms,
	g_szScanPrograms,
	g_szEvaluatePrograms,
	g_szDecryptPrograms,
	g_szReflectPrograms,
	g_szAttackBoostPrograms,
	g_szDefenseBoostPrograms,
	g_szStealthBoostPrograms,
	g_szAnalysisBoostPrograms,
};

const char *GetProgramClassName(int nClass)
{
	return g_szProgramClassName[nClass];
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProgram::CProgram()
{
	m_nLoadedRating = 0;
	m_bLoadByDefault = FALSE;
}

CProgram::~CProgram()
{

}

void CProgram::GetInitValues(int nClass, int nRating)
{
	if (nClass == PROGRAM_CLIENT)
	{
		m_szName = "Client Supplied Program";
		m_nSound = SOUND_DEFAULTCLIENT;
	}
	else 
	{
		if (nRating > 20)
		{
			m_szName = g_ProgramNames[nClass][19];
		}
		else
		{
			m_szName = g_ProgramNames[nClass][nRating-1];
		}
		m_nSound = nClass;
	}
	m_nClass = nClass;
	m_nRating = nRating;
}

int CProgram::GetLoadTime()
{
	int nTime;

	// If have a high-speed connection, time is 1 turn
	if ((g_pChar->m_pCurrentNode->m_pParentArea->m_pHighSpeedIONode != NULL) &&
		(g_pChar->m_pCurrentNode->m_pParentArea->m_pHighSpeedIONode->m_bActivated))
	{
		// One turn download
		nTime = 1;
	}
	else
	{
		// Time is size / (2^(bus size))
		int nRate = (1 << (g_pChar->m_nHardware[HW_HIGH_BW_BUS]));
		nTime =  ((GetSize() + nRate - 1) / nRate);
		if (nTime<1) 
		{
			nTime = 1;
		}
	}

	return nTime;
}

int CProgram::GetSize()
{
	return GetProgramSize(m_nClass, m_nRating);
}

void CProgram::Save(CArchive &ar)
{
	ar << m_szName;
	ar << m_nClass;
	ar << m_nRating;
	ar << m_bLoadByDefault;
	ar << m_nLoadedRating;
	ar << m_nSound;
}

void CProgram::Load(CArchive &ar, WORD wMajorVersion, WORD wMinorVersion)
{
	ar >> m_szName;
	ar >> m_nClass;
	ar >> m_nRating;
	ar >> m_bLoadByDefault;
	ar >> m_nLoadedRating;
	if (wMajorVersion < 1)
	{
		m_nSound = m_nClass;
	}
	else
	{
		ar >> m_nSound;
	}
}

// Global functions
int GetSoftwarePrice(int nClass, int nRating)
{
	return g_nProgramComplexity[nClass] * nRating * nRating * 25;
}

void GetSoftwareText(CString &szStr, int nClass, int nRating)
{
	int nIndex;

	if (nRating > 20)
		nIndex = 19;
	else
		nIndex = nRating-1;

	szStr.Format("%s (%s %d)",
		g_ProgramNames[nClass][nIndex],
		g_szProgramClassName[nClass],
		nRating);
}

int GetProgramComplexity(int nClass)
{
	return g_nProgramComplexity[nClass];
}

int GetProgramSize(int nClass, int nRating)
{
	return g_nProgramComplexity[nClass] * nRating;
}
