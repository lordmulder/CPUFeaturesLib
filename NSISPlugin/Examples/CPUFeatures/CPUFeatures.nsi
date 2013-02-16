Caption "CPUFeatures Test-Suite"

!addincludedir  "..\..\Include"

!ifdef NSIS_UNICODE
	!addplugindir "..\..\Plugins\Release_Unicode"
	OutFile "CPUFeatures-Unicode.exe"
!else
	!addplugindir "..\..\Plugins\Release_ANSI"
	OutFile "CPUFeatures-ANSI.exe"
!endif

RequestExecutionLevel user
ShowInstDetails show

Section
	CPUFeatures::GetCPUFeatures
	Pop $0
	MessageBox MB_TOPMOST "$0"
SectionEnd
