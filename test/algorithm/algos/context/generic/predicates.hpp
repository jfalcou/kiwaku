//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/predicates.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

namespace kwk::test
{
  // void reduce_in_1D_larger(Context&& ctx)
  template<typename Context>
  void all_of_in_func_1D(Context&& ctx)
  {
    int data[2];
    bool vdata = true;

    fill_data(data, kwk::of_size(2), false);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
    auto res = kwk::all_of(ctx, d, [&](auto e)
    {
      return (e == 0);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::all_of(in, func) 2D")
  template<typename Context>
  void all_of_in_func_2D(Context&& ctx)
  {
    int data[2*3];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3), false);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    auto res = kwk::all_of(ctx, d, [&](auto e)
    {
      return (e == 0);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::all_of(in, func) 3D")
  template<typename Context>
  void all_of_in_func_3D(Context&& ctx)
  {
    int data[2*3*4];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3,4), false);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    auto res = kwk::all_of(ctx, d, [&](auto e)
    {
      return (e == 0);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::all_of(in, func) 4D")
  template<typename Context>
  void all_of_in_func_4D(Context&& ctx)
  {
    int data[2*3*4*5];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3,4,5), false);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    auto res = kwk::all_of(ctx, d, [&](auto e)
    {
      return (e == 0);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::all_of(in, func) False")
  template<typename Context>
  void all_of_in_func_false(Context&& ctx)
  {
    int data[2*2]      = { 2,2
                          , 2,3
                          };

    bool vdata        =  false;

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

    auto res = kwk::all_of(ctx, d, [&](auto e)
    {
      return (e == 2);
    });

    TTS_EQUAL(res,   vdata);
  };


  // TTS_CASE("Check for kwk::any_of(in, func) 1D")
  template<typename Context>
  void any_of_in_func_1D(Context&& ctx)
  {
    int data[2];
    bool vdata = true;

    fill_data(data, kwk::of_size(2), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

    auto res = kwk::any_of(ctx, d, [&](auto e)
    {
      return (e == 1);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::any_of(in, func) 2D")
  template<typename Context>
  void any_of_in_func_2D(Context&& ctx)
  {
    int data[2*3];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    auto res = kwk::any_of(ctx, d, [&](auto e)
    {
      return (e == 12);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::any_of(in, func) 3D")
  template<typename Context>
  void any_of_in_func_3D(Context&& ctx)
  {
    int data[2*3*4];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3,4), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    auto res = kwk::any_of(ctx, d, [&](auto e)
    {
      return (e == 123);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::any_of(in, func) 4D")
  template<typename Context>
  void any_of_in_func_4D(Context&& ctx)
  {
    int data[2*3*4*5];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3,4,5), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    auto res = kwk::any_of(ctx, d, [&](auto e)
    {
      return (e == 1234);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::any_of(in, func) False")
  template<typename Context>
  void any_of_in_func_false(Context&& ctx)
  {
    int data[2*2]      = { 2,2
                          , 2,2
                          };

    bool vdata        =  false;

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

    auto res = kwk::any_of(ctx, d, [&](auto e)
    {
      return (e == 3);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::none_of(in, func) 1D")
  template<typename Context>
  void none_of_in_func_1D(Context&& ctx)
  {
    int data[2];
    bool vdata = true;

    fill_data(data, kwk::of_size(2), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

    auto res = kwk::none_of(ctx, d, [&](auto e)
    {
      return (e == 2);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::none_of(in, func) 2D")
  template<typename Context>
  void none_of_in_func_2D(Context&& ctx)
  {
    int data[2*3];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    auto res = kwk::none_of(ctx, d, [&](auto e)
    {
      return (e == 23);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::none_of(in, func) 3D")
  template<typename Context>
  void none_of_in_func_3D(Context&& ctx)
  {
    int data[2*3*4];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3,4), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    auto res = kwk::none_of(ctx, d, [&](auto e)
    {
      return (e == 234);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::none_of(in, func) 4D")
  template<typename Context>
  void none_of_in_func_4D(Context&& ctx)
  {
    int data[2*3*4*5];
    bool vdata = true;

    fill_data(data, kwk::of_size(2,3,4,5), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    auto res = kwk::none_of(ctx, d, [&](auto e)
    {
      return (e == 2345);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::none_of(in, func) False")
  template<typename Context>
  void none_of_in_func_false(Context&& ctx)
  {
    int data[2*2]      = { 3,2
                          , 2,2
                          };

    bool vdata        =  false;

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

    auto res = kwk::none_of(ctx, d, [&](auto e)
    {
      return (e == 3);
    });

    TTS_EQUAL(res,   vdata);
  };

  // TTS_CASE("Check for kwk::count(in, value) 1D")
  template<typename Context>
  void count_in_value_1D(Context&& ctx)
  {
    int data[2];

    fill_data(data, kwk::of_size(2), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

    unsigned long res = kwk::count(ctx, d, 1);

    TTS_EQUAL(res,   1UL);
  };

  // TTS_CASE("Check for kwk::count(in, value) 2D")
  template<typename Context>
  void count_in_value_2D(Context&& ctx)
  {
    int data[2*3];

    fill_data(data, kwk::of_size(2,3), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    unsigned long res = kwk::count(ctx, d, 12);

    TTS_EQUAL(res,   1UL);
  };

  // TTS_CASE("Check for kwk::count(in, value) 3D")
  template<typename Context>
  void count_in_value_3D(Context&& ctx)
  {
    int data[2*3*4];

    fill_data(data, kwk::of_size(2,3,4), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    unsigned long res = kwk::count(ctx, d, 123);

    TTS_EQUAL(res,   1UL);
  };

  // TTS_CASE("Check for kwk::count(in, value) 4D")
  template<typename Context>
  void count_in_value_4D(Context&& ctx)
  {
    int data[2*3*4*5];

    fill_data(data, kwk::of_size(2,3,4,5), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    unsigned long res = kwk::count(ctx, d, 1234);

    TTS_EQUAL(res,   1UL);
  };

  // TTS_CASE("Check for kwk::count_if(in, func) 1D")
  template<typename Context>
  void count_if_in_func_1D(Context&& ctx)
  {
    int data[2];

    fill_data(data, kwk::of_size(2), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

    unsigned long res = kwk::count_if(ctx, d, [&](auto e)
    {
      return (e >= 0);
    });

    TTS_EQUAL(res,   2UL);
  };

  // TTS_CASE("Check for kwk::count_if(in, func) 2D")
  template<typename Context>
  void count_if_in_func_2D(Context&& ctx)
  {
    int data[2*3];

    fill_data(data, kwk::of_size(2,3), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

    unsigned long res = kwk::count_if(ctx, d, [&](auto e)
    {
      return (e >= 10);
    });

    TTS_EQUAL(res,   3UL);
  };

  // TTS_CASE("Check for kwk::count_if(in, func) 3D")
  template<typename Context>
  void count_if_in_func_3D(Context&& ctx)
  {
    int data[2*3*4];

    fill_data(data, kwk::of_size(2,3,4), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

    unsigned long res = kwk::count_if(ctx, d, [&](auto e)
    {
      return (e >= 120);
    });

    TTS_EQUAL(res,   4UL);
  };

  // TTS_CASE("Check for kwk::count_if(in, func) 4D")
  template<typename Context>
  void count_if_in_func_4D(Context&& ctx)
  {
    int data[2*3*4*5];

    fill_data(data, kwk::of_size(2,3,4,5), true);

    auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

    unsigned long res = kwk::count_if(ctx, d, [&](auto e)
    {
      return (e >= 1230);
    });

    TTS_EQUAL(res,   5UL);
  };
}