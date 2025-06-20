//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE( "Check 1D floor interpolation" )
{
  int data[] = {1,2,3,4,5};
  auto v = kwk::view(kwk::source = data);

  for(int i=0;i<10;++i)
    TTS_ULP_EQUAL(kwk::floor.interpolate(v, i/2.), i/2, 0.5);
};

TTS_CASE( "Check 2D floor interpolation" )
{
  int data[4][4]  = {    1,   2,  3,    4
                    ,   51,  52,  53,  54
                    ,  101, 102, 103, 104
                    ,  151, 152, 153, 154
                    };
  auto v = kwk::view(kwk::source = data);

  for(int j=0;j<8;++j)
    for(int i=0;i<8;++i)
      TTS_ULP_EQUAL(kwk::floor.interpolate(v, j/2., i/2.), (i/2)+1+(50*(j/2)), 0.5);
};

TTS_CASE( "Check 3D floor interpolation" )
{
  int data[2][4][4] = {    1,   2,  3,    4
                      ,   51,  52,  53,  54
                      ,  101, 102, 103, 104
                      ,  151, 152, 153, 154
                      ,   -1,  -2,  -3,  -4
                      ,  -51, -52, -53, -54
                      , -101,-102,-103,-104
                      , -151,-152,-153,-154
                      };

  auto v = kwk::view(kwk::source = data);

  for(int k=0;k<4;++k)
    for(int j=0;j<8;++j)
      for(int i=0;i<8;++i)
        TTS_ULP_EQUAL ( kwk::floor.interpolate(v, k/2., j/2., i/2.)
                      , ((i/2)+1+(50*(j/2))) * (k>=2 ? -1 : 1)
                      , 0.5
                      );
};
