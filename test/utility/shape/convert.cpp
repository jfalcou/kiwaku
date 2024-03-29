//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

using kwk::_;

TTS_CASE( "Small shape can be used to construct large shape" )
{
  auto small = kwk::_2D{4,2};
  auto large = kwk::_4D(small);

  TTS_EQUAL( large, kwk::of_size(1,1,small[0],small[1]) );
};

TTS_CASE( "Small shape can be assigned to large shape" )
{
  auto      small = kwk::_2D{4,2};
  kwk::_4D  large;

  large = small;

  TTS_EQUAL( large, kwk::of_size(1,1,small[0],small[1]) );
};

TTS_CASE( "Large shape need to be explicitly converted to small shape" )
{
  auto large = kwk::_4D{4,2,6,9};
  auto small = kwk::_2D{large};

  TTS_EQUAL( small[0], large[0]*large[1]*large[2] );
  TTS_EQUAL( small[1], large[3] );
};

TTS_CASE( "Large shape need to be explicitly assigned to small shape" )
{
  auto    large = kwk::_4D{4,2,6,9};
  kwk::_2D small;

  small = kwk::_2D{large};

  TTS_EQUAL( small[0], large[0]*large[1]*large[2] );
  TTS_EQUAL( small[1], large[3] );
};
