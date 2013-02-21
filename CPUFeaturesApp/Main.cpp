// CPUFeatures.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <tchar.h>

#include "CPUFeatures.h"

#define CHECK_FEATURE(WHICH) do \
{ \
	bool __have = ((features & (WHICH)) != 0); \
	fprintf(stderr, #WHICH " : %s\n", __have ? "YES" : "NO"); \
} \
while(0)

int _tmain(int argc, _TCHAR* argv[])
{
	fprintf(stderr, "CPUFeatureLib Test App [%s]\n\n", __DATE__);
	
	/* Query CPU information */
	UINT32_T vendor_flags;
	UINT32_T features = cpulib_cpu_detect(&vendor_flags);
	int num_processors = cpulib_num_processors();
	
	/* Print basic information */
	fprintf(stderr, "CPU Count:    %d\n", num_processors);
	fprintf(stderr, "CPU Features: 0x%08X\n", features);
	fprintf(stderr, "CPU Vendor:   %u", vendor_flags);
	
	/* Print the vendor name */
	switch(vendor_flags)
	{
	case CPULIB_VENDOR_INTEL:
		fprintf(stderr, " (Intel)\n");
		break;
	case CPULIB_VENDOR_AMD:
		fprintf(stderr, " (AMD)\n");
		break;
	default:
		fprintf(stderr, " (Unknown)\n");
		break;
	}
	

	/* Print detailed feature information */
	fprintf(stderr, "\n[Details]\n");
	CHECK_FEATURE(CPULIB_CPU_CACHELINE_32);
	CHECK_FEATURE(CPULIB_CPU_CACHELINE_64);
	CHECK_FEATURE(CPULIB_CPU_ALTIVEC);
	CHECK_FEATURE(CPULIB_CPU_MMX);
	CHECK_FEATURE(CPULIB_CPU_MMX2);
	CHECK_FEATURE(CPULIB_CPU_SSE);
	CHECK_FEATURE(CPULIB_CPU_SSE2);
	CHECK_FEATURE(CPULIB_CPU_SSE2_IS_SLOW);
	CHECK_FEATURE(CPULIB_CPU_SSE2_IS_FAST);
	CHECK_FEATURE(CPULIB_CPU_SSE3);
	CHECK_FEATURE(CPULIB_CPU_SSSE3);
	CHECK_FEATURE(CPULIB_CPU_SHUFFLE_IS_FAST);
	CHECK_FEATURE(CPULIB_CPU_STACK_MOD4);
	CHECK_FEATURE(CPULIB_CPU_SSE4);
	CHECK_FEATURE(CPULIB_CPU_SSE42);
	CHECK_FEATURE(CPULIB_CPU_SSE_MISALIGN);
	CHECK_FEATURE(CPULIB_CPU_LZCNT);
	CHECK_FEATURE(CPULIB_CPU_ARMV6);
	CHECK_FEATURE(CPULIB_CPU_NEON);
	CHECK_FEATURE(CPULIB_CPU_FAST_NEON_MRC);
	CHECK_FEATURE(CPULIB_CPU_SLOW_CTZ);
	CHECK_FEATURE(CPULIB_CPU_SLOW_ATOM);
	CHECK_FEATURE(CPULIB_CPU_AVX);
	CHECK_FEATURE(CPULIB_CPU_XOP);
	CHECK_FEATURE(CPULIB_CPU_FMA4);
	CHECK_FEATURE(CPULIB_CPU_AVX2);
	CHECK_FEATURE(CPULIB_CPU_FMA3);
	CHECK_FEATURE(CPULIB_CPU_BMI1);
	CHECK_FEATURE(CPULIB_CPU_BMI2);
	CHECK_FEATURE(CPULIB_CPU_TBM);
	CHECK_FEATURE(CPULIB_CPU_3DNOW);
	CHECK_FEATURE(CPULIB_CPU_3DNOWEX);

	printf("\n");
	return 0;
}
