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
  TTS_EQUAL( kwk::as_stride(kwk::of_size(9)), kwk::with_strides(1_c) );
  TTS_EQUAL( kwk::as_stride(kwk::of_size(9)).is_unit, true );

  TTS_EQUAL( kwk::as_stride(kwk::of_size(3,4)), kwk::with_strides(4,1_c) );
  TTS_EQUAL( kwk::as_stride(kwk::of_size(3,4)).is_unit, true );

  TTS_EQUAL( kwk::as_stride(kwk::of_size(3,5,7)), kwk::with_strides(35,7,1_c) );
  TTS_EQUAL( kwk::as_stride(kwk::of_size(3,5,7)).is_unit, true );

  TTS_EQUAL( kwk::as_stride(kwk::of_size(2,4,6,8)), kwk::with_strides(192,48,8,1_c) );
  TTS_EQUAL( kwk::as_stride(kwk::of_size(2,4,6,8)).is_unit, true );
};

TTS_CASE( "Convert a shapes of small types to strides" )
{
  using namespace kwk::literals;
  std::uint8_t d0 = 5, d1 = 4, d2 = 3, d3 = 2;

  TTS_EQUAL( kwk::as_stride(kwk::of_size(d1))         , kwk::with_strides(1_c)                  );
  TTS_EQUAL( kwk::as_stride(kwk::of_size(d1,d0))      , kwk::with_strides(d0,1_c)               );
  TTS_EQUAL( kwk::as_stride(kwk::of_size(d2,d1,d0))   , kwk::with_strides(d0*d1,d0,1_c)         );
  TTS_EQUAL( kwk::as_stride(kwk::of_size(d3,d2,d1,d0)), kwk::with_strides(d0*d1*d2,d0*d1,d0,1_c));
};
