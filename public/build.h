/*
build.h - compile-time build information

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/
#pragma once
#ifndef BUILD_H
#define BUILD_H

/*
All IB_* macros set by this header are guaranteed to have positive value
otherwise not defined.

Every macro is intended to be the unified interface for buildsystems that lack
platform & CPU detection, and a neat quick way for checks in platform code
For Q_build* macros, refer to buildenums.h

Any new define must be undefined at first
You can generate #undef list below with this oneliner:
  $ sed 's/\t//g' build.h | grep '^#define XASH' | awk '{ print $2 }' | \
		sort | uniq | awk '{ print "#undef " $1 }'

Then you can use another oneliner to query all variables:
  $ grep '^#undef XASH' build.h | awk '{ print $2 }'
*/

#undef IB_64BIT
#undef IB_AMD64
#undef IB_ANDROID
#undef IB_APPLE
#undef IB_ARM
#undef IB_ARM_HARDFP
#undef IB_ARM_SOFTFP
#undef IB_ARMv4
#undef IB_ARMv5
#undef IB_ARMv6
#undef IB_ARMv7
#undef IB_ARMv8
#undef IB_BIG_ENDIAN
#undef IB_DOS4GW
#undef IB_E2K
#undef IB_EMSCRIPTEN
#undef IB_FREEBSD
#undef IB_HAIKU
#undef IB_IOS
#undef IB_IRIX
#undef IB_JS
#undef IB_LINUX
#undef IB_LITTLE_ENDIAN
#undef IB_MIPS
#undef IB_MOBILE_PLATFORM
#undef IB_NETBSD
#undef IB_OPENBSD
#undef IB_POSIX
#undef IB_PPC
#undef IB_RISCV
#undef IB_RISCV_DOUBLEFP
#undef IB_RISCV_SINGLEFP
#undef IB_RISCV_SOFTFP
#undef IB_SERENITY
#undef IB_WIN32
#undef IB_X86
#undef IB_NSWITCH
#undef IB_PSVITA

//================================================================
//
//           PLATFORM DETECTION CODE
//
//================================================================
#if defined _WIN32
	#define IB_WIN32 1
#else 
	#error
#endif


//================================================================
//
//           ENDIANNESS DEFINES
//
//================================================================

#if !defined IB_ENDIANNESS
	#if defined IB_WIN32 || __LITTLE_ENDIAN__
		//!!! Probably all WinNT installations runs in little endian
		#define IB_LITTLE_ENDIAN 1
	#else
		#error
	#endif 
#endif

//================================================================
//
//           CPU ARCHITECTURE DEFINES
//
//================================================================
#if defined __x86_64__ || defined _M_X64
	#define IB_64BIT 1
	#define IB_AMD64 1
#elif defined __i386__ || defined _X86_ || defined _M_IX86
	#define IB_X86 1
#elif defined __aarch64__ || defined _M_ARM64
	#define IB_64BIT 1
	#define IB_ARM   8
#elif defined __mips__
	#define IB_MIPS 1
#elif defined __EMSCRIPTEN__
	#define IB_JS 1
#elif defined __e2k__
	#define IB_64BIT 1
	#define IB_E2K 1
#elif defined __PPC__ || defined __powerpc__
	#define IB_PPC 1
	#if defined __PPC64__ || defined __powerpc64__
		#define IB_64BIT 1
	#endif
#elif defined _M_ARM // msvc
	#define IB_ARM 7
	#define IB_ARM_HARDFP 1
#elif defined __arm__
	#if __ARM_ARCH == 8 || __ARM_ARCH_8__
		#define IB_ARM 8
	#elif __ARM_ARCH == 7 || __ARM_ARCH_7__
		#define IB_ARM 7
	#elif __ARM_ARCH == 6 || __ARM_ARCH_6__ || __ARM_ARCH_6J__
		#define IB_ARM 6
	#elif __ARM_ARCH == 5 || __ARM_ARCH_5__
		#define IB_ARM 5
	#elif __ARM_ARCH == 4 || __ARM_ARCH_4__
		#define IB_ARM 4
	#else
		#error "Unknown ARM"
	#endif

	#if defined __SOFTFP__ || __ARM_PCS_VFP == 0
		#define IB_ARM_SOFTFP 1
	#else // __SOFTFP__
		#define IB_ARM_HARDFP 1
	#endif // __SOFTFP__
#elif defined __riscv
	#define IB_RISCV 1

	#if __riscv_xlen == 64
		#define IB_64BIT 1
	#elif __riscv_xlen != 32
		#error "Unknown RISC-V ABI"
	#endif

	#if defined __riscv_float_abi_soft
		#define IB_RISCV_SOFTFP 1
	#elif defined __riscv_float_abi_single
		#define IB_RISCV_SINGLEFP 1
	#elif defined __riscv_float_abi_double
		#define IB_RISCV_DOUBLEFP 1
	#else
		#error "Unknown RISC-V float ABI"
	#endif
#else
	#error "Place your architecture name here! If this is a mistake, try to fix conditions above and report a bug"
#endif

#if IB_ARM == 8
	#define IB_ARMv8 1
#elif IB_ARM == 7
	#define IB_ARMv7 1
#elif IB_ARM == 6
	#define IB_ARMv6 1
#elif IB_ARM == 5
	#define IB_ARMv5 1
#elif IB_ARM == 4
	#define IB_ARMv4 1
#endif

#endif // BUILD_H
