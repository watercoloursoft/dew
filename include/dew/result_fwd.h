#pragma once

#include "dew/base.h"

namespace dew {
template <typename T, typename E> struct Result;

template <typename T> struct ok_impl;
template <typename E> struct err_impl;

template <typename T> auto Ok(T &value) -> ok_impl<T>;
template <typename T> auto Ok(T &&value) -> ok_impl<T>;
template <typename T> auto Ok(const T &&value) -> ok_impl<T>;
template <typename T> auto Err(T &value) -> ok_impl<T>;
template <typename T> auto Err(T &&err) -> err_impl<T>;
template <typename T> auto Err(const T &&err) -> err_impl<T>;
} // namespace dew
