#pragma once

// from https://github.com/septag/sx/blob/master/include/sx/sx.h

#include "../macros.h"
#include "../primitives.h"

#include <memory.h>

#if defined(__cplusplus)
extern "C" {
#endif
DEW_INLINE void *dew_memset(void *dest, u32 value, usize size) {
  return memset(dest, value, size);
}

DEW_INLINE void *dew_memcpy(void *dest, const void *src, usize size) {
  return memcpy(dest, src, size);
}

DEW_INLINE void *dew_memmove(void *dest, const void *src, usize size) {
  return memmove(dest, src, size);
}

DEW_INLINE i32 dew_memcmp(const void *a, const void *b, usize size) {
  return memcmp(a, b, size);
}

#define dew_memswp(a, b, _type)                                                \
  {                                                                            \
    _type tmp = *a;                                                            \
    *a = *b;                                                                   \
    *b = tmp;                                                                  \
  }                                                                            \
  while (0)
#if defined(__cplusplus)
}
#endif

#if defined(__cplusplus)
namespace dew {
DEW_INLINE auto memset(void *dest, u32 value, usize size) -> void * {
  return dew_memset(dest, value, size);
}

DEW_INLINE auto memcpy(void *dest, const void *src, usize size) -> void * {
  return dew_memcpy(dest, src, size);
}

DEW_INLINE auto memmove(void *dest, const void *src, usize size) -> void * {
  return dew_memmove(dest, src, size);
}

DEW_INLINE auto memcmp(const void *a, const void *b, usize size) -> i32 {
  return dew_memcmp(a, b, size);
}

template <typename T, typename U> DEW_INLINE auto memswp(T *a, U *b) -> void {
  dew_memswp(a, b, T);
}
} // namespace dew
#endif
