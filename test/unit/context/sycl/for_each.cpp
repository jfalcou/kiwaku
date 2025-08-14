//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include "test.hpp"

#if KIWAKU_BUILD_TEST_SYCL

#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/container.hpp>
#include <kwk/context/sycl/context.hpp>
#include <kwk/context/cpu/context.hpp>

// Reference implementation on CPU
TTS_CASE("Check for kwk::for_each(kwk::cpu, func, container) 1D")
{
  using data_type = int;
  const std::size_t size = 20;
  std::array<data_type, size> input;
  std::array<data_type, size> output;
  std::array<data_type, size> expected_output;

  for (std::size_t i = 0; i < size; ++i)
  {
    input[i] = i * 2;
    output[i] = 0;
    expected_output[i] = input[i] * 3 + 5;
  }

  auto view_input  = kwk::view{kwk::source = input , kwk::of_size(size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(size)};

  kwk::for_each(kwk::cpu, [&](auto const& e1, auto& e2) { e2 = e1 * 3 + 5; }, view_input, view_output);

  TTS_ALL_EQUAL(expected_output, output);
};


TTS_CASE("Check for kwk::for_each(kwk::sycl, func, container) 1D")
{
  using data_type = int;
  const std::size_t size = 20;
  std::array<data_type, size> input;
  std::array<data_type, size> output;
  std::array<data_type, size> expected_output;

  for (std::size_t i = 0; i < size; ++i)
  {
    input[i] = i * 2;
    output[i] = 0;
    expected_output[i] = input[i] * 3 + 5;
  }

  auto view_input  = kwk::view{kwk::source = input , kwk::of_size(size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(size)};

  kwk::for_each(kwk::sycl::default_context, [&](auto const& e1, auto& e2) { e2 = e1 * 3 + 5; }, view_input, view_output);

  TTS_ALL_EQUAL(expected_output, output);
};


TTS_CASE("Check for kwk::for_each(kwk::sycl, func, container) 4D")
{
  using data_type = int;
  const std::size_t d0 = 20;
  const std::size_t d1 = 41;
  const std::size_t d2 = 14;
  const std::size_t d3 = 11;
  const std::size_t size = d0 * d1 * d2 * d3;;

  std::array<data_type, size> input;
  std::array<data_type, size> output;
  std::array<data_type, size> expected_output;

  for (std::size_t i = 0; i < size; ++i)
  {
    input[i] = i * 2;
    output[i] = 0;
    expected_output[i] = input[i] * 3 + 5;
  }

  auto view_input  = kwk::view{kwk::source = input , kwk::of_size(d0, d1, d2, d3)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(d0, d1, d2, d3)};

  kwk::for_each(kwk::sycl::default_context, [&](auto const& e1, auto& e2) { e2 = e1 * 3 + 5; }, view_input, view_output);

  TTS_ALL_EQUAL(expected_output, output);
};

#else // KIWAKU_BUILD_TEST_SYCL

TTS_CASE("SYCL disabled, kwk::for_each with SYCL context skipped.")
{
  TTS_PASS("SYCL disabled, skipping test.");
};

#endif // KIWAKU_BUILD_TEST_SYCL
