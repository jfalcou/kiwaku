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

TTS_CASE("Check for kwk::transform_reduce(In1, In2, Reduce, Transform, init) 1D")
{
  int init_value = 11;
  const std::size_t input_size = 200;
  std::array<int, input_size> input1, input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};

  std::size_t reduce_count = 0;
  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Reduce function
                                  , [&](auto r, auto d){ ++reduce_count; return (r + d); } 
                                    // Transform function
                                  , [&](auto i1, auto i2){ ++transform_count; return (i1 * i2); }
                                  );
    // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
  
  int chk = init_value;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    int transformed = input1[i] * input2[i];
    int reduced = chk + transformed;
    chk = reduced;
  }

  TTS_EQUAL(res, chk);
  TTS_EQUAL(reduce_count, input_size);
  TTS_EQUAL(transform_count, input_size);
};



TTS_CASE("Check for kwk::transform_reduce(In1, In2, Reduce, Transform, init) 2D")
{
  int init_value = 11;
  const std::size_t d0 = 18;
  const std::size_t d1 = 21;
  const std::size_t input_size = d0 * d1;
  std::array<int, input_size> input1, input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0, d1)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0, d1)};

  std::size_t reduce_count = 0;
  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Reduce function
                                  , [&](auto r, auto d){ ++reduce_count; return (r + d); } 
                                    // Transform function
                                  , [&](auto i1, auto i2){ ++transform_count; return (i1 * i2); }
                                  );
    // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
  
  int chk = init_value;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    int transformed = input1[i] * input2[i];
    int reduced = chk + transformed;
    chk = reduced;
  }

  TTS_EQUAL(res, chk);
  TTS_EQUAL(reduce_count, input_size);
  TTS_EQUAL(transform_count, input_size);
};

TTS_CASE("Check for kwk::transform_reduce(In1, In2, Reduce, Transform, init) 3D")
{
  int init_value = 11;
  const std::size_t d0 = 18;
  const std::size_t d1 = 21;
  const std::size_t d2 = 6;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<int, input_size> input1, input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0, d1, d2)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0, d1, d2)};

  std::size_t reduce_count = 0;
  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Reduce function
                                  , [&](auto r, auto d){ ++reduce_count; return (r + d); } 
                                    // Transform function
                                  , [&](auto i1, auto i2){ ++transform_count; return (i1 * i2); }
                                  );
    // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
  
  int chk = init_value;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    int transformed = input1[i] * input2[i];
    int reduced = chk + transformed;
    chk = reduced;
  }

  TTS_EQUAL(res, chk);
  TTS_EQUAL(reduce_count, input_size);
  TTS_EQUAL(transform_count, input_size);
};

TTS_CASE("Check for kwk::transform_reduce(In1, In2, Reduce, Transform, init) 4D")
{
  int init_value = 11;
  const std::size_t d0 = 18;
  const std::size_t d1 = 21;
  const std::size_t d2 = 6;
  const std::size_t d3 = 23;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<int, input_size> input1, input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0, d1, d2, d3)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0, d1, d2, d3)};

  std::size_t reduce_count = 0;
  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Reduce function
                                  , [&](auto r, auto d){ ++reduce_count; return (r + d); } 
                                    // Transform function
                                  , [&](auto i1, auto i2){ ++transform_count; return (i1 * i2); }
                                  );
    // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
  
  int chk = init_value;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    int transformed = input1[i] * input2[i];
    int reduced = chk + transformed;
    chk = reduced;
  }

  TTS_EQUAL(res, chk);
  TTS_EQUAL(reduce_count, input_size);
  TTS_EQUAL(transform_count, input_size);
};


