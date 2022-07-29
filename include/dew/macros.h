#pragma once

#include "config.h"
#include "platform.h"

// copied from https://github.com/septag/sx/blob/master/include/sx/macros.h

#define dew_makefourcc(_a, _b, _c, _d)                                         \
  (((uint32_t)(_a) | ((uint32_t)(_b) << 8) | ((uint32_t)(_c) << 16) |          \
    ((uint32_t)(_d) << 24)))

#define _dew_stringize(_x) #_x
#define dew_stringize(_x) _dew_stringize(_x)
#define _dew_concat(_a, _b) _a##_b
#define dew_concat(_a, _b) _dew_concat(_a, _b)

// Function decleration code helpers

// Default parameter value, only for C++ and kind of a self documenting for C
#ifdef __cplusplus
#define dew_default(_e) = (_e)
#else
#define dew_default(_e)
#endif

#define dew_align_mask(_value, _mask)                                          \
  (((_value) + (_mask)) & ((~0) & (~(_mask))))
#define dew_align_16(_value) dew_align_mask(_value, 0xf)
#define dew_align_256(_value) dew_align_mask(_value, 0xff)
#define dew_align_4096(_value) dew_align_mask(_value, 0xfff)

#if defined(__has_feature)
#define dew_clang_has_feature(_x) __has_feature(_x)
#else
#define dew_clang_has_feature(_x) 0
#endif // defined(__has_feature)

#if defined(__has_extension)
#define dew_clang_has_extension(_x) __has_extension(_x)
#else
#define dew_clang_has_extension(_x) 0
#endif // defined(__has_extension)

#if DEW_COMPILER_GCC || DEW_COMPILER_CLANG
#define DEW_ALLOW_UNUSED __attribute__((unused))
#define DEW_FORCE_INLINE DEW_INLINE __attribute__((__always_inline__))
#define DEW_FUNCTION __PRETTY_FUNCTION__
#define DEW_NO_INLINE __attribute__((noinline))
#define DEW_NO_RETURN __attribute__((noreturn))
#define DEW_CONSTFN __attribute__((const))
#define DEW_RESTRICT __restrict__
// https://awesomekling.github.io/Smarter-C++-inlining-with-attribute-flatten/
#define DEW_FLATTEN                                                            \
  __attribute__((flatten)) // inline everything in the function body
#if DEW_CRT_MSVC
#define __stdcall
#endif // DEW_CRT_MSVC
#if DEW_CONFIG_FORCE_INLINE_DEBUG
#define DEW_INLINE static
#else
#define DEW_INLINE static inline
#define DEW_NO_VTABLE
#endif
#elif DEW_COMPILER_MSVC
#define DEW_ALLOW_UNUSED
#define DEW_FORCE_INLINE __forceinline
#define DEW_FUNCTION __FUNCTION__
#define DEW_NO_INLINE __declspec(noinline)
#define DEW_NO_RETURN
#define DEW_CONSTFN __declspec(noalias)
#define DEW_RESTRICT __restrict
#define DEW_FLATTEN
#if DEW_CONFIG_FORCE_INLINE_DEBUG
#define DEW_INLINE static
#else
#define DEW_INLINE static inline
#endif
#define DEW_NO_VTABLE __declspec(novtable)
#ifndef __cplusplus
#define _Thread_local __declspec(thread)
#endif // __cplusplus
#else
#error "Unknown DEW_COMPILER_?"
#endif

#if DEW_COMPILER_GCC || DEW_COMPILER_CLANG
#define dew_align_decl(_align, _decl) _decl __attribute__((aligned(_align)))
#else
#define dew_align_decl(_align, _decl) __declspec(align(_align)) _decl
#endif

#if DEW_COMPILER_CLANG
#define DEW_PRAGMA_DIAGNOSTIC_PUSH_CLANG_() _Pragma("clang diagnostic push")
#define DEW_PRAGMA_DIAGNOSTIC_POP_CLANG_() _Pragma("clang diagnostic pop")
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG(_x)                                \
  _Pragma(dew_stringize(clang diagnostic ignored _x))
#else
#define DEW_PRAGMA_DIAGNOSTIC_PUSH_CLANG_()
#define DEW_PRAGMA_DIAGNOSTIC_POP_CLANG_()
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG(_x)
#endif // DEW_COMPILER_CLANG

