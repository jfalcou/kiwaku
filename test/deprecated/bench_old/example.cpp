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

// #define ANKERL_NANOBENCH_IMPLEMENT
// #include <benchmark/nanobench.h>
#include <benchmark/benchmark.hpp>
#include <vector>
#include <iostream>



TTS_CASE("Check for kwk::for_each2(func, shape)")
{
  auto shp = kwk::of_size(4, kwk::fixed<6>, 3ULL);
  std::int32_t count = 0;
  kwk::for_each( [&](auto...) { ++count; }, shp);
  TTS_EQUAL(count, shp.numel());

  double d = 1.0;
  std::size_t repeated = 0;

  auto fct = [&]
  {
    d += 1.0 / d;
    if (d > 5.0)
    {
      d -= 5.0;
      // usleep(10000);
    }
    ++repeated;

    ankerl::nanobench::doNotOptimizeAway(d);
  };

  auto b = kwk::bench::create();
  kwk::bench::add(b, fct, "My first function!");
  kwk::bench::execute(b);

};

TTS_CASE("Check for kwk::for_each2(func, shape)")
{
  auto shp = kwk::of_size(4, kwk::fixed<6>, 3ULL);
  std::int32_t count = 0;
  kwk::for_each( [&](auto...) { ++count; }, shp);
  TTS_EQUAL(count, shp.numel());

  double d = 1.0;
  std::size_t repeated = 0;

  auto fct = [&]
  {
    d += 1.0 / d;
    if (d > 5.0)
    {
      d -= 5.0;
      // usleep(10000);
    }
    ++repeated;

    ankerl::nanobench::doNotOptimizeAway(d);
  };

  auto b = kwk::bench::create();
  kwk::bench::add(b, fct, "My first function!");
  kwk::bench::execute(b);

};