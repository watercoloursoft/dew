#include "dew/dew.h"

#include <stdio.h>

int main() {
  const dew_allocator *alloc = dew_allocator_malloc();

  dew_with(char *p = dew_calloc(alloc, 16), dew_free(alloc, p)) {
    // one extra character for null termination
    dew_memcpy(p, "egg3 from C", 11);
    printf("%s\n", p);
  }
}
