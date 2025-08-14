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
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>

TTS_CASE("Check for kwk::all_of(in, func) 1D")
{
  int data[2];
  bool vdata = true;

  fill_data(data, kwk::of_size(2), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::all_of(kwk::simd, d, [&](auto e)
  {
    return (e == 0);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::all_of(in, func) 2D")
{
  int data[2*3];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::all_of(kwk::simd, d, [&](auto e)
  {
    return (e == 0);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::all_of(in, func) 3D")
{
  int data[2*3*4];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::all_of(kwk::simd, d, [&](auto e)
  {
    return (e == 0);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::all_of(in, func) 4D")
{
  int data[2*3*4*5];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4,5), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::all_of(kwk::simd, d, [&](auto e)
  {
    return (e == 0);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::all_of(in, func) False")
{
  int data[2*2]      = { 2,2
                        , 2,3
                        };

  bool vdata        =  false;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

  auto res = kwk::all_of(kwk::simd, d, [&](auto e)
  {
    return (e == 2);
  });

  TTS_EQUAL(res,   vdata);
};


TTS_CASE("Check for kwk::any_of(in, func) 1D")
{
  int data[2];
  bool vdata = true;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::any_of(kwk::simd, d, [&](auto e)
  {
    return (e == 1);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::any_of(in, func) 2D")
{
  int data[2*3];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::any_of(kwk::simd, d, [&](auto e)
  {
    return (e == 12);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::any_of(in, func) 3D")
{
  int data[2*3*4];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::any_of(kwk::simd, d, [&](auto e)
  {
    return (e == 123);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::any_of(in, func) 4D")
{
  int data[2*3*4*5];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::any_of(kwk::simd,d, [&](auto e)
  {
    return (e == 1234);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::any_of(in, func) False")
{
  int data[2*2]      = { 2,2
                        , 2,2
                        };

  bool vdata        =  false;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

  auto res = kwk::any_of(kwk::simd, d, [&](auto e)
  {
    return (e == 3);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::none_of(in, func) 1D")
{
  int data[2];
  bool vdata = true;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::none_of(kwk::simd, d, [&](auto e)
  {
    return (e == 2);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::none_of(in, func) 2D")
{
  int data[2*3];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::none_of(kwk::simd,d, [&](auto e)
  {
    return (e == 23);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::none_of(in, func) 3D")
{
  int data[2*3*4];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::none_of(kwk::simd,d, [&](auto e)
  {
    return (e == 234);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::none_of(in, func) 4D")
{
  int data[2*3*4*5];
  bool vdata = true;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::none_of(kwk::simd,d, [&](auto e)
  {
    return (e == 2345);
  });

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::none_of(in, func) False")
{
  int data[2*2]      = { 3,2
                        , 2,2
                        };

  bool vdata        =  false;

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,2)};

  auto res = kwk::none_of(kwk::simd,d, [&](auto e)
  {
    return (e == 3);
  });

  TTS_EQUAL(res,   vdata);
};



#else // KIWAKU_BUILD_TEST_SIMD

TTS_CASE("SYCL disabled, kwk::for_each with SYCL context skipped.")
{
  TTS_PASS("SYCL disabled, skipping test.");
};

#endif // KIWAKU_BUILD_TEST_SIMD