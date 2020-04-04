//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/misc/unit_stride.hpp>
#include <kiwaku/misc/shape.hpp>
#include <tts/tests/relation.hpp>

TTS_CASE( "Building a 1D unit_stride from a list of 0 integral values" )
{
  kwk::unit_stride<1>  st{};

  TTS_EQUAL(st.count(), 1   );
  TTS_EQUAL(st.size() , 1ULL);

  auto[a] = st;
  TTS_EQUAL(a, 1);
}

TTS_CASE( "Building a 2D unit_stride from a list of integral values" )
{
  kwk::unit_stride<2> st1{1337};
  kwk::unit_stride<2> st0{};

  {
    auto[s0,s1] = st1;
    TTS_EQUAL(s0, 1   );
    TTS_EQUAL(s1, 1337);
  }

  {
    auto[s0,s1] = st0;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 0 );
  }
}

TTS_CASE( "Building a 3D unit_stride from a list of integral values" )
{
  kwk::unit_stride<3> st2{6,7};
  kwk::unit_stride<3> st1{42};
  kwk::unit_stride<3> st0{};

  {
    auto[s0,s1,s2] = st2;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 6 );
    TTS_EQUAL(s2, 7 );
  }

  {
    auto[s0,s1,s2] = st1;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 42);
    TTS_EQUAL(s2, 0 );
  }

  {
    auto[s0,s1,s2] = st0;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 0 );
    TTS_EQUAL(s2, 0 );
  }
}

TTS_CASE( "Building a 4D unit_stride from a list of integral values" )
{
  kwk::unit_stride<4> st3{6,7,9};
  kwk::unit_stride<4> st2{6,7};
  kwk::unit_stride<4> st1{42};
  kwk::unit_stride<4> st0{};

  {
    auto[s0,s1,s2,s3] = st3;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 6 );
    TTS_EQUAL(s2, 7 );
    TTS_EQUAL(s3, 9 );
  }

  {
    auto[s0,s1,s2,s3] = st2;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 6 );
    TTS_EQUAL(s2, 7 );
    TTS_EQUAL(s3, 0 );
  }

  {
    auto[s0,s1,s2,s3] = st1;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 42);
    TTS_EQUAL(s2, 0 );
    TTS_EQUAL(s3, 0 );
  }

  {
    auto[s0,s1,s2,s3] = st0;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 0 );
    TTS_EQUAL(s2, 0 );
    TTS_EQUAL(s3, 0 );
  }
}
