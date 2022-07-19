#include "dew/result.h"
#include "dew/vec.h"

#include <iostream>
#include <random>

enum class Error { AnError };

using namespace dew;

auto function_that_sometimes_fails() -> Result<u32, Error> {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 10);

  auto n = dist6(rng);

  if (n % 2 == 0) {
    return Ok(n);
  } else {
    return Err(Error::AnError);
  }
}

auto test_function() -> void {
  auto new_vec = Vec<int>{};
  new_vec.push(5);
  new_vec.push(10);

  new_vec.insert(0, 13);
  std::cout << new_vec.remove(1) << std::endl;

  // auto second_item = new_vec.get(2);

  // if (second_item.is_some()) {
  //   std::cout << "Second item is " << second_item.unwrap() << std::endl;
  // } else {
  //   std::cout << "Second item is not present" << std::endl;
  // }

  try {
    auto piss = function_that_sometimes_fails();
    std::cout << piss.is_err() << std::endl;
  } catch (const Exception &e) {
    std::cerr << e.what() << '\n';
  }
}

auto main() -> int { test_function(); }