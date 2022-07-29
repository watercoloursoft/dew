#pragma once

// copied from https://github.com/septag/sx/blob/master/include/sx/macros.h

// Architecture
#define DEW_ARCH_32BIT 0
#define DEW_ARCH_64BIT 0

// Compiler
#define DEW_COMPILER_CLANG 0
#define DEW_COMPILER_CLANG_ANALYZER 0
#define DEW_COMPILER_CLANG_CL 0
#define DEW_COMPILER_GCC 0
#define DEW_COMPILER_MSVC 0

// Endianess
#define DEW_CPU_ENDIAN_BIG 0
#define DEW_CPU_ENDIAN_LITTLE 0

// CPU
#define DEW_CPU_ARM 0
#define DEW_CPU_JIT 0
#define DEW_CPU_MIPS 0
#define DEW_CPU_PPC 0
#define DEW_CPU_RISCV 0
#define DEW_CPU_X86 0

// C Runtime
#define DEW_CRT_BIONIC 0
#define DEW_CRT_GLIBC 0
#define DEW_CRT_LIBCXX 0
#define DEW_CRT_MINGW 0
#define DEW_CRT_MSVC 0
#define DEW_CRT_NEWLIB 0

#ifndef DEW_CRT_MUSL
#define DEW_CRT_MUSL 0
#endif // DEW_CRT_MUSL

#ifndef DEW_CRT_NONE
#define DEW_CRT_NONE 0
#endif // DEW_CRT_NONE

// Platform
#define DEW_PLATFORM_ANDROID 0
#define DEW_PLATFORM_BSD 0
#define DEW_PLATFORM_EMSCRIPTEN 0
#define DEW_PLATFORM_HURD 0
#define DEW_PLATFORM_IOS 0
#define DEW_PLATFORM_LINUX 0
#define DEW_PLATFORM_NX 0
#define DEW_PLATFORM_OSX 0
#define DEW_PLATFORM_PS4 0
#define DEW_PLATFORM_RPI 0
#define DEW_PLATFORM_WINDOWS 0
#define DEW_PLATFORM_WINRT 0
#define DEW_PLATFORM_XBOXONE 0

// C11 thread_local, Because we are missing threads.h
#if __STDC_VERSION__ >= 201112L
#define thread_local _Thread_local
#endif

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(__clang__)
// clang defines __GNUC__ or _MSC_VER
#undef DEW_COMPILER_CLANG
#define DEW_COMPILER_CLANG                                                     \
  (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#if defined(__clang_analyzer__)
#undef DEW_COMPILER_CLANG_ANALYZER
#define DEW_COMPILER_CLANG_ANALYZER 1
#endif // defined(__clang_analyzer__)
#if defined(_MSC_VER)
#undef DEW_COMPILER_MSVC
#define DEW_COMPILER_MSVC _MSC_VER
#undef DEW_COMPILER_CLANG_CL
#define DEW_COMPILER_CLANG_CL DEW_COMPILER_CLANG
#endif
#elif defined(_MSC_VER)
#undef DEW_COMPILER_MSVC
#define DEW_COMPILER_MSVC _MSC_VER
#elif defined(__GNUC__)
#undef DEW_COMPILER_GCC
#define DEW_COMPILER_GCC                                                       \
  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#error "DEW_COMPILER_* is not defined!"
#endif //

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
#if defined(__arm__) || defined(__aarch64__) || defined(_M_ARM)
#undef DEW_CPU_ARM
#define DEW_CPU_ARM 1
#define DEW_CACHE_LINE_SIZE 64
#elif defined(__MIPSEL__) || defined(__mips_isa_rev) || defined(__mips64)
#undef DEW_CPU_MIPS
#define DEW_CPU_MIPS 1
#define DEW_CACHE_LINE_SIZE 64
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
#undef DEW_CPU_PPC
#define DEW_CPU_PPC 1
#define DEW_CACHE_LINE_SIZE 128
#elif defined(__riscv) || defined(__riscv__) || defined(RISCVEL)
#undef DEW_CPU_RISCV
#define DEW_CPU_RISCV 1
#define DEW_CACHE_LINE_SIZE 64
#elif defined(_M_IX86) || defined(_M_X64) || defined(__i386__) ||              \
    defined(__x86_64__)
#undef DEW_CPU_X86
#define DEW_CPU_X86 1
#define DEW_CACHE_LINE_SIZE 64
#else // PNaCl doesn't have CPU defined.
#undef DEW_CPU_JIT
#define DEW_CPU_JIT 1
#define DEW_CACHE_LINE_SIZE 64
#endif //

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__) ||          \
    defined(__64BIT__) || defined(__mips64) || defined(__powerpc64__) ||       \
    defined(__ppc64__) || defined(__LP64__)
