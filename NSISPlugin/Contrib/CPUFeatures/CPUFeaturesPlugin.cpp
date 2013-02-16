///////////////////////////////////////////////////////////////////////////////
// StdUtils plug-in for NSIS
// Copyright (C) 2004-2013 LoRd_MuldeR <MuldeR2@GMX.de>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// http://www.gnu.org/licenses/lgpl-2.1.txt
///////////////////////////////////////////////////////////////////////////////

#include "CPUFeaturesPlugin.h"
#include <CPUFeatures.h>

HANDLE g_hInstance;
bool g_bCallbackRegistred;
bool g_bVerbose;

///////////////////////////////////////////////////////////////////////////////

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		g_hInstance = hInst;
		g_bCallbackRegistred = false;
		g_bVerbose = false;
		srand(static_cast<unsigned int>(time(NULL)));
	}
	return TRUE;
}

static UINT_PTR PluginCallback(enum NSPIM msg)
{
	switch(msg)
	{
	case NSPIM_UNLOAD:
	case NSPIM_GUIUNLOAD:
		break;
	default:
		MessageBoxA(NULL, "Unknown callback message. Take care!", "CPUFeatures", MB_ICONWARNING|MB_TOPMOST|MB_TASKMODAL);
		break;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

#define APPEND(STR, TEXT) do \
{ \
  if(STR[0]) \
	_tcscat(STR, _T(", ") TEXT); \
  else \
	_tcscpy(STR, TEXT); \
} \
while(0)

#define CHECK_FEATURE(FLAG, NAME) do \
{ \
	if((features & (FLAG)) != 0) APPEND(str, NAME); \
} \
while(0)

NSISFUNC(GetCPUFeatures)
{
	EXDLL_INIT();
	REGSITER_CALLBACK(g_hInstance);
	MAKESTR(str, g_stringsize);

	UINT32_T features = cpulib_cpu_detect();

	CHECK_FEATURE(CPULIB_CPU_MMX, _T("MMX1"));
	CHECK_FEATURE(CPULIB_CPU_MMX2, _T("MMX2"));
	CHECK_FEATURE(CPULIB_CPU_SSE, _T("SSE1"));
	CHECK_FEATURE(CPULIB_CPU_SSE2, _T("SSE2"));
	CHECK_FEATURE(CPULIB_CPU_SSE2_IS_SLOW, _T("SSE2_SLOW"));
	CHECK_FEATURE(CPULIB_CPU_SSE2_IS_FAST, _T("SSE2_FAST"));
	CHECK_FEATURE(CPULIB_CPU_SSE3, _T("SSE3"));
	CHECK_FEATURE(CPULIB_CPU_SSSE3, _T("SSSE3"));
	CHECK_FEATURE(CPULIB_CPU_SHUFFLE_IS_FAST, _T("FAST_SHUFFLE"));
	CHECK_FEATURE(CPULIB_CPU_STACK_MOD4, _T("STACK_MOD4"));
	CHECK_FEATURE(CPULIB_CPU_SSE4, _T("SSE4"));
	CHECK_FEATURE(CPULIB_CPU_SSE42, _T("SSE4.2"));
	CHECK_FEATURE(CPULIB_CPU_SSE_MISALIGN, _T("SSE_MISALIGN"));
	CHECK_FEATURE(CPULIB_CPU_LZCNT, _T("LZCNT"));
	CHECK_FEATURE(CPULIB_CPU_SLOW_CTZ, _T("SLOW_CTZ"));
	CHECK_FEATURE(CPULIB_CPU_SLOW_ATOM, _T("SLOW_ATOM"));
	CHECK_FEATURE(CPULIB_CPU_AVX, _T("AVX1"));
	CHECK_FEATURE(CPULIB_CPU_XOP, _T("XOP"));
	CHECK_FEATURE(CPULIB_CPU_FMA4, _T("FMA4"));
	CHECK_FEATURE(CPULIB_CPU_AVX2, _T("AVX2"));
	CHECK_FEATURE(CPULIB_CPU_FMA3, _T("FMA3"));

	pushstring(str);
	delete [] str;
}
