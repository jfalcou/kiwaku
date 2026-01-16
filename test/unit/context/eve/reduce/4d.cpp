/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"

#if KIWAKU_BUILD_TEST_SIMD

#include <kwk/context/eve/context.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include <numeric>

TTS_CASE("Check for kwk::reduce(kwk::simd, in) 4D")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t d1 = 11;
  const std::size_t d2 = 13;
  const std::size_t d3 = 5;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1, d2, d3)};

  auto res = kwk::reduce(kwk::simd, view_in);
  auto res_std = std::reduce(input.begin(), input.end());

  TTS_EQUAL(res, res_std);
};


TTS_CASE("Check for kwk::reduce(kwk::simd, in, func) and kwk::reduce(kwk::simd, in, func, init) 4D")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t d1 = 11;
  const std::size_t d2 = 13;
  const std::size_t d3 = 5;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input;

  auto monoid = std::pair{[&](auto e1, auto e2){ return e1 + e2; }, 0};

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1, d2, d3)};

  auto res = kwk::reduce(kwk::simd, view_in, monoid);
  auto res2 = kwk::reduce(kwk::simd, view_in, monoid, 87);
  auto res_std = std::reduce(input.begin(), input.end(), 0, [&](auto e1, auto e2) { return e1 + e2; });

  TTS_EQUAL(res, res_std);
  TTS_EQUAL(static_cast<decltype(view_in)::value_type>(res2), res_std + 87);
};

#else

TTS_CASE("EVE disabled, kwk::reduce with EVE context skipped.")
{
  TTS_PASS("EVE disabled, skipping test.");
};

#endif
