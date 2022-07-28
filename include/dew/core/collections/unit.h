#pragma once

namespace dew {
struct Unit {
  Unit() = default;
  Unit(const Unit &) = default;
  Unit(Unit &&) = default;
  auto operator=(const Unit &) -> Unit & = default;
  auto operator=(Unit &&) -> Unit & = default;

  auto operator==(Unit &) const -> bool { return true; }
  auto operator!=(Unit &) const -> bool { return false; }
};
} // namespace dew
