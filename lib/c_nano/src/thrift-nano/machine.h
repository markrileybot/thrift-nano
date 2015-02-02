/*
 * libmowgli: A collection of useful routines for programming.
 * machine.h: Defines to discover what machine we're on easily
 *
 * Copyright (c) 2012, 2013 Patrick McFarland <pmcfarland@adterrasperaspera.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* Machine environment specific macros, mostly sourced from this URL:
 * http://sourceforge.net/apps/mediawiki/predef/
 *
 * Please note: Just because a compiler, CPU, or OS is listed in this file,
 * that doesn't mean that it is supported by libmowgli. libmowgli requires
 * a 32-bit or higher CPU and an OS that supports a supported mutex and
 * thread scheme (if you intend on having functional multithreaded
 * operation).
 */

#ifndef __TN_MACHINE_H__
#define __TN_MACHINE_H__

#if defined __clang__
# define TN_COMPILER_CLANG
# define TN_COMPILER clang
#elif defined __INTEL_COMPILER || defined __ICC || defined __ICL
# define TN_COMPILER_ICC
# define TN_COMPILER icc
#elif defined __CC_ARM
# define TN_COMPILER_ARM
# define TN_COMPILER arm
#elif defined __xlc__ || defined __xlC__
# define TN_COMPILER_IBM
# define TN_COMPILER ibm
#elif defined __SUNPRO_C || defined __SUNPRO_CC
# define TN_COMPILER_SUN
# define TN_COMPILER sun
#elif defined __GNUC__
# define TN_COMPILER_GCC
# define TN_COMPILER gcc
#elif defined _MSC_VER
# define TN_COMPILER_MSVC
# define TN_COMPILER msvc
#else
# define TN_COMPILER_UNKNOWN
# define TN_COMPILER unknown
#endif

#if defined __GNUC__
# define TN_COMPILER_GCC_COMPAT
# define TN_COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
# define TN_COMPILER_GCC_VERSION (0)
#endif

#if defined __amd64__ || defined __amd64 || defined __x86_64__ || defined __x86_64 || defined _M_X64 || defined _M_AMD64
# define TN_CPU_X86_64
# define TN_CPU x86_64
# define TN_CPU_BITS_64
# define TN_CPU_BITS 64
# define TN_CPU_ENDIAN_LITTLE
# define TN_CPU_ENDIAN little
#elif defined __i386__ || defined __i386 || defined __IA32__ || defined _M_IX86 || defined __X86__ || defined _X86__ || defined __I86__
# define TN_CPU_X86
# define TN_CPU x86
# define TN_CPU_BITS_32
# define TN_CPU_BITS 32
# define TN_CPU_ENDIAN_LITTLE
# define TN_CPU_ENDIAN little
#elif defined __arm__ || defined __TARGET_ARCH_ARM || defined _ARM
# if defined __thumb__ || defined __TARGET_ARCH_THUMB
#  define TN_CPU_ARM_THUMB
# endif
# define TN_CPU_ARM
# define TN_CPU arm
# define TN_CPU_BITS_32
# define TN_CPU_BITS 32

// ARM can be either endian
#elif defined __hppa__ || defined __HPPA__ || defined __hppa
# if defined _PA_RISC2_0 || defined __HPPA20__ || defined __RISC2_0__
#  define TN_CPU_HPPA20
#  define TN_CPU hppa20
#  define TN_CPU_BITS_64
#  define TN_CPU_BITS 64
# else
#  define TN_CPU_HPPA10
#  define TN_CPU hppa10
#  define TN_CPU_BITS_32
#  define TN_CPU_BITS 32
# endif
# define TN_CPU_ENDIAN_BIG
# define TN_CPU_ENDIAN big
#elif defined __ia64__ || defined __IA64__ || defined _M_IA64 || defined __ia64 || defined __itanium__
# define TN_CPU_ITANIUM
# define TN_CPU itanium
# define TN_CPU_BITS_64
# define TN_CPU_BITS 64

