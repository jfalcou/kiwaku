//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE("extremum behavior - Arithmetic with values")
{
  using namespace kwk::literals;

  TTS_WHEN("Initializing extrema")
  {
    auto b = kwk::begin; // (n*0 + 0)/1
    auto e = kwk::end;   // (n*1 + 0)/1

    TTS_AND_THEN("Evaluation")
    {
      int n = 100;
      int m = 200;
      auto g = 10_c;
      // Traditionnal math on the result
      TTS_EQUAL(e(n), 100);
      TTS_EQUAL(e(m), 200);
      TTS_EQUAL(e(g), 10_c);

      TTS_EQUAL(b(n), 0);
      TTS_EQUAL(b(m), 0);
      TTS_EQUAL(b(g), 0);
    }

    TTS_AND_THEN("Multiplication")
    {
      // f(n) = n * 2
      auto e2 = e * 2;
      TTS_EQUAL(e2(10), 20);
      TTS_EQUAL(e2(50), 100);

      auto e2c = e * 2_c;
      TTS_EQUAL(e2c(10_c), 20_c);
      TTS_EQUAL(e2c(50_c), 100_c);

      // f(n) = n * 0
      auto e0 = e * 0;
      TTS_EQUAL(e0(100), 0);

      auto e0c = e * 0_c;
      TTS_EQUAL(e0c(100_c), 0_c);
    }

    TTS_AND_THEN("Addition")
    {
      // f(n) = n + 5
      auto e_plus_5 = e + 5;
      TTS_EQUAL(e_plus_5(10), 15);
      TTS_EQUAL(e_plus_5(0), 5);

      auto e_plus_5c = e + 5_c;
      TTS_EQUAL(e_plus_5c(10_c), 15_c);
      TTS_EQUAL(e_plus_5c(0_c), 5_c);

      // f(n) = 0 + 10
      auto b_plus_10 = b + 10;
      TTS_EQUAL(b_plus_10(100), 10);

      // f(n) = 0 + 10
      auto b_plus_10c = b + 10_c;
      TTS_EQUAL(b_plus_10c(100_c), 10_c);
    }

    TTS_AND_THEN("Chains")
    {
      // f(n) = (n + 10) * 2 = 2n + 20
      auto chain = (e + 10) * 2;
      TTS_EQUAL(chain(10), 40);
      TTS_EQUAL(chain.offset(), 20);
      TTS_EQUAL(chain.factor(), 2);

      // f(n) = (n + 10) * 2 = 2n + 20
      auto chainc = (e + 10_c) * 2_c;
      TTS_EQUAL(chainc(10_c), 40_c);
      TTS_EQUAL(chainc.offset(), 20_c);
      TTS_EQUAL(chainc.factor(), 2_c);
    }
  }
};
