/*****************************************************************************
 * cpu.c: cpu detection
 *****************************************************************************
 * Copyright (C) 2003-2013 x264 project
 *
 * Authors: Loren Merritt <lorenm@u.washington.edu>
 *          Laurent Aimar <fenrir@via.ecp.fr>
 *          Jason Garrett-Glaser <darkshikari@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at licensing@x264.com.
 *****************************************************************************/

#include "CPUFeatures.h"

#include <string.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef _WIN64
#define ARCH_X86_64 1
#endif

#define cpulib_log __noop
#define cpulib_cpu_mask_misalign_sse __noop
#define CPULIB_LOG_WARNING 1

int cpulib_cpu_cpuid_test( void );
void cpulib_cpu_cpuid( UINT32_T op, UINT32_T *eax, UINT32_T *ebx, UINT32_T *ecx, UINT32_T *edx );
void cpulib_cpu_xgetbv( UINT32_T op, UINT32_T *eax, UINT32_T *edx );

UINT32_T cpulib_cpu_detect( UINT32_T *vendorflag )
{
    UINT32_T cpu = 0;
    UINT32_T eax, ebx, ecx, edx;
    UINT32_T vendor[4] = {0};
    UINT32_T max_extended_cap;
    int cache;

    if( vendorflag )
        *vendorflag = CPULIB_VENDOR_OTHER;

#if !ARCH_X86_64
    if( !cpulib_cpu_cpuid_test() )
        return 0;
#endif

    cpulib_cpu_cpuid( 0, &eax, vendor+0, vendor+2, vendor+1 );
    if( eax == 0 )
        return 0;

    cpulib_cpu_cpuid( 1, &eax, &ebx, &ecx, &edx );
    if( edx&0x00800000 )
        cpu |= CPULIB_CPU_MMX;
    else
        return 0;
    if( edx&0x02000000 )
        cpu |= CPULIB_CPU_MMX2|CPULIB_CPU_SSE;
    if( edx&0x04000000 )
        cpu |= CPULIB_CPU_SSE2;
    if( ecx&0x00000001 )
        cpu |= CPULIB_CPU_SSE3;
    if( ecx&0x00000200 )
        cpu |= CPULIB_CPU_SSSE3;
    if( ecx&0x00080000 )
        cpu |= CPULIB_CPU_SSE4;
    if( ecx&0x00100000 )
        cpu |= CPULIB_CPU_SSE42;
    /* Check OXSAVE and AVX bits */
    if( (ecx&0x18000000) == 0x18000000 )
    {
        /* Check for OS support */
        cpulib_cpu_xgetbv( 0, &eax, &edx );
        if( (eax&0x6) == 0x6 )
        {
            cpu |= CPULIB_CPU_AVX;
            if( ecx&0x00001000 )
                cpu |= CPULIB_CPU_FMA3;
        }
    }

    cpulib_cpu_cpuid( 7, &eax, &ebx, &ecx, &edx );
    /* AVX2 requires OS support, but BMI1/2 don't. */
    if( (cpu&CPULIB_CPU_AVX) && (ebx&0x00000020) )
        cpu |= CPULIB_CPU_AVX2;
    if( ebx&0x00000008 )
    {
        cpu |= CPULIB_CPU_BMI1;
        if( ebx&0x00000100 )
            cpu |= CPULIB_CPU_BMI2;
    }

    if( cpu & CPULIB_CPU_SSSE3 )
        cpu |= CPULIB_CPU_SSE2_IS_FAST;
    if( cpu & CPULIB_CPU_SSE4 )
        cpu |= CPULIB_CPU_SHUFFLE_IS_FAST;

    cpulib_cpu_cpuid( 0x80000000, &eax, &ebx, &ecx, &edx );
    max_extended_cap = eax;

    if( !strcmp((char*)vendor, "AuthenticAMD") )
    {
        if( vendorflag )
            *vendorflag = CPULIB_VENDOR_AMD;
    }

    if( !strcmp((char*)vendor, "AuthenticAMD") && max_extended_cap >= 0x80000001 )
    {
        cpu |= CPULIB_CPU_SLOW_CTZ;
        cpulib_cpu_cpuid( 0x80000001, &eax, &ebx, &ecx, &edx );
        if( edx&0x00400000 )
            cpu |= CPULIB_CPU_MMX2;
        if( cpu & CPULIB_CPU_SSE2 )
        {
            if( ecx&0x00000040 ) /* SSE4a */
            {
                cpu |= CPULIB_CPU_SSE2_IS_FAST;
                cpu |= CPULIB_CPU_LZCNT;
                cpu |= CPULIB_CPU_SHUFFLE_IS_FAST;
                cpu &= ~CPULIB_CPU_SLOW_CTZ;
            }
            else
                cpu |= CPULIB_CPU_SSE2_IS_SLOW;

            if( ecx&0x00000080 ) /* Misalign SSE */
            {
                cpu |= CPULIB_CPU_SSE_MISALIGN;
                cpulib_cpu_mask_misalign_sse();
            }

            if( cpu & CPULIB_CPU_AVX )
            {
                if( ecx&0x00000800 ) /* XOP */
                    cpu |= CPULIB_CPU_XOP;
                if( ecx&0x00010000 ) /* FMA4 */
                    cpu |= CPULIB_CPU_FMA4;
            }

            if( ecx&0x00200000 )
                cpu |= CPULIB_CPU_TBM;
        }
        if( edx&0x80000000 )
        {
            cpu |= CPULIB_CPU_3DNOW;
        }
        if( edx&0x40000000 )
        {
                cpu |= CPULIB_CPU_3DNOWEX;
        }
    }

    if( !strcmp((char*)vendor, "GenuineIntel") )
    {
        int model, family;
        if( vendorflag )
            *vendorflag = CPULIB_VENDOR_INTEL;
        cpulib_cpu_cpuid( 1, &eax, &ebx, &ecx, &edx );
        family = ((eax>>8)&0xf) + ((eax>>20)&0xff);
        model  = ((eax>>4)&0xf) + ((eax>>12)&0xf0);
        if( family == 6 )
        {
            /* 6/9 (pentium-m "banias"), 6/13 (pentium-m "dothan"), and 6/14 (core1 "yonah")
             * theoretically support sse2, but it's significantly slower than mmx for
             * almost all of x264's functions, so let's just pretend they don't. */
            if( model == 9 || model == 13 || model == 14 )
            {
                cpu &= ~(CPULIB_CPU_SSE2|CPULIB_CPU_SSE3);
                assert(!(cpu&(CPULIB_CPU_SSSE3|CPULIB_CPU_SSE4)));
            }
            /* Detect Atom CPU */
            else if( model == 28 )
            {
                cpu |= CPULIB_CPU_SLOW_ATOM;
                cpu |= CPULIB_CPU_SLOW_CTZ;
            }
            /* Some Penryns and Nehalems are pointlessly crippled (SSE4 disabled), so
             * detect them here. */
            else if( model >= 23 )
                cpu |= CPULIB_CPU_SHUFFLE_IS_FAST;
        }
    }

    if( (!strcmp((char*)vendor, "GenuineIntel") || !strcmp((char*)vendor, "CyrixInstead")) && !(cpu&CPULIB_CPU_SSE42))
    {
        /* cacheline size is specified in 3 places, any of which may be missing */
        cpulib_cpu_cpuid( 1, &eax, &ebx, &ecx, &edx );
        cache = (ebx&0xff00)>>5; // cflush size
        if( !cache && max_extended_cap >= 0x80000006 )
        {
            cpulib_cpu_cpuid( 0x80000006, &eax, &ebx, &ecx, &edx );
            cache = ecx&0xff; // cacheline size
        }
        if( !cache )
        {
            // Cache and TLB Information
            static const char cache32_ids[] = { 0x0a, 0x0c, 0x41, 0x42, 0x43, 0x44, 0x45, 0x82, 0x83, 0x84, 0x85, 0 };
            static const char cache64_ids[] = { 0x22, 0x23, 0x25, 0x29, 0x2c, 0x46, 0x47, 0x49, 0x60, 0x66, 0x67,
                                                0x68, 0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7c, 0x7f, 0x86, 0x87, 0 };
            UINT32_T buf[4];
            int max, i = 0;
            do {
                int j;
                cpulib_cpu_cpuid( 2, buf+0, buf+1, buf+2, buf+3 );
                max = buf[0]&0xff;
                buf[0] &= ~0xff;
                for( j = 0; j < 4; j++ )
                    if( !(buf[j]>>31) )
                        while( buf[j] )
                        {
                            if( strchr( cache32_ids, buf[j]&0xff ) )
                                cache = 32;
                            if( strchr( cache64_ids, buf[j]&0xff ) )
                                cache = 64;
                            buf[j] >>= 8;
                        }
            } while( ++i < max );
        }

        if( cache == 32 )
            cpu |= CPULIB_CPU_CACHELINE_32;
        else if( cache == 64 )
            cpu |= CPULIB_CPU_CACHELINE_64;
        else
            cpulib_log( NULL, CPULIB_LOG_WARNING, "unable to determine cacheline size\n" );
    }

#if BROKEN_STACK_ALIGNMENT
    cpu |= CPULIB_CPU_STACK_MOD4;
#endif

    return cpu;
}

