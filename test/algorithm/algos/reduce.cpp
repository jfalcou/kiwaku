//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::reduce(in) 1D")
{
  int data[2];
  int vdata = 1;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::reduce(d);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::reduce(in) 1D - larger")
{
  const std::size_t size = 1600000;
  double data[size];

  auto global_fct = [](auto init, auto x) { return init + x; };

  double sum = 0;
  for (std::size_t i = 0; i < size; ++i) { data[i] = i; sum = global_fct(sum, i); }

  auto d = kwk::view{kwk::source = data, kwk::of_size(size)};

  auto res = kwk::reduce(d, global_fct);

  std::cout << res << "\n";

  TTS_EQUAL(res, sum);
};

TTS_CASE("Check for kwk::reduce(in) 2D")
{
  int data[2*3];
  int vdata = 36;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::reduce(d);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::reduce(in) 2D - with CPU context")
{
  int data[2*3];
  int vdata = 36;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::reduce(kwk::cpu, d);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::reduce(in) 3D")
{
  int data[2*3*4];
  int vdata = 1476;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::reduce(d);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::reduce(in) 4D")
{
  int data[2*3*4*5];
  int vdata = 74040;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::reduce(d);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::reduce(in, func) 1D")
{
  int data[2];
  int vdata = 10;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+10*e);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func) 2D")
{
  int data[2*3];
  int vdata = 360;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+10*e);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func) 2D - with CPU context")
{
  int data[2*3];
  int vdata = 360;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = reduce(kwk::cpu, d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+10*e);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func) 3D")
{
  int data[2*3*4];
  int vdata = 14760;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+10*e);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func) 4D")
{
  int data[2*3*4*5];
  int vdata = 740400;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+10*e);
  });

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for float kwk::reduce(in, func)")
{
  float data[2*2]      = { 1.f,2.2f
                          , 3.3f,4.4f
                          };

  float vdata        =  10.9f;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

  
  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+e);
  });

  TTS_EQUAL(res,   vdata);

  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for float kwk::reduce(in, func) - with CPU context")
{
  float data[2*2]      = { 1.f,2.2f
                          , 3.3f,4.4f
                          };

  float vdata        =  10.9f;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

  
  int count = 0;
  auto res = reduce(kwk::cpu, d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+e);
  });

  TTS_EQUAL(res,   vdata);

  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func, init) 1D")
{
  int data[2];
  int vdata = 11;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+e);
  }, 10);

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func, init) 2D")
{
  int data[2*3];
  int vdata = 136;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+e);
  }, 100);

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func, init) 3D")
{
  int data[2*3*4];
  int vdata = 2476;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+e);
  }, 1000);

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func, init) 3D - with CPU context")
{
  int data[2*3*4];
  int vdata = 2476;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  int count = 0;
  auto res = reduce(kwk::cpu, d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+e);
  }, 1000);

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::reduce(in, func, init) 4D")
{
  int data[2*3*4*5];
  int vdata = 84040;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  int count = 0;
  auto res = reduce(d,
  [&count](auto a, auto e)
  { 
    count++;
    return (a+e);
  }, 10000);

  TTS_EQUAL(res,   vdata);
  TTS_EQUAL(count,   d.numel());
};