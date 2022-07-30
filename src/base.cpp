#include "dew/dew.h"
#include <iostream>

int main() {
  dew::allocator al = dew::allocator::get_leak_detector();

  dew_with(char *p = (char *)al.malloc(16), al.free(p)) {
    // one extra character for null termination
    dew::memcpy(p, "egg3 from C++", 14);
    std::cout << p << std::endl;
  }
}
