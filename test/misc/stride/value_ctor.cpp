//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/misc/stride.hpp>
#include <kiwaku/misc/shape.hpp>
#include <tts/tests/relation.hpp>

TTS_CASE( "Building a 1D stride from a list of 0 integral values" )
{
  kwk::stride<1>  st{7};

  TTS_EQUAL(st.count(), 1   );
  TTS_EQUAL(st.size() , 1ULL);

  auto[a] = st;
  TTS_EQUAL(a, 7);
}

TTS_CASE( "Building a 2D stride from a list of integral values" )
{
  kwk::stride<2> st1{13,37};
  kwk::stride<2> st0{1337};

  {
    auto[s0,s1] = st1;
    TTS_EQUAL(s0, 13  );
    TTS_EQUAL(s1, 37);
  }

  {
    auto[s0,s1] = st0;
    TTS_EQUAL(s0, 1337 );
    TTS_EQUAL(s1, 0 );
  }
}

TTS_CASE( "Building a 3D stride from a list of integral values" )
{
  kwk::stride<3> st2{6,7,9};
  kwk::stride<3> st1{42,69};
  kwk::stride<3> st0{8};

  {
    auto[s0,s1,s2] = st2;
    TTS_EQUAL(s0, 6 );
    TTS_EQUAL(s1, 7 );
    TTS_EQUAL(s2, 9 );
  }

  {
    auto[s0,s1,s2] = st1;
    TTS_EQUAL(s0, 42);
    TTS_EQUAL(s1, 69);
    TTS_EQUAL(s2, 0 );
  }

  {
    auto[s0,s1,s2] = st0;
    TTS_EQUAL(s0, 8 );
    TTS_EQUAL(s1, 0 );
    TTS_EQUAL(s2, 0 );
  }
}

TTS_CASE( "Building a 4D stride from a list of integral values" )
{
  kwk::stride<4> st3{6,7,9,11};
  kwk::stride<4> st2{6,7,11};
  kwk::stride<4> st1{42,69};
  kwk::stride<4> st0{3};

  {
    auto[s0,s1,s2,s3] = st3;
    TTS_EQUAL(s0, 6 );
    TTS_EQUAL(s1, 7 );
    TTS_EQUAL(s2, 9 );
    TTS_EQUAL(s3, 11);
  }

  {
    auto[s0,s1,s2,s3] = st2;
    TTS_EQUAL(s0, 6 );
    TTS_EQUAL(s1, 7 );
    TTS_EQUAL(s2, 11);
    TTS_EQUAL(s3, 0 );
  }

  {
    auto[s0,s1,s2,s3] = st1;
    TTS_EQUAL(s0, 42);
    TTS_EQUAL(s1, 69);
    TTS_EQUAL(s2, 0 );
    TTS_EQUAL(s3, 0 );
  }

  {
    auto[s0,s1,s2,s3] = st0;
    TTS_EQUAL(s0, 3 );
    TTS_EQUAL(s1, 0 );
    TTS_EQUAL(s2, 0 );
    TTS_EQUAL(s3, 0 );
  }
}
