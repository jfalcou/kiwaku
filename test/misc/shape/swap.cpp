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

TTS_CASE( "swap behavior on 1D shape" )
{
  kwk::shape first{4}     , second{2};
  kwk::shape old_first{4} , old_second{2};

  first.swap(second);

  TTS_EQUAL(first , old_second);
  TTS_EQUAL(second, old_first );
}

TTS_CASE( "swap behavior on 2D shape" )
{
  kwk::shape first{4,2}     , second{6,9};
  kwk::shape old_first{4,2} , old_second{6,9};

  first.swap(second);

  TTS_EQUAL(first , old_second);
  TTS_EQUAL(second, old_first );
}

TTS_CASE( "swap behavior on 3D shape" )
{
  kwk::shape first{13,3,7}     , second{1,66,4};
  kwk::shape old_first{13,3,7} , old_second{1,66,4};

  first.swap(second);

  TTS_EQUAL(first , old_second);
  TTS_EQUAL(second, old_first );
}

TTS_CASE( "swap behavior on 4D shape" )
{
  kwk::shape first{1,3,3,7}     , second{1,6,6,4};
  kwk::shape old_first{1,3,3,7} , old_second{1,6,6,4};

  first.swap(second);

  TTS_EQUAL(first , old_second);
  TTS_EQUAL(second, old_first );
}
