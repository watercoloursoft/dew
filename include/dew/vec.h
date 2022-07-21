#pragma once

#include "dew/base.h"
#include "dew/option.h"
#include "dew/unit.h"

#include <cstdlib>
#include <cstring>

namespace dew {
template <typename T> class Vec {
public:
  enum class Error {
    None,
    OutOfBounds,
  };

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

  auto pop() -> const T {
    if (m_len == 0) {
      return T{};
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

  auto operator[](usize index) const -> const T { return get_unchecked(index); }

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

  auto begin() const -> const Iterator { return Iterator((T *)m_ptr); }

  auto end() const -> const Iterator { return Iterator((T *)m_ptr + m_len); }

  auto cbegin() const -> const ConstantIterator {
    return ConstantIterator((T *)m_ptr);
  }

  auto cend() const -> const ConstantIterator {
    return ConstantIterator((T *)m_ptr + m_len);
  }

  [[nodiscard]] auto len() const -> usize { return m_len; }

private:
  void *m_ptr{};
  usize m_cap{};
  usize m_len{};
};
} // namespace dew
