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


TTS_CASE("Check for kwk::fill(container, value) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  // std::fill(input.begin(), input.end(), 0);
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  kwk::fill(view, 8);
  std::array<int, input_size> expected;
  std::fill(expected.begin(), expected.end(), 8);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::fill(container, value) 2D with long long")
{
  using data_type = long long;
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

TTS_CASE("Check for kwk::fill(container, value) 3D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 10;
  const std::size_t d2 = 4;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2)};

  kwk::fill(view, 8);
  std::array<int, input_size> expected;
  std::fill(expected.begin(), expected.end(), 8);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::fill(container, value) 4D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 10;
  const std::size_t d2 = 4;
  const std::size_t d3 = 12;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::fill(view, 8);
  std::array<int, input_size> expected;
  std::fill(expected.begin(), expected.end(), 8);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::generate(func, container) 1D")
{
  const std::size_t d0 = 6;
  const std::size_t input_size = d0;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0)};

  std::size_t count = 0;
  kwk::generate ([&](auto i0) { 
                    count++;
                    return i0 * 3; 
                  }
                , view
                );

  std::array<int, input_size> expected;
  for (std::size_t i0 = 0; i0 < d0; ++i0)
  {
    expected[i0] = i0 * 3;
  }

  TTS_ALL_EQUAL(input, expected);
  TTS_EQUAL(count, input_size);
};

TTS_CASE("Check for kwk::generate(func, container) 2D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 7;
  const std::size_t input_size = d0 * d1;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  std::size_t count = 0;
  kwk::generate ([&](auto i0, auto i1) { 
                    count++;
                    return i0 * 100 + i1; 
                  }
                , view
                );

  std::array<int, input_size> expected;
  std::size_t cpt{0};
  for (std::size_t i0 = 0; i0 < d0; ++i0)
    for (std::size_t i1 = 0; i1 < d1; ++i1)
    {
      expected[cpt++] = i0 * 100 + i1;
    }

  TTS_ALL_EQUAL(input, expected);
  TTS_EQUAL(count, input_size);
};

TTS_CASE("Check for kwk::generate(func, container) 3D with long long")
{
  using data_type = long long;
  const std::size_t d0 = 6;
  const std::size_t d1 = 7;
  const std::size_t d2 = 9;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<data_type, input_size> input;
  for (data_type& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2)};

  std::size_t count = 0;
  kwk::generate ([&](auto i0, auto i1, auto i2) { 
                    count++;
                    return i0 * 10000 + i1 * 100 + i2; 
                  }
                , view
                );

  std::array<data_type, input_size> expected;
  std::size_t cpt{0};
  for (std::size_t i0 = 0; i0 < d0; ++i0)
    for (std::size_t i1 = 0; i1 < d1; ++i1)
      for (std::size_t i2 = 0; i2 < d2; ++i2)
      {
        expected[cpt++] = i0 * 10000 + i1 * 100 + i2;
      }

  TTS_ALL_EQUAL(input, expected);
  TTS_EQUAL(count, input_size);
};

TTS_CASE("Check for kwk::generate(func, container) 4D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 7;
  const std::size_t d2 = 9;
  const std::size_t d3 = 4;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  std::size_t count = 0;
  kwk::generate ([&](auto i0, auto i1, auto i2, auto i3) { 
                    count++;
                    return i0 * 1000000 + i1 * 10000 + i2 * 100 + i3; 
                  }
                , view
                );

  std::array<int, input_size> expected;
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


TTS_CASE("Check for kwk::iota(out) 1D")
{
  const std::size_t d0 = 6;
  const std::size_t input_size = d0;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0)};

  kwk::iota(view);

  std::array<int, input_size> expected;
  for (std::size_t i0 = 0; i0 < d0; ++i0)
  {
    expected[i0] = i0;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out) 2D with long long")
{
  using data_type = long long;
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

TTS_CASE("Check for kwk::iota(out) 3D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2)};

  kwk::iota(view);

  std::array<int, input_size> expected;
  std::iota(expected.begin(), expected.end(), 0);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out) 4D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t d3 = 3;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::iota(view);

  std::array<int, input_size> expected;
  std::iota(expected.begin(), expected.end(), 0);

  TTS_ALL_EQUAL(input, expected);
};


TTS_CASE("Check for kwk::iota(out, value) 1D with long long")
{
  using data_type = long long;
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

TTS_CASE("Check for kwk::iota(out, value) 2D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  int initial_value = 87;
  const std::size_t input_size = d0 * d1;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  kwk::iota(view, initial_value);

  std::array<int, input_size> expected;
  std::iota(expected.begin(), expected.end(), initial_value);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value) 3D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  int initial_value = 87;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2)};

  kwk::iota(view, initial_value);

  std::array<int, input_size> expected;
  std::iota(expected.begin(), expected.end(), initial_value);

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value) 4D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t d3 = 3;
  int initial_value = 87;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::iota(view, initial_value);

  std::array<int, input_size> expected;
  std::iota(expected.begin(), expected.end(), initial_value);

  TTS_ALL_EQUAL(input, expected);
};


TTS_CASE("Check for kwk::iota(out, value, step) 1D")
{
  const std::size_t d0 = 6;
  const std::size_t input_size = d0;
  int initial_value = 87;
  std::size_t step = 5;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0)};

  kwk::iota(view, initial_value, step);

  std::array<int, input_size> expected;
  for (std::size_t i0 = 0; i0 < d0; ++i0)
  {
    expected[i0] = i0 * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value, step) 2D with long long")
{
  using data_type = long long;
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

TTS_CASE("Check for kwk::iota(out, value, step) 3D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  int initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2)};

  kwk::iota(view, initial_value, step);

  std::array<int, input_size> expected;
  // std::iota(expected.begin(), expected.end(), initial_value, step);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value, step) 4D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t d3 = 3;
  int initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<int, input_size> input;
  for (int& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::iota(view, initial_value, step);

  std::array<int, input_size> expected;
  // std::iota(expected.begin(), expected.end(), initial_value, step);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};


TTS_CASE("Check for kwk::iota(out, value, step) with float 1D")
{
  const std::size_t d0 = 6;
  const std::size_t input_size = d0;
  float initial_value = 87;
  std::size_t step = 5;
  std::array<float, input_size> input;
  for (float& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0)};

  kwk::iota(view, initial_value, step);

  std::array<float, input_size> expected;
  for (std::size_t i0 = 0; i0 < d0; ++i0)
  {
    expected[i0] = i0 * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value, step), with float 2D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  float initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1;
  std::array<float, input_size> input;
  for (float& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  kwk::iota(view, initial_value, step);

  std::array<float, input_size> expected;
  // std::iota(expected.begin(), expected.end(), initial_value, step);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value, step), with float 3D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  float initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<float, input_size> input;
  for (float& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2)};

  kwk::iota(view, initial_value, step);

  std::array<float, input_size> expected;
  // std::iota(expected.begin(), expected.end(), initial_value, step);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};

TTS_CASE("Check for kwk::iota(out, value, step), with float 4D")
{
  const std::size_t d0 = 6;
  const std::size_t d1 = 8;
  const std::size_t d2 = 11;
  const std::size_t d3 = 3;
  float initial_value = 87;
  std::size_t step = 5;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<float, input_size> input;
  for (float& e : input) { e = 0; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  kwk::iota(view, initial_value, step);

  std::array<float, input_size> expected;
  // std::iota(expected.begin(), expected.end(), initial_value, step);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    expected[i] = i * step + initial_value;
  }

  TTS_ALL_EQUAL(input, expected);
};
