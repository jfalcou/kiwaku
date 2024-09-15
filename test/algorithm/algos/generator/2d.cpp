//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/algorithm/algos/generator.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

// generator 2D

TTS_CASE("Check for kwk::fill(container, value) 2D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t d1 = 10;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  kwk::fill(view, 8);
  std::array<data_type, input_size> expected;
  std::fill(expected.begin(), expected.end(), 8);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::generate(func, container) 2D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t d1 = 7;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  std::size_t count = 0;
  kwk::generate ([&](auto i0, auto i1) { 
                    count++;
                    return i0 * 100 + i1; 
                  }
                , view
                );

  std::array<data_type, input_size> expected;
  std::size_t cpt{0};
  for (std::size_t i0 = 0; i0 < d0; ++i0)
    for (std::size_t i1 = 0; i1 < d1; ++i1)
    {
      expected[cpt++] = i0 * 100 + i1;
    }

  TTS_ALL_EQUAL(input, expected);
  TTS_EQUAL(count, input_size);
};

TTS_CASE("Check for kwk::iota(out) 2D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  kwk::iota(view);

  std::array<data_type, input_size> expected;
  std::iota(expected.begin(), expected.end(), 0);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value) 2D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  int initial_value = 87;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  kwk::iota(view, initial_value);

  std::array<data_type, input_size> expected;
  std::iota(expected.begin(), expected.end(), initial_value);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value, step) 2D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  data_type initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  kwk::iota(view, initial_value, step);

  std::array<data_type, input_size> expected;
  // std::iota(expected.begin(), expected.end(), initial_value, step);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value, step) 2D")
{
  using data_type = int;
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  data_type initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  kwk::iota(view, initial_value, step);

  std::array<data_type, input_size> expected;
  // std::iota(expected.begin(), expected.end(), initial_value, step);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};
