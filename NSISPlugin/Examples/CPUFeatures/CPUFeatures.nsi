!define /date BUILD_DATE "%Y-%m-%d"
Caption "CPUFeatures Test-Suite"
BrandingText "Built on ${BUILD_DATE}"

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
	
	${CPUFeatures.GetCount} $0
	DetailPrint "CPU Count: $0"
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

	${CPUFeatures.CheckFeature} "MMX2" $0
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

	${CPUFeatures.CheckFeature} "AVX1" $0
	DetailPrint "Has AVX1: $0"

	${CPUFeatures.CheckFeature} "AVX2" $0
	DetailPrint "Has AVX2: $0"

	${CPUFeatures.CheckFeature} "3DNOW" $0
	DetailPrint "Has 3DNOW: $0"

	${CPUFeatures.CheckFeature} "3DNOW_EX" $0
	DetailPrint "Has 3DNOW_EX: $0"

	${CPUFeatures.CheckFeature} "FMA3" $0
	DetailPrint "Has FMA3: $0"

	${CPUFeatures.CheckFeature} "FMA4" $0
	DetailPrint "Has FMA4: $0"

	; Next call is supposed to fail!
	${CPUFeatures.CheckFeature} "SSE7" $0
	DetailPrint "Has SSE7: $0"
SectionEnd
