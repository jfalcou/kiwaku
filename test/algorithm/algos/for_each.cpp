//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::for_each(func, shape)")
{
  auto shp = kwk::of_size(4,kwk::fixed<6>,3ULL);

  int count = 0;
  kwk::for_each( [&](auto...) { ++count; }, shp);

  TTS_EQUAL(count, shp.numel());
};


TTS_CASE("Check for kwk::for_each(func, container) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  int chk_total{0};
  for (int e : input) { chk_total += e; }

  std::size_t count{0};
  int total{0};

  kwk::for_each( [&](auto e) { ++count; total += e; }, view);

  TTS_EQUAL(count, input_size);
  TTS_EQUAL(total, chk_total);
};


TTS_CASE("Check for kwk::for_each_index(func, container) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  int chk_total{0};
  for (int e : input) { chk_total += e; }

  std::size_t count{0};
  int total{0};

  kwk::for_each_index( [&](auto& e, auto i0) { ++count; total += e; e = i0 + 1; }, view);

  TTS_EQUAL(count, input_size);
  TTS_EQUAL(total, chk_total);

  std::array<int, input_size> expected_result;
  for (std::size_t i = 0; i < input_size; ++i) { expected_result[i] = i + 1; }

  TTS_ALL_EQUAL(input, expected_result);
};


TTS_CASE("Check for kwk::for_each_index(func, container) 2D")
{
  const std::size_t d0 = 4;
  const std::size_t d1 = 10;
  const std::size_t input_size = d0 * d1;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1)};

  int chk_total{0};
  for (int e : input) { chk_total += e; }
  std::size_t count{0};
  int total{0};

  auto pos_to_value = [](std::size_t i0, std::size_t i1) { return i0*100 + i1; };

  kwk::for_each_index( [&](auto& e, auto i0, auto i1) { ++count; total += e; e = pos_to_value(i0, i1); }, view);

  TTS_EQUAL(count, input_size);
  TTS_EQUAL(total, chk_total);

  std::array<int, input_size> expected_result;
  std::size_t cpt{0};
  for (std::size_t i0 = 0; i0 < d0; ++i0)
    for (std::size_t i1 = 0; i1 < d1; ++i1)
    {
      expected_result[cpt++] = pos_to_value(i0, i1);
    }

  TTS_ALL_EQUAL(input, expected_result);
};


TTS_CASE("Check for kwk::for_each_index(func, container) 3D")
{
  const std::size_t d0 = 2;
  const std::size_t d1 = 10;
  const std::size_t d2 = 6;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2)};

  int chk_total{0};
  for (int e : input) { chk_total += e; }
  std::size_t count{0};
  int total{0};

  auto pos_to_value = [](std::size_t i0, std::size_t i1, std::size_t i2) {
    return i0 * 10000 + i1 * 100 + i2;
  };

  kwk::for_each_index (
                        [&](auto& e, auto i0, auto i1, auto i2) {
                          ++count; total += e; e = pos_to_value(i0, i1, i2);
                        }
                      , view);

  TTS_EQUAL(count, input_size);
  TTS_EQUAL(total, chk_total);

  std::array<int, input_size> expected_result;
  std::size_t cpt{0};
  for (std::size_t i0 = 0; i0 < d0; ++i0)
    for (std::size_t i1 = 0; i1 < d1; ++i1)
      for (std::size_t i2 = 0; i2 < d2; ++i2)
      {
        expected_result[cpt++] = pos_to_value(i0, i1, i2);
      }

  TTS_ALL_EQUAL(input, expected_result);
};


TTS_CASE("Check for kwk::for_each_index(func, container) 4D")
{
  const std::size_t d0 = 2;
  const std::size_t d1 = 10;
  const std::size_t d2 = 6;
  const std::size_t d3 = 3;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(d0, d1, d2, d3)};

  int chk_total{0};
  for (int e : input) { chk_total += e; }
  std::size_t count{0};
  int total{0};

  auto pos_to_value = [](std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3) {
    return i0 * 1000000 + i1 * 10000 + i2 * 100 + i3;
  };

  kwk::for_each_index (
                        [&](auto& e, auto i0, auto i1, auto i2, auto i3) {
                          ++count; total += e; e = pos_to_value(i0, i1, i2, i3);
                        }
                      , view);

  TTS_EQUAL(count, input_size);
  TTS_EQUAL(total, chk_total);

  std::array<int, input_size> expected_result;
  std::size_t cpt{0};
  for (std::size_t i0 = 0; i0 < d0; ++i0)
    for (std::size_t i1 = 0; i1 < d1; ++i1)
      for (std::size_t i2 = 0; i2 < d2; ++i2)
        for (std::size_t i3 = 0; i3 < d3; ++i3)
        {
          expected_result[cpt++] = pos_to_value(i0, i1, i2, i3);
        }

  TTS_ALL_EQUAL(input, expected_result);
};


