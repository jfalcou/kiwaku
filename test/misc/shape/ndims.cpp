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

TTS_CASE( "nbdims behavior on nD shape" )
{
  TTS_EQUAL( (kwk::shape{4,2,6,9}.nbdims()), 4);
  TTS_EQUAL( (kwk::shape{4,2,6,1}.nbdims()), 3);
  TTS_EQUAL( (kwk::shape{4,2,1,1}.nbdims()), 2);
  TTS_EQUAL( (kwk::shape{4,1,1,1}.nbdims()), 1);
  TTS_EQUAL( (kwk::shape{1,1,1,1}.nbdims()), 0);
};
