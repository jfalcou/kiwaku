//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/shape.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/basic.hpp>

TTS_CASE( "Small shape can be used to construct large shape" )
{
  auto small = kwk::shape{4,2};
  auto large = kwk::_4D(small);

  TTS_EQUAL( large[0], small[0] );
  TTS_EQUAL( large[1], small[1] );
  TTS_EQUAL( large[2], 1 );
  TTS_EQUAL( large[3], 1 );
}

TTS_CASE( "Small shape can be assigned to large shape" )
{
  auto          small = kwk::shape{4,2};
  kwk::shape<4> large;

  large = small;

  TTS_EQUAL( large[0], small[0] );
  TTS_EQUAL( large[1], small[1] );
  TTS_EQUAL( large[2], 1 );
  TTS_EQUAL( large[3], 1 );
}

TTS_CASE( "Large shape need to be explicitly converted to small shape" )
{
  auto large = kwk::shape{4,2,6,9};
  auto small = kwk::shape<2>{large};

  TTS_EQUAL( small[0], large[0] );
  TTS_EQUAL( small[1], large[1]*large[2]*large[3] );
}

TTS_CASE( "Large shape need to be explicitly assigned to small shape" )
{
  auto          large = kwk::shape{4,2,6,9};
  kwk::shape<2> small;

  small = kwk::shape<2>{large};

  TTS_EQUAL( small[0], large[0] );
  TTS_EQUAL( small[1], large[1]*large[2]*large[3] );
}
