#include "dew/dew.h"

#include <stdio.h>

int main() {
  const dew_allocator *alloc = dew_allocator_malloc();

  dew_with(dew_slice block = dew_calloc_slice(alloc, 16),
           dew_free_slice(alloc, block)) {
    // one extra character for null termination
    dew_memcpy_slice(block, dew_slice_make("egg3 from C", 12));
    printf("%s\n", (char *)block.ptr);
  }
}