// Itanium can be either endian
#elif defined __mips__ || defined mips || defined __mips || defined __MIPS__
# if defined __mips64 || defined __mips64__
#  define TN_CPU_MIPS64
#  define TN_CPU mips64
#  define TN_CPU_BITS_64
#  define TN_CPU_BITS 64
# else
#  define TN_CPU_MIPS
#  define TN_CPU mips
#  define TN_CPU_BITS_32
#  define TN_CPU_BITS 32
# endif

// MIPS can be either endian
#elif defined __powerpc || defined __powerpc__ || defined __POWERPC__ || defined __ppc__ || defined _M_PPC
# if defined __ppc64__ || defined __PPC64__
#  define TN_CPU_POWERPC64
#  define TN_CPU powerpc64
#  define TN_CPU_BITS_64
#  define TN_CPU_BITS 64
# else
#  define TN_CPU powerpc
#  define TN_CPU_BITS_32
#  define TN_CPU_BITS 32
# endif
# define TN_CPU_POWERPC

// PowerPC can be either endian
#elif defined __sparc__ || defined __sparc
# if defined __sparcv9 || defined __sparc64 || defined __sparc64__
#  define TN_CPU_SPARC64
#  define TN_CPU sparc64
#  define TN_CPU_BITS_64
#  define TN_CPU_BITS 64
# else
#  define TN_CPU_SPARC
#  define TN_CPU sparc
#  define TN_CPU_BITS_32
#  define TN_CPU_BITS 32
# endif
# define TN_CPU_ENDIAN_BIG
# define TN_CPU_ENDIAN big
#elif defined __alpha || defined __alpha__ || defined _M_ALPHA
# define TN_CPU_ALPHA
# define TN_CPU alpha
# define TN_CPU_BITS_64
# define TN_CPU_BITS 64
# define TN_CPU_ENDIAN_LITTLE
# define TN_CPU_ENDIAN little
#elif defined __avr32__ || defined __AVR32__
# define TN_CPU_AVR32
# define TN_CPU avr
# define TN_CPU_BITS_32
# define TN_CPU_BITS 32
# define TN_CPU_ENDIAN_LITTLE
# define TN_CPU_ENDIAN little
#elif defined __sh__ || defined __SH__
# define TN_CPU_SuperH
# define TN_CPU superh
# define TN_CPU_BITS_32
# define TN_CPU_BITS 32

// SyoerH can be either endian
#endif

#ifndef TN_CPU
# define TN_CPU_UNKNOWN
# define TN_CPU unknown
#endif

#ifndef TN_CPU_BITS
# if defined _LP64 || defined __LP64
#  define TN_CPU_BITS 64
#  define TN_CPU_BITS_64
# else
#  define TN_CPU_BITS 32
#  define TN_CPU_BITS_32
# endif
#endif

#if defined __linux || defined __linux__
# define TN_OS_LINUX
# define TN_OS linux
# define TN_OS_THREADS_POSIX
# define TN_OS_THREADS posix
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
#elif defined __APPLE__
# define TN_OS_OSX
# define TN_OS osx
# define TN_OS_THREADS_POSIX
# define TN_OS_THREADS posix
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
# define TN_OS_BSD_TYPE

# ifdef TN_CPU_POWERPC
#  define TN_CPU_ENDIAN_BIG
#  define TN_CPU_ENDIAN big
# endif
#elif defined __WINDOWS__ || defined _WIN32 || defined __WIN32__ || defined __TOS_WIN__
# if defined _WIN64
#  define TN_OS_WIN64
#  define TN_OS win64
# else
#  define TN_OS_WIN32
#  define TN_OS win32
# endif
# define TN_OS_WIN
# define TN_OS_THREADS_WIN
# define TN_OS_THREADS win
# define TN_OS_MUTEX_WIN
# define TN_OS_MUTEX win

# if defined __CYGWIN__
#  define TN_OS_WIN_CYGWIN
# elif defined __MINGW32__
#  define TN_OS_WIN_MINGW
# endif

