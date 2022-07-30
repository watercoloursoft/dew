#include "dew/collections/array.h"
#include "dew/dew.h"
#include <iostream>

int main() {
  dew::allocator al = dew::allocator::get_leak_detector();

  dew::array<int> array(al);
  array.push(10);
  array.push(40);

  for (auto i = array.begin(); i != array.end(); i++) {
    std::cout << *i << "\n" << std::flush;
  }

  dew_with(dew::slice p = al.calloc(16), al.free(p)) {
    dew::memcpy(p, dew::slice((madd) "eggs from C++", 14));
    std::cout << (char *)p.ptr() << std::endl;
  }
}
