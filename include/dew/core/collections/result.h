#pragma once

#include "dew/core/base.h"
#include "dew/core/collections/collections_fwd.h"

namespace dew {

template <typename T> struct ok_impl {
  T value;
  ok_impl(T &&value) : value(move(value)) {}
};

template <typename T> struct err_impl {
  T value;
  // err_impl(T value) : value(move(value)) {}
  // err_impl(T &value) : value(move(value)) {}
  err_impl(T &&value) : value(move(value)) {}
  err_impl(const T &&value) : value(move(value)) {}
};

template <typename T, typename E> class Result {
public:
  Result(ok_impl<T> value) : m_is_ok(true), m_ok(value) {}
  Result(err_impl<E> value) : m_err(value) {}

  [[nodiscard]] auto is_ok() const -> bool { return m_is_ok; }
  template <typename F> [[nodiscard]] auto is_ok_and(F fn) const -> bool {
    return m_ok && fn(m_ok);
  }
  [[nodiscard]] auto is_err() const -> bool { return !is_ok(); }

  template <typename F> auto is_err_and(F fn) const -> bool {
    return !is_ok() && fn();
  }

  [[nodiscard]] auto ok() const -> Option<T> {
    if (is_ok()) {
      return m_ok.value;
    }
    return None;
  }
  [[nodiscard]] auto err() const -> Option<E> {
    if (is_err()) {
      return m_ok.value;
    }
    return None;
  }
  [[nodiscard]] auto expect() const -> T {
    if (is_ok()) {
      return m_ok.value;
    }
    throw Exception("expect() called on Err.");
  }

private:
  bool m_is_ok{};
  union {
    ok_impl<T> m_ok;
    err_impl<E> m_err;
  };
};

template <typename T> auto Ok(T &value) -> ok_impl<T> {
  return ok_impl<T>(move(value));
}

template <typename T> auto Ok(T &&value) -> ok_impl<T> {
  return ok_impl<T>(move(value));
};

template <typename T> auto Ok(const T &&value) -> ok_impl<T> {
  return ok_impl<T>(move(value));
};

template <typename T> auto Err(T &value) -> ok_impl<T> {
  return ok_impl<T>(move(value));
}

template <typename T> auto Err(T &&err) -> err_impl<T> {
  return err_impl<T>(move(err));
};

template <typename T> auto Err(const T &&err) -> err_impl<T> {
  return err_impl<T>(move(err));
};

} // namespace dew
