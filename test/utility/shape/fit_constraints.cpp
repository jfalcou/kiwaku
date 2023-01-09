//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

TTS_CASE( "Check fit_constraints with dynamic reference" )
{
  using namespace kwk::literals;
  auto ref = kwk::of_size(7,8,9);
  TTS_EXPECT( kwk::of_size(11,12,13).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(11,12_c,13).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(11_c,12_c,13_c).fit_constraints(ref) );
};

TTS_CASE( "Check fit_constraints with hybrid reference" )
{
  using namespace kwk::literals;
  auto ref = kwk::of_size(7,8_c,9);
  TTS_EXPECT_NOT( kwk::of_size(11,12,13).fit_constraints(ref) );
  TTS_EXPECT_NOT( kwk::of_size(11,12_c,13).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(11,8,13).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(11,8_c,13).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(11_c,8_c,13_c).fit_constraints(ref) );
};

TTS_CASE( "Check fit_constraints with full-static reference" )
{
  using namespace kwk::literals;
  auto ref = kwk::of_size(7_c,8_c,9_c);
  TTS_EXPECT_NOT( kwk::of_size(11 ,12   ,13 ).fit_constraints(ref) );
  TTS_EXPECT_NOT( kwk::of_size(11 ,12_c ,13 ).fit_constraints(ref) );
  TTS_EXPECT_NOT( kwk::of_size(11 ,8_c  ,13 ).fit_constraints(ref) );
  TTS_EXPECT_NOT( kwk::of_size(11 ,8_c  ,9_c).fit_constraints(ref) );
  TTS_EXPECT_NOT( kwk::of_size(7_c,8_c  ,13 ).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(7_c,8_c,9_c).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(7  ,8_c,9_c).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(7_c,8  ,9_c).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(7_c,8_c,9  ).fit_constraints(ref) );
  TTS_EXPECT( kwk::of_size(7_c,8  ,9  ).fit_constraints(ref) );
};
