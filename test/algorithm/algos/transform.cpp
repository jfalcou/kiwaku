//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <algorithm>

TTS_CASE("Check for kwk::transform(value, new_value) 1D")
{

  using data_type = int;
  const std::size_t d0 = 87;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input1, input2, output, check;

  for (std::size_t i = 0; i < input_size; ++i) { input1[i] = i * 3; input2[i] = i * 2; }

  auto view_in1  = kwk::view{kwk::source = input1 , kwk::of_size(d0)};
  auto view_in2  = kwk::view{kwk::source = input2 , kwk::of_size(d0)};
  auto view_out  = kwk::view{kwk::source = output , kwk::of_size(d0)};

  kwk::transform( [&](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                , view_out
                , view_in1
                , view_in2
                );

  std::transform( input1.begin(), input1.end()
                , input2.begin()
                , check.begin()
                , [](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                );

  TTS_ALL_EQUAL(output, check);
};
