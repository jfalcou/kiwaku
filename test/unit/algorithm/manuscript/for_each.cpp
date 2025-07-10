//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/container.hpp>
#include <vector>
#include "test.hpp"

// for_each 1D - 4D

// Only expects a 2D kiwaku view of floats: constraint via C++20 concepts
void square_each(kwk::concepts::view<kwk::_1D> auto& view)
{
  // For each value of v: square the value
  kwk::for_each( [](auto& e) { e = e * 2; }, view);
}

TTS_CASE("Check for kwk::for_each(func, container) 1D")
{
  using data_type = float;
  const std::size_t input_size = 20;
  std::vector<data_type> kwk_data;
  std::vector<data_type> std_data;
  kwk_data.resize(input_size);
  std_data.resize(input_size);

  for (std::size_t i = 0; i < input_size; ++i)
  {
    std_data[i] = i;
    kwk_data[i] = std_data[i];
  }

  // std::vector<float> kwk_data;
  auto kwk_view = kwk::view{kwk::source = kwk_data.data(), kwk::of_size(input_size)};
  square_each(kwk_view); // kwk_data.data()

  std::for_each(std_data.data(), std_data.end(), [](auto& e) { e = e * 2; } );

  TTS_ALL_RELATIVE_EQUAL(std_data, kwk_data, 10);
};
