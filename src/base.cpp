#include "dew/dew.h"

#include <iostream>

int main() {
  int a = 1;
  int b = 2;
  dew::memswp(&a, &b);
  std::cout << a << " " << b << " sus?" << std::endl;
}
