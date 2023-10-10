//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::for_each(func, shape)")
{
  auto shp = kwk::of_size(4,kwk::fixed<6>,3ULL);

  int count = 0;
  kwk::for_each( [&](auto...) { count++; }, shp);

  TTS_EQUAL(count, shp.numel());
};

TTS_CASE("Check for kwk::for_each(func, container) 1D")
{
  float data[2];

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  float sum = 0.f;
  int count = 0;
  kwk::for_each( [&](auto e) { count++; sum +=e; }, d);

  TTS_EQUAL(sum, 1.f);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::for_each(func, container) 2D")
{
  float data[2*3];

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  float sum = 0.f;
  int count = 0;
  kwk::for_each( [&](auto e) { count++; sum +=e; }, d);

  TTS_EQUAL(sum, 36.f);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::for_each(func, container) 2D - with CPU context")
{
  float data[2*3];

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  float sum = 0.f;
  int count = 0;
  kwk::for_each(kwk::cpu, [&](auto e) { count++; sum +=e; }, d);

  TTS_EQUAL(sum, 36.f);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::for_each(func, container) 3D")
{
  float data[2*3*4];

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  float sum = 0.f;
  int count = 0;
  kwk::for_each( [&](auto e) { count++; sum +=e; }, d);

  TTS_EQUAL(sum, 1476.f);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::for_each(func, container) 4D")
{
  float data[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  float sum = 0.f;
  int count = 0;
  kwk::for_each( [&](auto e) { count++; sum +=e; }, d);

  TTS_EQUAL(sum, 74040.f);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::for_each_index(func, container) 1D")
{
  int data[2];
  int vdata[2];

  fill_data(vdata, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  kwk::for_each_index( [&](auto& e, auto i0) { count++; e = i0; }, d);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::for_each_index(func, container) 2D")
{
  int data[2*3];
  int vdata[2*3];

  fill_data(vdata, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  kwk::for_each_index( [&](auto& e, auto i1, auto i0) { count++; e = i0 + i1*10; }, d);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::for_each_index(func, container) 3D")
{
  int data[2*3*4];
  int vdata[2*3*4];

  fill_data(vdata, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  kwk::for_each_index( [&](auto& e,  auto i2, auto i1, auto i0) { count++; e = i0 + i1*10 + i2*100; }, d);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::for_each_index(func, container) 4D")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  fill_data(vdata, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  kwk::for_each_index( [&](auto& e, auto i3, auto i2, auto i1, auto i0) { count++; e = i0 + i1*10 + i2*100 + i3*1000; }, d);

  TTS_ALL_EQUAL(data, vdata);
  TTS_EQUAL(count, d.numel());
};
