//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"

#if KIWAKU_BUILD_TEST_SYCL

#include <cstdlib>
#include <kwk/context/sycl/context.hpp>
#include <kwk/algorithm/algos/numeric.hpp>
#include <kwk/container.hpp>

#include <numeric>


TTS_CASE("Check for kwk::transform_reduce(kwk::sycl::default_context, In1, In2, init, Reduce, Transform) 1D")
{
  float init_value = 11;
  const std::size_t input_size = 200000;
  std::array<int, input_size> input1;
  std::array<float, input_size> input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 0.3f + 10.875f;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};

  auto res = kwk::transform_reduce(kwk::sycl::default_context, view1, view2, init_value
                                    // Reduce function
                                  , [&](auto r, auto d) { return (r + d); } 
                                    // Transform function
                                  , [&](auto i1, auto i2) { return (i1 * i2); }
                                  );

  auto chk = std::transform_reduce ( input1.begin(), input1.end(), input2.begin(), init_value
                                      // Reduce function
                                    , [](auto r, auto d) { return (r + d); }
                                      // Transform function
                                    , [](auto i1, auto i2) { return (i1 * i2); });

  TTS_EQUAL(typeid(res), typeid(float));
  TTS_RELATIVE_EQUAL(res, chk, FLOAT_TOLERANCE_PERCENT);
  // TTS_EXPECT(floats_are_same(res, chk));
};


TTS_CASE("Check for kwk::transform_reduce(kwk::sycl::default_context, In1, In2, init, Transform) 1D")
{
  double init_value = 11;
  const std::size_t input_size = 200000;
  std::array<int, input_size> input1;
  std::array<double, input_size> input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 0.3 + 10.875;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};

  auto res = kwk::transform_reduce(kwk::sycl::default_context, view1, view2, init_value
                                    // Transform function
                                  , [&](auto i1, auto i2) { return (i1 * i2); }
                                  );

  auto chk = std::transform_reduce(input1.begin(), input1.end(), input2.begin(), init_value);

  TTS_EQUAL(typeid(res), typeid(double));
  TTS_RELATIVE_EQUAL(res, chk, FLOAT_TOLERANCE_PERCENT);
};


TTS_CASE("Check for kwk::transform_reduce(kwk::sycl::default_context, In1, In2) 1D")
{
  double init_value = 11;
  const std::size_t input_size = 84521;
  std::array<int, input_size> input1;
  std::array<double, input_size> input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 0.3 + 10.875;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};

  auto res = kwk::transform_reduce(kwk::sycl::default_context, view1, view2, init_value);
  auto chk = std::transform_reduce(input1.begin(), input1.end(), input2.begin(), init_value);

  TTS_EQUAL(typeid(res), typeid(double));
  TTS_RELATIVE_EQUAL(res, chk, FLOAT_TOLERANCE_PERCENT);
};

#else // KIWAKU_BUILD_TEST_SYCL

TTS_CASE("SYCL disabled, kwk::numeric with SYCL context skipped.")
{
  TTS_PASS("SYCL disabled, skipping test.");
};

#endif // KIWAKU_BUILD_TEST_SYCL
