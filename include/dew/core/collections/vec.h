#pragma once

#include "dew/core/base.h"
#include "dew/core/collections/option.h"
#include "dew/core/collections/result.h"
#include "dew/core/collections/unit.h"

#include <cstdlib>
#include <cstring>

namespace dew {
template <typename T> class Vec {
public:
  enum class Error { None, OutOfBounds, LargerShrinkAmount, AllocationError };

public:
  Vec() { static_assert(sizeof(T) != 0, "Unable to use a zero-sized type."); }

  ~Vec() {
    if (m_cap != 0) {
      ::free(m_ptr);
      m_len = 0;
    }
  }

  auto grow() {
    if (m_cap == 0) {
      m_cap = 1;
      m_ptr = std::malloc(m_cap * sizeof(T));
    } else {
      m_cap *= 2;
      m_ptr = std::realloc(m_ptr, m_cap * sizeof(T));
    }
  }

  auto push(T &&element) {
    if (m_len == m_cap) {
      grow();
    }
    ((T *)m_ptr)[m_len++] = move(element);
  }

  auto pop() -> Option<const T> {
    if (m_len == 0) {
      return None;
    }
    m_len--;
    return ((T *)m_ptr)[m_len];
  }

  auto get_unchecked(usize index) const -> const T {
    return ((T *)m_ptr)[index];
  }

  auto get(usize index) const -> const Option<T> {
    if (index >= m_len) {
      return None;
    }
    return get_unchecked(index);
  }

  auto operator[](usize index) -> T & { return get_unchecked(index); }
  auto operator[](usize index) const -> const T & {
    return get_unchecked(index);
  }

  auto insert_unchecked(usize index, T &&element) {
    if (m_len == m_cap) {
      grow();
    }
    std::memcpy((T *)m_ptr + index + 1, (T *)m_ptr + index,
                (m_len - index) * sizeof(T));
    ((T *)m_ptr)[index] = move(element);
    m_len++;
  }

  auto insert(usize index, T &&element) -> Result<Unit, Error> {
    if (index > m_len) {
      return Err(Error::OutOfBounds);
    }
    insert_unchecked(index, move(element));
    return Ok(Unit{});
  }

  auto remove_unchecked(usize index) -> const T {
    m_len--;
    auto result = *((T *)m_ptr + index);
    std::memcpy((T *)m_ptr + index, (T *)m_ptr + index + 1,
                (m_len - index) * sizeof(T));
    return result;
  }

  auto remove(usize index) -> Result<const T, Error> {
    if (index >= m_len) {
      return Err(Error::OutOfBounds);
    }
    return Ok<const T>(remove_unchecked(index));
  }

  // capacity
  [[nodiscard]] auto empty() const -> bool { return len() == 0; }
  [[nodiscard]] auto len() const -> usize { return m_len; }
  [[nodiscard]] auto cap() const -> usize { return m_cap; }

  [[nodiscard]] auto data() const -> const T * { return m_ptr; }
  [[nodiscard]] auto data() -> T * { return m_ptr; }

  // modifiers
  auto clear() -> void {
    while (pop().is_some()) {
    }
  }

  auto shrink_to_size(usize amount) -> Result<Unit, Error> {
    const usize element_size = sizeof(T);

    if (m_cap >= amount) {
      return Err(Error::LargerShrinkAmount);
    }

    if (amount == 0) {
      std::free(m_ptr);
      m_ptr = nullptr;
    } else if (amount != m_cap) {
      usize new_size = element_size * amount;

      if (void *memory = std::realloc(m_ptr, new_size)) {
        m_ptr = memory;
      } else {
        Err(Error::AllocationError);
      }

      m_cap = amount;
    }

    return Ok(Unit{});
  }

  struct Iterator {
    Iterator(T *ptr) : m_ptr(ptr) {}

    auto operator*() -> T & { return *m_ptr; }
    auto operator->() -> T * { return m_ptr; }

    auto operator++() -> Iterator & {
      m_ptr++;
      return *this;
    }

    auto operator++(int) -> Iterator {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    auto operator==(const Iterator &other) const -> bool {
      return m_ptr == other.m_ptr;
    };
    auto operator!=(const Iterator &other) const -> bool {
      return m_ptr != other.m_ptr;
    };

  private:
    T *m_ptr;
  };

  struct ConstantIterator {
    ConstantIterator(T *ptr) : m_ptr(ptr) {}

    auto operator*() const -> T & { return *m_ptr; }
    auto operator->() const -> T * { return m_ptr; }

    auto operator++() -> ConstantIterator & {
      m_ptr++;
      return *this;
    }

    auto operator++(int) -> ConstantIterator {
      ConstantIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    auto operator==(const ConstantIterator &other) const -> bool {
      return m_ptr == other.m_ptr;
    };
    auto operator!=(const ConstantIterator &other) const -> bool {
      return m_ptr != other.m_ptr;
    };

  private:
    T *m_ptr;
  };

  struct ReverseIterator {
    ReverseIterator(T *ptr) : m_ptr(ptr) {}

    auto operator*() -> T & { return *m_ptr; }
    auto operator->() -> T * { return m_ptr; }

    auto operator++() -> ReverseIterator & {
      m_ptr--;
      return *this;
    }

    auto operator++(int) -> ReverseIterator {
      ReverseIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    auto operator==(const ReverseIterator &other) const -> bool {
      return m_ptr == other.m_ptr;
    };
    auto operator!=(const ReverseIterator &other) const -> bool {
      return m_ptr != other.m_ptr;
    };

  private:
    T *m_ptr;
  };

  struct ConstantReverseIterator {
    ConstantReverseIterator(T *ptr) : m_ptr(ptr) {}

    auto operator*() const -> T & { return *m_ptr; }
    auto operator->() const -> T * { return m_ptr; }

    auto operator++() -> ConstantReverseIterator & {
      m_ptr--;
      return *this;
    }

    auto operator++(int) -> ConstantReverseIterator {
      ConstantReverseIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    auto operator==(const ConstantReverseIterator &other) const -> bool {
      return m_ptr == other.m_ptr;
    };
    auto operator!=(const ConstantReverseIterator &other) const -> bool {
      return m_ptr != other.m_ptr;
    };

  private:
    T *m_ptr;
  };

  // Iterators
  auto begin() const -> const Iterator { return Iterator((T *)m_ptr); }
  auto end() const -> const Iterator { return Iterator((T *)m_ptr + m_len); }

  auto cbegin() const -> const ConstantIterator {
    return ConstantIterator((T *)m_ptr);
  }
  auto cend() const -> const ConstantIterator {
    return ConstantIterator((T *)m_ptr + m_len);
  }

  auto rbegin() const -> const ReverseIterator {
    return ReverseIterator((T *)m_ptr + m_len - 1);
  }
  auto rend() const -> const ReverseIterator {
    return ReverseIterator((T *)m_ptr - 1);
  }

  auto crbegin() const -> const ConstantReverseIterator {
    return ConstantReverseIterator((T *)m_ptr + m_len - 1);
  }
  auto crend() const -> const ConstantReverseIterator {
    return ConstantReverseIterator((T *)m_ptr - 1);
  }

private:
  void *m_ptr{};
  usize m_cap{};
  usize m_len{};
};
} // namespace dew