#undef DEW_ARCH_64BIT
#define DEW_ARCH_64BIT 64
#else
#undef DEW_ARCH_32BIT
#define DEW_ARCH_32BIT 32
#endif //

#if DEW_CPU_PPC
#undef DEW_CPU_ENDIAN_BIG
#define DEW_CPU_ENDIAN_BIG 1
#else
#undef DEW_CPU_ENDIAN_LITTLE
#define DEW_CPU_ENDIAN_LITTLE 1
#endif // DEW_PLATFORM_

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_DURANGO) || defined(_XBOX_ONE)
#undef DEW_PLATFORM_XBOXONE
#define DEW_PLATFORM_XBOXONE 1
#elif defined(__ANDROID__) || defined(ANDROID)
// Android compiler defines __linux__
#include <sys/cdefs.h> // Defines __BIONIC__ and includes android/api-level.h
#undef DEW_PLATFORM_ANDROID
#define DEW_PLATFORM_ANDROID __ANDROID_API__
#elif defined(_WIN32) || defined(_WIN64)
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
//  If _USING_V110_SDK71_ is defined it means we are using the v110_xp or
//  v120_xp toolset.
#if defined(_MSC_VER) && (_MSC_VER >= 1700) && (!_USING_V110_SDK71_)
#include <winapifamily.h>
#endif // defined(_MSC_VER) && (_MSC_VER >= 1700) && (!_USING_V110_SDK71_)
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#undef DEW_PLATFORM_WINDOWS
#if !defined(WINVER) && !defined(_WIN32_WINNT)
#if DEW_ARCH_64BIT
//				When building 64-bit target Win7 and above.
#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
#else
//				Windows Server 2003 with SP1, Windows XP with
// SP2 and above
#define WINVER 0x0502
#define _WIN32_WINNT 0x0502
#endif // DEW_ARCH_64BIT
#endif // !defined(WINVER) && !defined(_WIN32_WINNT)
#define DEW_PLATFORM_WINDOWS _WIN32_WINNT
#else
#undef DEW_PLATFORM_WINRT
#define DEW_PLATFORM_WINRT 1
#endif
#elif defined(__VCCOREVER__) || defined(__RPI__)
// RaspberryPi compiler defines __linux__
#undef DEW_PLATFORM_RPI
#define DEW_PLATFORM_RPI 1
#elif defined(__linux__)
#undef DEW_PLATFORM_LINUX
#define DEW_PLATFORM_LINUX 1
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__) ||               \
    defined(__ENVIRONMENT_TV_OS_VERSION_MIN_REQUIRED__)
#undef DEW_PLATFORM_IOS
#define DEW_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#undef DEW_PLATFORM_OSX
#define DEW_PLATFORM_OSX __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__
#elif defined(__EMSCRIPTEN__)
#undef DEW_PLATFORM_EMSCRIPTEN
#define DEW_PLATFORM_EMSCRIPTEN 1
#elif defined(__ORBIS__)
#undef DEW_PLATFORM_PS4
#define DEW_PLATFORM_PS4 1
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__) ||                   \
    defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#undef DEW_PLATFORM_BSD
#define DEW_PLATFORM_BSD 1
#elif defined(__GNU__)
#undef DEW_PLATFORM_HURD
#define DEW_PLATFORM_HURD 1
#elif defined(__NX__)
#undef DEW_PLATFORM_NX
#define DEW_PLATFORM_NX 1
#endif //

