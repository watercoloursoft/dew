#pragma once

#include "dew/core/base.h"
#include "dew/core/collections/option.h"

#include <cstdlib>
#include <cstring>
#include <initializer_list>

namespace dew {
template <typename T> class Array {
public:
  Array() = default;
  Array(usize capacity) : m_cap(capacity) {
    if (capacity > 0) {
      m_ptr = std::malloc(capacity * sizeof(T));
    }
  }
  Array(std::initializer_list<T> init) {
    m_cap = m_len = init.size();
    if (m_cap > 0) {
      m_ptr = std::malloc(m_cap * sizeof(T));
      std::memcpy(m_ptr, init.begin(), m_cap * sizeof(T));
    }
  }
  Array(const T *data, usize len) {
    m_cap = m_len = len;
    if (len > 0) {
      m_ptr = std::malloc(len * sizeof(T));
      std::memcpy(m_ptr, data, len * sizeof(T));
    }
  }
  Array(const Array &other) {
    m_cap = m_len = other.m_len;
    if (m_cap) {
      m_ptr = std::malloc(m_cap * sizeof(T));
      std::memcpy(m_ptr, other.m_ptr, m_cap * sizeof(T));
    }
  }
  Array(const Array &other, usize extra) {
    m_cap = m_len = other.m_len + extra;
    if (m_cap) {
      m_ptr = std::malloc(m_cap * sizeof(T));
      std::memset(m_ptr, 0, m_cap * sizeof(T));
      std::memcpy(m_ptr, other.m_ptr, m_cap * sizeof(T));
    }
  }
  template <typename U> Array(const Array<U> &other) {
    m_cap = m_len = other.m_len;
    if (m_cap) {
      m_ptr = std::malloc(m_cap * sizeof(T));
      std::memcpy(m_ptr, other.m_ptr, m_cap * sizeof(T));
    }
  }
  Array(Array &&other) {
    m_cap = other.m_cap;
    m_len = other.m_len;
    m_ptr = other.m_ptr;
    other.m_cap = 0;
    other.m_len = 0;
    other.m_ptr = nullptr;
  }

  ~Array() { clear(); }

  [[nodiscard]] auto empty() const -> bool { return m_len == 0; }
  [[nodiscard]] auto len() const -> usize { return m_len; }
  [[nodiscard]] auto cap() const -> usize { return m_cap; }

  [[nodiscard]] auto data() const -> const T * { return m_ptr; }
  [[nodiscard]] auto data() -> T * { return m_ptr; }

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

  auto first() const -> const Option<T> { return get(0); }
  auto first() -> Option<T> { return get(0); }

  auto last() const -> const Option<T> { return get(m_len - 1); }
  auto last() -> Option<T> { return get(m_len - 1); }

  auto clear() -> void {
    for (usize i = 0; i < m_len; ++i) {
      reinterpret_cast<T *>(m_ptr)[i]->~T();
    }
    if (m_cap != 0) {
      std::free(m_ptr);
      m_cap = 0;
      m_len = 0;
      m_ptr = nullptr;
    }
  }

  auto ensureCapacity(usize capacity) -> void {
    if (capacity > m_cap) {
      usize new_cap = m_cap;
      while (new_cap < capacity) {
        new_cap *= 2;
      }
      m_ptr = std::realloc(m_ptr, new_cap * sizeof(T));
      m_cap = new_cap;
    }
  }

  auto resize(usize new_len) -> void {
    if (new_len == m_len) {
      return;
    }
    if (m_len > new_len) {
      std::memset(reinterpret_cast<T *>(m_ptr) + m_len, 0,
                  (new_len - m_len) * sizeof(T));
    } else {
      ensureCapacity(new_len);
      
    }
    m_len = new_len;
  }

  auto insert(const T &item) -> void {
    if (m_len == m_cap) {
      m_cap = m_cap == 0 ? 1 : m_cap * 2;
      m_ptr = std::realloc(m_ptr, m_cap * sizeof(T));
    }
    std::memmove(reinterpret_cast<T *>(m_ptr) + 1, m_ptr,
                 (m_len + 1) * sizeof(T));
    reinterpret_cast<T *>(m_ptr)[0] = item;
    ++m_len;
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
