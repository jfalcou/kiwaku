//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/end.hpp>
#include <kwk/utility/fixed.hpp>

TTS_CASE( "end offset generation" )
{
  using namespace kwk::literals;

  auto d = kwk::end - 4;
  auto m = (kwk::end - 4) - 9;
  auto s = kwk::end - 8_c;

  TTS_EQUAL(d.offset,  4);
  TTS_EQUAL(m.offset, 13);

  TTS_EQUAL(s.offset,  8);
  TTS_EXPR_IS(s.offset, kwk::constant<std::uint8_t(8)>);
};