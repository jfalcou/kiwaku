//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/context/sycl/context.hpp>
#include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <optional>
#include <benchmark/benchmark_nano.hpp>
#include <cmath>





TTS_CASE("Check for kwk::find_if(kwk::sycl::default_context, In, func) 1D")
{

  // Array of size 20
  const std::size_t input_size = 1024 * 1024 * 256 * 4;
  std::vector<int> input;
  input.resize(input_size);
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 79865 + 871 ; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  
  int initial_value = 45121;

  // auto func = [&](auto item) { return (item % initial_value) == 0; };

  // auto convert_func = [=](auto item)
  // {
  //   return
  //   static_cast<int>(std::cos(std::sin(std::cos(std::sin(static_cast<float>(item * 11) * 7.) * 3.8) * 8.7) * 9.48) * 89647681.) % 1784586545;
  // };

  auto convert_func = [=](auto item)
  {
    return item;
  };

  int image_value = convert_func(initial_value);

  auto compare_func = [=](auto item)
  {
    return (convert_func(item) == image_value);
  };

  // std::cout << "Looking for value: " << image_value << "\n";
  // std::cout << "Value at pos 10: " << compare_func(input[10]) << "\n";
  // for (std::size_t i = 0; i < 200; ++i) { std::cout << convert_func(input[i]) << " "; }
  // std::cout << "\n";

  int put_at_pos = input_size-6;

  input[put_at_pos] = initial_value;

  {
    auto res = kwk::find_if(kwk::sycl::default_context, view, compare_func);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{put_at_pos});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }


  int res_kwk_sycl, res_kwk_cpu, res_hand, res_std;

  auto fct_kwk_sycl = [&]()
  {
    res_kwk_sycl = kwk::utils::tools::pos_to_linear(view, kwk::find_if(kwk::sycl::default_context, view, compare_func));
  };

  auto fct_kwk_cpu = [&]()
  {
    res_kwk_cpu = kwk::utils::tools::pos_to_linear(view, kwk::find_if(view, compare_func));
  };

  auto fct_std = [&]()
  {
    auto res = std::find_if(input.begin(), input.end(), compare_func);
    if (res != input.end()) res_std = std::distance(input.begin(), res);
    else                    res_std = -1;
  };

  auto fct_hand = [&]()
  {
    res_hand = -1;
    for (std::size_t i = 0; i < input_size; ++i)
    {
      if (compare_func(input[i]))
      {
        res_hand = i;
        break;
      }
    }
  };

  // TODO:   reduce à la main pour voir le temps pris
  //       + reduce de la manière la plus vectorisée possible ?
  //         (faire en sorte que le compilateur l'auto-vectorise de lui-même)
  //         (et/ou tester avec EVE)

  auto b = kwk::bench::create();
  kwk::bench::add(b, fct_kwk_cpu , "Kiwaku CPU find_if");
  kwk::bench::add(b, fct_kwk_sycl, "Kiwaku SYCL find_if");
  kwk::bench::add(b, fct_std     , "std find_if");
  kwk::bench::add(b, fct_hand    , "find_if by hand");
  // b.set_iterations(180);
  kwk::bench::execute(b);

  // TTS_RELATIVE_EQUAL(res, res_std, FLOAT_TOLERANCE_PERCENT);
  TTS_EQUAL(res_kwk_cpu, res_hand);
  TTS_EQUAL(res_kwk_sycl, res_hand);
  TTS_EQUAL(res_std     , res_hand);

  std::cout << "res_hand = " << res_hand << "\n";
  // TTS_EQUAL(res_hand, res_hand);
};

// Compute-bound
// const std::size_t input_size = 1024 * 1024 * 16;
// auto convert_func
//   static_cast<int>(std::cos(std::sin(std::cos(std::sin(static_cast<float>(item * 11) * 7.) * 3.8) * 8.7) * 9.48) * 89647681.) % 1784586545;
// |               ns/op |                op/s |    err% |     total | Benchmark name
// |--------------------:|--------------------:|--------:|----------:|:---------------
// |    1,321,597,312.00 |                0.76 |    0.1% |     14.58 | `Kiwaku CPU find_if`
// |       61,177,199.00 |               16.35 |    0.8% |      0.72 | `Kiwaku SYCL find_if`
// |    1,319,917,503.00 |                0.76 |    0.2% |     14.51 | `std find_if`
// |    1,324,743,301.00 |                0.75 |    0.1% |     14.58 | `find_if by hand`

// I/O bound
// const std::size_t input_size = 1024 * 1024 * 1024;
// convert_func = [=](auto item) { return item; }
// |               ns/op |                op/s |    err% |     total | Benchmark name
// |--------------------:|--------------------:|--------:|----------:|:---------------
// |      221,857,205.00 |                4.51 |    0.2% |      2.48 | `Kiwaku CPU find_if`
// |      899,922,403.00 |                1.11 |    0.1% |      9.97 | `Kiwaku SYCL find_if`
// |      184,391,744.00 |                5.42 |    0.4% |      2.07 | `std find_if`
// |      184,374,010.00 |                5.42 |    0.5% |      2.08 | `find_if by hand`


