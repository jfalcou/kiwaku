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

TTS_CASE( "Building a 1D unit_stride from a shape" )
{
  kwk::shape        sh{422};
  kwk::unit_stride  st{sh};

  TTS_EQUAL(st.count(), 1   );
  TTS_EQUAL(st.size() , 1ULL);

  auto[a] = st;
  TTS_EQUAL(a         , 1);
}

TTS_CASE( "Building a 2D unit_stride from a shape" )
{
  auto sh = kwk::shape{13,37};
  kwk::unit_stride  st{sh};

  TTS_EQUAL(st.count(), 2   );
  TTS_EQUAL(st.size() , 2ULL);

  auto[s0,s1] = st;
  TTS_EQUAL(s0, 1 );
  TTS_EQUAL(s1, 13);
}

TTS_CASE( "Building a 3D unit_stride from a shape" )
{
  auto sh = kwk::shape{6,7,69};
  kwk::unit_stride  st{sh};

  TTS_EQUAL(st.count(), 3   );
  TTS_EQUAL(st.size() , 3ULL);

  auto[s0,s1,s2] = st;
  TTS_EQUAL(s0, 1 );
  TTS_EQUAL(s1, 6);
  TTS_EQUAL(s2, 42);
}

TTS_CASE( "Building a 4D unit_stride from a shape" )
{
  auto sh = kwk::shape{6,7,8,9};
  kwk::unit_stride  st{sh};

  TTS_EQUAL(st.count(), 4   );
  TTS_EQUAL(st.size() , 4ULL);

  auto[s0,s1,s2,s3] = st;
  TTS_EQUAL(s0, 1 );
  TTS_EQUAL(s1, 6);
  TTS_EQUAL(s2, 42);
  TTS_EQUAL(s3, 336);
}
