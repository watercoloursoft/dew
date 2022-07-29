#pragma once

#include "macros.h"
#include "primitives.h"

typedef void(dew_assert_cb)(const char *text, const char *sourcefile,
                            uint32_t line);
DEW_API void dew_set_assert_callback(dew_assert_cb *callback);

#if DEW_COMPILER_MSVC && !DEW_COMPILER_CLANG_CL
#define dew_hwbreak() __debugbreak()
#elif DEW_COMPILER_CLANG
#if (__has_builtin(__builtin_debugtrap))
#define dew_hwbreak() __builtin_debugtrap()
#else
#define dew_hwbreak()                                                          \
  __builtin_trap() // this cannot be used in constexpr functions
#endif
#elif DEW_COMPILER_GCC
#define dew_hwbreak() __builtin_trap()
#endif

DEW_API void dew__debug_message(const char *sourcefile, uint32_t line,
                                const char *fmt, ...);

#if SX_CONFIG_ENABLE_ASSERT
#define dew_assert(_e)                                                         \
  do {                                                                         \
    if (!(_e)) {                                                               \
      dew__debug_message(__FILE__, __LINE__, #_e);                             \
      dew_hwbreak();                                                           \
    }                                                                          \
  } while (0)
#define dew_assertf(_e, ...)                                                   \
  do {                                                                         \
    if (!(_e)) {                                                               \
      dew__debug_message(__FILE__, __LINE__, __VA_ARGS__);                     \
      dew_hwbreak();                                                           \
    }                                                                          \
  } while (0)
#else
#define dew_assert(_e)
#define dew_assertf(_e, ...)
#endif

// going to deprecate dew_assert_rel in favor of dew_assert_always
#if SX_CONFIG_DISABLE_ASSERT_ALWAYS
#define dew_assert_rel(_e)
#define dew_assert_always dew_assert_rel
#define dew_assert_alwaysf(_e, ...)
#else
#define dew_assert_rel(_e)                                                     \
  do {                                                                         \
    if (!(_e)) {                                                               \
      dew__debug_message(__FILE__, __LINE__, #_e);                             \
      dew_hwbreak();                                                           \
    }                                                                          \
  } while (0)
#define dew_assert_always dew_assert_rel
#define dew_assert_alwaysf(_e, ...)                                            \
  do {                                                                         \
    if (!(_e)) {                                                               \
      dew__debug_message(__FILE__, __LINE__, __VA_ARGS__);                     \
      dew_hwbreak();                                                           \
    }                                                                          \
  } while (0)
#endif

#define dew_unreachable() dew_assert_alwaysf(0, "unreachable code");
