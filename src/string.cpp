#include "dew/string.h"
#include "dew/string_util.h"

dew_str_buf dew_str_buf_make(usize size, const dew_allocator *allocator) {
  dew_str_buf sb = {};
  sb.data = (char *)dew_malloc(allocator, size);
  sb.size = 0;
  sb.capacity = size;
  sb.allocator = allocator;
  return sb;
};

void dew_str_buf_append(dew_str_buf *src, dew_str add) {
  dew_assertf(src, "dew_str_buf_append: src is null");
  dew_assertf(src->allocator, "dew_str_buf_append: src->allocator is null");
  dew_assertf(add.data, "dew_str_buf_append: add.data is null");
  dew_assertf(add.size, "dew_str_buf_append: add.size is 0");
  if (src->size + add.size > src->capacity) {
    usize new_capacity = src->capacity * 2;
    if (new_capacity < src->size + add.size) {
      new_capacity = src->size + add.size;
    }
    char *new_data = (char *)dew_malloc(src->allocator, new_capacity);
    dew_memcpy(new_data, src->data, src->size);
    dew_free(src->allocator, src->data);
    src->data = new_data;
    src->capacity = new_capacity;
  }
  dew_memcpy(src->data + src->size, (void *)add.data, add.size);
  src->size += add.size;
}

void dew_str_buf_remove(dew_str_buf *src, usize start, usize size) {
  dew_assertf(src, "dew_str_buf_remove: src is null");
  dew_assertf(src->allocator, "dew_str_buf_remove: src->allocator is null");
  dew_assertf(start + size <= src->size,
              "dew_str_buf_remove: out of bounds remove slice");
  dew_assertf(start >= 0, "dew_str_buf_remove: start of slice is negative");

  if (start + size > src->size) {
    return;
  }

  if (size == 0) {
    return;
  }
  usize new_size = 0;

  char *p1_start = src->data;
  char *p1_end = p1_start + start;
  char *p2_start = p1_end + size;
  char *p2_end = src->data + src->size;

  usize p1_size = p1_end - p1_start;
  usize p2_size = p2_end - p2_start;

  new_size += p1_size;
  new_size += p2_size;

  char *new_data = (char *)dew_calloc(src->allocator, new_size);

  src->capacity = new_size;
  src->size = new_size;

  if (p1_size > 0) {
    dew_memcpy(new_data, p1_start, p1_size);
  }

  if (p2_size > 0) {
    dew_memcpy(new_data + p1_size, p2_start, p2_size);
  }

  // this is freed here because we do memcpy from it
  dew_free(src->allocator, src->data);
  src->data = new_data;
}

dew_str dew_str_from(const char *data, usize size) {
  dew_str s = {};
  s.data = data;
  s.size = size;
  return s;
};

dew_str dew_str_make(const char *value) {
  return dew_str_from(value, dew_strlen(value));
};

bool dew_str_valid(dew_str str) { return str.data && str.size; };

bool dew_str_match(dew_str a, dew_str b) {
  for (usize i = 0; i <= (a.size - b.size); i++) {
    if (dew_memcmp((const madd)(a.data + i), (const madd)b.data, b.size) == 0)
      return true;
  }
  return false;
};

bool dew_str_equal(dew_str a, dew_str b) {
  if (a.size != b.size) {
    return false;
  }
  return dew_memcmp((const madd)a.data, (const madd)b.data, a.size) == 0;
};

dew_str dew_str_sub(dew_str str, usize start, usize end) {
  dew_assertf(str.data, "dew_str_sub: str.data is null");
  dew_assertf(str.size, "dew_str_sub: str.size is 0");
  dew_assertf(start >= 0, "dew_str_sub: start is negative");
  dew_assertf(end <= str.size, "dew_str_sub: end is out of bounds");
  dew_assertf(start <= end, "dew_str_sub: start is greater than end");

  dew_str sub = {};
  sub.data = str.data + start;
  sub.size = end - start;
  return sub;
};
