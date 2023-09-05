//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/views/reverse.hpp>
#include <kwk/algorithm/algos/generator.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

// Wait for slice, restride()

TTS_CASE("Check for kwk::reshape(in, name) 1D")
{
  int data[2];
  int res[2];
  int vdata[2];

  fill_data(data, kwk::of_size(2), true);
  fill_data(res, kwk::of_size(2), false);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  auto r = kwk::view{kwk::source = res, kwk::of_size(2)};
  auto v = kwk::view{kwk::source = vdata, kwk::of_size(2)};

  auto e = kwk::reverse(d);
  
  kwk::for_each([](auto& out, auto in)
  {
    out = in;
  }
  , r, e);

  kwk::generate([](auto p1)
  { 
    return 1-p1; 
  }
  , v);

  TTS_ALL_EQUAL(res, vdata);
};



// TTS_CASE("Check for kwk::reshape(in, name) 3D")
// {
//   int data[2*3*4];
//   int res[2*3*4];
//   int vdata[2*3*4];

//   fill_data(data, kwk::of_size(2,3,4), true);
//   fill_data(res, kwk::of_size(2,3,4), false);

//   auto d = kwk::view{kwk::source = data, kwk::of_size(2,3,4)};
//   auto r = kwk::view{kwk::source = res, kwk::of_size(2,3,4)};
//   auto v = kwk::view{kwk::source = vdata, kwk::of_size(2,3,4)};

//   auto e = kwk::reverse(d);
  
//   kwk::for_each([](auto& out, auto in)
//   {
//     out = in;
//   }
//   , r, e);

//   kwk::generate([](auto p1, auto p2, auto p3)
//   { 
//     return 100-p1*100 + 20-p2*10 + 3-p3; 
//   }
//   , v);

//   std::cout<<v<<std::endl;
//   TTS_ALL_EQUAL(res, vdata);
// };