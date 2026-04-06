//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility.hpp>

TTS_CASE("fixed<N> behavior - Type Optimization (Clamping)")
{
  // Test positive boundaries
  TTS_EXPR_IS(kwk::fixed<0>.value, std::uint8_t const);
  TTS_EXPR_IS(kwk::fixed<255>.value, std::uint8_t const);
  TTS_EXPR_IS(kwk::fixed<256>.value, std::uint16_t const);
  TTS_EXPR_IS(kwk::fixed<65535>.value, std::uint16_t const);
  TTS_EXPR_IS(kwk::fixed<65536>.value, std::uint32_t const);

  // Test negative boundaries
  TTS_EXPR_IS(kwk::fixed<-1>.value, std::int8_t const);
  TTS_EXPR_IS(kwk::fixed<-128>.value, std::int8_t const);
  TTS_EXPR_IS(kwk::fixed<-129>.value, std::int16_t const);
  TTS_EXPR_IS(kwk::fixed<-32768>.value, std::int16_t const);
  TTS_EXPR_IS(kwk::fixed<-32769>.value, std::int32_t const);
};

TTS_CASE("fixed<N> behavior - Arithmetic & Type Promotion")
{
  using namespace kwk::literals;

  // Addition: uint8 + uint8 -> uint8
  // Notice the extra parentheses around the arithmetic
  constexpr auto v = (10_c + 20_c).value;
  TTS_CONSTEXPR_EQUAL(v, 30);
  TTS_EXPR_IS(v, std::uint8_t const);

  // Addition: uint8 + uint8 -> uint16
  constexpr auto v2 = (200_c + 100_c).value;
  TTS_CONSTEXPR_EQUAL(v2, 300);
  TTS_EXPR_IS(v2, std::uint16_t const);

  // Subtraction: uint8 - uint8 -> int8
  constexpr auto v3 = (10_c - 20_c).value;
  TTS_CONSTEXPR_EQUAL(v3, -10);
  TTS_EXPR_IS(v3, std::int8_t const);

  // Multiplication: uint16 * uint16 -> uint32
  constexpr auto v4 = (1000_c * 1000_c).value;
  TTS_CONSTEXPR_EQUAL(v4, 1000000U);
  TTS_EXPR_IS(v4, std::uint32_t const);
};

TTS_CASE("fixed<N> behavior - Unary Operators")
{
  using namespace kwk::literals;

  // Unary operators also need grouping before accessing .value
  constexpr auto v = (-10_c).value;
  TTS_CONSTEXPR_EQUAL(v, -10);
  TTS_EXPR_IS(v, std::int8_t const);

  constexpr auto v2 = (-(-10_c)).value;
  TTS_CONSTEXPR_EQUAL(v2, 10);
  TTS_EXPR_IS(v2, std::uint8_t const);

  constexpr auto v3 = (+42_c).value;
  TTS_CONSTEXPR_EQUAL(v3, 42);
  TTS_EXPR_IS(v3, std::uint8_t const);
};

TTS_CASE("fixed<N> behavior - Functional API")
{
  using namespace kwk::literals;

  // Test operator() in constexpr context
  TTS_CONSTEXPR_EQUAL(123_c(), 123);

  // Test implicit conversion
  constexpr int x = 123_c;
  TTS_CONSTEXPR_EQUAL(x, 123);

  // Test equality
  TTS_CONSTEXPR_EXPECT(123_c == 123_c);
  TTS_CONSTEXPR_EXPECT_NOT(123_c == 124_c);
};
