#pragma once

#include "dew/primitives.h"

#define dew_result_template(N, T, E)                                           \
  typedef struct N {                                                           \
    bool success;                                                              \
    union {                                                                    \
      T ok_value;                                                              \
      E err_value;                                                             \
    };                                                                         \
  } N;

#define dew_result_ok(T, V)                                                    \
  (T) { .success = true, .ok_value = V, }

#define dew_result_err(T, V)                                                   \
  (T) { .success = false, .err_value = V, }

#define dew_result_is_ok(T) (T).success
#define dew_result_is_err(T) !(T).success
#define dew_result_(T) !(T).success
