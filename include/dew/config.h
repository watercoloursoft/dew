#pragma once

#ifndef DEW_CONFIG_SHARED_LIB
#define DEW_CONFIG_SHARED_LIB 0
#endif

// Debug is forced to be off, so we undef _DEBUG if it's already defined
#if defined(DEW_DEBUG) && !DEW_DEBUG
#ifdef _DEBUG
#undef _DEBUG
#endif
#endif

#if defined(_DEBUG) || (defined(DEW_DEBUG) && DEW_DEBUG)
#ifndef DEW_CONFIG_DEBUG_ALLOCATOR
#define DEW_CONFIG_DEBUG_ALLOCATOR 1
#endif

#ifndef DEW_CONFIG_ENABLE_ASSERT
#define DEW_CONFIG_ENABLE_ASSERT 1
#endif

// There is an issue with msvc+clang_c2 where NDEBUG (and some other release
// flags) are always defined
#ifdef NDEBUG
#undef NDEBUG
#endif

#ifndef DEW_DEBUG
#define DEW_DEBUG 1
#endif
#endif // _DEBUG || DEW_DEBUG

#ifndef DEW_DEBUG
#define DEW_DEBUG 0
#endif

#ifndef DEW_CONFIG_DEBUG_ALLOCATOR
#define DEW_CONFIG_DEBUG_ALLOCATOR 0
#endif

#ifndef DEW_CONFIG_ENABLE_ASSERT
#define DEW_CONFIG_ENABLE_ASSERT 1
#endif

// define DEW_CONFIG_DISABLE_ASSERT_ALWAYS=1 to disable sx_assert_always
#ifndef DEW_CONFIG_DISABLE_ASSERT_ALWAYS
#define DEW_CONFIG_DISABLE_ASSERT_ALWAYS 0
#endif

// Natural aligment is the default memory alignment for each platform
// All memory allocators aligns pointers to this value if 'align' parameter is
// less than natural alignment. MacOS/iOS devices seems to be 16-byte aligned by
// default:
// https://developer.apple.com/library/archive/documentation/Performance/Conceptual/ManagingMemory/Articles/MemoryAlloc.html
#ifndef DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT
#if defined(__APPLE__) && defined(__MACH__)
#define DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT 16
#else
#define DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT 8
#endif
#endif // BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT

// Inserts code for hash-table debugging, used only for efficiency tests, see
// hash.h
#ifndef DEW_CONFIG_HASHTBL_DEBUG
#define DEW_CONFIG_HASHTBL_DEBUG 1
#endif

// Use stdc math lib for basic math functions, see math.h
#ifndef DEW_CONFIG_STDMATH
#define DEW_CONFIG_STDMATH 1
#endif

#ifndef DEW_CONFIG_HANDLE_GEN_BITS
#define DEW_CONFIG_HANDLE_GEN_BITS 14
#endif

#ifndef DEW_CONFIG_SIMD_DISABLE
#define DEW_CONFIG_SIMD_DISABLE 0
#endif

#ifndef DEW_CONFIG_ARRAY_INIT_SIZE
#define DEW_CONFIG_ARRAY_INIT_SIZE 8
#endif

// This preprocessor applies a trick for hot functions that need to be inlined
// even in debug builds only applies to UC_ALWAYS_INLINE functions, but leaves
// DEW_INLINE not inlined And also, the build should be compiled with /Ob1 flag
#ifndef DEW_CONFIG_FORCE_INLINE_DEBUG
#define DEW_CONFIG_FORCE_INLINE_DEBUG 0
#endif

#ifndef DEW_CONFIG_INCLUDE_BANNED
#define DEW_CONFIG_INCLUDE_BANNED 0
#endif

#ifndef DEW_CONFIG_OBSOLETE_CODE
#define DEW_CONFIG_OBSOLETE_CODE 0
#endif
