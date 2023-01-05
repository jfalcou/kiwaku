//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/linear_index.hpp>
#include <array>

TTS_CASE( "1D behavior of linear_index" )
{
  kwk::shape sh{13};

  for(int i = 0; i < sh.numel(); ++i)
  {
    TTS_EQUAL(i, kwk::linear_index(sh,i));
    TTS_EQUAL(i, kwk::linear_index(sh,kumi::tuple{i}));
    TTS_EQUAL(i, kwk::linear_index(sh,std::array<int,1>{i}));
  }
};

TTS_CASE( "2D behavior of linear_index" )
{
  kwk::shape sh{4,3};

  for(int i1 = 0; i1 < 4; ++i1)
  {
    for(int i0 = 0; i0 < 3; ++i0)
    {
      TTS_EQUAL((i0+3*i1), (kwk::linear_index(sh, i0, i1)) );
      TTS_EQUAL((i0+3*i1), (kwk::linear_index(sh,kumi::tuple{i0,i1})) );
      TTS_EQUAL((i0+3*i1), (kwk::linear_index(sh,std::array<int,2>{i0,i1})) );
    }
  }
};

TTS_CASE( "3D behavior of linear_index" )
{
  kwk::shape sh{4,3,2};

  for(int i2 = 0; i2 < 4; ++i2)
  {
    for(int i1 = 0; i1 < 3; ++i1)
    {
      for(int i0 = 0; i0 < 2; ++i0)
      {
        TTS_EQUAL((i0+2*i1+6*i2), (kwk::linear_index(sh, i0, i1, i2)) );
        TTS_EQUAL((i0+2*i1+6*i2), (kwk::linear_index(sh,kumi::tuple{i0,i1,i2})) );
        TTS_EQUAL((i0+2*i1+6*i2), (kwk::linear_index(sh,std::array<int,3>{i0,i1,i2})) );
      }
    }
  }
};

TTS_CASE( "4D behavior of linear_index" )
{
  kwk::shape sh{5,4,3,2};

  for(int i3 = 0; i3 < 5; ++i3)
  {
    for(int i2 = 0; i2 < 4; ++i2)
    {
      for(int i1 = 0; i1 < 3; ++i1)
      {
        for(int i0 = 0; i0 < 2; ++i0)
        {
          TTS_EQUAL((i0+2*i1+6*i2+24*i3), (kwk::linear_index(sh, i0, i1, i2, i3)) );
          TTS_EQUAL((i0+2*i1+6*i2+24*i3), (kwk::linear_index(sh,kumi::tuple{i0,i1,i2,i3})) );
          TTS_EQUAL((i0+2*i1+6*i2+24*i3), (kwk::linear_index(sh,std::array<int,4>{i0,i1,i2,i3})) );
        }
      }
    }
  }
};
