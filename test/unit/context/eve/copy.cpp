//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

#include "test.hpp"

#if KIWAKU_BUILD_TEST_SIMD

#include <kwk/context/eve/context.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/copy.hpp>
#include <kwk/container.hpp>

TTS_CASE("Check for kwk::copy(out, in) 1D")
{
  int data[2];

  fill_data(data, kwk::of_size(2), true);

  int vdata[2];

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  auto v = kwk::view{kwk::source = vdata, kwk::of_size(2)};
  
  kwk::copy(kwk::simd, v, d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::copy(out, in) 2D")
{
  int data[2*3];

  fill_data(data, kwk::of_size(2,3), true);

  int vdata[2*3];

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  auto v = kwk::view{kwk::source = vdata, kwk::of_size(2,3)};
  
  kwk::copy(kwk::simd,v, d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::copy(out, in) 3D")
{
  int data[2*3*4];

  fill_data(data, kwk::of_size(2,3,4), true);

  int vdata[2*3*4];

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  auto v = kwk::view{kwk::source = vdata, kwk::of_size(2,3,4)};
  
  kwk::copy(kwk::simd,v, d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::copy(out, in) 4D")
{
  int data[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);

  int vdata[2*3*4*5];

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  auto v = kwk::view{kwk::source = vdata, kwk::of_size(2,3,4,5)};
  
  kwk::copy(kwk::simd,v, d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for double kwk::copy(out, in) 2D")
{
  double data[2*3];

  fill_data(data, kwk::of_size(2,3), true);

  double vdata[2*3];

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  auto v = kwk::view{kwk::source = vdata, kwk::of_size(2,3)};
  
  kwk::copy(kwk::simd,v, d);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::copy_if(func, out, in) 1D")
{
  int data[2];

  fill_data(data, kwk::of_size(2), true);

  int rdata[2];
  
  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  auto r = kwk::view{kwk::source = rdata, kwk::of_size(2)};

  kwk::copy_if(kwk::simd,[&](auto e)
  {
    return (e<1);
  }
  ,r, d);

  TTS_EQUAL(rdata[0], 0);
};

TTS_CASE("Check for kwk::copy_if(func, out, in) 2D")
{
  int data[2*3];
  fill_data(data, kwk::of_size(2,3), true);

  int rdata[2*3];
  fill_data(rdata, kwk::of_size(2,3), false);

  int vdata[2*3];
  fill_data(vdata, kwk::of_size(2,3), false);

  vdata[1*3+0] = 10;
  
  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  auto r = kwk::view{kwk::source = rdata, kwk::of_size(2,3)};
  auto v = kwk::view{kwk::source = vdata, kwk::of_size(2,3)}; 

  kwk::copy_if(kwk::simd,[&](auto e)
  {
    return (e==10);
  }
  ,r, d);

  TTS_ALL_EQUAL(rdata, vdata);
};

TTS_CASE("Check for kwk::copy_if(func, out, in) 3D")
{
  int data[2*3*4];
  fill_data(data, kwk::of_size(2,3,4), true);

  int rdata[2*3*4];
  fill_data(rdata, kwk::of_size(2,3,4), false);

  int vdata[2*3*4];
  fill_data(vdata, kwk::of_size(2,3,4), false);

  vdata[1*4*3+0+3] = 103;
  vdata[0+1*4+3] = 13;
  
  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  auto r = kwk::view{kwk::source = rdata, kwk::of_size(2,3,4)};
  
  kwk::copy_if(kwk::simd,[&](auto e)
  {
    return (e==103 || e==13);
  }
  ,r, d);

  TTS_ALL_EQUAL(rdata, vdata);
};


TTS_CASE("Check for kwk::copy_if(func, out, in) 4D")
{
  int data[2*3*4*5];
  fill_data(data, kwk::of_size(2,3,4,5), true);

  int rdata[2*3*4*5];
  fill_data(rdata, kwk::of_size(2,3,4,5), false);

  int vdata[2*3*4*5];
  fill_data(vdata, kwk::of_size(2,3,4,5), false);

  vdata[0+0+0+1] = 1;
  vdata[0+0+0+2] = 2;
  vdata[0+0+0+3] = 3;
  vdata[0+0+0+4] = 4;
  
  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  auto r = kwk::view{kwk::source = rdata, kwk::of_size(2,3,4,5)};
  
  kwk::copy_if(kwk::simd,[&](auto e)
  {
    return (e<10);
  }
  ,r, d);

  TTS_ALL_EQUAL(rdata, vdata);
};

#else

TTS_CASE("EVE disabled, kwk::for_each with EVE context skipped.")
{
  TTS_PASS("EVE disabled, skipping test.");
};

#endif