#ifdef __INTEL_COMPILER
/* Agner's patch to Intel's CPU dispatcher from pages 131-132 of
 * http://agner.org/optimize/optimizing_cpp.pdf (2011-01-30)
 * adapted to x264's cpu schema. */

// Global variable indicating cpu
int __intel_cpu_indicator = 0;
// CPU dispatcher function
void cpulib_intel_cpu_indicator_init( void )
{
    unsigned int cpu = cpulib_cpu_detect();
    if( cpu&CPULIB_CPU_AVX )
        __intel_cpu_indicator = 0x20000;
    else if( cpu&CPULIB_CPU_SSE42 )
        __intel_cpu_indicator = 0x8000;
    else if( cpu&CPULIB_CPU_SSE4 )
        __intel_cpu_indicator = 0x2000;
    else if( cpu&CPULIB_CPU_SSSE3 )
        __intel_cpu_indicator = 0x1000;
    else if( cpu&CPULIB_CPU_SSE3 )
        __intel_cpu_indicator = 0x800;
    else if( cpu&CPULIB_CPU_SSE2 && !(cpu&CPULIB_CPU_SSE2_IS_SLOW) )
        __intel_cpu_indicator = 0x200;
    else if( cpu&CPULIB_CPU_SSE )
        __intel_cpu_indicator = 0x80;
    else if( cpu&CPULIB_CPU_MMX2 )
        __intel_cpu_indicator = 8;
    else
        __intel_cpu_indicator = 1;
}

