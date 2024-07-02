//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

// TODO: update these tests

namespace kwk::test
{

  template<typename Context>
  void reduce_in_1D_larger(Context&& ctx)
  {
    const std::size_t size = 1600000;
    // double* data = new double[size];
    std::vector<double> data(size);

    auto global_fct = [](auto init, auto x) { return init + x; };

    double sum = 0;
    for (std::size_t i = 0; i < size; ++i) { data[i] = i; sum = global_fct(sum, i); }

    auto d = kwk::view{kwk::source = data, kwk::of_size(size)};

    auto res = kwk::reduce(ctx, d, global_fct);

    std::cout << res << "\n";
    // delete[] data;

    TTS_EQUAL(res, sum);
  };

  template<typename Context>
  void reduce_in_1D(Context&& ctx)
  {
    int data[2];
    int vdata = 1;

    fill_data(data, kwk::of_size(2), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

    auto res = kwk::reduce(ctx, d);

    TTS_EQUAL(res, vdata);
  };

  template<typename Context>
  void reduce_in_2D(Context&& ctx)
  {
    int data[2*3];
    int vdata = 36;

    fill_data(data, kwk::of_size(2,3), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    auto res = kwk::reduce(ctx, d);

    TTS_EQUAL(res, vdata);
  };

  template<typename Context>
  void reduce_in_3D(Context&& ctx)
  {
    int data[2*3*4];
    int vdata = 1476;

    fill_data(data, kwk::of_size(2,3,4), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    auto res = kwk::reduce(ctx, d);

    TTS_EQUAL(res, vdata);
  };

  template<typename Context>
  void reduce_in_4D(Context&& ctx)
  {
    int data[2*3*4*5];
    int vdata = 74040;

    fill_data(data, kwk::of_size(2,3,4,5), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    auto res = kwk::reduce(ctx, d);

    TTS_EQUAL(res, vdata);
  };

  template<typename Context>
  void reduce_in_func_1D(Context&& ctx)
  {
    int data[2];
    int vdata = 10;

    fill_data(data, kwk::of_size(2), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    { 
      return (a+10*e);
    });

    TTS_EQUAL(res,   vdata);
  };

  template<typename Context>
  void reduce_in_func_2D(Context&& ctx)
  {
    int data[2*3];
    int vdata = 360;

    fill_data(data, kwk::of_size(2,3), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    { 
      return (a+10*e);
    });

    TTS_EQUAL(res,   vdata);
  };


  template<typename Context>
  void reduce_in_func_3D(Context&& ctx)
  {
    int data[2*3*4];
    int vdata = 14760;

    fill_data(data, kwk::of_size(2,3,4), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    { 
      return (a+10*e);
    });

    TTS_EQUAL(res,   vdata);
  };

  template<typename Context>
  void reduce_in_func_4D(Context&& ctx)
  {
    int data[2*3*4*5];
    int vdata = 740400;

    fill_data(data, kwk::of_size(2,3,4,5), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    { 
      return (a+10*e);
    });

    TTS_EQUAL(res,   vdata);
  };


  template<typename Context>
  void reduce_in_func_float(Context&& ctx)
  {
    float data[2*2]      = { 1.f,2.2f
                            , 3.3f,4.4f
                            };

    float vdata        =  10.9f;

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    { 
      return (a+e);
    });

    TTS_EQUAL(res,   vdata);
  };

  template<typename Context>
  void reduce_in_func_init_1D(Context&& ctx)
  {
    int data[2];
    int vdata = 11;

    fill_data(data, kwk::of_size(2), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    {
      return (a+e);
    }, 10);

    TTS_EQUAL(res,   vdata);
  };

  template<typename Context>
  void reduce_in_func_init_2D(Context&& ctx)
  {
    int data[2*3];
    int vdata = 136;

    fill_data(data, kwk::of_size(2,3), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    {
      return (a+e);
    }, 100);

    TTS_EQUAL(res,   vdata);
  };

  template<typename Context>
  void reduce_in_func_init_3D(Context&& ctx)
  {
    int data[2*3*4];
    int vdata = 2476;

    fill_data(data, kwk::of_size(2,3,4), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    {
      return (a+e);
    }, 1000);

    TTS_EQUAL(res,   vdata);
  };

  template<typename Context>
  void reduce_in_func_init_4D(Context&& ctx)
  {
    int data[2*3*4*5];
    int vdata = 84040;

    fill_data(data, kwk::of_size(2,3,4,5), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    auto res = reduce(ctx, d,
    [](auto a, auto e)
    {
      return (a+e);
    }, 10000);

    TTS_EQUAL(res,   vdata);
  };

}