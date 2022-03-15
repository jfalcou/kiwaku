//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/stride.hpp>
#include <kiwaku/container/shape.hpp>

TTS_CASE( "Convert a 1D shape to 1D stride" )
{
  TTS_EQUAL( kwk::shape(9).as_stride().get<0>(), 1   );
  TTS_EQUAL( kwk::shape(9).as_stride().is_unit, true );
};

TTS_CASE( "Convert a 2D shape to 2D stride" )
{
  TTS_EQUAL( kwk::shape(3,4).as_stride().get<0>(), 1);
  TTS_EQUAL( kwk::shape(3,4).as_stride().get<1>(), 3);
  TTS_EQUAL( kwk::shape(3,4).as_stride().is_unit, true );
};

TTS_CASE( "Convert a 3D shape to 3D stride" )
{
  TTS_EQUAL( kwk::shape(3,5,7).as_stride().get<0>(), 1 );
  TTS_EQUAL( kwk::shape(3,5,7).as_stride().get<1>(), 3 );
  TTS_EQUAL( kwk::shape(3,5,7).as_stride().get<2>(), 15);
  TTS_EQUAL( kwk::shape(3,5,7).as_stride().is_unit, true );
};

TTS_CASE( "Convert a 4D shape to 4D stride" )
{
  TTS_EQUAL( kwk::shape(2,4,6,8).as_stride().get<0>(), 1 );
  TTS_EQUAL( kwk::shape(2,4,6,8).as_stride().get<1>(), 2 );
  TTS_EQUAL( kwk::shape(2,4,6,8).as_stride().get<2>(), 8 );
  TTS_EQUAL( kwk::shape(2,4,6,8).as_stride().get<3>(), 48);
  TTS_EQUAL( kwk::shape(2,4,6,8).as_stride().is_unit, true );
};
