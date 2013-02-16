Caption "CPUFeatures Test-Suite"

!addincludedir  "..\..\Include"

!ifdef NSIS_UNICODE
	!addplugindir "..\..\Plugins\Release_Unicode"
	OutFile "CPUFeatures-Unicode.exe"
!else
	!addplugindir "..\..\Plugins\Release_ANSI"
	OutFile "CPUFeatures-ANSI.exe"
!endif

!include "CPUFeatures.nsh"

RequestExecutionLevel user
ShowInstDetails show

Section
	${CPUFeatures.GetFlags} $0
	DetailPrint "CPU Flags: $0"

	${CPUFeatures.GetFeatures} $0
	DetailPrint "CPU Features: $0"

	${CPUFeatures.GetVendor} $0
	DetailPrint "CPU Vendor: $0"

	${CPUFeatures.CheckFeature} "MMX1" $0
	DetailPrint "Has MMX: $0"

	${CPUFeatures.CheckFeature} "SSSE3" $0
	DetailPrint "Has SSSE3: $0"

	${CPUFeatures.CheckFeature} "SSE4.2" $0
	DetailPrint "Has SSE4.2: $0"

	${CPUFeatures.CheckFeature} "SSE7" $0
	DetailPrint "Has SSE7: $0"
SectionEnd
