//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/sycl/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <benchmark/benchmark_nano.hpp>

TTS_CASE("Check for kwk::reduce(in) 1D")
{
  using data_type = int;
  const std::size_t d0 = 1024 * 1024 * 256;
  const std::size_t input_size = d0;
  std::vector<data_type> input;
  input.resize(input_size);

  // std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  std::cout << "STD array ok\n";

  auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(d0)};

  data_type res_kwk_cpu, res_kwk_sycl, res_std, res_hand;

  auto func = [](auto a, auto b) { return (((a + b) % 50) % 25) % 30; };
  // auto func = [](auto a, auto b) { return (a + b) % 50; };

// auto func = [](auto a, auto b) { return a + b; };
// |               ns/op |                op/s |    err% |     total | Benchmark name
// |--------------------:|--------------------:|--------:|----------:|:---------------
// |       71,837,034.00 |               13.92 |    0.7% |      0.86 | `Kiwaku CPU reduce`
// |      104,814,624.00 |                9.54 |    0.1% |      1.42 | `Kiwaku SYCL reduce`
// |       81,333,063.00 |               12.30 |    0.9% |      0.94 | `std reduce`

// auto func = [](auto a, auto b) { return (a + b) % 50; };
// |               ns/op |                op/s |    err% |     total | Benchmark name
// |--------------------:|--------------------:|--------:|----------:|:---------------
// |      767,498,099.00 |                1.30 |    0.0% |      8.45 | `Kiwaku CPU reduce`
// |      126,097,508.00 |                7.93 |    0.5% |      1.66 | `Kiwaku SYCL reduce`
// |      320,147,871.00 |                3.12 |    0.1% |      3.57 | `std reduce`

// auto func = [](auto a, auto b) { return (((a + b) % 50) % 25) % 30; };
// |               ns/op |                op/s |    err% |     total | Benchmark name
// |--------------------:|--------------------:|--------:|----------:|:---------------
// |    1,514,426,949.00 |                0.66 |    0.0% |     16.66 | `Kiwaku CPU reduce`
// |      171,731,833.00 |                5.82 |    0.2% |      2.20 | `Kiwaku SYCL reduce`
// |      624,873,332.00 |                1.60 |    0.1% |      6.89 | `std reduce`
// |    1,456,639,954.00 |                0.69 |    0.1% |     16.02 | `reduce by hand`

  auto fct_kwk_sycl = [&res_kwk_sycl, &view_in, func]()
  {
    res_kwk_sycl = kwk::reduce(kwk::sycl::default_context, view_in, func, 0);
  };

  auto fct_kwk_cpu = [&]()
  {
    res_kwk_cpu = kwk::reduce(view_in, func, 0);
  };

  auto fct_std = [&]()
  {
    res_std = std::reduce(input.begin(), input.end(), 0, func);
  };

  auto fct_hand = [&]()
  {
    res_hand = 0;
    for (std::size_t i = 0; i < input_size; ++i)
    {
      res_hand = func(res_hand, input[i]);
    }
  };

  // TODO:   reduce à la main pour voir le temps pris
  //       + reduce de la manière la plus vectorisée possible ?
  //         (faire en sorte que le compilateur l'auto-vectorise de lui-même)
  //         (et/ou tester avec EVE)

  auto b = kwk::bench::create();
  kwk::bench::add(b, fct_kwk_cpu , "Kiwaku CPU reduce");
  kwk::bench::add(b, fct_kwk_sycl, "Kiwaku SYCL reduce");
  kwk::bench::add(b, fct_std     , "std reduce");
  kwk::bench::add(b, fct_hand    , "reduce by hand");
  // b.set_iterations(180);
  kwk::bench::execute(b);

  // TTS_RELATIVE_EQUAL(res, res_std, FLOAT_TOLERANCE_PERCENT);
  TTS_EQUAL(res_kwk_cpu, res_std);
  TTS_EQUAL(res_kwk_sycl, res_std);
  TTS_EQUAL(res_hand, res_std);
};


// TTS_CASE("Check for kwk::reduce(in, func) and kwk::reduce(in, func, init) 1D")
// {
//   using data_type = int;
//   const std::size_t d0 = 471;
//   const std::size_t input_size = d0;
//   std::array<data_type, input_size> input;

//   for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

//   auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

//   std::size_t count = 0;
//   auto res = kwk::reduce(view_in, [&](auto e1, auto e2) { ++count; return e1 + e2 + 1; });
//   auto res2 = kwk::reduce(view_in, [&](auto e1, auto e2) { return e1 + e2 + 1; }, 87);
//   auto res_std = std::reduce(input.begin(), input.end(), 0, [&](auto e1, auto e2) { return e1 + e2 + 1; });

//   // TTS_RELATIVE_EQUAL(res, res_std, FLOAT_TOLERANCE_PERCENT);
//   TTS_EQUAL(res, res_std);
//   TTS_EQUAL(static_cast<decltype(view_in)::value_type>(res2), res_std + 87);
//   TTS_EQUAL(count, input_size);
// };


// TTS_CASE("Check for REDUCE")
// {
//   auto shp = kwk::of_size(4, kwk::fixed<6>, 3ULL);
//   std::int32_t count = 0;
//   kwk::for_each( [&](auto...) { ++count; }, shp);
//   TTS_EQUAL(count, shp.numel());

//   double d = 1.0;
//   std::size_t repeated = 0;

//   auto fct = [&]
//   {
//     d += 1.0 / d;
//     if (d > 5.0)
//     {
//       d -= 5.0;
//       // usleep(10000);
//     }
//     ++repeated;

//     ankerl::nanobench::doNotOptimizeAway(d);
//   };

//   auto b = kwk::bench::create();
//   kwk::bench::add(b, fct, "My first function!");
//   kwk::bench::execute(b);

// };