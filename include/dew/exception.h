#pragma once

#include "dew/base.h"

namespace dew {
struct exception {
  exception(char const *message) : m_message(message) {}
  [[nodiscard]] auto message() const -> char const * { return m_message; }
  [[nodiscard]] auto what() const -> char const * { return m_message; }
  ~exception() noexcept = default;

private:
  char const *m_message;
};
} // namespace dew
