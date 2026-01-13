//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"

#if KIWAKU_BUILD_TEST_SYCL

#include <kwk/context/sycl/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include <numeric>

// clear && ninja algorithm.algos.context.sycl.reduce.1d.exe && ./unit/algorithm.algos.context.sycl.reduce.1d.exe
TTS_CASE("Check for kwk::reduce(kwk::sycl::default_context, in) 1D")
{
  using data_type = int;
  const std::size_t d0 = 47100;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  auto res = kwk::reduce(kwk::sycl::default_context, view_in);
  auto res_std = std::reduce(input.begin(), input.end());

  // TTS_RELATIVE_EQUAL(res, res_std, FLOAT_TOLERANCE_PERCENT);
  TTS_EQUAL(res, res_std);
};


TTS_CASE("Check for kwk::reduce(kwk::sycl::default_context, in, func) and kwk::reduce(kwk::sycl::default_context, in, func, init) 1D")
{
  using data_type = int;
  const std::size_t d0 = 47100;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  // std::size_t count = 0;
  auto res = kwk::reduce(kwk::sycl::default_context, view_in, [&](auto e1, auto e2) { return e1 + e2 + 1; });
  auto res2 = kwk::reduce(kwk::sycl::default_context, view_in, [&](auto e1, auto e2) { return e1 + e2 + 1; }, 87);
  auto res_std = std::reduce(input.begin(), input.end(), 0, [&](auto e1, auto e2) { return e1 + e2 + 1; });

  // TTS_RELATIVE_EQUAL(res, res_std, FLOAT_TOLERANCE_PERCENT);
  TTS_EQUAL(res, res_std);
  TTS_EQUAL(static_cast<decltype(view_in)::value_type>(res2), res_std + 87);
  // TTS_EQUAL(count, input_size);
};

#else // KIWAKU_BUILD_TEST_SYCL

TTS_CASE("SYCL disabled, kwk::reduce with SYCL context skipped.")
{
  TTS_PASS("SYCL disabled, skipping test.");
};

#endif // KIWAKU_BUILD_TEST_SYCL