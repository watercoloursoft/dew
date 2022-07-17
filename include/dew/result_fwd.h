#pragma once

namespace dew {
template <typename T, typename E> struct result;

template <typename T> struct ok_impl;
template <typename E> struct err_impl;

template <typename T> auto ok(T &value) -> ok_impl<T>;
template <typename T> auto ok(T &&value) -> ok_impl<T>;
template <typename T> auto ok(const T &&value) -> ok_impl<T>;
template <typename T> auto err(T &value) -> ok_impl<T>;
template <typename T> auto err(T &&err) -> err_impl<T>;
template <typename T> auto err(const T &&err) -> err_impl<T>;
} // namespace dew