#if DEW_COMPILER_GCC && DEW_COMPILER_GCC >= 40600
#define DEW_PRAGMA_DIAGNOSTIC_PUSH_GCC_() _Pragma("GCC diagnostic push")
#define DEW_PRAGMA_DIAGNOSTIC_POP_GCC_() _Pragma("GCC diagnostic pop")
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_GCC(_x)                                  \
  _Pragma(dew_stringize(GCC diagnostic ignored _x))
#else
#define DEW_PRAGMA_DIAGNOSTIC_PUSH_GCC_()
#define DEW_PRAGMA_DIAGNOSTIC_POP_GCC_()
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_GCC(_x)
#endif // DEW_COMPILER_GCC

#if DEW_COMPILER_MSVC
#define DEW_PRAGMA_DIAGNOSTIC_PUSH_MSVC_() __pragma(warning(push))
#define DEW_PRAGMA_DIAGNOSTIC_POP_MSVC_() __pragma(warning(pop))
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(_x) __pragma(warning(disable : _x))
#else
#define DEW_PRAGMA_DIAGNOSTIC_PUSH_MSVC_()
#define DEW_PRAGMA_DIAGNOSTIC_POP_MSVC_()
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(_x)
#endif // DEW_COMPILER_MSVC

#if DEW_COMPILER_CLANG
#define DEW_PRAGMA_DIAGNOSTIC_PUSH DEW_PRAGMA_DIAGNOSTIC_PUSH_CLANG_
#define DEW_PRAGMA_DIAGNOSTIC_POP DEW_PRAGMA_DIAGNOSTIC_POP_CLANG_
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC                                \
  DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG
#elif DEW_COMPILER_GCC
#define DEW_PRAGMA_DIAGNOSTIC_PUSH DEW_PRAGMA_DIAGNOSTIC_PUSH_GCC_
#define DEW_PRAGMA_DIAGNOSTIC_POP DEW_PRAGMA_DIAGNOSTIC_POP_GCC_
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC                                \
  DEW_PRAGMA_DIAGNOSTIC_IGNORED_GCC
#elif DEW_COMPILER_MSVC
#define DEW_PRAGMA_DIAGNOSTIC_PUSH DEW_PRAGMA_DIAGNOSTIC_PUSH_MSVC_
#define DEW_PRAGMA_DIAGNOSTIC_POP DEW_PRAGMA_DIAGNOSTIC_POP_MSVC_
#define DEW_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC(_x)
#endif // DEW_COMPILER_

#ifdef __cplusplus
#define _DEW_EXTERN extern "C"
#else
#define _DEW_EXTERN extern
#endif

#if DEW_CONFIG_SHARED_LIB
#if DEW_COMPILER_MSVC
#ifdef dew_EXPORTS
#define _DEW_API_DECL __declspec(dllexport)
#else
#define _DEW_API_DECL __declspec(dllimport)
#endif
#else
#define _DEW_API_DECL __attribute__((visibility("default")))
#endif
#else
#define _DEW_API_DECL
#endif

#define DEW_API _DEW_EXTERN _DEW_API_DECL

#define dew_enabled(_f) ((_f) != 0)
#define dew_unused(_a) (void)(_a)

#ifdef __cplusplus
#define dew_cppbool(_b) (_b) ? true : false;
#define DEW_CONSTEXPR constexpr
#else
#define dew_cppbool(_b) _b
#define DEW_CONSTEXPR
#endif

// see array.h
#ifndef DEW_ARRAY
#define DEW_ARRAY
#endif

// Idea: https://www.youtube.com/watch?v=QpAhX-gsHMs&t=967s
// dew_defer and scope can be used to mimick the behavior of go and zig's defer
// "start" is the statement that you would like to run at the begining of the
// scope "end" is the statement that you would like to run at the end of the
// scope
#define _dew_var(_name) dew_concat(_name, __LINE__)
#define dew_defer(_start, _end)                                                \
  for (int _dew_var(_i_) = (_start, 0); !_dew_var(_i_);                        \
       (_dew_var(_i_) += 1), _end)
#define dew_scope(_end)                                                        \
  for (int _dew_var(_i_) = 0; !_dew_var(_i_); (_dew_var(_i_) += 1), _end)

// somewhat like python's `with` statement
#define dew_with(_init, _release)                                              \
  _init;                                                                       \
  for (int _dew_var(_i_) = 0; !_dew_var(_i_); (_dew_var(_i_) += 1), _release)
