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

#ifndef __DECKERSOUND_H__
#define __DECKERSOUND_H__

//------------------
// Sound identifiers
//------------------
// Special note: Do not change program sound order because
// programs use their class as default sound effect.
#define SOUND_DEFAULTATTACK				0
#define SOUND_DEFAULTAREAATTACK			1
#define SOUND_DEFAULTPIERCINGATTACK		2
#define SOUND_DEFAULTSLOW				3
#define SOUND_DEFAULTVIRUS				4
#define SOUND_DEFAULTSILENCE			5
#define SOUND_DEFAULTCONFUSE			6
#define SOUND_DEFAULTWEAKEN				7
#define SOUND_DEFAULTSHIELD				8
#define SOUND_DEFAULTSMOKE				9
#define SOUND_DEFAULTDECOY				10
#define SOUND_DEFAULTMEDIC				11
#define SOUND_DEFAULTARMOR				12
#define SOUND_DEFAULTHIDE				13
#define SOUND_DEFAULTDECEIVE			14
#define SOUND_DEFAULTRELOCATE			15
#define SOUND_DEFAULTANALYZE			16
#define SOUND_DEFAULTSCAN				17
#define SOUND_DEFAULTEVALUATE			18
#define SOUND_DEFAULTDECRYPT			19
#define SOUND_DEFAULTREFLECT			20
#define SOUND_DEFAULTATTACKBOOST		21
#define SOUND_DEFAULTDEFENSEBOOST		22
#define SOUND_DEFAULTSTEALTHBOOST		23
#define SOUND_DEFAULTANALYSISBOOST		24
#define SOUND_DOWNLOADFILE				25
#define SOUND_EDITFILE					26
#define SOUND_ERASEFILE					27
#define SOUND_CRASHSYSTEM				28
#define SOUND_USEIO						29
#define SOUND_GETMAP					30
#define SOUND_CREATEBACKDOOR			31
#define SOUND_CANCELALERT				32
#define SOUND_ENTERPORTAL				33
#define SOUND_CANCELTRACE				34
#define SOUND_CANCELSHUTDOWN			35
#define SOUND_MOVE						36
#define SOUND_UNLOADPROGRAM				37
#define SOUND_ENTERMATRIX				38
#define SOUND_DUMPDISCONNECT			39
#define SOUND_DUMPDECKDAMAGE			40
#define SOUND_DUMPUNCONSCIOUS			41
#define SOUND_DUMPDEATH					42
#define SOUND_DUMPTRACE					43
#define SOUND_DUMPSYSCRASH				44
#define SOUND_DUMPSYSOFFLINE			45
#define SOUND_QUERIED					46
#define SOUND_ATTACKED					47
#define SOUND_YELLOWALERT				48
#define SOUND_REDALERT					49
#define SOUND_SHUTDOWN_COMMENCE			50
#define SOUND_TRACESTART				51
#define SOUND_ICECRASH					52
#define SOUND_DOWNLOADCOMPLETE			53
#define SOUND_EDITCOMPLETE				54
#define SOUND_ICEENTERNODE				55
#define SOUND_ICELEAVENODE				56
#define SOUND_PROGRAMLOADED				57
#define SOUND_SCANCOMPLETE				58
#define SOUND_EVALCOMPLETE				59
#define SOUND_FILEDESTROYED				60
#define SOUND_CUSTOM1					61
#define SOUND_CUSTOM2					62
#define SOUND_CUSTOM3					63
#define SOUND_CUSTOM4					64
#define SOUND_CUSTOM5					65
#define SOUND_CUSTOM6					66
#define SOUND_CUSTOM7					67
#define SOUND_CUSTOM8					68
#define SOUND_CUSTOM9					69
#define SOUND_CUSTOM10					70
#define SOUND_CUSTOM11					71
#define SOUND_CUSTOM12					72
#define SOUND_CUSTOM13					73
#define SOUND_CUSTOM14					74
#define SOUND_CUSTOM15					75
#define SOUND_CUSTOM16					76
#define SOUND_CUSTOM17					77
#define SOUND_CUSTOM18					78
#define SOUND_CUSTOM19					79
#define SOUND_CUSTOM20					80
#define SOUND_DEFAULTCLIENT				81

#define NUM_SOUNDS						82

//------------------
// Global Variables
//------------------
extern BOOL g_bSound;

//-----------------
// Sound functions
//-----------------
void LoadSounds(const char *szDirectory);
void PlayGameSound(int nSound);
void UnloadSounds();

#endif //__DECKERSOUND_H__