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

; Get CPU flags (this is for debugging!)
Section
	${CPUFeatures.GetFlags} $0
	DetailPrint "CPU Flags: $0"
SectionEnd

; Get all supported features + vendor name
Section
	${CPUFeatures.GetFeatures} $0
	DetailPrint "CPU Features: $0"

	${CPUFeatures.GetVendor} $0
	DetailPrint "CPU Vendor: $0"
SectionEnd

; Check individual feature flags
; Find a list of supported feature flags in CPUFeatures.nsh!
Section
	${CPUFeatures.CheckFeature} "MMX1" $0
	DetailPrint "Has MMX: $0"

	${CPUFeatures.CheckFeature} "SSE1" $0
	DetailPrint "Has SSE: $0"

	${CPUFeatures.CheckFeature} "SSE2" $0
	DetailPrint "Has SSE2: $0"

	${CPUFeatures.CheckFeature} "SSE3" $0
	DetailPrint "Has SSE3: $0"

	${CPUFeatures.CheckFeature} "SSSE3" $0
	DetailPrint "Has SSSE3: $0"

	${CPUFeatures.CheckFeature} "SSE4.2" $0
	DetailPrint "Has SSE4.2: $0"

	; Next call is supposed to fail!
	${CPUFeatures.CheckFeature} "SSE7" $0
	DetailPrint "Has SSE7: $0"
SectionEnd
