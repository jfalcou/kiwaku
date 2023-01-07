//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/stride.hpp>
#include <kwk/utility/container/shape.hpp>

TTS_CASE( "Convert a shapes to strides" )
{
  using namespace kwk::literals;
  TTS_EQUAL( kwk::as_stride(kwk::shape(9)), kwk::with_strides(1_c) );
  TTS_EQUAL( kwk::as_stride(kwk::shape(9)).is_unit, true );

  TTS_EQUAL( kwk::as_stride(kwk::shape(3,4)), kwk::with_strides(4,1_c) );
  TTS_EQUAL( kwk::as_stride(kwk::shape(3,4)).is_unit, true );

  TTS_EQUAL( kwk::as_stride(kwk::shape(3,5,7)), kwk::with_strides(35,7,1_c) );
  TTS_EQUAL( kwk::as_stride(kwk::shape(3,5,7)).is_unit, true );

  TTS_EQUAL( kwk::as_stride(kwk::shape(2,4,6,8)), kwk::with_strides(192,48,8,1_c) );
  TTS_EQUAL( kwk::as_stride(kwk::shape(2,4,6,8)).is_unit, true );
};

TTS_CASE( "Convert a shapes of small types to strides" )
{
  using namespace kwk::literals;
  std::uint8_t d0 = 17, d1 = 19, d2 = 21, d3 = 23;

  TTS_EQUAL( kwk::as_stride(kwk::shape(d1))         , kwk::with_strides(1_c)                  );
  TTS_EQUAL( kwk::as_stride(kwk::shape(d1,d0))      , kwk::with_strides(d0,1_c)               );
  TTS_EQUAL( kwk::as_stride(kwk::shape(d2,d1,d0))   , kwk::with_strides(d0*d1,d0,1_c)         );
  TTS_EQUAL( kwk::as_stride(kwk::shape(d3,d2,d1,d0)), kwk::with_strides(d0*d1*d2,d0*d1,d0,1_c));
};
