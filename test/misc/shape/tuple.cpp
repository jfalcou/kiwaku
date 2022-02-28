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

TTS_CASE( "Checks tuple API for kwk::shape - read only access")
{
  kwk::shape const _7d{3,5,7,9,11,13,69};
  auto [c70,c71,c72,c73,c74,c75,c76] = _7d;
  auto d70 = c70;
  auto d71 = c71;
  auto d72 = c72;
  auto d73 = c73;
  auto d74 = c74;
  auto d75 = c75;
  auto d76 = c76;

  TTS_EQUAL(d70, _7d.get<0>());
  TTS_EQUAL(d71, _7d.get<1>());
  TTS_EQUAL(d72, _7d.get<2>());
  TTS_EQUAL(d73, _7d.get<3>());
  TTS_EQUAL(d74, _7d.get<4>());
  TTS_EQUAL(d75, _7d.get<5>());
  TTS_EQUAL(d76, _7d.get<6>());
};
