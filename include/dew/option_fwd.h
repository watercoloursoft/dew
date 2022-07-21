#pragma once

#include "dew/base.h"

namespace dew {
struct none_t {};
static none_t None{};

template <class T> class Option;

template <class T> auto Some(T value) -> Option<T>;
} // namespace dew
