#include "dew/dew.h"
#include <iostream>

int main() {
  dew::allocator al = dew::allocator::get_leak_detector();

  dew_with(dew::slice p = al.malloc(16), al.free(p)) {
    // one extra character for null termination
    dew::memcpy(p, dew::slice((madd) "eggs from C++", 14));
    std::cout << (char *)p.ptr() << std::endl;
  }
}
