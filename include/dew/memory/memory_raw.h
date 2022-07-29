#pragma once

// from https://github.com/septag/sx/blob/master/include/sx/allocator.h

#include "../macros.h"
#include "../primitives.h"

typedef void(dew_mem_fail_cb)(const char *sourcefile, u32 line);

DEW_API void dew_mem_set_fail_callback(dew_mem_fail_cb *callback);
DEW_API void dew__mem_run_fail_callback(const char *sourcefile,
                                        u32 line); // internal

#define dew_memory_fail()                                                      \
  do {                                                                         \
    dew__mem_run_fail_callback(__FILE__, __LINE__);                            \
    dew_assert_alwaysf(0, "Out of memory");                                    \
  } while (0)
#define dew_out_of_memory() dew_memory_fail()

#if defined(DEW_CONFIG_DEBUG_ALLOCATOR)
#define dew_malloc(_allocator, _size)                                          \
  dew__malloc(_allocator, _size, 0, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_realloc(_allocator, _ptr, _size)                                   \
  dew__realloc(_allocator, _ptr, _size, 0, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_free(_allocator, _ptr)                                             \
  dew__free(_allocator, _ptr, 0, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_aligned_malloc(_allocator, _size, _align)                          \
  dew__malloc(_allocator, _size, _align, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_aligned_realloc(_allocator, _ptr, _size, _align)                   \
  dew__realloc(_allocator, _ptr, _size, _align, __FILE__, DEW_FUNCTION,        \
               __LINE__)
#define dew_aligned_free(_allocator, _ptr, _align)                             \
  dew__free(_allocator, _ptr, _align, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_calloc(_allocator, _size)                                          \
  dew__calloc(_allocator, _size, 0, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_aligned_calloc(_allocator, _size, _align)                          \
  dew__calloc(_allocator, _size, _align, __FILE__, DEW_FUNCTION, __LINE__)
#else
#define dew_malloc(_allocator, _size)                                          \
  dew__malloc(_allocator, _size, 0, nullptr, nullptr, 0)
#define dew_realloc(_allocator, _ptr, _size)                                   \
  dew__realloc(_allocator, _ptr, _size, 0, nullptr, nullptr, 0)
#define dew_free(_allocator, _ptr)                                             \
  dew__free(_allocator, _ptr, 0, nullptr, nullptr, 0)
#define dew_aligned_malloc(_allocator, _size, _align)                          \
  dew__malloc(_allocator, _size, _align, nullptr, nullptr, 0)
#define dew_aligned_realloc(_allocator, _ptr, _size, _align)                   \
  dew__realloc(_allocator, _ptr, _size, _align, nullptr, nullptr, 0)
#define dew_aligned_free(_allocator, _ptr, _align)                             \
  dew__free(_allocator, _ptr, _align, nullptr, nullptr, 0)
#define dew_calloc(_allocator, _size)                                          \
  dew__calloc(_allocator, _size, 0, nullptr, nullptr, 0)
#define dew_aligned_calloc(_allocator, _size, _align)                          \
  dew__calloc(_allocator, _size, _align, nullptr, nullptr, 0)
#endif
