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
#include <kwk/utility/fixed.hpp>

TTS_CASE( "fixed<N> behavior - Explicit usage" )
{
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<0>.value                     , 0                     );
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<short(-1)>.value             , short(-1)             );
  TTS_CONSTEXPR_EQUAL ( kwk::fixed<0x1234567812345678ULL>.value , 0x1234567812345678ULL );

  TTS_EXPR_IS ( kwk::fixed<0>.value                       , std::int32_t const );
  TTS_EXPR_IS ( kwk::fixed<short(-1)>.value               , std::int16_t const );
  TTS_EXPR_IS ( kwk::fixed<std::int64_t{123456789}>.value , std::int64_t const );
};

TTS_CASE( "fixed<N> behavior - Literal API" )
{
  using namespace kwk::literals;

  TTS_CONSTEXPR_EQUAL ( (0_c).value      , 0            );
  TTS_CONSTEXPR_EQUAL ( (16540_c).value  , short(16540) );
  TTS_CONSTEXPR_EQUAL ( (123456_c).value , 123456U      );

  TTS_EXPR_IS( (0_c).value      , std::uint8_t  const);
  TTS_EXPR_IS( (16540_c).value  , std::uint16_t const);
  TTS_EXPR_IS( (123456_c).value , std::uint32_t const);
};
