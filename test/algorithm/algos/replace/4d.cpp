//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/replace.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <algorithm>

TTS_CASE("Check for kwk::replace(container, value, new_value) 4D")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t d1 = 11;
  const std::size_t d2 = 13;
  const std::size_t d3 = 5;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input, check;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; check[i] = i; }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1, d2, d3)};

  kwk::replace(view_in, 5, 3);
  kwk::replace(view_in, 8, 4);
  kwk::replace(view_in, 10, 7825);

  std::replace(check.begin(), check.end(), 5, 3);
  std::replace(check.begin(), check.end(), 8, 4);
  std::replace(check.begin(), check.end(), 10, 7825);

  TTS_ALL_EQUAL(input, check);
};

TTS_CASE("Check for kwk::replace_if(container, func, new_value) 4D")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t d1 = 11;
  const std::size_t d2 = 13;
  const std::size_t d3 = 5;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input, check;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; check[i] = i; }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1, d2, d3)};

  kwk::replace_if(view_in, [](auto const e){ return (e < 50) || (e == 123); }, 3);
  std::replace_if(check.begin(), check.end(), [](auto const e){ return (e < 50) || (e == 123); }, 3);

  TTS_ALL_EQUAL(input, check);
};

