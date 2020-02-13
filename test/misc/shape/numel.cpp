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

TTS_CASE( "numel behavior on nD shape" )
{
  TTS_EQUAL( (kwk::shape{4,2,6,9}.numel()), 432 );
  TTS_EQUAL( (kwk::shape{4,2,6,1}.numel()), 48  );
  TTS_EQUAL( (kwk::shape{4,2,1,1}.numel()), 8   );
  TTS_EQUAL( (kwk::shape{4,1,1,1}.numel()), 4   );
  TTS_EQUAL( (kwk::shape{1,1,1,1}.numel()), 1   );
}
