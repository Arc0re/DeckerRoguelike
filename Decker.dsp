# Microsoft Developer Studio Project File - Name="Decker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Decker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Decker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Decker.mak" CFG="Decker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Decker - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Decker - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Decker - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /Og- /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /map /debug
# Begin Custom Build
TargetPath=.\Release\Decker.exe
TargetName=Decker
InputPath=.\Release\Decker.exe
SOURCE="$(InputPath)"

".\bin\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) .\bin

# End Custom Build

!ELSEIF  "$(CFG)" == "Decker - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Custom Build
TargetPath=.\Debug\Decker.exe
TargetName=Decker
InputPath=.\Debug\Decker.exe
SOURCE="$(InputPath)"

".\bin\$(TargetName).exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(TargetPath) .\bin

# End Custom Build

!ENDIF 

# Begin Target

# Name "Decker - Win32 Release"
# Name "Decker - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActiveBar.cpp
# End Source File
# Begin Source File

SOURCE=.\AlarmBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Area.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Character.cpp
# End Source File
# Begin Source File

SOURCE=.\CharDataDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ClockView.cpp
# End Source File
# Begin Source File

SOURCE=.\Contract.cpp
# End Source File
# Begin Source File

SOURCE=.\ContractDataDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ContractListDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckDataDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Decker.cpp
# End Source File
# Begin Source File

SOURCE=.\Decker.rc
# End Source File
# Begin Source File

SOURCE=.\DeckerGraphics.cpp
# End Source File
# Begin Source File

SOURCE=.\DeckerSound.cpp
# End Source File
# Begin Source File

SOURCE=.\DSFile.cpp
# End Source File
# Begin Source File

SOURCE=.\EntryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FileAccessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Global.cpp
# End Source File
# Begin Source File

SOURCE=.\HealthBar.cpp
# End Source File
# Begin Source File

SOURCE=.\HomeView.cpp
# End Source File
# Begin Source File

SOURCE=.\Ice.cpp
# End Source File
# Begin Source File

SOURCE=.\IceDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageArray.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\IntroDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MapView.cpp
# End Source File
# Begin Source File

SOURCE=.\MatrixView.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageView.cpp
# End Source File
# Begin Source File

SOURCE=.\MissionResultsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NameBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCharDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewProjectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Node.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeView.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Program.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RestDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Shop.cpp
# End Source File
# Begin Source File

SOURCE=.\ShopItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Source.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\System.cpp
# End Source File
# Begin Source File

SOURCE=.\TransferDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActiveBar.h
# End Source File
# Begin Source File

SOURCE=.\AlarmBar.h
# End Source File
# Begin Source File

SOURCE=.\Area.h
# End Source File
# Begin Source File

SOURCE=.\BuildDialog.h
# End Source File
# Begin Source File

SOURCE=.\Character.h
# End Source File
# Begin Source File

SOURCE=.\CharDataDialog.h
# End Source File
# Begin Source File

SOURCE=.\ClockView.h
# End Source File
# Begin Source File

SOURCE=.\Contract.h
# End Source File
# Begin Source File

SOURCE=.\ContractDataDialog.h
# End Source File
# Begin Source File

SOURCE=.\ContractListDialog.h
# End Source File
# Begin Source File

SOURCE=.\CustomButton.h
# End Source File
# Begin Source File

SOURCE=.\DeckDataDialog.h
# End Source File
# Begin Source File

SOURCE=.\Decker.h
# End Source File
# Begin Source File

SOURCE=.\DeckerGraphics.h
# End Source File
# Begin Source File

SOURCE=.\DeckerSound.h
# End Source File
# Begin Source File

SOURCE=.\DSFile.h
# End Source File
# Begin Source File

SOURCE=.\EntryDlg.h
# End Source File
# Begin Source File

SOURCE=.\FileAccessDlg.h
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\HealthBar.h
# End Source File
# Begin Source File

SOURCE=.\HomeView.h
# End Source File
# Begin Source File

SOURCE=.\Ice.h
# End Source File
# Begin Source File

