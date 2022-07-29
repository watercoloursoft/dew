#include "dew/dew.h"

#include <stdio.h>

int main() {
  int a = 9;
  int b = 6;
  dew_memswp(&a, &b, int);
  printf("%d %d\n", a, b);
}
