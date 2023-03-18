//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/for_each.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::for_each(func, shape)")
{
  auto shp = kwk::of_size(4,kwk::fixed<6>,3ULL);

  int count = 0;
  kwk::for_each( [&](auto...) { count++; }, shp);

  TTS_EQUAL(count, shp.numel());
};

TTS_CASE("Check for kwk::for_each(func, container)")
{
  float data[3][5]  = { 1,2,3,4,5
                      , 10,20,30,40,50
                      , 100,200,300,400,500
                      };

  auto v = kwk::view{kwk::source = data};

  float sum = 0.f;
  kwk::for_each( [&](auto e) { sum +=e; }, v);

  TTS_EQUAL(sum, 1665.f);
};

TTS_CASE("Check for kwk::for_each_index(func, container)")
{
  int data[2][2][2]  = {};

  auto v = kwk::view{kwk::source = data};

  kwk::for_each_index( [&](auto& e, auto i2, auto i1, auto i0) { e = i0 + 10*i1 + 100*i2; }, v);

  TTS_EQUAL(data[0][0][0],   0);
  TTS_EQUAL(data[0][0][1],   1);
  TTS_EQUAL(data[0][1][0],  10);
  TTS_EQUAL(data[0][1][1],  11);
  TTS_EQUAL(data[1][0][0], 100);
  TTS_EQUAL(data[1][0][1], 101);
  TTS_EQUAL(data[1][1][0], 110);
  TTS_EQUAL(data[1][1][1], 111);
};
