//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::transform(value, new_value) 1D")
{
  int data[2];
  double res[2];
  double vdata[2] = {1, 0.5};
  
  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  auto v = kwk::view{kwk::source = res, kwk::of_size(2)};
  
  int count = 0;
  transform(
  [&](auto e)
  { 
    count++;
    return 1.0/(1.0+e);
  }, v, d);

  TTS_ALL_EQUAL(res, vdata);

  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::transform(value, new_value) 2D")
{
  int data[2*3];
  double res[2*3];
  double vdata[2*3];
  
  fill_data(data, kwk::of_size(2,3), true);
  fill_data(vdata, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  auto v = kwk::view{kwk::source = res, kwk::of_size(2,3)};

  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      vdata[i*3+j] = 1.0/(1.0+vdata[i*3+j]);
  
  int count = 0;
  transform(
  [&](auto e)
  { 
    count++;
    return 1.0/(1.0+e);
  }, v, d);

  TTS_ALL_EQUAL(res, vdata);

  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::transform(value, new_value) 2D - with contexts")
{
  int data[2*3];
  double res[2*3];
  double vdata[2*3];
  
  fill_data(data, kwk::of_size(2,3), true);
  fill_data(vdata, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};
  auto v = kwk::view{kwk::source = res, kwk::of_size(2,3)};

  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      vdata[i*3+j] = 1.0/(1.0+vdata[i*3+j]);
  
  int count = 0;
  transform( kwk::cpu, 
  [&](auto e)
  { 
    count++;
    return 1.0/(1.0+e);
  }, v, d);

  TTS_ALL_EQUAL(res, vdata);

  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::transform(value, new_value) 3D")
{
  int data[2*3*4];
  double res[2*3*4];
  double vdata[2*3*4];
  
  fill_data(data, kwk::of_size(2,3,4), true);
  fill_data(vdata, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
  auto v = kwk::view{kwk::source = res, kwk::of_size(2,3,4)};

  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
        vdata[i*4*3+j*4+k] = 1.0/(1.0+vdata[i*4*3+j*4+k]);
  
  int count = 0;
  transform(
  [&](auto e)
  { 
    count++;
    return 1.0/(1.0+e);
  }, v, d);

  TTS_ALL_EQUAL(res, vdata);

  TTS_EQUAL(count,   d.numel());
};

TTS_CASE("Check for kwk::transform(value, new_value) 4D")
{
  int data[2*3*4*5];
  double res[2*3*4*5];
  double vdata[2*3*4*5];
  
  fill_data(data, kwk::of_size(2,3,4,5), true);
  fill_data(vdata, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};
  auto v = kwk::view{kwk::source = res, kwk::of_size(2,3,4,5)};

  for(int i = 0; i<2; i++)
    for(int j = 0; j<3; j++)
      for(int k = 0; k<4; k++)
        for(int l = 0; l<5; l++)
          vdata[i*5*4*3+j*5*4+k*5+l] = 1.0/(1.0+vdata[i*5*4*3+j*5*4+k*5+l]);
  
  int count = 0;
  transform(
  [&](auto e)
  { 
    count++;
    return 1.0/(1.0+e);
  }, v, d);

  TTS_ALL_EQUAL(res, vdata);

  TTS_EQUAL(count,   d.numel());
};