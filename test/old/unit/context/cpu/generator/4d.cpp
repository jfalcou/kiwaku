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
#include <kwk/context/cpu/context.hpp>

// generator 4D

TTS_CASE("Check for kwk::fill(kwk::cpu, container, value) 4D with float")
{
  using data_type = float;
  const std::size_t d0 = 6;
  const std::size_t d1 = 10;
  const std::size_t d2 = 4;
  const std::size_t d3 = 12;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::fill(kwk::cpu, view, 8);
  std::array<data_type, input_size> expected;
  std::fill(expected.begin(), expected.end(), 8);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::generate(kwk::cpu, func, container) 4D with float")
{
  using data_type = float;
  const std::size_t d0 = 6;
  const std::size_t d1 = 7;
  const std::size_t d2 = 9;
  const std::size_t d3 = 4;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  std::size_t count = 0;
  kwk::generate (kwk::cpu, [&](auto i0, auto i1, auto i2, auto i3) { 
                    count++;
                    return i0 * 1000000 + i1 * 10000 + i2 * 100 + i3; 
                  }
                , view
                );

  std::array<data_type, input_size> expected;
  std::size_t cpt{0};
  for (std::size_t i0 = 0; i0 < d0; ++i0)
    for (std::size_t i1 = 0; i1 < d1; ++i1)
      for (std::size_t i2 = 0; i2 < d2; ++i2)
        for (std::size_t i3 = 0; i3 < d3; ++i3)
        {
          expected[cpt++] = i0 * 1000000 + i1 * 10000 + i2 * 100 + i3;
        }

  TTS_ALL_EQUAL(input, expected);
  TTS_EQUAL(count, input_size);
};

TTS_CASE("Check for kwk::iota(kwk::cpu, out) 4D with float")
{
  using data_type = float;
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t d3 = 3;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::iota(kwk::cpu, view);

  std::array<data_type, input_size> expected;
  std::iota(expected.begin(), expected.end(), 0);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(kwk::cpu, out, value) 4D with float")
{
  using data_type = float;
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t d3 = 3;
  data_type initial_value = 87;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::iota(kwk::cpu, view, initial_value);

  std::array<data_type, input_size> expected;
  std::iota(expected.begin(), expected.end(), initial_value);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(kwk::cpu, out, value, step) 4D with float")
{
  using data_type = float;
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t d3 = 3;
  data_type initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::iota(kwk::cpu, view, initial_value, step);

  std::array<data_type, input_size> expected;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(kwk::cpu, out, value, step), with float 4D with float")
{
  using data_type = float;
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t d3 = 3;
  data_type initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::iota(kwk::cpu, view, initial_value, step);

  std::array<data_type, input_size> expected;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};
