/*****************************************************************************
 * x264.h: x264 public header
 *****************************************************************************
 * Copyright (C) 2003-2013 x264 project
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *          Loren Merritt <lorenm@u.washington.edu>
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

#ifndef CPULIB_CPU_H
#define CPULIB_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Types
 */
typedef unsigned int UINT32_T;

/* CPU flags
 */
#define CPULIB_CPU_CACHELINE_32    0x0000001  /* avoid memory loads that span the border between two cachelines */
#define CPULIB_CPU_CACHELINE_64    0x0000002  /* 32/64 is the size of a cacheline in bytes */
#define CPULIB_CPU_ALTIVEC         0x0000004
#define CPULIB_CPU_MMX             0x0000008
#define CPULIB_CPU_MMX2            0x0000010  /* MMX2 aka MMXEXT aka ISSE */
#define CPULIB_CPU_MMXEXT          CPULIB_CPU_MMX2
#define CPULIB_CPU_SSE             0x0000020
#define CPULIB_CPU_SSE2            0x0000040
#define CPULIB_CPU_SSE2_IS_SLOW    0x0000080  /* avoid most SSE2 functions on Athlon64 */
#define CPULIB_CPU_SSE2_IS_FAST    0x0000100  /* a few functions are only faster on Core2 and Phenom */
#define CPULIB_CPU_SSE3            0x0000200
#define CPULIB_CPU_SSSE3           0x0000400
#define CPULIB_CPU_SHUFFLE_IS_FAST 0x0000800  /* Penryn, Nehalem, and Phenom have fast shuffle units */
#define CPULIB_CPU_STACK_MOD4      0x0001000  /* if stack is only mod4 and not mod16 */
#define CPULIB_CPU_SSE4            0x0002000  /* SSE4.1 */
#define CPULIB_CPU_SSE42           0x0004000  /* SSE4.2 */
#define CPULIB_CPU_SSE_MISALIGN    0x0008000  /* Phenom support for misaligned SSE instruction arguments */
#define CPULIB_CPU_LZCNT           0x0010000  /* Phenom support for "leading zero count" instruction. */
#define CPULIB_CPU_ARMV6           0x0020000
#define CPULIB_CPU_NEON            0x0040000  /* ARM NEON */
#define CPULIB_CPU_FAST_NEON_MRC   0x0080000  /* Transfer from NEON to ARM register is fast (Cortex-A9) */
#define CPULIB_CPU_SLOW_CTZ        0x0100000  /* BSR/BSF x86 instructions are really slow on some CPUs */
#define CPULIB_CPU_SLOW_ATOM       0x0200000  /* The Atom just sucks */
#define CPULIB_CPU_AVX             0x0400000  /* AVX support: requires OS support even if YMM registers
                                             * aren't used. */
#define CPULIB_CPU_XOP             0x0800000  /* AMD XOP */
#define CPULIB_CPU_FMA4            0x1000000  /* AMD FMA4 */
#define CPULIB_CPU_AVX2            0x2000000  /* AVX2 */
#define CPULIB_CPU_FMA3            0x4000000  /* Intel FMA3 */
#define CPULIB_CPU_BMI1            0x8000000  /* BMI1 */
#define CPULIB_CPU_BMI2           0x10000000  /* BMI2 */
#define CPULIB_CPU_TBM            0x20000000  /* AMD TBM */
#define CPULIB_CPU_3DNOW          0x40000000  /* 3DNow! */
#define CPULIB_CPU_3DNOWEX        0x80000000  /* 3DNow!Ex aka 3DNow!2 */

/* CPU vendor flags
 */
#define CPULIB_VENDOR_OTHER       0x00000000  /* Unknown */
#define CPULIB_VENDOR_INTEL       0x00000001  /* Intel */
#define CPULIB_VENDOR_AMD         0x00000002  /* AMD */

/* Functions
 */
UINT32_T cpulib_cpu_detect( UINT32_T *vendorflag );
int cpulib_num_processors( void );

#ifdef __cplusplus
}
#endif

#endif CPULIB_CPU_H
