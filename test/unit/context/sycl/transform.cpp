//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"

#if KIWAKU_BUILD_TEST_SYCL

#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include <algorithm>
#include <kwk/context/sycl/context.hpp>

// ninja algorithm.algos.context.sycl.transform.exe && ./unit/algorithm.algos.context.sycl.transform.exe

TTS_CASE("Check for kwk::transform(kwk::sycl::default_context, value, new_value) 1D")
{
  #if KIWAKU_BUILD_TEST_SYCL
    std::cout << "=== KIWAKU_BUILD_TEST_SYCL -> True\n";
  #else
    std::cout << "=== KIWAKU_BUILD_TEST_SYCL -> False\n";
  #endif

  using data_type = int;
  const std::size_t d0 = 87;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input1, input2, output, check;

  for (std::size_t i = 0; i < input_size; ++i) { input1[i] = i * 3; input2[i] = i * 2; }

  auto view_in1  = kwk::view{kwk::source = input1 , kwk::of_size(d0)};
  auto view_in2  = kwk::view{kwk::source = input2 , kwk::of_size(d0)};
  auto view_out  = kwk::view{kwk::source = output , kwk::of_size(d0)};

  kwk::transform( kwk::sycl::default_context, [](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                , view_out
                , view_in1
                , view_in2
                );

  std::transform( input1.begin(), input1.end()
                , input2.begin()
                , check.begin()
                , [](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                );

  TTS_ALL_EQUAL(output, check);
};

TTS_CASE("Check for kwk::transform(kwk::sycl::default_context, value, new_value) 1D with float")
{
  using data_type = float;
  const std::size_t d0 = 87;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input1, input2, output, check;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i * static_cast<data_type>(3.88);
    input2[i] = i * static_cast<data_type>(2.87);
  }

  auto view_in1  = kwk::view{kwk::source = input1 , kwk::of_size(d0)};
  auto view_in2  = kwk::view{kwk::source = input2 , kwk::of_size(d0)};
  auto view_out  = kwk::view{kwk::source = output , kwk::of_size(d0)};

  kwk::transform( kwk::sycl::default_context, [&](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                , view_out
                , view_in1
                , view_in2
                );

  std::transform( input1.begin(), input1.end()
                , input2.begin()
                , check.begin()
                , [](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                );

  TTS_ALL_EQUAL(output, check);
};

TTS_CASE("Check for kwk::transform(kwk::sycl::default_context, value, new_value) 1D with std::uint64_t")
{
  using data_type = std::uint64_t;
  const std::size_t d0 = 87;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input1, input2, output, check;

  for (std::size_t i = 0; i < input_size; ++i) { input1[i] = i * 3; input2[i] = i * 2; }

  auto view_in1  = kwk::view{kwk::source = input1 , kwk::of_size(d0)};
  auto view_in2  = kwk::view{kwk::source = input2 , kwk::of_size(d0)};
  auto view_out  = kwk::view{kwk::source = output , kwk::of_size(d0)};

  kwk::transform( kwk::sycl::default_context, [&](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                , view_out
                , view_in1
                , view_in2
                );

  std::transform( input1.begin(), input1.end()
                , input2.begin()
                , check.begin()
                , [](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                );

  TTS_ALL_EQUAL(output, check);
};

TTS_CASE("Check for kwk::transform(kwk::sycl::default_context, value, new_value) 2D")
{
  using data_type = int;
  const std::size_t d0 = 87;
  const std::size_t d1 = 18;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input1, input2, output, check;

  for (std::size_t i = 0; i < input_size; ++i) { input1[i] = i * 3; input2[i] = i * 2; }

  auto view_in1  = kwk::view{kwk::source = input1 , kwk::of_size(d0, d1)};
  auto view_in2  = kwk::view{kwk::source = input2 , kwk::of_size(d0, d1)};
  auto view_out  = kwk::view{kwk::source = output , kwk::of_size(d0, d1)};

  kwk::transform( kwk::sycl::default_context, [&](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                , view_out
                , view_in1
                , view_in2
                );

  std::transform( input1.begin(), input1.end()
                , input2.begin()
                , check.begin()
                , [](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                );

  TTS_ALL_EQUAL(output, check);
};

TTS_CASE("Check for kwk::transform(kwk::sycl::default_context, value, new_value) 4D")
{
  using data_type = int;
  const std::size_t d0 = 87;
  const std::size_t d1 = 18;
  const std::size_t d2 = 41;
  const std::size_t d3 = 8;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<data_type, input_size> input1, input2, output, check;

  for (std::size_t i = 0; i < input_size; ++i) { input1[i] = i * 3; input2[i] = i * 2; }

  auto view_in1  = kwk::view{kwk::source = input1 , kwk::of_size(d0, d1, d2, d3)};
  auto view_in2  = kwk::view{kwk::source = input2 , kwk::of_size(d0, d1, d2, d3)};
  auto view_out  = kwk::view{kwk::source = output , kwk::of_size(d0, d1, d2, d3)};

  kwk::transform( kwk::sycl::default_context, [&](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                , view_out
                , view_in1
                , view_in2
                );

  std::transform( input1.begin(), input1.end()
                , input2.begin()
                , check.begin()
                , [](auto const e1, auto const e2) {
                    return e1 + e2;
                  }
                );

  TTS_ALL_EQUAL(output, check);
};

#else // KIWAKU_BUILD_TEST_SYCL

TTS_CASE("SYCL disabled, kwk::transform with SYCL context skipped.")
{
  TTS_PASS("SYCL disabled, skipping test.");
};

#endif // KIWAKU_BUILD_TEST_SYCL