#include "dew/collections/array.h"
#include "dew/dew.h"
#include "dew/string.h"
#include "dew/string_util.h"

#include <stdio.h>

int main() {
  const dew_allocator *alloc = dew_allocator_malloc_leak_detect();

  int *array = nullptr;
  dew_array_push(alloc, array, 10);
  dew_array_push(alloc, array, 40);
  for (int i = 0; i < dew_array_count(array); i++) {
    printf("%i\n", array[i]);
  }
  dew_array_free(alloc, array);

  dew_str_buf buf = dew_str_buf_make(5, alloc);
  dew_str_buf_append(&buf, dew_str_make("hello"));
  dew_str str = dew_str_from(buf.data, buf.size);
  printf("%.*s\n", (i32)str.size, str.data);

  bool has_sub = dew_str_match(str, dew_str_make("lle"));
  printf("%i\n", has_sub);

  const char *test = "30.14";
  printf("%s %f\n", test, dew_tofloat(test, 5));

  dew_with(dew_slice block = dew_calloc_slice(alloc, 16),
           dew_free_slice(alloc, block)) {
    dew_memcpy_slice(block, dew_slice_make("egg3 from C", 12));
    printf("%s\n", (char *)block.ptr);
  }
}
