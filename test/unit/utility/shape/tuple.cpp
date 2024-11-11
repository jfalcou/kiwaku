//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE( "Checks tuple API for kwk::shape - read only access")
{
  auto const _7d = kwk::of_size(3,5,7,9,11,13,69);
  auto [c70,c71,c72,c73,c74,c75,c76] = _7d;

  TTS_EQUAL(c70, get<0>(_7d));
  TTS_EQUAL(c71, get<1>(_7d));
  TTS_EQUAL(c72, get<2>(_7d));
  TTS_EQUAL(c73, get<3>(_7d));
  TTS_EQUAL(c74, get<4>(_7d));
  TTS_EQUAL(c75, get<5>(_7d));
  TTS_EQUAL(c76, get<6>(_7d));
};
