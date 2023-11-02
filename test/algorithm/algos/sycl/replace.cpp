//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/replace.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

#include <kwk/context/sycl/context.hpp>

TTS_CASE("Check for kwk::replace(value, new_value) 2D - with SYCL context")
{
  int data[2*3];
  int vdata[2*3];

  fill_data(data, kwk::of_size(2,3), true);
  fill_data(vdata, kwk::of_size(2,3), true);

  vdata[1*3+2] = 120;

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  kwk::replace(kwk::sycl::sycl_context, v, 12, 120);

  TTS_ALL_EQUAL(data, vdata);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 2D - with SYCL context")
{
  int data[2*3];
  int vdata[2*3];

  fill_data(data, kwk::of_size(2,3), true);
  fill_data(vdata, kwk::of_size(2,3), false);

  auto v = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  // int count = 0;
  kwk::replace_if(kwk::sycl::sycl_context, v, [](auto e)
  { 
    // count++;
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
  // TTS_EQUAL(count,   v.numel());
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 4D smaller view - with SYCL context")
{
  int data[2*3*4*5];
  int vdata[2*3*4*5];

  fill_data(data, kwk::of_size(2,3,4,5), true);
  fill_data(vdata, kwk::of_size(2,3,4,5), false);

  for(int j = 0; j<3; j++)
    for(int k = 0; k<4; k++)
      for(int l = 0; l<5; l++)
        vdata[1*5*4*3+j*5*4+k*5+l] = 1000+100*j+10*k+l;

  auto v = kwk::view{kwk::source = data, kwk::of_size(1,3,4,5)};

  kwk::replace_if(kwk::sycl::sycl_context, v, [](auto e)
  { 
    return (e>=0);
  } 
  , 0);

  TTS_ALL_EQUAL(data, vdata);
};