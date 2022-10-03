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

#ifndef __DECKERGRAPHICS_H__
#define __DECKERGRAPHICS_H__

//----------------------
// Graphics identifiers
//----------------------
// Non-alterable graphics
//#define GR_TITLE  
//#define GR_IL_ICE                      
//#define GR_IL_CHAR                     
//#define GR_IL_MISC                     
//#define GR_IL_TIME                     
//#define GR_ITEM_IL                     
//#define GR_SOFTWARE_IL                 
//#define GR_NODE_IL_32                  
//#define GR_NODE_IL_16                  
//#define GR_IL_SPECIAL_NODES_16         
//#define GR_IL_SPECIAL_NODES_32         
//#define GR_NODE_CPU                    
//#define GR_NODE_SPU                    
//#define GR_NODE_COP                    
//#define GR_NODE_DS                     
//#define GR_NODE_LINE_EW                
//#define GR_NODE_LINE_NS                
//#define GR_NODE_IO                     
//#define GR_NODE_PORTAL                 
//#define GR_NODE_JUNCTION               
//#define GR_UP                          
//#define GR_DOWN                        

// Home graphics                     
#define GR_HOME_MAIN				0
#define GR_HOME_CHAR0				1
#define GR_HOME_CHAR1				2
#define GR_HOME_DECK0				3
#define GR_HOME_DECK1				4
#define GR_HOME_CONTRACT0			5
#define GR_HOME_CONTRACT1			6
#define GR_HOME_SHOP0				7
#define GR_HOME_SHOP1				8
#define GR_HOME_PROJECT1			9
#define GR_HOME_PROJECT0			10
#define GR_HOME_REST0				11
#define GR_HOME_REST1				12
#define GR_HOME_MATRIX0				13
#define GR_HOME_MATRIX1				14
#define GR_HOME_OPTIONS0			15
#define GR_HOME_OPTIONS1			16

// Matrix graphics
#define GR_MATRIX_MAIN				17
#define GR_MATRIX_ARROW_N0			18          
#define GR_MATRIX_ARROW_N1			19          
#define GR_MATRIX_ARROW_N2			20        
#define GR_MATRIX_ARROW_E0			21        
#define GR_MATRIX_ARROW_E1			22        
#define GR_MATRIX_ARROW_E2			23        
#define GR_MATRIX_ARROW_S0			24        
#define GR_MATRIX_ARROW_S1			25        
#define GR_MATRIX_ARROW_S2			26        
#define GR_MATRIX_ARROW_W0			27        
#define GR_MATRIX_ARROW_W1			28        
#define GR_MATRIX_ARROW_W2			29        
#define GR_MATRIX_WAIT0				30        
#define GR_MATRIX_WAIT1				31        
#define GR_MATRIX_ATTACK0			32        
#define GR_MATRIX_ATTACK1			33        
#define GR_MATRIX_ATTACK2			34        
#define GR_MATRIX_DECEIVE0			35        
#define GR_MATRIX_DECEIVE1			36        
#define GR_MATRIX_DECEIVE2			37
#define GR_MATRIX_VIEW_ICE0			38        
#define GR_MATRIX_VIEW_ICE1         39          
#define GR_MATRIX_VIEW_ICE2         40          
#define GR_MATRIX_SCAN0             41          
#define GR_MATRIX_SCAN1             42          
#define GR_MATRIX_SCAN2             43          
#define GR_MATRIX_CHAR0				44
#define GR_MATRIX_CHAR1				45
#define GR_MATRIX_DECK0				46
#define GR_MATRIX_DECK1				47
#define GR_MATRIX_CONTRACT0			48
#define GR_MATRIX_CONTRACT1			49
#define GR_MATRIX_DISCONNECT0		50
#define GR_MATRIX_DISCONNECT1		51        
#define GR_MATRIX_OPTIONS0			52
#define GR_MATRIX_OPTIONS1			53
#define GR_MATRIX_RUN_PROGRAM0		54
#define GR_MATRIX_RUN_PROGRAM1		55
#define GR_MATRIX_RUN_PROGRAM2		56
#define GR_MATRIX_SET_DEF0			57
#define GR_MATRIX_SET_DEF1			58
#define GR_MATRIX_SET_DEF2			59
#define GR_MATRIX_LOAD0				60
#define GR_MATRIX_LOAD1				61
#define GR_MATRIX_LOAD2				62
#define GR_MATRIX_UNLOAD0			63
#define GR_MATRIX_UNLOAD1			64
#define GR_MATRIX_UNLOAD2			65
#define GR_MATRIX_GETFILE0			66
#define GR_MATRIX_GETFILE1			67
#define GR_MATRIX_GETFILE2			68
#define GR_MATRIX_EDIT0				69
#define GR_MATRIX_EDIT1				70
#define GR_MATRIX_EDIT2				71
#define GR_MATRIX_ERASE0			72
#define GR_MATRIX_ERASE1			73
#define GR_MATRIX_ERASE2			74
#define GR_MATRIX_CRASH0			75
#define GR_MATRIX_CRASH1			76
#define GR_MATRIX_CRASH2			77
#define GR_MATRIX_USE_IO0			78
#define GR_MATRIX_USE_IO1			79
#define GR_MATRIX_USE_IO2			80
#define GR_MATRIX_GETMAP0			81
#define GR_MATRIX_GETMAP1			82        
#define GR_MATRIX_GETMAP2			83
#define GR_MATRIX_BACKDOOR0			84
#define GR_MATRIX_BACKDOOR1			85
#define GR_MATRIX_BACKDOOR2			86
#define GR_MATRIX_KILL_ALARM0		87
#define GR_MATRIX_KILL_ALARM1		88
#define GR_MATRIX_KILL_ALARM2		89
#define GR_MATRIX_ENTER_PORTAL0		90
#define GR_MATRIX_ENTER_PORTAL1		91
#define GR_MATRIX_ENTER_PORTAL2		92
#define GR_MATRIX_KILL_TRACE0		93
#define GR_MATRIX_KILL_TRACE1		94	
#define GR_MATRIX_KILL_TRACE2		95	
#define GR_MATRIX_KILL_SHUTDOWN0	96	
#define GR_MATRIX_KILL_SHUTDOWN1	97	  
#define GR_MATRIX_KILL_SHUTDOWN2	98	  
#define GR_MATRIX_MAP_ZOOM0			99		  
#define GR_MATRIX_MAP_ZOOM1			100		  
#define GR_MATRIX_BIO_COVER			101		  
#define GR_MATRIX_TRACE_COVER		102	

// Miscellaneous graphics
#define GR_MISC_CHARACTERS			103

#define GR_COUNT					104

// Size of a character image
#define CHAR_IMAGE_SIZE				24

//------------------
// Global Variables
//------------------
extern HBITMAP g_hBitmaps[GR_COUNT];
extern int g_nNumCharImages;

//-----------
// Functions
//-----------
BOOL LoadGraphics(const char *szDirectory);
void UnloadGraphics();


#endif //__DECKERGRAPHICS_H__