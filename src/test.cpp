#include "dew/result.h"
#include "dew/vec.h"

#include <iostream>
#include <random>

enum class Error { AnError };

using namespace dew;

auto function_that_sometimes_fails() -> result<u32, Error> {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 10);

  auto n = dist6(rng);

  if (n % 2 == 0) {
    return ok(n);
  } else {
    return err(Error::AnError);
  }
}

auto test_function() -> void {
  auto new_vec = vec<int>{};
  new_vec.push(5);
  new_vec.push(10);

  try {
    auto piss = function_that_sometimes_fails();
    std::cout << piss.is_err() << std::endl;
  } catch (const exception &e) {
    std::cerr << e.what() << '\n';
  }
}

auto main() -> int { test_function(); }
