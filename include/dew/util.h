#pragma once

#include "macros.h"
#include "primitives.h"

#if defined(__cplusplus)
#define literal(T) T
#else
#define literal(T) (T)
#endif

#if DEW_COMPILER_GCC || DEW_COMPILER_CLANG
#define dew_max(a, b)                                                          \
  ({                                                                           \
    typeof(a) var__a = (a);                                                    \
    typeof(b) var__b = (b);                                                    \
    (void)(&var__a == &var__b);                                                \
    var__a > var__b ? var__a : var__b;                                         \
  })

#define dew_min(a, b)                                                          \
  ({                                                                           \
    typeof(a) var__a = (a);                                                    \
    typeof(b) var__b = (b);                                                    \
    (void)(&var__a == &var__b);                                                \
    var__a < var__b ? var__a : var__b;                                         \
  })

#define dew_clamp(v_, min_, max_)                                              \
  ({                                                                           \
    typeof(v_) var__v = (v_);                                                  \
    typeof(min_) var__min = (min_);                                            \
    typeof(max_) var__max = (max_);                                            \
    (void)(&var__min == &var__max);                                            \
    var__v = var__v < var__max ? var__v : var__max;                            \
    var__v > var__min ? var__v : var__min;                                     \
  })
#elif DEW_COMPILER_MSVC
#define dew_max(a, b) ((a) > (b) ? (a) : (b))
#define dew_min(a, b) ((a) < (b) ? (a) : (b))
#define dew_clamp(v, min_, max_) dew_max(dew_min((v), (max_)), (min_))
#endif // DEW_COMPILER_GCC||DEW_COMPILER_CLANG

#if defined(__cplusplus)
namespace dew {
template <typename T> DEW_INLINE T max(T a, T b) { return (a > b) ? a : b; }
template <typename T> DEW_INLINE T min(T a, T b) { return (a < b) ? a : b; }
template <typename T> DEW_INLINE T clamp(T v, T _min, T _max) {
  return max(min(v, _max), _min);
}

// max
template i8 max(i8, i8);
template i16 max(i16, i16);
template i32 max(i32, i32);
template i64 max(i64, i64);

template u8 max(u8, u8);
template u16 max(u16, u16);
template u32 max(u32, u32);
template u64 max(u64, u64);

template f32 max(f32, f32);
template f64 max(f64, f64);

// min
template i8 min(i8, i8);
template i16 min(i16, i16);
template i32 min(i32, i32);
template i64 min(i64, i64);

template u8 min(u8, u8);
template u16 min(u16, u16);
template u32 min(u32, u32);
template u64 min(u64, u64);

template f32 min(f32, f32);
template f64 min(f64, f64);

// clamp
template i8 clamp(i8, i8, i8);
template i16 clamp(i16, i16, i16);
template i32 clamp(i32, i32, i32);
template i64 clamp(i64, i64, i64);

template u8 clamp(u8, u8, u8);
template u16 clamp(u16, u16, u16);
template u32 clamp(u32, u32, u32);
template u64 clamp(u64, u64, u64);

template f32 clamp(f32, f32, f32);
template f64 clamp(f64, f64, f64);
} // namespace dew
#endif
