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

TTS_CASE( "Checks tuple API for kwk::shape - read only access")
{
  kwk::shape const _7d = {3,5,7,9,11,13,69};
  auto[d70,d71,d72,d73,d74,d75,d76] = _7d;
  TTS_EQUAL(d70, _7d[0]);
  TTS_EQUAL(d71, _7d[1]);
  TTS_EQUAL(d72, _7d[2]);
  TTS_EQUAL(d73, _7d[3]);
  TTS_EQUAL(d74, _7d[4]);
  TTS_EQUAL(d75, _7d[5]);
  TTS_EQUAL(d76, _7d[6]);
}

TTS_CASE( "Checks tuple API for kwk::shape - write access")
{
  kwk::shape<3> _3d;
  auto&[d30,d31,d32] = _3d;

  d30 = 13;
  d31 = 37;
  d32 = 42;

  TTS_EQUAL(d30, 13);
  TTS_EQUAL(d31, 37);
  TTS_EQUAL(d32, 42);
}