TTS_CASE("Check for kwk::transform_reduce(In1, In2, init, Transform) 1D")
{
  int init_value = 11;
  const std::size_t d0 = 18;
  const std::size_t input_size = d0;
  std::array<int, input_size> input1, input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0)};

  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Transform function
                                  , [&](auto i1, auto i2){ ++transform_count; return (i1 * i2); }
                                  );
  // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
  
  int chk = init_value;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    int transformed = input1[i] * input2[i];
    int reduced = chk + transformed;
    chk = reduced;
  }

  TTS_EQUAL(res, chk);
  TTS_EQUAL(transform_count, input_size);
};
TTS_CASE("Check for kwk::transform_reduce(In1, In2, init, Transform) 2D")
{
  int init_value = 11;
  const std::size_t d0 = 18;
  const std::size_t d1 = 21;
  const std::size_t input_size = d0 * d1;
  std::array<int, input_size> input1, input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0, d1)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0, d1)};

  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Transform function
                                  , [&](auto i1, auto i2){ ++transform_count; return (i1 * i2); }
                                  );
  // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
  
  int chk = init_value;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    int transformed = input1[i] * input2[i];
    int reduced = chk + transformed;
    chk = reduced;
  }

  TTS_EQUAL(res, chk);
  TTS_EQUAL(transform_count, input_size);
};
TTS_CASE("Check for kwk::transform_reduce(In1, In2, init, Transform) 3D")
{
  int init_value = 11;
  const std::size_t d0 = 18;
  const std::size_t d1 = 21;
  const std::size_t d2 = 6;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<int, input_size> input1, input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0, d1, d2)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0, d1, d2)};

  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Transform function
                                  , [&](auto i1, auto i2){ ++transform_count; return (i1 * i2); }
                                  );
  // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
  
  int chk = init_value;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    int transformed = input1[i] * input2[i];
    int reduced = chk + transformed;
    chk = reduced;
  }

  TTS_EQUAL(res, chk);
  TTS_EQUAL(transform_count, input_size);
};

TTS_CASE("Check for kwk::transform_reduce(In1, In2, init, Transform) 4D")
{
  int init_value = 11;
  const std::size_t d0 = 18;
  const std::size_t d1 = 21;
  const std::size_t d2 = 6;
  const std::size_t d3 = 23;
  const std::size_t input_size = d0 * d1 * d2 * d3;
  std::array<int, input_size> input1, input2;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0, d1, d2, d3)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0, d1, d2, d3)};

  std::size_t transform_count = 0;

  auto res = kwk::transform_reduce( view1, view2, init_value
                                    // Transform function
                                  , [&](auto i1, auto i2){ ++transform_count; return (i1 * i2); }
                                  );
  // ctx.map([&](auto const& i1, auto const& i2) { init = R(init, T(i1, i2)); }, ctx.in(in1), ctx.in(in2));
  
  int chk = init_value;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    int transformed = input1[i] * input2[i];
    int reduced = chk + transformed;
    chk = reduced;
  }

  TTS_EQUAL(res, chk);
  TTS_EQUAL(transform_count, input_size);
};






// int myaccumulator(int x, int y)
// {
//     return x + y;
// }

// int myproduct(int x, int y)
// {
//     return x * y;
// }

// TTS_CASE("TEST FOR std::inner_product(In1, In2, value, sum, product) 1D")
// {
//   // The value which is added after
//   // finding inner_product b/w elements
//   int init = 100;
//   int series1[] = { 10, 20, 30 };
//   int series2[] = { 1, 2, 3 };
//   int n = sizeof(series1) / sizeof(series1[0]);

//   // Elements in series1
//   std::cout << "First array contains :";
//   for (int i = 0; i < n; i++)
//       std::cout << " " << series1[i];
//   std::cout << "\n";

//   // Elements in series2
//   std::cout << "Second array contains :";
//   for (int i = 0; i < n; i++)
//       std::cout << " " << series2[i];
//   std::cout << "\n\n";

//   std::cout << "Using custom functions: ";
//   std::cout << std::inner_product(series1, series1 + 3, series2, init,
//                                   myaccumulator, myproduct);
//   std::cout << '\n';
// };

TTS_CASE("Check for kwk::inner_product(In1, In2, value, sum, product) 1D")
{
  long long init_value = 87;
  const std::size_t d0 = 2784;
  const std::size_t input_size = d0;
  std::array<long long, input_size> input1, input2;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
    // input1[i] = i+1;
    // input2[i] = (i+1) * 10;
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

  // std::cout << "res = "     << res     << "\n";
  // std::cout << "res_std = " << res_std << "\n";

  TTS_EQUAL(res, res_std);
};


