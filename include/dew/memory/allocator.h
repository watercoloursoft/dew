#pragma once

// from https://github.com/septag/sx/blob/master/include/sx/allocator.h

#include "dew/debug.h"
#include "dew/macros.h"
#include "dew/memory/modify.h"
#include "dew/primitives.h"
#include "dew/util.h"

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
#define dew_malloc_slice(_allocator, _size)                                    \
  dew_slice_make(                                                              \
      dew__malloc(_allocator, _size, 0, __FILE__, DEW_FUNCTION, __LINE__),     \
      _size)

#define dew_realloc(_allocator, _ptr, _size)                                   \
  dew__realloc(_allocator, _ptr, _size, 0, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_realloc_slice(_allocator, _slice)                                  \
  dew_slice_make(dew__realloc(_allocator, _slice.ptr, _slice.size, 0,          \
                              __FILE__, DEW_FUNCTION, __LINE__),               \
                 _slice.size)

#define dew_free(_allocator, _ptr)                                             \
  dew__free(_allocator, _ptr, 0, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_free_slice(_allocator, _slice)                                     \
  dew__free(_allocator, _slice.ptr, 0, __FILE__, DEW_FUNCTION, __LINE__)

#define dew_aligned_malloc(_allocator, _size, _align)                          \
  dew__malloc(_allocator, _size, _align, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_aligned_malloc_slice(_allocator, _size, _align)                    \
  dew_slice_make(dew__malloc(_allocator, _size, _align, __FILE__,              \
                             DEW_FUNCTION, __LINE__),                          \
                 _size)

#define dew_aligned_realloc(_allocator, _ptr, _size, _align)                   \
  dew__realloc(_allocator, _ptr, _size, _align, __FILE__, DEW_FUNCTION,        \
               __LINE__)
#define dew_aligned_realloc_slice(_allocator, _slice, _align)                  \
  dew_slice_make(dew__realloc(_allocator, _slice.ptr, _slice.size, _align,     \
                              __FILE__, DEW_FUNCTION, __LINE__),               \
                 _slice.size)

#define dew_aligned_free(_allocator, _ptr, _align)                             \
  dew__free(_allocator, _ptr, _align, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_aligned_free_slice(_allocator, _slice, _align)                     \
  dew__free(_allocator, _slice.ptr, _align, __FILE__, DEW_FUNCTION, __LINE__)

#define dew_calloc(_allocator, _size)                                          \
  dew__calloc(_allocator, _size, 0, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_calloc_slice(_allocator, _size)                                    \
  dew_slice_make(                                                              \
      dew__calloc(_allocator, _size, 0, __FILE__, DEW_FUNCTION, __LINE__),     \
      _size)

#define dew_aligned_calloc(_allocator, _size, _align)                          \
  dew__calloc(_allocator, _size, _align, __FILE__, DEW_FUNCTION, __LINE__)
#define dew_aligned_calloc_slice(_allocator, _size, _align)                    \
  dew_slice_make(dew__calloc(_allocator, _size, _align, __FILE__,              \
                             DEW_FUNCTION, __LINE__),                          \
                 _size)
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

typedef struct dew_allocator {
  madd (*alloc_cb)(madd s, usize size, u32 align, const char *file,
                   const char *func_name, u32 line_number, madd user_data);
  madd user_data;
} dew_allocator;

// Default allocator: allocate from heap
DEW_API const dew_allocator *dew_allocator_malloc(void);

// Leak checking allocator, useful for debug and DEW_CONFIG_DEBUG_ALLOCATOR=1
// dew_allocator* dew_allocator_malloc_leak_detect();
DEW_API const dew_allocator *dew_allocator_malloc_leak_detect(void);

typedef void (*dew_dump_leak_cb)(const char *formatted_msg, const char *file,
                                 const char *func_name, u32 line_number,
                                 usize size, madd ptr);
DEW_API void dew_dump_leaks(dew_dump_leak_cb dump_leak_fn);

DEW_INLINE bool dew_is_aligned(const madd ptr, u32 align) {
  union {
    madd ptr;
    uptr addr;
  } un;
  un.ptr = ptr;
  return 0 == (un.addr & (align - 1));
}

DEW_INLINE madd dew_align_ptr(madd ptr, usize extra, u32 align) {
  union {
    madd ptr;
    uptr addr;
  } un;
  un.ptr = ptr;
  uptr unaligned = un.addr + extra; // space for header
  uptr mask = align - 1;
  uptr aligned = dew_align_mask(unaligned, mask);
  un.addr = aligned;
  return un.ptr;
}

#ifdef __cplusplus
struct dew__pnew_tag {};

#define dew_pnew(_ptr, _type) ::new (dew__pnew_tag(), _ptr) _type
#define dew_new(_allocator, _type)                                             \
  dew_pnew(dew_malloc(_allocator, sizeof(_type)), _type)
#define dew_aligned_new(_allocator, _type, _align)                             \
  dew_pnew(dew_aligned_malloc(_allocator, sizeof(_type), _align), _type)
#define dew_delete(_allocator, _type, _ptr)                                    \
  if (_ptr) {                                                                  \
    (_ptr)->~_type();                                                          \
    dew_free(_allocator, _ptr);                                                \
  }

inline madd operator new(usize, dew__pnew_tag, madd _ptr) { return _ptr; }

inline void operator delete(madd, dew__pnew_tag, madd) throw() {}
#endif

// impl
DEW_INLINE void *dew__malloc(const dew_allocator *alloc, usize size, u32 align,
                             const char *file, const char *func, u32 line) {
  return alloc->alloc_cb(NULL, size, align, file, func, line, alloc->user_data);
}

DEW_INLINE void dew__free(const dew_allocator *alloc, void *ptr, u32 align,
                          const char *file, const char *func, u32 line) {
  alloc->alloc_cb(ptr, 0, align, file, func, line, alloc->user_data);
}

DEW_INLINE void *dew__realloc(const dew_allocator *alloc, void *ptr, usize size,
                              u32 align, const char *file, const char *func,
                              u32 line) {
  return alloc->alloc_cb(ptr, size, align, file, func, line, alloc->user_data);
}

DEW_INLINE void *dew__aligned_alloc(const dew_allocator *alloc, usize size,
                                    u32 align, const char *file,
                                    const char *func, u32 line) {
  align = dew_max((int)align, DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT);
  const usize total = size + align + sizeof(u32);
  uint8_t *ptr = (uint8_t *)dew__malloc(alloc, total, 0, file, func, line);
  dew_assert(ptr);
  uint8_t *aligned = (uint8_t *)dew_align_ptr(ptr, sizeof(u32), align);
  u32 *header = (u32 *)aligned - 1;
  *header = (u32)(uintptr_t)(aligned - ptr);
  return aligned;
}

DEW_INLINE void dew__aligned_free(const dew_allocator *alloc, void *ptr,
                                  const char *file, const char *func,
                                  u32 line) {
  uint8_t *aligned = (uint8_t *)ptr;
  u32 *header = (u32 *)aligned - 1;
  ptr = aligned - *header;
  dew__free(alloc, ptr, 0, file, func, line);
}

DEW_INLINE void *dew__aligned_realloc(const dew_allocator *alloc, void *ptr,
                                      usize size, u32 align, const char *file,
                                      const char *func, u32 line) {
  if (ptr == NULL)
    return dew__aligned_alloc(alloc, size, align, file, func, line);

  uint8_t *aligned = (uint8_t *)ptr;
  u32 offset = *((u32 *)aligned - 1);
  ptr = aligned - offset;

  align = dew_max((int)align, DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT);
  const usize total = size + align + sizeof(u32);
  ptr = dew__realloc(alloc, ptr, total, 0, file, func, line);
  dew_assert(ptr);
  uint8_t *new_aligned = (uint8_t *)dew_align_ptr(ptr, sizeof(u32), align);

  if (new_aligned == aligned)
    return aligned;

  aligned = (uint8_t *)ptr + offset;
  dew_memmove(new_aligned, aligned, size);
  u32 *header = (u32 *)new_aligned - 1;
  *header = (u32)(new_aligned - (uint8_t *)ptr);
  return new_aligned;
}

DEW_INLINE void *dew__calloc(const dew_allocator *alloc, usize size, u32 align,
                             const char *file, const char *func, u32 line) {
  void *ptr =
      alloc->alloc_cb(NULL, size, align, file, func, line, alloc->user_data);
  if (ptr) {
    dew_memset(ptr, 0x0, size);
  }
  return ptr;
}

#if defined(__cplusplus)
namespace dew {
class allocator {
public:
  allocator() = delete;
  static allocator get_default() { return allocator(dew_allocator_malloc()); };
  static allocator get_leak_detector() {
    return allocator(dew_allocator_malloc_leak_detect());
  }

  madd malloc(usize size) { return madd(dew_malloc(m_alloc, size)); }
  madd aligned_malloc(usize size,
                      u32 align = DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT) {
    return dew_aligned_malloc(m_alloc, size, align);
  }
  madd realloc(madd ptr, usize size) { return dew_realloc(m_alloc, ptr, size); }
  madd aligned_realloc(madd ptr, usize size,
                       u32 align = DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT) {
    return dew_aligned_realloc(m_alloc, ptr, size, align);
  }
  void free(madd ptr) { dew_free(m_alloc, ptr); }
  void free(dew::slice slice) { dew_free(m_alloc, slice.ptr()); }
  void aligned_free(madd ptr,
                    u32 align = DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT) {
    dew_aligned_free(m_alloc, ptr, align);
  }
  madd calloc(usize size) { return dew_calloc(m_alloc, size); }
  const dew_allocator *get_raw() const { return m_alloc; }

  operator const dew_allocator *() const { return m_alloc; }

private:
  allocator(const dew_allocator *allocator) : m_alloc(allocator) {}

  const dew_allocator *m_alloc;
};
} // namespace dew
#endif
