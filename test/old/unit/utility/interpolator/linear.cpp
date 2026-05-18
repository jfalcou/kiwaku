//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE( "Check 1D linear interpolation" )
{
  int data[] = {0,2};
  auto v = kwk::view(kwk::source = data);

  for(int i=0;i<10;++i)
  {
    double pos = i*0.1;
    TTS_ULP_EQUAL(kwk::linear.interpolate(v, pos), i*0.2, 0.5);
  }
};

TTS_CASE( "Check 2D linear interpolation" )
{
  int data[2][2] = {1,2,3,4};
  auto v = kwk::view(kwk::source = data);

  for(int j=0;j<4;++j)
    for(int i=0;i<4;++i)
    {
      double p0 = i*0.25;
      double p1 = j*0.25;
      TTS_ULP_EQUAL(kwk::linear.interpolate(v, p1, p0), 1+p0+2*p1, 0.5);
    }
};

TTS_CASE( "Check 3D linear interpolation" )
{
  int data[2][2][2] = {1,2,3,4,5,6,7,8};
  auto v = kwk::view(kwk::source = data);

  for(int k=0;k<4;++k)
    for(int j=0;j<4;++j)
      for(int i=0;i<4;++i)
      {
        double p0 = i*0.25;
        double p1 = j*0.25;
        double p2 = k*0.25;
        TTS_ULP_EQUAL(kwk::linear.interpolate(v, p2, p1, p0), 1+p0+2*p1+4*p2, 0.5);
      }
};
