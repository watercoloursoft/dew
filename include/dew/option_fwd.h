#pragma once

namespace dew {
struct none_t {};
static none_t None{};

template <class T> struct option;

template <class T> auto Some(T value) -> option<T>;
} // namespace dew
