#pragma once

#include "dew/base.h"
#include "dew/option.h"

#include <cstdlib>

namespace dew {
template <typename T> struct vec {
  void *ptr{};
  usize cap{};
  usize len{};

  vec() { static_assert(sizeof(T) != 0, "Unable to use a zero-sized type."); }

  ~vec() {
    if (cap != 0) {
      ::free(ptr);
      len = 0;
    }
  }

  auto grow() {
    if (cap == 0) {
      cap = 1;
      ptr = ::malloc(cap * sizeof(T));
    } else {
      cap *= 2;
      ptr = ::realloc(ptr, cap * sizeof(T));
    }
  }

  auto push(T &&element) {
    if (len == cap) {
      grow();
    }
    ((T *)ptr)[len++] = move(element);
  }

  auto pop() -> const T {
    if (len == 0) {
      return T{};
    }
    len--;
    return ((T *)ptr)[len];
  }
};
} // namespace dew
