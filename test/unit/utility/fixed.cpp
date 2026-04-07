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

TTS_CASE("fixed<N> behavior - Comparisons (== and <=>)")
{
  using namespace kwk::literals;

  // Basic equality
  TTS_CONSTEXPR_EXPECT(123_c == 123_c);
  TTS_CONSTEXPR_EXPECT_NOT(123_c == 124_c);

  // Basic relational operators (generated via <=>)
  TTS_CONSTEXPR_EXPECT(10_c < 20_c);
  TTS_CONSTEXPR_EXPECT(10_c <= 20_c);
  TTS_CONSTEXPR_EXPECT(10_c <= 10_c);
  TTS_CONSTEXPR_EXPECT(20_c > 10_c);
  TTS_CONSTEXPR_EXPECT(20_c >= 10_c);
  TTS_CONSTEXPR_EXPECT(20_c >= 20_c);

  // Corner Cases: The Signed/Unsigned Trap
  // If standard C++ promotion was applied, -1 would promote to max uint64_t
  TTS_CONSTEXPR_EXPECT_NOT(-1_c == 18446744073709551615_c);
  TTS_CONSTEXPR_EXPECT(-1_c < 10_c);
  TTS_CONSTEXPR_EXPECT(-1_c < 18446744073709551615_c);
  TTS_CONSTEXPR_EXPECT(10_c > -1_c);
  TTS_CONSTEXPR_EXPECT(18446744073709551615_c > -1_c);

  // Corner Cases: Mixed bit-widths and signs
  TTS_CONSTEXPR_EXPECT(-32768_c < 65535_c);
  TTS_CONSTEXPR_EXPECT(65536_c > -128_c);
  TTS_CONSTEXPR_EXPECT(0_c == -0_c);
};

TTS_CASE("fixed<N> behavior - Functional API")
{
  using namespace kwk::literals;

  TTS_CONSTEXPR_EQUAL(123_c(), 123);

  constexpr int x = 123_c;
  TTS_CONSTEXPR_EQUAL(x, 123);
};
