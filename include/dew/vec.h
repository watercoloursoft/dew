#pragma once

#include "dew/base.h"
#include "dew/option.h"

#include <cstdlib>
#include <cstring>

namespace dew {
template <typename T> struct Vec {
  void *ptr{};
  usize cap{};
  usize len{};

  Vec() { static_assert(sizeof(T) != 0, "Unable to use a zero-sized type."); }

  ~Vec() {
    if (cap != 0) {
      ::free(ptr);
      len = 0;
    }
  }

  auto grow() {
    if (cap == 0) {
      cap = 1;
      ptr = std::malloc(cap * sizeof(T));
    } else {
      cap *= 2;
      ptr = std::realloc(ptr, cap * sizeof(T));
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

  auto get_unchecked(usize index) const -> const T { return ((T *)ptr)[index]; }

  auto get(usize index) const -> const Option<T> {
    if (index >= len) {
      return None;
    }
    return get_unchecked(index);
  }

  auto operator[](usize index) const -> const T { return get_unchecked(index); }

  auto insert_unchecked(usize index, T &&element) {
    if (len == cap) {
      grow();
    }
    std::memcpy((T *)ptr + index + 1, (T *)ptr + index,
                (len - index) * sizeof(T));
    ((T *)ptr)[index] = move(element);
    len++;
  }

  auto insert(usize index, T &&element) {
    if (index > len) {
      throw OutOfBoundsException("insert() called with out of bounds index.");
    }
    insert_unchecked(index, move(element));
  }

  auto remove_unchecked(usize index) -> const T {
    len--;
    auto result = *((T *)ptr + index);
    std::memcpy((T *)ptr + index, (T *)ptr + index + 1,
                (len - index - 1) * sizeof(T));
    return result;
  }

  auto remove(usize index) -> const T {
    if (index >= len) {
      throw OutOfBoundsException("remove() called with out of bounds index.");
    }
    return remove_unchecked(index);
  }
};
} // namespace dew