SOURCE=.\IceDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImageArray.h
# End Source File
# Begin Source File

SOURCE=.\ImageDisplay.h
# End Source File
# Begin Source File

SOURCE=.\IntroDlg.h
# End Source File
# Begin Source File

SOURCE=.\MapView.h
# End Source File
# Begin Source File

SOURCE=.\MatrixView.h
# End Source File
# Begin Source File

SOURCE=.\MessageView.h
# End Source File
# Begin Source File

SOURCE=.\MissionResultsDlg.h
# End Source File
# Begin Source File

SOURCE=.\NameBar.h
# End Source File
# Begin Source File

SOURCE=.\NameDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewCharDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewProjectDlg.h
# End Source File
# Begin Source File

SOURCE=.\Node.h
# End Source File
# Begin Source File

SOURCE=.\NodeView.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\OrderDlg.h
# End Source File
# Begin Source File

SOURCE=.\Program.h
# End Source File
# Begin Source File

SOURCE=.\ProjectDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RestDlg.h
# End Source File
# Begin Source File

SOURCE=.\Shop.h
# End Source File
# Begin Source File

SOURCE=.\ShopItem.h
# End Source File
# Begin Source File

SOURCE=.\SoundEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\Source.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\System.h
# End Source File
# Begin Source File

SOURCE=.\TransferDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\analyze0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\analyze1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\analyze2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_e.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_e1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_e2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_n.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_n1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_n2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_s.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_s1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_s2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_up.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_w.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_w1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\arrow_w2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\attack0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\attack1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\attack2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\backdoor0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\backdoor1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\backdoor2.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bio_cover.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_disabled.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buy0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\buy1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\char0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\char1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\char2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\contract0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\contract1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\contract2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\COP_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CPU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CPU_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\crash0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\crash1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\crash2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\deceive0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\deceive1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\deceive2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\deck0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\deck1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\deck2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Decker.ico
# End Source File
# Begin Source File

SOURCE=.\res\Decker.rc2
# End Source File
# Begin Source File

SOURCE=.\RES\disconnect.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\disconnect0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\disconnect1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\disconnect2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\down.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DS_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\edit0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\edit1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\edit2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\enter_portal0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\enter_portal1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\enter_portal2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\erase0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\erase1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\erase2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\getfile0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\getfile1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\getfile2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\getmap0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\getmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\getmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\hide0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\hide1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\hide2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Home.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ice_attack.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ice_gateway.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ice_guardian.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ice_probe.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\ice_tapeworm.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\ice_trace.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\il_char.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\il_ice.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\il_misc.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\il_special_nodes_16.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\il_special_nodes_32.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\il_time.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IO.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IO_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\item_il.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Junction.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Junction_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\Junction_EW.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\junction_NS.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\killalarm0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\killalarm1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\killalarm2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\KillShutdown0.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\KillShutdown1.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\KillShutdown2.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\killtrace0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\killtrace1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\killtrace2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\line_ew.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\line_ns.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\load0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\load1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\load2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\load3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainRun.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\map_loc.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\map_zoom0.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\map_zoom1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\matrix0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\matrix1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\msg0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\msg1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\msg2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\msg3.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\node_char.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\node_cop.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\node_cpu.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\node_ds.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Node_IL_16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Node_IL_32.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Node_io.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\node_jun.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\node_lin.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\node_portal.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\node_spu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\options0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\options1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Portal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Portal_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\res\program0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\program1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rest0.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rest1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\run_program0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\run_program1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\run_program2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\scan0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\scan1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\scan2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\set_def0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\set_def1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\set_def2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\set_def3.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\software_il.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SPU.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SPU_3D.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Title.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\trace.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\trace_cover.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\unload0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\unload1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\unload2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\UP.BMP
# End Source File
# Begin Source File

SOURCE=.\RES\use_io0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\use_io1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\use_io2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\use_io3.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\view_ice0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\view_ice1.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\view_ice2.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\wait0.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\wait1.bmp
# End Source File
# End Group
# End Target
# End Project
