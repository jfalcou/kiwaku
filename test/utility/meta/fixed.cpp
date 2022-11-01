//==================================================================================================
/*
  KIWAKU - Containers Well Made
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

  constexpr auto cst_0 = 0_c;
  constexpr auto cst_1 = 16540_c;
  constexpr auto cst_2 = 123456_c;

  TTS_EQUAL ( cst_0.value , 0            );
  TTS_EQUAL ( cst_1.value , short(16540) );
  TTS_EQUAL ( cst_2.value , 123456U      );

  TTS_EXPR_IS( cst_0.value , std::int8_t  const);
  TTS_EXPR_IS( cst_1.value , std::int16_t const);
  TTS_EXPR_IS( cst_2.value , std::int32_t const);
};
