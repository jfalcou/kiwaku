//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/sycl/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/numeric.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <benchmark/benchmark.hpp>

// ninja bench.transform_reduce_1d.exe && ./unit/bench.transform_reduce_1d.exe

// TTS_CASE("Benchmark for kwk::transform_reduce(kwk::sycl::default_context, In1, In2, init, Reduce, Transform) 1D")
// {
//   float init_value = 11;
//   const std::size_t input_size = 200000;
//   std::array<int, input_size> input1;
//   std::array<float, input_size> input2;
//   for (std::size_t i = 0; i < input_size; ++i)
//   {
//     input1[i] = i;
//     input2[i] = i * 0.3f + 10.875f;
//   }

//   auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
//   auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};

//   auto res = kwk::transform_reduce(kwk::sycl::default_context, view1, view2, init_value
//                                     // Reduce function
//                                   , [&](auto r, auto d) { return (r + d); } 
//                                     // Transform function
//                                   , [&](auto i1, auto i2) { return (i1 * i2); }
//                                   );

//   auto chk = std::transform_reduce ( input1.begin(), input1.end(), input2.begin(), init_value
//                                       // Reduce function
//                                     , [](auto r, auto d) { return (r + d); }
//                                       // Transform function
//                                     , [](auto i1, auto i2) { return (i1 * i2); });

//   TTS_EQUAL(typeid(res), typeid(float));
//   TTS_RELATIVE_EQUAL(res, chk, FLOAT_TOLERANCE_PERCENT);
//   // TTS_EXPECT(floats_are_same(res, chk));
// };



TTS_CASE("Benchmark for kwk::transform_reduce(In1, In2, init, Reduce, Transform) 1D")
{
  std::cout << "STD array allocate...\n";
  std::cout << "STD array allocate...\n";
  using result_type = int; //std::uint32_t;
  result_type init_value = 11;
  const std::size_t d0 = 1024 * 1024 * 256 ;
  const std::size_t input_size = d0;

  std::vector<result_type> input1;
  std::vector<result_type> input2;
  input1.resize(input_size);
  input2.resize(input_size);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = i * 1.5f;
    input2[i] = i * 2;
  }

  auto view_in1  = kwk::view{kwk::source = input1.data() , kwk::of_size(d0)};
  auto view_in2  = kwk::view{kwk::source = input2.data() , kwk::of_size(d0)};

  auto f_reduce = [](auto a, auto b) { return (((((((a + b) % 200) % 150) % 100) % 50) % 25) % 12) % 7; };
  auto f_transform = [](auto a, auto b) { return (a * b); };

  // auto f_reduce = [](auto r, auto d) { return (r + d); };
  // auto f_transform = [](auto i1, auto i2) { return (i1 * i2); };

  result_type res_kwk_sycl, res_std, res_kwk_cpu, res_hand;

  auto fct_kwk_sycl = [&]()
  {
    res_kwk_sycl = kwk::transform_reduce(kwk::sycl::default_context, view_in1, view_in2, init_value
      // Reduce function
    , f_reduce
      // Transform function
    , f_transform
    );
  };

  auto fct_kwk_cpu = [&]()
  {
    res_kwk_cpu = kwk::transform_reduce(view_in1, view_in2, init_value
      // Reduce function
    , f_reduce
      // Transform function
    , f_transform
    );
  };

  auto fct_std = [&]()
  {
    res_std = std::transform_reduce ( input1.begin(), input1.end(), input2.begin(), init_value
    // Reduce function
  , f_reduce
    // Transform function
  , f_transform
  );
  };

  auto fct_hand = [&]()
  {
    // auto f_reduce = [](auto r, auto d) { return (r + d); };
    // auto f_transform = [](auto i1, auto i2) { return (i1 * i2); };
    res_hand = init_value;
    for (std::size_t i = 0; i < input_size; ++i)
    {
      res_hand = f_reduce(res_hand, f_transform(input1[i], input2[i]));
    }
  };

  auto b = kwk::bench::create();
  kwk::bench::add(b, fct_kwk_cpu , "Kiwaku CPU transform_reduce");
  kwk::bench::add(b, fct_kwk_sycl, "Kiwaku SYCL transform_reduce");
  kwk::bench::add(b, fct_std     , "std transform_reduce");
  kwk::bench::add(b, fct_hand    , "transform_reduce by hand");
  // b.set_iterations(180);
  kwk::bench::execute(b);

  // TTS_RELATIVE_EQUAL(res, res_std, FLOAT_TOLERANCE_PERCENT);

  TTS_RELATIVE_EQUAL(res_kwk_cpu , res_std, FLOAT_TOLERANCE_PERCENT);
  TTS_RELATIVE_EQUAL(res_kwk_sycl, res_std, FLOAT_TOLERANCE_PERCENT);
  TTS_RELATIVE_EQUAL(res_hand    , res_std, FLOAT_TOLERANCE_PERCENT);
};

// auto f_reduce = [](auto r, auto d) { return (r + d); };
// auto f_transform = [](auto i1, auto i2) { return (i1 * i2); };
// |               ns/op |                op/s |    err% |     total | Benchmark name
// |--------------------:|--------------------:|--------:|----------:|:---------------
// |      156,797,732.00 |                6.38 |    0.5% |      1.78 | `Kiwaku CPU transform_reduce`
// |      209,513,513.00 |                4.77 |    0.5% |      2.58 | `Kiwaku SYCL transform_reduce`
// |      163,933,778.00 |                6.10 |    0.4% |      1.84 | `std transform_reduce`
// |      150,506,950.00 |                6.64 |    0.2% |      1.71 | `transform_reduce by hand`


// auto f_reduce = [](auto a, auto b) { return (((a + b) % 50) % 25) % 30; };
// auto f_transform = [](auto a, auto b) { return (a * b); };
// |               ns/op |                op/s |    err% |     total | Benchmark name
// |--------------------:|--------------------:|--------:|----------:|:---------------
// |    1,485,986,445.00 |                0.67 |    0.0% |     16.36 | `Kiwaku CPU transform_reduce`
// |      216,660,042.00 |                4.62 |    0.7% |      2.78 | `Kiwaku SYCL transform_reduce`
// |      715,096,743.00 |                1.40 |    0.2% |      7.89 | `std transform_reduce`
// |    1,518,207,181.00 |                0.66 |    0.1% |     16.71 | `transform_reduce by hand`


// auto f_reduce = [](auto a, auto b) { return (((((((a + b) % 200) % 150) % 100) % 50) % 25) % 12) % 7; };
// auto f_transform = [](auto a, auto b) { return (a * b); };
// |               ns/op |                op/s |    err% |     total | Benchmark name
// |--------------------:|--------------------:|--------:|----------:|:---------------
// |    5,395,895,455.00 |                0.19 |    0.0% |     59.39 | `Kiwaku CPU transform_reduce`
// |      329,611,457.00 |                3.03 |    2.1% |      4.43 | `Kiwaku SYCL transform_reduce`
// |    3,147,303,205.00 |                0.32 |    0.1% |     34.73 | `std transform_reduce`
// |    5,254,016,249.00 |                0.19 |    0.0% |     57.85 | `transform_reduce by hand`