#if !DEW_CRT_NONE
// https://sourceforge.net/p/predef/wiki/Libraries/
#if defined(__BIONIC__)
#undef DEW_CRT_BIONIC
#define DEW_CRT_BIONIC 1
#elif defined(_MSC_VER)
#undef DEW_CRT_MSVC
#define DEW_CRT_MSVC 1
#elif defined(__GLIBC__)
#undef DEW_CRT_GLIBC
#define DEW_CRT_GLIBC (__GLIBC__ * 10000 + __GLIBC_MINOR__ * 100)
#elif defined(__MINGW32__) || defined(__MINGW64__)
#undef DEW_CRT_MINGW
#define DEW_CRT_MINGW 1
#elif defined(__apple_build_version__) || defined(__ORBIS__) ||                \
    defined(__EMSCRIPTEN__) || defined(__llvm__)
#undef DEW_CRT_LIBCXX
#define DEW_CRT_LIBCXX 1
#endif //

#if !DEW_CRT_BIONIC && !DEW_CRT_GLIBC && !DEW_CRT_LIBCXX && !DEW_CRT_MINGW &&  \
    !DEW_CRT_MSVC && !DEW_CRT_MUSL && !DEW_CRT_NEWLIB
#undef DEW_CRT_NONE
#define DEW_CRT_NONE 1
#endif // DEW_CRT_*
#endif // !DEW_CRT_NONE

#define DEW_PLATFORM_POSIX                                                     \
  (0 || DEW_PLATFORM_ANDROID || DEW_PLATFORM_BSD || DEW_PLATFORM_EMSCRIPTEN || \
   DEW_PLATFORM_HURD || DEW_PLATFORM_IOS || DEW_PLATFORM_LINUX ||              \
   DEW_PLATFORM_NX || DEW_PLATFORM_OSX || DEW_PLATFORM_PS4 ||                  \
   DEW_PLATFORM_RPI)

#define DEW_PLATFORM_NONE                                                      \
  !(0 || DEW_PLATFORM_ANDROID || DEW_PLATFORM_BSD ||                           \
    DEW_PLATFORM_EMSCRIPTEN || DEW_PLATFORM_HURD || DEW_PLATFORM_IOS ||        \
    DEW_PLATFORM_LINUX || DEW_PLATFORM_NX || DEW_PLATFORM_OSX ||               \
    DEW_PLATFORM_PS4 || DEW_PLATFORM_RPI || DEW_PLATFORM_WINDOWS ||            \
    DEW_PLATFORM_WINRT || DEW_PLATFORM_XBOXONE)

#if DEW_COMPILER_GCC
#define DEW_COMPILER_NAME                                                      \
  "GCC " dew_stringize(__GNUC__) "." dew_stringize(                            \
      __GNUC_MINOR__) "." dew_stringize(__GNUC_PATCHLEVEL__)
#elif DEW_COMPILER_CLANG
#define DEW_COMPILER_NAME                                                      \
  "Clang " dew_stringize(__clang_major__) "." dew_stringize(                   \
      __clang_minor__) "." dew_stringize(__clang_patchlevel__)
#elif DEW_COMPILER_MSVC
#if DEW_COMPILER_MSVC >= 1920 // Visual Studio 2019
#define DEW_COMPILER_NAME "MSVC 16.0"
#elif DEW_COMPILER_MSVC >= 1910 // Visual Studio 2017
#define DEW_COMPILER_NAME "MSVC 15.0"
#elif DEW_COMPILER_MSVC >= 1900 // Visual Studio 2015
#define DEW_COMPILER_NAME "MSVC 14.0"
#elif DEW_COMPILER_MSVC >= 1800 // Visual Studio 2013
#define DEW_COMPILER_NAME "MSVC 12.0"
#elif DEW_COMPILER_MSVC >= 1700 // Visual Studio 2012
#define DEW_COMPILER_NAME "MSVC 11.0"
#elif DEW_COMPILER_MSVC >= 1600 // Visual Studio 2010
#define DEW_COMPILER_NAME "MSVC 10.0"
#elif DEW_COMPILER_MSVC >= 1500 // Visual Studio 2008
#define DEW_COMPILER_NAME "MSVC 9.0"
#else
#define DEW_COMPILER_NAME "MSVC"
#endif //
#endif // DEW_COMPILER_

