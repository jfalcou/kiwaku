//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/numeric.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>


TTS_CASE("Check for kwk::transform_reduce(In1, In2, init, Reduce, Transform) 1D")
{
  float init_value = 11;
  const std::size_t input_size = 200;
  std::array<int, input_size> input1;
  std::array<float, input_size> input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 0.3 + 10.875;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};

  std::size_t reduce_count = 0;
  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Reduce function
                                  , [&](auto r, auto d) { ++reduce_count; return (r + d); } 
                                    // Transform function
                                  , [&](auto i1, auto i2) { ++transform_count; return (i1 * i2); }
                                  );

  auto chk = std::transform_reduce ( input1.begin(), input1.end(), input2.begin(), init_value
                                      // Reduce function
                                    , [](auto r, auto d) { return (r + d); }
                                      // Transform function
                                    , [](auto i1, auto i2) { return (i1 * i2); });

  TTS_EQUAL(typeid(res), typeid(float));
  TTS_EQUAL(res, chk);
  TTS_EQUAL(reduce_count, input_size);
  TTS_EQUAL(transform_count, input_size);
};


TTS_CASE("Check for kwk::transform_reduce(In1, In2, init, Transform) 1D")
{
  double init_value = 11;
  const std::size_t input_size = 200;
  std::array<int, input_size> input1;
  std::array<double, input_size> input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 0.3 + 10.875;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};

  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Transform function
                                  , [&](auto i1, auto i2) { ++transform_count; return (i1 * i2); }
                                  );

  auto chk = std::transform_reduce(input1.begin(), input1.end(), input2.begin(), init_value);

  TTS_EQUAL(typeid(res), typeid(double));
  TTS_EQUAL(res, chk);
  TTS_EQUAL(transform_count, input_size);
};


TTS_CASE("Check for kwk::transform_reduce(In1, In2) 1D")
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

  auto res = kwk::transform_reduce(view1, view2, init_value);
  auto chk = std::transform_reduce(input1.begin(), input1.end(), input2.begin(), init_value);

  TTS_EQUAL(typeid(res), typeid(double));
  TTS_EQUAL(res, chk);
};


TTS_CASE("Check for kwk::inner_product(In1, In2, value, sum, product) 1D")
{
  float init_value = 87;
  const std::size_t d0 = 2784;
  const std::size_t input_size = d0;
  std::array<int, input_size> input1;
  std::array<float, input_size> input2;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 0.3 + 10.875;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0)};

  auto res = kwk::inner_product(view1, view2, init_value,
            [](auto r, auto d) { return (r + d); },
            [](auto i1, auto i2) { return (i1 * i2); }
            );

  auto res_std = std::inner_product (input1.begin(), input1.end(), input2.begin(), init_value
                                    , [](auto r, auto d) { return (r + d); }
                                    , [](auto i1, auto i2) { return (i1 * i2); }
                                    );

  TTS_EQUAL(res, res_std);
  TTS_EQUAL(typeid(res), typeid(float));
};


TTS_CASE("Check for kwk::inner_product(In1, In2, init) 1D")
{
  float init_value = 87.7;
  const std::size_t d0 = 2784;
  const std::size_t input_size = d0;
  std::array<int, input_size> input1;
  std::array<float, input_size> input2;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 0.3 + 10.875;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0)};

  auto res = kwk::inner_product(view1, view2, init_value);
  auto res_std = std::inner_product (input1.begin(), input1.end(), input2.begin(), init_value);

  TTS_EQUAL(res, res_std);
  TTS_EQUAL(typeid(res), typeid(float));
  TTS_EQUAL(typeid(res_std), typeid(float));
};


TTS_CASE("Check for kwk::inner_product(In1, In2) 1D")
{
  const std::size_t d0 = 2784;
  const std::size_t input_size = d0;
  std::array<int, input_size> input1;
  std::array<float, input_size> input2;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 0.3 + 10.875;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0)};

  auto res = kwk::inner_product(view1, view2);
  auto res_std = std::inner_product(input1.begin(), input1.end(), input2.begin(), float{});

  TTS_EQUAL(res, res_std);
  TTS_EQUAL(typeid(res), typeid(float));
  TTS_EQUAL(typeid(res_std), typeid(float));
};


TTS_CASE("Check for kwk::transform_exclusive_scan(In, Out, init, f1, f2) 1D")
{
  double init_value = 87;
  const std::size_t d0 = 4651;
  const std::size_t input_size = d0;
  std::array<int, input_size> input;
  std::array<double, input_size> output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input, kwk::of_size(d0)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0)};

  kwk::transform_exclusive_scan ( view_in, view_out
                                , init_value
                                , [](auto r, auto d){ return (r / d); }
                                , [](auto i1){ return (i1 * 2); }
                                );

  std::transform_exclusive_scan ( input.begin(), input.end()
                                , output_std.begin()
                                , init_value
                                , [](auto r, auto d){ return (r / d); }
                                , [](auto i1){ return (i1 * 2); }
                                );

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::transform_inclusive_scan(In, Out, init, f1, f2) 1D")
{
  double init_value = 0;
  const std::size_t d0 = 4816;
  const std::size_t input_size = d0;
  std::array<int, input_size> input;
  std::array<double, input_size> output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input, kwk::of_size(d0)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0)};

  kwk::transform_inclusive_scan ( view_in, view_out
                                , init_value
                                , [](auto r, auto d){ return (r / d); }
                                , [](auto i1){ return (i1 * 2); }
                                );

  std::transform_inclusive_scan ( input.begin(), input.end()
                                , output_std.begin()
                                , [](auto r, auto d){ return (r / d); }
                                , [](auto i1){ return (i1 * 2); }
                                , init_value
                                );

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::exclusive_scan(In, Out, init, func) 1D")
{
  float initial_value = 0;
  const std::size_t d0 = 8716;
  const std::size_t input_size = d0;
  std::array<int, input_size> input;
  std::array<double, input_size> output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input, kwk::of_size(d0)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0)};

  kwk::exclusive_scan ( view_in, view_out
                      , initial_value
                      , [](auto r, auto d){ return (r / d); }
                      );

  std::exclusive_scan ( input.begin(), input.end()
                      , output_std.begin()
                      , initial_value
                      , [](auto r, auto d){ return (r / d); }
                      );

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::exclusive_scan(In, Out, init) 1D")
{
  int initial_value = 0;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<int, input_size> input;
  std::array<double, input_size> output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0)};

  kwk::exclusive_scan(view_in, view_out, initial_value);

  std::exclusive_scan(input.begin(), input.end(), output_std.begin(), initial_value);

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::inclusive_scan(In, Out, init, func) 1D")
{
  const std::size_t d0 = 485;
  const std::size_t input_size = d0;
  std::array<int, input_size> input;
  std::array<double, input_size> output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0)};

  kwk::inclusive_scan(view_in, view_out, double{0}, [](auto r, auto d){ return r + d * 0.2452; });

  // for (int i = 0; i < 10; ++i) { std::cout << output[i] << " "; } std::cout << "\n";

  std::inclusive_scan ( input.begin(), input.end(), output_std.begin()
                      , [](auto r, auto d){ return r + d * 0.2452; }
                      , double{0}
                      );

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::inclusive_scan(In, Out, init) 1D")
{
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<int, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0)};

  kwk::inclusive_scan(view_in, view_out, int{0});

  // for (int i = 0; i < 10; ++i) { std::cout << output[i] << " "; } std::cout << "\n";

  std::inclusive_scan(input.begin(), input.end(), output_std.begin());

  TTS_ALL_EQUAL(output, output_std);
};