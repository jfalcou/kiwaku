//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

namespace kwk::test
{
  template<typename Context>
  void transform_value_new_value_1D(Context&& ctx)
  {
    int data[2];
    double res[2];
    double vdata[2] = {1, 0.5};

    fill_data(data, kwk::of_size(2), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
    auto v = kwk::view{kwk::source = res, kwk::of_size(2)};

    ::kwk::transform(ctx, [&](auto e) { return 1.0/(1.0+e); }, v, d);

    TTS_ALL_EQUAL(res, vdata);
  }

  template<typename Context>
  void transform_value_new_value_2D(Context&& ctx)
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

    ::kwk::transform(ctx, [&](auto e) { return 1.0/(1.0+e); }, v, d);

    TTS_ALL_EQUAL(res, vdata);
  };

  template<typename Context>
  void transform_value_new_value_3D(Context&& ctx)
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

    ::kwk::transform(ctx, [&](auto e) {  return 1.0/(1.0+e);  }, v, d);

    TTS_ALL_EQUAL(res, vdata);
  };

  template<typename Context>
  void transform_value_new_value_4D(Context&& ctx)
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

    ::kwk::transform(ctx, [&](auto e) {  return 1.0/(1.0+e);  }, v, d);

    TTS_ALL_EQUAL(res, vdata);
  };

}