/* __intel_cpu_indicator_init appears to have a non-standard calling convention that
 * assumes certain registers aren't preserved, so we'll route it through a function
 * that backs up all the registers. */
void __intel_cpu_indicator_init( void )
{
    cpulib_safe_intel_cpu_indicator_init();
}
#else
void cpulib_intel_cpu_indicator_init( void )
{}
#endif

int cpulib_num_processors()
{
    DWORD_PTR system_cpus, bit, process_cpus = 0;
    int cpus = 0;

    /* GetProcessAffinityMask returns affinities of 0 when the process has threads in multiple processor groups.
     * On platforms that support processor grouping, use GetThreadGroupAffinity to get the current thread's affinity instead. */
#if ARCH_X86_64
    /* find function pointers to API functions specific to x86_64 platforms, if they exist */
    HANDLE kernel_dll = GetModuleHandle( TEXT( "kernel32.dll" ) );
    BOOL (*get_thread_affinity)( HANDLE thread, x264_group_affinity_t *group_affinity ) = (void*)GetProcAddress( kernel_dll, "GetThreadGroupAffinity" );
    if( get_thread_affinity )
    {
        /* running on a platform that supports >64 logical cpus */
        x264_group_affinity_t thread_affinity;
        if( get_thread_affinity( GetCurrentThread(), &thread_affinity ) )
            process_cpus = thread_affinity.mask;
    }
#endif
    if( !process_cpus )
        GetProcessAffinityMask( GetCurrentProcess(), &process_cpus, &system_cpus );
    for( bit = 1; bit; bit <<= 1 )
        cpus += !!(process_cpus & bit);
    return cpus ? cpus : 1;
}