// TODO: 3D, 4D
TTS_CASE("Check for kwk::inner_product(In1, In2, value, sum, product) 2D")
{
  long long init_value = 87;
  const std::size_t d0 = 278;
  const std::size_t d1 = 71;
  const std::size_t input_size = d0 * d1;
  std::array<long long, input_size> input1, input2;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0, d1)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0, d1)};

  auto res = kwk::inner_product(view1, view2, init_value,
            [](auto r, auto d) { return (r + d); },
            [](auto i1, auto i2) { return (i1 * i2); }
            );

  auto res_std = std::inner_product (input1.begin(), input1.end(), input2.begin(), init_value
                                    , [](auto r, auto d) { return (r + d); }
                                    , [](auto i1, auto i2) { return (i1 * i2); }
                                    );

  TTS_EQUAL(res, res_std);
};



TTS_CASE("Check for kwk::inner_product(In1, In2) 1D")
{
  long long init_value = 87;
  const std::size_t d0 = 2784;
  const std::size_t input_size = d0;
  std::array<long long, input_size> input1, input2;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0)};

  auto res = kwk::inner_product(view1, view2, init_value);
  auto res_std = std::inner_product (input1.begin(), input1.end(), input2.begin(), init_value);

  TTS_EQUAL(res, res_std);
};

TTS_CASE("Check for kwk::inner_product(In1, In2) 2D")
{
  long long init_value = 87;
  const std::size_t d0 = 2784;
  const std::size_t d1 = 81;
  const std::size_t input_size = d0 * d1;
  std::array<long long, input_size> input1, input2;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i;
    input2[i] = i * 3 + 10;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(d0, d1)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(d0, d1)};

  auto res = kwk::inner_product(view1, view2, init_value);
  auto res_std = std::inner_product (input1.begin(), input1.end(), input2.begin(), init_value);

  TTS_EQUAL(res, res_std);
};

TTS_CASE("Check for kwk::transform_exclusive_scan(In, Out, init, f1, f2) 1D")
{
  long long init_value = 87;
  const std::size_t d0 = 9;
  const std::size_t input_size = d0;
  std::array<long long, input_size> input, output, output_std;

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
                                , [](auto r, auto d){ return (r + d); }
                                , [](auto i1){ return (i1); }
                                );

  std::transform_exclusive_scan ( input.begin(), input.end()
                                , output_std.begin()
                                , init_value
                                , [](auto r, auto d){ return (r + d); }
                                , [](auto i1){ return (i1); }
                                );

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::transform_exclusive_scan(In, Out, init, f1, f2) 2D with long long")
{
  using data_type = long long;
  data_type init_value = 561;
  const std::size_t d0 = 87;
  const std::size_t d1 = 112;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0, d1)};

  kwk::transform_exclusive_scan ( view_in, view_out
                                , init_value
                                , [](auto r, auto d){ return (r + d); }
                                , [](auto i1){ return (i1); }
                                );

  std::transform_exclusive_scan ( input.begin(), input.end()
                                , output_std.begin()
                                , init_value
                                , [](auto r, auto d){ return (r + d); }
                                , [](auto i1){ return (i1); }
                                );

  TTS_ALL_EQUAL(output, output_std);
};

TTS_CASE("Check for kwk::transform_inclusive_scan(In, Out, init, f1, f2) 1D")
{
  const std::size_t d0 = 9;
  const std::size_t input_size = d0;
  std::array<int, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input, kwk::of_size(d0)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0)};

  kwk::transform_inclusive_scan ( view_in, view_out
                                , 0
                                , [](auto r, auto d){ return (r + d); }
                                , [](auto i1){ return (i1); }
                                );

  std::transform_inclusive_scan ( input.begin(), input.end()
                                , output_std.begin()
                                , [](auto r, auto d){ return (r + d); }
                                , [](auto i1){ return (i1); }
                                );

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::transform_inclusive_scan(In, Out, init, f1, f2) 2D with long long")
{
  using data_type = long long;
  const std::size_t d0 = 87;
  const std::size_t d1 = 112;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0, d1)};

  kwk::transform_inclusive_scan ( view_in, view_out
                                , data_type{0}
                                , [](auto r, auto d){ return (r + d); }
                                , [](auto i1){ return (i1); }
                                );

  std::transform_inclusive_scan ( input.begin(), input.end()
                                , output_std.begin()
                                , [](auto r, auto d){ return (r + d); }
                                , [](auto i1){ return (i1); }
                                );

  TTS_ALL_EQUAL(output, output_std);
};



