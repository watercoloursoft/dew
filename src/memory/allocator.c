#include "dew/memory/allocator.h"

// from https://github.com/septag/sx/blob/master/src/allocator.c

#include <malloc.h>

static void *dew__malloc_cb(void *ptr, usize size, u32 align, const char *file,
                            const char *func, u32 line, void *user_data);
static void *dew__malloc_leakd_cb(void *ptr, usize size, u32 align,
                                  const char *file, const char *func, u32 line,
                                  void *user_data);

static const dew_allocator g_alloc_malloc = {dew__malloc_cb, nullptr};
static const dew_allocator g_alloc_malloc_leakd = {dew__malloc_leakd_cb,
                                                   nullptr};
static dew_mem_fail_cb *g_alloc_failed_cb;

void dew_mem_set_fail_callback(dew_mem_fail_cb *callback) {
  g_alloc_failed_cb = callback;
}

void dew__mem_run_fail_callback(const char *sourcefile, u32 line) {
  if (g_alloc_failed_cb) {
    g_alloc_failed_cb(sourcefile, line);
  }
}

const dew_allocator *dew_allocator_malloc(void) { return &g_alloc_malloc; }

const dew_allocator *dew_allocator_malloc_leak_detect(void) {
  return &g_alloc_malloc_leakd;
}

static void *dew__malloc_cb(void *ptr, usize size, u32 align, const char *file,
                            const char *func, u32 line, void *user_data) {
  dew_unused(user_data);
  dew_unused(line);
  dew_unused(func);
  dew_unused(file);

  if (size == 0) {
    if (ptr) {
      if (align <= DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT) {
        free(ptr);
        return nullptr;
      }

#if DEW_COMPILER_MSVC
      _aligned_free(ptr);
#else
      dew__aligned_free(&g_alloc_malloc, ptr, file, func, line);
#endif
    }
    return nullptr;
  } else if (ptr == nullptr) {
    if (align <= DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT)
      return malloc(size);

#if DEW_COMPILER_MSVC
    return _aligned_malloc(size, align);
#else
    return dew__aligned_alloc(&g_alloc_malloc, size, align, file, func, line);
#endif
  } else {
    if (align <= DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT)
      return realloc(ptr, size);

#if DEW_COMPILER_MSVC
    return _aligned_realloc(ptr, size, align);
#else
    return dew__aligned_realloc(&g_alloc_malloc, ptr, size, align, file, func,
                                line);
#endif
  }
}

//
// Leakchecker:
// https://raw.githubusercontent.com/nothings/stb/master/stb_leakcheck.h
// stb_leakcheck.h - v0.4 - quick & dirty malloc leak-checking - public domain
// LICENSE
//
//   See end of file.

// if we've already included leakcheck before, undefine the macros
#ifdef malloc
#undef malloc
#undef free
#undef realloc
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct stb__leakcheck_malloc_info {
  char file[128];
  char func[64];
  int line;
  usize size;
  struct stb__leakcheck_malloc_info *next, *prev;
} stb__leakcheck_malloc_info;

static stb__leakcheck_malloc_info *mi_head;
// TODO: make locks
// static dew_lock_t mi_lock;

static void *stb_leakcheck_malloc(usize sz, const char *file, const char *func,
                                  int line) {
  stb__leakcheck_malloc_info *mi =
      (stb__leakcheck_malloc_info *)malloc(sz + sizeof(*mi));
  if (mi == NULL)
    return mi;

  // dew_strcpy(mi->file, sizeof(mi->file), file);
  // dew_strcpy(mi->func, sizeof(mi->func), func);
  // dew_lock(mi_lock) {
  //   mi->line = line;
  //   mi->next = mi_head;
  //   if (mi_head)
  //     mi->next->prev = mi;
  //   mi->prev = NULL;
  //   mi->size = (int)sz;
  //   mi_head = mi;
  // }
  return mi + 1;
}

static void stb_leakcheck_free(void *ptr) {
  if (ptr != NULL) {
    stb__leakcheck_malloc_info *mi = (stb__leakcheck_malloc_info *)ptr - 1;
    mi->size = ~mi->size;
    // dew_lock(mi_lock) {
    //   if (mi->prev == NULL) {
    //     dew_assert(mi_head == mi);
    //     mi_head = mi->next;
    //   } else
    //     mi->prev->next = mi->next;
    //   if (mi->next)
    //     mi->next->prev = mi->prev;
    // }
    free(mi);
  }
}

static void *stb_leakcheck_realloc(void *ptr, usize sz, const char *file,
                                   const char *func, int line) {
  if (ptr == NULL) {
    return stb_leakcheck_malloc(sz, file, func, line);
  } else if (sz == 0) {
    stb_leakcheck_free(ptr);
    return NULL;
  } else {
    stb__leakcheck_malloc_info *mi = (stb__leakcheck_malloc_info *)ptr - 1;
    if (sz <= mi->size) {
      return ptr;
    } else {
      void *q = stb_leakcheck_malloc(sz, file, func, line);
      if (q) {
        dew_memcpy(q, ptr, mi->size);
        stb_leakcheck_free(ptr);
      }
      return q;
    }
  }
}

static void stblkck_internal_print(dew_dump_leak_cb dump_leak_fn,
                                   const char *reason, const char *file,
                                   const char *func, int line, usize size,
                                   void *ptr) {
  // char text[512];
  // dew_snprintf(text, sizeof(text), "%-6s: %s@%d: %s: %$.2d at 0x%p", reason,
  //              file ? file : "[NA]", line, func ? func : "[NA]", size, ptr);
  // if (dump_leak_fn) {
  //   dump_leak_fn(text, file, func, line, size, ptr);
  // } else {
  //   puts(text);
  // }
}

void dew_dump_leaks(dew_dump_leak_cb dump_leak_fn) {
  stb__leakcheck_malloc_info *mi = mi_head;
  while (mi) {
    if ((ptrdiff_t)mi->size >= 0) {
      stblkck_internal_print(dump_leak_fn, "LEAKED", mi->file, mi->func,
                             mi->line, mi->size, mi + 1);
    }
    mi = mi->next;
  }
}

static void *dew__malloc_leakd_cb(void *ptr, usize size, u32 align,
                                  const char *file, const char *func, u32 line,
                                  void *user_data) {
  dew_unused(user_data);
  if (size == 0) {
    if (ptr) {
      if (align <= DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT) {
        stb_leakcheck_free(ptr);
        return NULL;
      }

      dew__aligned_free(&g_alloc_malloc_leakd, ptr, file, func, line);
    }
    return NULL;
  } else if (ptr == NULL) {
    if (align <= DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT)
      return stb_leakcheck_malloc(size, file, func, (int)line);

    return dew__aligned_alloc(&g_alloc_malloc_leakd, size, align, file, func,
                              line);
  } else {
    if (align <= DEW_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT)
      return stb_leakcheck_realloc(ptr, size, file, func, (int)line);

    return dew__aligned_realloc(&g_alloc_malloc_leakd, ptr, size, align, file,
                                func, line);
  }
}
