//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/generator.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

// generator 1D

TTS_CASE("Check for kwk::fill(container, value) 1D")
{
  using data_type = int;
  const std::size_t input_size = 20;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  // std::fill(input.begin(), input.end(), 0);
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  kwk::fill(view, 8);
  std::array<data_type, input_size> expected;
  std::fill(expected.begin(), expected.end(), 8);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::generate(func, container) 1D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0)};

  std::size_t count = 0;
  kwk::generate ([&](auto i0) { 
                    count++;
                    return i0 * 3; 
                  }
                , view
                );

  std::array<data_type, input_size> expected;
  for (std::size_t i0 = 0; i0 < d0; ++i0)
  {
    expected[i0] = i0 * 3;
  }

  TTS_ALL_EQUAL(input, expected);
  TTS_EQUAL(count, input_size);
};

TTS_CASE("Check for kwk::iota(out) 1D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0)};

  kwk::iota(view);

  std::array<data_type, input_size> expected;
  for (std::size_t i0 = 0; i0 < d0; ++i0)
  {
    expected[i0] = i0;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value) 1D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t input_size = d0;
  data_type initial_value = 87;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0)};

  kwk::iota(view, initial_value);

  std::array<data_type, input_size> expected;
  for (std::size_t i0 = 0; i0 < d0; ++i0)
  {
    expected[i0] = i0 + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value, step) 1D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t input_size = d0;
  data_type initial_value = 87;
  std::size_t step = 5;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0)};

  kwk::iota(view, initial_value, step);

  std::array<data_type, input_size> expected;
  for (std::size_t i0 = 0; i0 < d0; ++i0)
  {
    expected[i0] = i0 * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};
