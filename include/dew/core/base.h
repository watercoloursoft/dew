#pragma once

namespace dew {
// Unsigned int types.
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

// Signed int types.
using i8 = signed char;
using i16 = short;
using i32 = int;
using i64 = long long;

// Floating point types
using f32 = float;
using f64 = double;

// Boolean types
using b8 = bool;

using range = struct range {
  u64 offset;
  u64 size;
};

using usize = size_t;

// Ensure all types are of the correct size.

static_assert(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
static_assert(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
static_assert(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
static_assert(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

static_assert(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
static_assert(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
static_assert(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
static_assert(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

static_assert(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
static_assert(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define DEW_PLATFORM_WINDOWS 1
#elif defined(__linux__) || defined(__gnu_linux__)
// Linux OS
#define DEW_PLATFORM_LINUX 1
#if defined(__ANDROID__)
#define DEW_PLATFORM_ANDROID 1
#endif
#elif defined(__unix__)
// Catch anything not caught by the above.
#define DEW_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
// Posix
#define DEW_PLATFORM_POSIX 1
#elif __APPLE__
// Apple platforms
#define DEW_PLATFORM_APPLE 1
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#define DEW_PLATFORM_IOS 1
#define DEW_PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define DEW_PLATFORM_IOS 1
// iOS device
#elif TARGET_OS_MAC
// Other DEW_inds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#else
#error "Unknown platform!"
#endif

#define DEW_CLAMP(value, min, max)                                             \
  ((value <= min) ? min : (value >= max) ? max : value)

// Inlining
#if defined(__clang__) || defined(__gcc__)
/** @brief Inline qualifier */
#define DEW_INLINE __attribute__((always_inline)) inline

/** @brief No-inline qualifier */
#define DEW_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)

/** @brief Inline qualifier */
#define DEW_INLINE __forceinline

/** @brief No-inline qualifier */
#define DEW_NOINLINE __declspec(noinline)
#else

/** @brief Inline qualifier */
#define DEW_INLINE static inline

/** @brief No-inline qualifier */
#define DEW_NOINLINE
#endif

template <typename T> auto move(T &value) -> T && {
  return static_cast<T &&>(value);
};

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

} // namespace dew
