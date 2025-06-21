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
#include <benchmark/benchmark.hpp>

TTS_CASE("Check for REDUCE SMALL TEST 1D")
{
  using data_type = int;
  const std::size_t d0 = 10;
  const std::size_t input_size = d0;
  std::vector<data_type> input;
  input.resize(input_size);

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = 1; }

  auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(d0)};

  data_type res_kwk_cpu;
  data_type res_kwk_sycl;

  res_kwk_sycl = kwk::reduce(kwk::sycl::default_context, view_in);
  res_kwk_cpu  = kwk::reduce(view_in);

  std::cout << "REDUCE SYCL = " << res_kwk_sycl << "\n";
  std::cout << "REDUCE CPU  = " << res_kwk_cpu  << "\n";


  // TTS_RELATIVE_EQUAL(res, res_std, FLOAT_TOLERANCE_PERCENT);
  TTS_EQUAL(res_kwk_cpu, res_kwk_sycl);
  // TTS_EQUAL(res_kwk_sycl, res_std);
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