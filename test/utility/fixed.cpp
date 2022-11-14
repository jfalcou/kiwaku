//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier:   KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/options/fixed.hpp>

TTS_CASE( "fixed<N> behavior - Explicit usage" )
{
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<0>.value                     , 0 );
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<short(-1)>.value             , short(-1) );
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<0x1234567812345678ULL>.value , 0x1234567812345678ULL );
};

TTS_CASE( "fixed<N> behavior - Literal API" )
{
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<0>.value                     , 0 );
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<short(-1)>.value             , short(-1) );
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<0x1234567812345678ULL>.value , 0x1234567812345678ULL );
};