#if DEW_PLATFORM_ANDROID
#define DEW_PLATFORM_NAME "Android " dew_stringize(DEW_PLATFORM_ANDROID)
#elif DEW_PLATFORM_BSD
#define DEW_PLATFORM_NAME "BSD"
#elif DEW_PLATFORM_EMSCRIPTEN
#define DEW_PLATFORM_NAME                                                      \
  "asm.js " dew_stringize(__EMSCRIPTEN_major__) "." dew_stringize(             \
      __EMSCRIPTEN_minor__) "." dew_stringize(__EMSCRIPTEN_tiny__)
#elif DEW_PLATFORM_HURD
#define DEW_PLATFORM_NAME "Hurd"
#elif DEW_PLATFORM_IOS
#define DEW_PLATFORM_NAME "iOS"
#elif DEW_PLATFORM_LINUX
#define DEW_PLATFORM_NAME "Linux"
#elif DEW_PLATFORM_NONE
#define DEW_PLATFORM_NAME "None"
#elif DEW_PLATFORM_NX
#define DEW_PLATFORM_NAME "NX"
#elif DEW_PLATFORM_OSX
#define DEW_PLATFORM_NAME "OSX"
#elif DEW_PLATFORM_PS4
#define DEW_PLATFORM_NAME "PlayStation 4"
#elif DEW_PLATFORM_RPI
#define DEW_PLATFORM_NAME "RaspberryPi"
#elif DEW_PLATFORM_WINDOWS
#define DEW_PLATFORM_NAME "Windows"
#elif DEW_PLATFORM_WINRT
#define DEW_PLATFORM_NAME "WinRT"
#elif DEW_PLATFORM_XBOXONE
#define DEW_PLATFORM_NAME "Xbox One"
#else
#error "Unknown DEW_PLATFORM!"
#endif // DEW_PLATFORM_

#define DEW_PLATFORM_APPLE (0 || DEW_PLATFORM_IOS || DEW_PLATFORM_OSX)

#if DEW_CPU_ARM
#define DEW_CPU_NAME "ARM"
#elif DEW_CPU_JIT
#define DEW_CPU_NAME "JIT-VM"
#elif DEW_CPU_MIPS
#define DEW_CPU_NAME "MIPS"
#elif DEW_CPU_PPC
#define DEW_CPU_NAME "PowerPC"
#elif DEW_CPU_RISCV
#define DEW_CPU_NAME "RISC-V"
#elif DEW_CPU_X86
#define DEW_CPU_NAME "x86"
#endif // DEW_CPU_

#if DEW_CRT_BIONIC
#define DEW_CRT_NAME "Bionic libc"
#elif DEW_CRT_GLIBC
#define DEW_CRT_NAME "GNU C Library"
#elif DEW_CRT_MSVC
#define DEW_CRT_NAME "MSVC C Runtime"
#elif DEW_CRT_MINGW
#define DEW_CRT_NAME "MinGW C Runtime"
#elif DEW_CRT_LIBCXX
#define DEW_CRT_NAME "Clang C Library"
#elif DEW_CRT_NEWLIB
#define DEW_CRT_NAME "Newlib"
#elif DEW_CRT_MUSL
#define DEW_CRT_NAME "musl libc"
#elif DEW_CRT_NONE
#define DEW_CRT_NAME "None"
#else
#error "Unknown DEW_CRT!"
#endif // DEW_CRT_

#if DEW_ARCH_32BIT
#define DEW_ARCH_NAME "32-bit"
#elif DEW_ARCH_64BIT
#define DEW_ARCH_NAME "64-bit"
#endif // DEW_ARCH_

#define DEW_PLATFORM_MOBILE (DEW_PLATFORM_ANDROID || DEW_PLATFORM_IOS)
#define DEW_PLATFORM_PC                                                        \
  (DEW_PLATFORM_WINDOWS || DEW_PLATFORM_LINUX || DEW_PLATFORM_OSX)
