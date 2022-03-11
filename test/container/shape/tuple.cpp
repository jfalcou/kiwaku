//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/shape.hpp>

TTS_CASE( "Checks tuple API for kwk::shape - read only access")
{
  auto const _7d = kwk::of_size(3,5,7,9,11,13,69);
  auto [c70,c71,c72,c73,c74,c75,c76] = _7d;

  TTS_EQUAL(c70, _7d.get<0>());
  TTS_EQUAL(c71, _7d.get<1>());
  TTS_EQUAL(c72, _7d.get<2>());
  TTS_EQUAL(c73, _7d.get<3>());
  TTS_EQUAL(c74, _7d.get<4>());
  TTS_EQUAL(c75, _7d.get<5>());
  TTS_EQUAL(c76, _7d.get<6>());
};
