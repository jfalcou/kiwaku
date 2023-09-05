//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/predicates.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::all_of(in, func) 1D")
{
  int data[2];
  bool vdata = true;

  fill_data(data, kwk::of_size(2), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  auto res = kwk::all_of(d, [&](auto e)
  {
    count++;
    return (e == 0);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::all_of(in, func) 2D")
{
  int data[2*3];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = kwk::all_of(d, [&](auto e)
  {
    count++;
    return (e == 0);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::all_of(in, func) 3D")
{
  int data[2*3*4];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = kwk::all_of(d, [&](auto e)
  {
    count++;
    return (e == 0);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::all_of(in, func) 4D")
{
  int data[2*3*4*5];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4,5), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  auto res = kwk::all_of(d, [&](auto e)
  {
    count++;
    return (e == 0);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::all_of(in, func) False")
{
  int data[2*2]      = { 2,2
                        , 2,3
                        };

  bool vdata        =  false;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

  int count = 0;
  auto res = kwk::all_of(d, [&](auto e)
  {
    count++;
    return (e == 2);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};


TTS_CASE("Check for kwk::any_of(in, func) 1D")
{
  int data[2];
  bool vdata = true;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  auto res = kwk::any_of(d, [&](auto e)
  {
    count++;
    return (e == 1);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::any_of(in, func) 2D")
{
  int data[2*3];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = kwk::any_of(d, [&](auto e)
  {
    count++;
    return (e == 12);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::any_of(in, func) 3D")
{
  int data[2*3*4];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = kwk::any_of(d, [&](auto e)
  {
    count++;
    return (e == 123);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::any_of(in, func) 4D")
{
  int data[2*3*4*5];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  auto res = kwk::any_of(d, [&](auto e)
  {
    count++;
    return (e == 1234);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::any_of(in, func) False")
{
  int data[2*2]      = { 2,2
                        , 2,2
                        };

  bool vdata        =  false;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

  int count = 0;
  auto res = kwk::any_of(d, [&](auto e)
  {
    count++;
    return (e == 3);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::none_of(in, func) 1D")
{
  int data[2];
  bool vdata = true;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  auto res = kwk::none_of(d, [&](auto e)
  {
    count++;
    return (e == 2);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::none_of(in, func) 2D")
{
  int data[2*3];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = kwk::none_of(d, [&](auto e)
  {
    count++;
    return (e == 23);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::none_of(in, func) 3D")
{
  int data[2*3*4];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = kwk::none_of(d, [&](auto e)
  {
    count++;
    return (e == 234);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::none_of(in, func) 4D")
{
  int data[2*3*4*5];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  auto res = kwk::none_of(d, [&](auto e)
  {
    count++;
    return (e == 2345);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::none_of(in, func) False")
{
  int data[2*2]      = { 3,2
                        , 2,2
                        };

  bool vdata        =  false;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

  int count = 0;
  auto res = kwk::none_of(d, [&](auto e)
  {
    count++;
    return (e == 3);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count, 1);
};

TTS_CASE("Check for kwk::count(in, value) 1D")
{
  int data[2];

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::count(d, 1);

  TTS_EQUAL(res,   1UL);
};

TTS_CASE("Check for kwk::count(in, value) 2D")
{
  int data[2*3];

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::count(d, 12);

  TTS_EQUAL(res,   1UL);
};

TTS_CASE("Check for kwk::count(in, value) 3D")
{
  int data[2*3*4];

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::count(d, 123);

  TTS_EQUAL(res,   1UL);
};

TTS_CASE("Check for kwk::count(in, value) 4D")
{
  int data[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::count(d, 1234);

  TTS_EQUAL(res,   1UL);
};

TTS_CASE("Check for kwk::count_if(in, func) 1D")
{
  int data[2];

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  auto res = kwk::count_if(d, [&](auto e)
  {
    count++;
    return (e >= 0);
  });

  TTS_EQUAL(res,   2UL);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::count_if(in, func) 2D")
{
  int data[2*3];

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = kwk::count_if(d, [&](auto e)
  {
    count++;
    return (e >= 10);
  });

  TTS_EQUAL(res,   3UL);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::count_if(in, func) 3D")
{
  int data[2*3*4];

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = kwk::count_if(d, [&](auto e)
  {
    count++;
    return (e >= 120);
  });

  TTS_EQUAL(res,   4UL);
  TTS_EQUAL(count, d.numel());
};

TTS_CASE("Check for kwk::count_if(in, func) 4D")
{
  int data[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  auto res = kwk::count_if(d, [&](auto e)
  {
    count++;
    return (e >= 1230);
  });

  TTS_EQUAL(res,   5UL);
  TTS_EQUAL(count, d.numel());
};