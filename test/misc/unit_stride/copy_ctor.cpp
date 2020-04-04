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

TTS_CASE( "Copy into a 1D stride" )
{
  kwk::unit_stride<1>  ref{};
  kwk::unit_stride<1>  st{ref};

  TTS_EQUAL(st.count(), 1   );
  TTS_EQUAL(st.size() , 1ULL);

  auto[a] = st;
  TTS_EQUAL(a, 1);
}

TTS_CASE( "Copy into a 2D stride" )
{
  kwk::unit_stride<2> ref2{1337};
  kwk::unit_stride<1> ref1{};

  {
    kwk::unit_stride<2> dst{ref1};

    auto[s0,s1] = dst;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 0 );
  }

  {
    kwk::unit_stride<2> dst{ref2};

    auto[s0,s1] = dst;
    TTS_EQUAL(s0, 1   );
    TTS_EQUAL(s1, 1337);
  }
}

TTS_CASE( "Copy into a 3D stride" )
{
  kwk::unit_stride<3> ref3{42,69};
  kwk::unit_stride<2> ref2{1337};
  kwk::unit_stride<1> ref1{};

  {
    kwk::unit_stride<3> dst{ref1};

    auto[s0,s1,s2] = dst;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 0 );
    TTS_EQUAL(s2, 0 );
  }

  {
    kwk::unit_stride<3> dst{ref2};

    auto[s0,s1,s2] = dst;
    TTS_EQUAL(s0, 1   );
    TTS_EQUAL(s1, 1337);
    TTS_EQUAL(s2, 0   );
  }

  {
    kwk::unit_stride<3> dst{ref3};

    auto[s0,s1,s2] = dst;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 42);
    TTS_EQUAL(s2, 69);
  }
}

TTS_CASE( "Copy into a 4D stride" )
{
  kwk::unit_stride<4> ref4{2,5,7};
  kwk::unit_stride<3> ref3{42,69};
  kwk::unit_stride<2> ref2{1337};
  kwk::unit_stride<1> ref1{};

  {
    kwk::unit_stride<4> dst{ref1};

    auto[s0,s1,s2,s3] = dst;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 0 );
    TTS_EQUAL(s2, 0 );
    TTS_EQUAL(s3, 0 );
  }

  {
    kwk::unit_stride<4> dst{ref2};

    auto[s0,s1,s2,s3] = dst;
    TTS_EQUAL(s0, 1   );
    TTS_EQUAL(s1, 1337);
    TTS_EQUAL(s2, 0   );
    TTS_EQUAL(s3, 0   );
  }

  {
    kwk::unit_stride<4> dst{ref3};

    auto[s0,s1,s2,s3] = dst;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 42);
    TTS_EQUAL(s2, 69);
    TTS_EQUAL(s3, 0 );
  }

  {
    kwk::unit_stride<4> dst{ref4};

    auto[s0,s1,s2,s3] = dst;
    TTS_EQUAL(s0, 1 );
    TTS_EQUAL(s1, 2 );
    TTS_EQUAL(s2, 5 );
    TTS_EQUAL(s3, 7 );
  }
}