# if defined TN_CPU_POWERPC || defined TN_CPU_MIPS || defined TN_CPU_ITANIUM
#  define TN_ENDIAN_CPU_LITTLE
# endif
#elif defined __NetBSD__ || defined __OpenBSD__ || defined __FreeBSD__ || defined __bsdi__ || defined __DragonFly__ || defined BSD || defined _SYSTYPE_BSD
# define TN_OS_BSD
# define TN_OS bsd
# define TN_OS_THREADS_POSIX
# define TN_OS_THREADS posix
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
# define TN_OS_BSD_TYPE
#elif defined __GNU__
# define TN_OS_HURD
# define TN_OS hurd
# define TN_OS_THREADS_POSIX
# define TN_OS_THREADS posix
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
#elif defined sco || defined __sco
# define TN_OS_SCO
# define TN_OS sco
# define TN_OS_THREADS_POSIX
# define TN_OS_THREADS posix
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
# define TN_OS_UNIX_TYPE
#elif defined sun || defined __sun
# define TN_OS_SOLARIS
# define TN_OS solaris
# define TN_OS_THREADS_SOLARIS
# define TN_OS_THREADS solaris
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
# define TN_OS_UNIX_TYPE
# if defined TN_CPU_POWERPC
#  define TN_CPU_ENDIAN_LITTLE
#  define TN_CPU_ENDIAN little
# endif
#elif defined _hpux || defined hpux || defined __hpux
# define TN_OS_HPUX
# define TN_OS hpux
# define TN_OS_THREADS_POSIX
# define TN_OS_THREADS posix
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
# define TN_OS_UNIX_TYPE
# if defined TN_CPU_ITANIUM
#  define TN_CPU_ENDIAN_BIG
#  define TN_CPU_ENDIAN big
# endif
#elif defined __QNX__ || defined __QNXNTO__
# define TN_OS_QNX
# define TN_OS qnx
# define TN_OS_THREADS_QNX
# define TN_OS_THREADS qnx
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
#elif defined __vms || defined __VMS
# define TN_OS_THREADS_POSIX
# define TN_OS_THREADS posix
# define TN_OS_MUTEX_POSIX
# define TN_OS_MUTEX posix
# if defined TN_CPU_ITANIUM
#  define TN_CPU_ENDIAN_LITTLE
#  define TN_CPU_ENDIAN little
# endif
#endif

#ifndef TN_OS
# if defined __unix__ || __unix || unix
#  define TN_OS_UNIX
#  define TN_OS unix
#  define TN_OS_THREADS_POSIX
#  define TN_OS_THREADS posix
#  define TN_OS_MUTEX_POSIX
#  define TN_OS_MUTEX posix
#  define TN_OS_UNIX_TYPE
# endif
# define TN_OS_UNKNOWN
# define TN_OS unknown
# ifndef TN_OS_WARN
#  warning OS unknown.  This may be ok.  You will AT LEAST need to call tn_init yourself.
#  define TN_OS_WARN
# endif
#endif

#ifndef TN_OS_THREADS
# define TN_OS_THREADS_NULL
# define TN_OS_THREADS null
#endif

#ifndef TN_OS_MUTEX
# define TN_OS_MUTEX_NULL
# define TN_OS_MUTEX null
#endif

#ifndef TN_CPU_ENDIAN
# if defined __BIG_ENDIAN__ || defined __ARMEB__ || defined __THUMBEB__ || defined _MIPSEB || defined __MIPSEB || defined __MIPSEB__ || __BYTE_ORDER == __BIG_ENDIAN
#  define TN_CPU_ENDIAN_BIG
#  define TN_CPU_ENDIAN big
# elif defined __LITTLE_ENDIAN__ || defined __ARMEL__ || defined __THUMBEL__ || defined _MIPSEL || defined __MIPSEL || defined __MIPSEL__ || __BYTE_ORDER == __LITTLE_ENDIAN
#  define TN_CPU_ENDIAN_LITTLE
#  define TN_CPU_ENDIAN little
# else
#  warning CPU endianness unknown, some functions of libmowgli will not work
# endif
#endif

#endif