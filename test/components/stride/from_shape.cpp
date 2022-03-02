//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/components/stride.hpp>
#include <kiwaku/components/shape.hpp>

TTS_CASE( "Building a 1D stride using a 1D shape" )
{
  TTS_EQUAL( kwk::stride{kwk::shape(9)}.get<0>(), 1);
  TTS_EQUAL( kwk::stride{kwk::shape(9)}.is_unit, true );
};

TTS_CASE( "Building a 2D stride using a 2D shape" )
{
  TTS_EQUAL( kwk::stride{kwk::shape(3,4)}.get<0>(), 1);
  TTS_EQUAL( kwk::stride{kwk::shape(3,4)}.get<1>(), 3);
  TTS_EQUAL( kwk::stride{kwk::shape(3,4)}.is_unit, true );
};

TTS_CASE( "Building a 3D stride using a 3D shape" )
{
  TTS_EQUAL( kwk::stride{kwk::shape(3,5,7)}.get<0>(), 1 );
  TTS_EQUAL( kwk::stride{kwk::shape(3,5,7)}.get<1>(), 3 );
  TTS_EQUAL( kwk::stride{kwk::shape(3,5,7)}.get<2>(), 15);
  TTS_EQUAL( kwk::stride{kwk::shape(3,5,7)}.is_unit, true );
};

TTS_CASE( "Building a 4D stride using a 4D shape" )
{
  TTS_EQUAL( kwk::stride{kwk::shape(2,4,6,8)}.get<0>(), 1 );
  TTS_EQUAL( kwk::stride{kwk::shape(2,4,6,8)}.get<1>(), 2 );
  TTS_EQUAL( kwk::stride{kwk::shape(2,4,6,8)}.get<2>(), 8 );
  TTS_EQUAL( kwk::stride{kwk::shape(2,4,6,8)}.get<3>(), 48);
  TTS_EQUAL( kwk::stride{kwk::shape(2,4,6,8)}.is_unit, true );
};
