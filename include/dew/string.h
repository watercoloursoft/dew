#pragma once

#include "dew/macros.h"
#include "dew/memory/allocator.h"
#include "dew/primitives.h"

typedef struct dew_str {
  const char *data;
  usize size;
} dew_str;

typedef struct dew_str_buf {
  char *data;
  usize size;
  usize capacity;
  const dew_allocator *allocator;
} dew_str_buf;

DEW_API dew_str_buf dew_str_buf_make(usize size,
                                     const dew_allocator *allocator);
DEW_API void dew_str_buf_append(dew_str_buf *src, dew_str add);
DEW_API void dew_str_buf_remove(dew_str_buf *src, usize start, usize size);
DEW_API dew_str dew_str_from(const char *data, usize size);

DEW_API dew_str dew_str_make(const char *value);
DEW_API bool dew_str_valid(dew_str str);
DEW_API bool dew_str_match(dew_str a, dew_str b);
DEW_API bool dew_str_equal(dew_str a, dew_str b);
DEW_API dew_str dew_str_sub(dew_str str, usize start, usize end);
