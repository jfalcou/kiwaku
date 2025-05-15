//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <kwk/context/sycl/context.hpp>
#include <kwk/context/cpu/context.hpp>

// For each SYCL
// ninja algorithm.algos.context.sycl.for_each.exe && ./unit/algorithm.algos.context.sycl.for_each.exe

// Reference implementation on CPU
TTS_CASE("Check for kwk::for_each(kwk::cpu, func, container) 1D")
{
  using data_type = int;
  const std::size_t size = 20;
  std::array<data_type, size> input;
  std::array<data_type, size> output;
  std::array<data_type, size> expected_output;

  // auto fn = [](data_type const e) { return e * 3 + 5; };

  for (std::size_t i = 0; i < size; ++i)
  {
    input[i] = i * 2;
    output[i] = 0;
    expected_output[i] = input[i] * 3 + 5;
  }

  auto view_input  = kwk::view{kwk::source = input , kwk::of_size(size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(size)};

  // data_type chk_total{0};
  // for (data_type e : input) { chk_total += e; }

  // std::size_t count{0};
  // data_type total{0};

  kwk::for_each(kwk::cpu, [&](auto const& e1, auto& e2) { e2 = e1 * 3 + 5; }, view_input, view_output);

  TTS_ALL_EQUAL(expected_output, output);
  // TTS_RELATIVE_EQUAL(total, chk_total, FLOAT_TOLERANCE_PERCENT);
};


TTS_CASE("Check for kwk::for_each(kwk::sycl, func, container) 1D")
{
  using data_type = int;
  const std::size_t size = 20;
  std::array<data_type, size> input;
  std::array<data_type, size> output;
  std::array<data_type, size> expected_output;

  // auto fn = [](data_type const e) { return e * 3 + 5; };

  for (std::size_t i = 0; i < size; ++i)
  {
    input[i] = i * 2;
    output[i] = 0;
    expected_output[i] = input[i] * 3 + 5;
  }

  auto view_input  = kwk::view{kwk::source = input , kwk::of_size(size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(size)};

  // data_type chk_total{0};
  // for (data_type e : input) { chk_total += e; }

  // std::size_t count{0};
  // data_type total{0};

  kwk::for_each(kwk::sycl::default_context, [&](auto const& e1, auto& e2) { e2 = e1 * 3 + 5; }, view_input, view_output);

  TTS_ALL_EQUAL(expected_output, output);
  // TTS_RELATIVE_EQUAL(total, chk_total, FLOAT_TOLERANCE_PERCENT);
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

  // auto fn = [](data_type const e) { return e * 3 + 5; };

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