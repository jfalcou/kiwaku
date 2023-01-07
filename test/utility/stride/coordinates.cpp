//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/coordinates.hpp>
#include <kwk/utility/container/shape.hpp>

TTS_CASE( "1D behavior of coordinates" )
{
  for(int i = 0; i < 120; ++i)
  {
    TTS_EQUAL( kwk::coordinates(i, kwk::of_size(120)), kumi::tuple{i} );
  }
};

TTS_CASE( "2D behavior of coordinates" )
{
  int pos = 0;
  for(int i1 = 0; i1 < 60; ++i1)
    for(int i0 = 0; i0 < 2; ++i0)
    {
      TTS_EQUAL( kwk::coordinates(pos, kwk::of_size(60,2)), (kumi::tuple{i1,i0}) );
      pos++;
    }
};

TTS_CASE( "3D behavior of coordinates" )
{
  int pos = 0;
  for(int i2 = 0; i2 < 20; ++i2)
    for(int i1 = 0; i1 < 3; ++i1)
      for(int i0 = 0; i0 < 2; ++i0)
      {
        TTS_EQUAL( kwk::coordinates(pos, kwk::of_size(20,3,2)), (kumi::tuple{i2,i1,i0}) );
        pos++;
      }
};

TTS_CASE( "4D behavior of coordinates" )
{
  int pos = 0;
  for(int i3 = 0; i3 < 5; ++i3)
    for(int i2 = 0; i2 < 4; ++i2)
      for(int i1 = 0; i1 < 3; ++i1)
        for(int i0 = 0; i0 < 2; ++i0)
        {
          TTS_EQUAL( kwk::coordinates(pos, kwk::of_size(5,4,3,2)), (kumi::tuple{i3,i2,i1,i0}) );
          pos++;
        }
};
