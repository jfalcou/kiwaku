//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
//#include "../vecto_extension.hpp"
//#include "test.hpp"
#include <kwk/context/eve/context.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::reduce(in) 1D")
{
  int data[2];
  int vdata = 1;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::reduce(kwk::simd, d);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::reduce(in) 2D")
{
  int data[2*3];
  int vdata = 36;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = kwk::reduce(kwk::simd, d);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::reduce(in) 3D")
{
  int data[2*3*4];
  int vdata = 1476;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = kwk::reduce(kwk::simd, d);

  TTS_EQUAL(res, vdata);
};

TTS_CASE("Check for kwk::reduce(in) 4D")
{
  int data[2*3*4*5];
  int vdata = 74040;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = kwk::reduce(kwk::simd, d);

  TTS_EQUAL(res, vdata);
}; 


TTS_CASE("Check for kwk::reduce(in, func, init) 1D")
{
  int data[2];
  int vdata = 11;

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = reduce(kwk::simd, d,std::pair{eve::add, 0}, 10);

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::reduce(in, func, init) 2D")
{
  int data[2*3];
  int vdata = 136;

  fill_data(data, kwk::of_size(2,3), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3)};

  auto res = reduce(kwk::simd,d,std::pair{eve::add, 0},100);

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::reduce(in, func, init) 3D")
{
  int data[2*3*4];
  int vdata = 2476;

  fill_data(data, kwk::of_size(2,3,4), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};

  auto res = reduce(kwk::simd, d,std::pair{eve::add, 0},1000);

  TTS_EQUAL(res,   vdata);
};

TTS_CASE("Check for kwk::reduce(in, func, init) 4D")
{
  int data[2*3*4*5];
  int vdata = 84040;

  fill_data(data, kwk::of_size(2,3,4,5), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4,5)};

  auto res = reduce(kwk::simd,d,std::pair{eve::add, 0},10000);

  TTS_EQUAL(res,   vdata);
};
