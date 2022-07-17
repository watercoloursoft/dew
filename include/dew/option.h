#pragma once

#include "dew/base.h"
#include "dew/exception.h"

#include "dew/option_fwd.h"
#include "dew/result_fwd.h"

namespace dew {
template <typename T> struct option {
  option(none_t) {}
  option(T value) : m_has(true), m_value(value) {}
  option() = default;

  [[nodiscard]] DEW_INLINE auto is_some() const -> bool { return m_has; }
  template <typename F> [[nodiscard]] auto is_some_and(F fn) const -> bool {
    return m_has && fn(m_value);
  }
  [[nodiscard]] auto is_none() const -> bool { return !is_some(); }

  auto as_ref() const -> option<T const &> { return m_value; }
  auto as_mut() -> option<T &> { return m_value; }

  auto expect(char const *message) const -> T {
    if (is_some()) {
      return m_value;
    }
    throw exception(message);
  }

  DEW_INLINE auto unwrap() const -> T {
    if (is_some()) {
      return m_value;
    }
    return expect("unwrap() called on None.");
  }

  auto unwrap_or(T def) const -> T {
    if (is_some()) {
      return m_value;
    }
    return def;
  }

  auto unwrap_or_else(T (*fn)()) const -> T {
    if (is_some()) {
      return m_value;
    }
    return fn();
  }

  auto unwrap_or_default() const -> T {
    if (is_some()) {
      return m_value;
    }
    return T{};
  }

  template <typename E> auto ok_or(E &&value) const -> result<T, E> {
    if (is_some()) {
      return ok<T, E>(move(m_value));
    }
    return err<T, E>(move(value));
  }

  template <typename E, typename F>
  auto ok_or_else(F fn) const -> result<T, E> {
    if (is_some()) {
      return ok<T, E>(move(m_value));
    }
    return err<T, E>(move(fn()));
  }

private:
  T m_value;
  bool m_has{};
};

template <typename T> auto Some(T value) -> option<T> {
  return option<T>(value);
}

} // namespace dew
