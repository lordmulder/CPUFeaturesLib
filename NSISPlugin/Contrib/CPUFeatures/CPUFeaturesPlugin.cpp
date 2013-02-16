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
		//srand(static_cast<unsigned int>(time(NULL)));
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

static const struct
{
	UINT32_T flag;
	const TCHAR *name;
}
s_cpu_features[] =
{
	{CPULIB_CPU_MMX, _T("MMX1")},
	{CPULIB_CPU_MMX2, _T("MMX2")},
	{CPULIB_CPU_SSE, _T("SSE1")},
	{CPULIB_CPU_SSE2, _T("SSE2")},
	{CPULIB_CPU_SSE2_IS_SLOW, _T("SSE2_SLOW")},
	{CPULIB_CPU_SSE2_IS_FAST, _T("SSE2_FAST")},
	{CPULIB_CPU_SSE3, _T("SSE3")},
	{CPULIB_CPU_SSSE3, _T("SSSE3")},
	{CPULIB_CPU_SHUFFLE_IS_FAST, _T("FAST_SHUFFLE")},
	{CPULIB_CPU_STACK_MOD4, _T("STACK_MOD4")},
	{CPULIB_CPU_SSE4, _T("SSE4")},
	{CPULIB_CPU_SSE42, _T("SSE4.2")},
	{CPULIB_CPU_SSE_MISALIGN, _T("SSE_MISALIGN")},
	{CPULIB_CPU_LZCNT, _T("LZCNT")},
	{CPULIB_CPU_SLOW_CTZ, _T("SLOW_CTZ")},
	{CPULIB_CPU_SLOW_ATOM, _T("SLOW_ATOM")},
	{CPULIB_CPU_AVX, _T("AVX1")},
	{CPULIB_CPU_XOP, _T("XOP")},
	{CPULIB_CPU_FMA4, _T("FMA4")},
	{CPULIB_CPU_AVX2, _T("AVX2")},
	{CPULIB_CPU_FMA3, _T("FMA3")},
	{0x00000000, NULL}
};

static UINT32_T name2flag(const TCHAR *_name)
{
	for(size_t i = 0; s_cpu_features[i].flag; i++)
	{
		if(_tcsicmp(_name, s_cpu_features[i].name) == 0)
		{
			return s_cpu_features[i].flag;
		}
	}
	return 0;
}

static const TCHAR *flag2name(const UINT32_T _flag)
{
	for(size_t i = 0; s_cpu_features[i].flag; i++)
	{
		if(s_cpu_features[i].flag == _flag)
		{
			return s_cpu_features[i].name;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////

NSISFUNC(GetCPUFlags)
{
	EXDLL_INIT();
	REGSITER_CALLBACK(g_hInstance);
	MAKESTR(str, g_stringsize);

	UINT32_T features = cpulib_cpu_detect();
	_sntprintf(str, g_stringsize, _T("0x%08X"), features);

	pushstring(str);
	delete [] str;
}

///////////////////////////////////////////////////////////////////////////////

#define APPEND(STR, TEXT) do \
{ \
	if(STR[0]) \
	{ \
		_tcscat(STR, _T(", ")); \
		_tcscat(STR, TEXT); \
	} \
	else \
	{ \
		_tcscpy(STR, TEXT); \
	} \
} \
while(0)

NSISFUNC(GetCPUFeatures)
{
	EXDLL_INIT();
	REGSITER_CALLBACK(g_hInstance);
	MAKESTR(str, g_stringsize);

	UINT32_T features = cpulib_cpu_detect();

	for(size_t i = 0; s_cpu_features[i].flag; i++)
	{
		if(features & s_cpu_features[i].flag)
		{
			APPEND(str, s_cpu_features[i].name);
		}
	}

	pushstring(str);
	delete [] str;
}

///////////////////////////////////////////////////////////////////////////////

NSISFUNC(CheckCPUFeature)
{
	EXDLL_INIT();
	REGSITER_CALLBACK(g_hInstance);
	MAKESTR(str, g_stringsize);

	if(popstring(str) != 0)
	{
		pushstring(_T("error"));
		delete [] str;
		return;
	}

	UINT32_T flag = name2flag(str);

	if(flag == 0)
	{
		pushstring(_T("error"));
		delete [] str;
		return;
	}

	UINT32_T features = cpulib_cpu_detect();
	pushstring((features & flag) ? _T("yes") : _T("no"));

	delete [] str;
}

///////////////////////////////////////////////////////////////////////////////

NSISFUNC(GetCPUVendor)
{
	EXDLL_INIT();
	REGSITER_CALLBACK(g_hInstance);
	MAKESTR(str, g_stringsize);

	UINT32_T features = cpulib_cpu_detect();

	if(features & CPULIB_CPU_VENDOR_INTEL)
	{
		_tcscpy(str, _T("Intel"));
	}
	else if(features & CPULIB_CPU_VENDOR_AMD)
	{
		_tcscpy(str, _T("AMD"));
	}
	else
	{
		_tcscpy(str, _T("Other"));
	}

	pushstring(str);
	delete [] str;
}