TTS_CASE("Check for kwk::exclusive_scan(In, Out, init, func) 1D")
{
  int initial_value = 0;
  const std::size_t d0 = 9;
  const std::size_t input_size = d0;
  std::array<int, input_size> input, output, output_std;

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
                      , [](auto r, auto d){ return (r * d); }
                      );

  std::exclusive_scan ( input.begin(), input.end()
                      , output_std.begin()
                      , initial_value
                      , [](auto r, auto d){ return (r * d); }
                      );

  TTS_ALL_EQUAL(output, output_std);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init, func) 2D with long long")
{
  long long initial_value = 0;
  const std::size_t d0 = 122;
  const std::size_t d1 = 45;
  const std::size_t input_size = d0 * d1;
  std::array<long long, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0, d1)};

  kwk::exclusive_scan ( view_in, view_out
                      , initial_value
                      , [](auto r, auto d){ return (r * d); }
                      );

  std::exclusive_scan ( input.begin(), input.end()
                      , output_std.begin()
                      , initial_value
                      , [](auto r, auto d){ return (r * d); }
                      );

  TTS_ALL_EQUAL(output, output_std);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init) 1D")
{
  int initial_value = 0;
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

  kwk::exclusive_scan(view_in, view_out, initial_value);

  std::exclusive_scan(input.begin(), input.end(), output_std.begin(), initial_value);

  TTS_ALL_EQUAL(output, output_std);
};

TTS_CASE("Check for kwk::exclusive_scan(In, Out, init) 2D with long long")
{
  long long initial_value = 0;
  const std::size_t d0 = 122;
  const std::size_t d1 = 45;
  const std::size_t input_size = d0 * d1;
  std::array<long long, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0, d1)};

  kwk::exclusive_scan(view_in, view_out, initial_value);

  std::exclusive_scan(input.begin(), input.end(), output_std.begin(), initial_value);

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::inclusive_scan(In, Out, init, func) 1D")
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

  kwk::inclusive_scan(view_in, view_out, 0, [](auto r, auto d){ return r + d; });

  std::inclusive_scan(input.begin(), input.end(), output_std.begin(), [](auto r, auto d){ return r + d; });

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::inclusive_scan(In, Out, init, func) 2D with long long")
{
  using data_type = long long;
  const std::size_t d0 = 471;
  const std::size_t d1 = 18;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0, d1)};

  kwk::inclusive_scan(view_in, view_out, 0, [](auto r, auto d){ return r + d; });

  std::inclusive_scan(input.begin(), input.end(), output_std.begin(), [](auto r, auto d){ return r + d; });

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

  kwk::inclusive_scan(view_in, view_out, 0);

  std::inclusive_scan(input.begin(), input.end(), output_std.begin());

  TTS_ALL_EQUAL(output, output_std);
};


TTS_CASE("Check for kwk::inclusive_scan(In, Out, init) 2D with long long")
{
  using data_type = long long;
  const std::size_t d0 = 471;
  const std::size_t d1 = 18;
  const std::size_t input_size = d0 * d1;
  std::array<data_type, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0, d1)};

  kwk::inclusive_scan(view_in, view_out, 0);

  std::inclusive_scan(input.begin(), input.end(), output_std.begin());

  TTS_ALL_EQUAL(output, output_std);
};

TTS_CASE("Check for kwk::inclusive_scan(In, Out, init) 3D")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t d1 = 18;
  const std::size_t d2 = 3;
  const std::size_t input_size = d0 * d1 * d2;
  std::array<data_type, input_size> input, output, output_std;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    input[i] = i * 3 + 10;
    output[i] = 0;
    output_std[i] = 0;
  }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0, d1, d2)};
  auto view_out = kwk::view{kwk::source = output, kwk::of_size(d0, d1, d2)};

  kwk::inclusive_scan(view_in, view_out, 0);

  std::inclusive_scan(input.begin(), input.end(), output_std.begin());

  TTS_ALL_EQUAL(output, output_std);
};



