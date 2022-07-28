#pragma once

namespace dew {
// Option
struct none_t {};
static none_t None{};

template <class T> class Option;

template <class T> auto Some(T value) -> Option<T>;

// Result
template <typename T, typename E> class Result;

template <typename T> struct ok_impl;
template <typename E> struct err_impl;

template <typename T> auto Ok(T &value) -> ok_impl<T>;
template <typename T> auto Ok(T &&value) -> ok_impl<T>;
template <typename T> auto Ok(const T &&value) -> ok_impl<T>;
template <typename T> auto Err(T &value) -> err_impl<T>;
template <typename T> auto Err(T &&err) -> err_impl<T>;
template <typename T> auto Err(const T &&err) -> err_impl<T>;

// Unit
struct Unit;

// Vec
template <class T> class Vec;
} // namespace dew
