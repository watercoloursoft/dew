#pragma once

// from https://github.com/septag/sx/blob/master/include/sx/sx.h

#include "dew/macros.h"
#include "dew/memory/slice.h"
#include "dew/primitives.h"

#include <memory.h>

DEW_INLINE madd dew_memset(madd dest, u32 value, usize size) {
  return memset(dest, value, size);
}
DEW_INLINE dew_slice dew_memset_slice(dew_slice slice, u32 value) {
  return dew_slice_make(dew_memset(slice.ptr, value, slice.size), slice.size);
}

DEW_INLINE madd dew_memcpy(madd dest, const madd src, usize size) {
  return memcpy(dest, src, size);
}
DEW_INLINE dew_slice dew_memcpy_slice(dew_slice dest, dew_slice src) {
  return dew_slice_make(dew_memcpy(dest.ptr, src.ptr, src.size), dest.size);
}

DEW_INLINE madd dew_memmove(madd dest, const madd src, usize size) {
  return memmove(dest, src, size);
}
DEW_INLINE dew_slice dew_memmove_slice(dew_slice dest, const dew_slice src) {
  return dew_slice_make(dew_memmove(dest.ptr, src.ptr, src.size), dest.size);
}

DEW_INLINE i32 dew_memcmp(const madd a, const madd b, usize size) {
  return memcmp(a, b, size);
}
DEW_INLINE i32 dew_memcmp_slice(const dew_slice a, const dew_slice b) {
  return dew_memcmp(a.ptr, b.ptr, b.size);
}

#define dew_memswp(a, b, _type)                                                \
  {                                                                            \
    _type tmp = *a;                                                            \
    *a = *b;                                                                   \
    *b = tmp;                                                                  \
  }                                                                            \
  while (0)

#if defined(__cplusplus)
namespace dew {
DEW_INLINE auto memset(madd dest, u32 value, usize size) -> madd {
  return dew_memset(dest, value, size);
}
DEW_INLINE auto memset(dew_slice dest, u32 value) -> slice {
  return dew_memset_slice(dest, value);
}

DEW_INLINE auto memcpy(madd dest, const madd src, usize size) -> madd {
  return dew_memcpy(dest, src, size);
}
DEW_INLINE auto memcpy(slice dest, slice src) -> slice {
  return dew_memcpy_slice(dest, src);
}

DEW_INLINE auto memmove(madd dest, const madd src, usize size) -> madd {
  return dew_memmove(dest, src, size);
}
DEW_INLINE auto memmove(slice dest, const slice src) -> slice {
  return dew_memmove_slice(dest, (slice)src);
}

DEW_INLINE auto memcmp(const madd a, const madd b, usize size) -> i32 {
  return dew_memcmp(a, b, size);
}
DEW_INLINE auto memcmp(const slice a, const slice b) -> i32 {
  return dew_memcmp_slice((slice)a, (slice)b);
}

template <typename T, typename U> DEW_INLINE auto memswp(T *a, U *b) -> void {
  dew_memswp(a, b, T);
}
} // namespace dew
#endif
