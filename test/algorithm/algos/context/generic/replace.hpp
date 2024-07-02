//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

// #include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/replace.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

// TODO: update these tests


namespace kwk::test
{
  template<typename Context>
  void replace_value_new_value_1D(Context&& ctx)
  {
    int data[2];
    int vdata[2];

    fill_data(data, kwk::of_size(2), true);
    fill_data(vdata, kwk::of_size(2), true);

    vdata[1] = 10;

    auto v = kwk::view{kwk::source = data, kwk::of_size(2)};

    ::kwk::replace(ctx, v, 1, 10);

    TTS_ALL_EQUAL(data, vdata);
  };

  template<typename Context>
  void replace_value_new_value_2D(Context&& ctx)
  {
    int data[2*3];
    int vdata[2*3];

    fill_data(data, kwk::of_size(2,3), true);
    fill_data(vdata, kwk::of_size(2,3), true);

    vdata[1*3+2] = 120;

    auto v = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    ::kwk::replace(ctx, v, 12, 120);

    TTS_ALL_EQUAL(data, vdata);
  };

  template<typename Context>
  void replace_value_new_value_3D(Context&& ctx)
  {
    int data[2*3*4];
    int vdata[2*3*4];

    fill_data(data, kwk::of_size(2,3,4), true);
    fill_data(vdata, kwk::of_size(2,3,4), true);

    vdata[1*4*3+2*4+3] = 1230;

    auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    ::kwk::replace(ctx, v, 123, 1230);

    TTS_ALL_EQUAL(data, vdata);
  };

  template<typename Context>
  void replace_value_new_value_4D(Context&& ctx)
  {
    int data[2*3*4*5];
    int vdata[2*3*4*5];

    fill_data(data, kwk::of_size(2,3,4,5), true);
    fill_data(vdata, kwk::of_size(2,3,4,5), true);

    vdata[1*5*4*3+2*5*4+3*5+4] = 12340;

    auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    ::kwk::replace(ctx, v, 1234, 12340);

    TTS_ALL_EQUAL(data, vdata);
  };

  template<typename Context>
  void replace_if_func_new_value_1D(Context&& ctx)
  {
    int data[2];
    int vdata[2];

    fill_data(data, kwk::of_size(2), true);
    fill_data(vdata, kwk::of_size(2), false);

    auto v = kwk::view{kwk::source = data, kwk::of_size(2)};

    ::kwk::replace_if(ctx, v, [&](auto e) { return (e>=0); }, 0);

    TTS_ALL_EQUAL(data, vdata);
  };

  template<typename Context>
  void replace_if_func_new_value_2D(Context&& ctx)
  {
    int data[2*3];
    int vdata[2*3];

    fill_data(data, kwk::of_size(2,3), true);
    fill_data(vdata, kwk::of_size(2,3), false);

    auto v = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    ::kwk::replace_if(ctx, v, [&](auto e) { return (e>=0); }, 0);

    TTS_ALL_EQUAL(data, vdata);
  };

  template<typename Context>
  void replace_if_func_new_value_3D(Context&& ctx)
  {
    int data[2*3*4];
    int vdata[2*3*4];

    fill_data(data, kwk::of_size(2,3,4), true);
    fill_data(vdata, kwk::of_size(2,3,4), false);

    auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    ::kwk::replace_if(ctx, v, [&](auto e) { return (e>=0); }, 0);

    TTS_ALL_EQUAL(data, vdata);
  };

  template<typename Context>
  void replace_if_func_new_value_4D(Context&& ctx)
  {
    int data[2*3*4*5];
    int vdata[2*3*4*5];

    fill_data(data, kwk::of_size(2,3,4,5), true);
    fill_data(vdata, kwk::of_size(2,3,4,5), false);

    auto v = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    ::kwk::replace_if(ctx, v, [&](auto e) { return (e>=0); }, 0);

    TTS_ALL_EQUAL(data, vdata);
  };

  template<typename Context>
  void replace_if_func_new_value_4D_smaller_view(Context&& ctx)
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

    ::kwk::replace_if(ctx, v, [&](auto e) { return (e>=0); }, 0);

    TTS_ALL_EQUAL(data, vdata);
  };
}