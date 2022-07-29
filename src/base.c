#include "dew/dew.h"

#include <stdio.h>

int main() {
  const dew_allocator *alloc = dew_allocator_malloc();

  dew_with(char *p = dew_calloc(alloc, 5), dew_free(alloc, p)) {
    // one extra character for null termination
    dew_memcpy(p, "egg", 4);
    printf("%s\n", p);
  }
}
