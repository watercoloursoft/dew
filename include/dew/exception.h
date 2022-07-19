#pragma once

#include "dew/base.h"

namespace dew {
struct Exception {
  Exception(char const *message) : m_message(message) {}
  [[nodiscard]] auto message() const -> char const * { return m_message; }
  [[nodiscard]] auto what() const -> char const * { return m_message; }
  ~Exception() noexcept = default;

private:
  char const *m_message;
};

struct OutOfBoundsException : Exception {
  OutOfBoundsException(char const *message) : Exception(message) {}
};

} // namespace dew
