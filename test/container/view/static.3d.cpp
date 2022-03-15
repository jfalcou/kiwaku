//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include "test.hpp"
#include "tts.hpp"
#include <kiwaku/container.hpp>
#include <algorithm>
#include <array>
#include <vector>

TTS_CASE( "Build a 3D view from a C array" )
{
  using namespace kwk::literals;

  float ref[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                ,13,14,15,16,17,18,19,20,21,22,23,24
                };

  auto v = kwk::view{ kwk::source = ref, kwk::of_size(4_c, 3_c, 2_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)      );
  TTS_EQUAL ( v.size() , 24                 );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3,2));
  TTS_EXPECT( v.shape().is_fully_static     );
  TTS_EXPECT( (std::equal( v.begin(), v.end(), &ref[0])) );

  for(std::size_t i2 = 0;i2<kwk::dim<2>(v);++i2)
    for(std::size_t i1 = 0;i1<kwk::dim<1>(v);++i1)
      for(std::size_t i0 = 0;i0<kwk::dim<0>(v);++i0)
        TTS_EQUAL(v(i0,i1,i2), ref[i0+kwk::dim<0>(v)*(i1+kwk::dim<1>(v)*i2)]);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(3_c, 4_c, 2_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)      );
  TTS_EQUAL ( w.size() , 24                 );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4,2));
  TTS_EXPECT( w.shape().is_fully_static     );
  TTS_EXPECT( (std::equal( w.begin(), w.end(), &ref[0])) );

  for(std::size_t i2 = 0;i2<kwk::dim<2>(w);++i2)
    for(std::size_t i1 = 0;i1<kwk::dim<1>(w);++i1)
      for(std::size_t i0 = 0;i0<kwk::dim<0>(w);++i0)
        TTS_EQUAL(w(i0,i1,i2), ref[i0+kwk::dim<0>(w)*(i1+kwk::dim<1>(w)*i2)]);
};

TTS_CASE( "Build a 3D view from std::array" )
{
  using namespace kwk::literals;

  std::array<float,24> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                              ,13,14,15,16,17,18,19,20,21,22,23,24
                              };

  auto v = kwk::view{ kwk::source = ref, kwk::of_size(4_c, 3_c, 2_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)      );
  TTS_EQUAL ( v.size() , 24                 );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3,2));
  TTS_EXPECT( v.shape().is_fully_static     );
  TTS_EXPECT( (std::equal( v.begin(), v.end(), &ref[0])) );

  for(std::size_t i2 = 0;i2<kwk::dim<2>(v);++i2)
    for(std::size_t i1 = 0;i1<kwk::dim<1>(v);++i1)
      for(std::size_t i0 = 0;i0<kwk::dim<0>(v);++i0)
        TTS_EQUAL(v(i0,i1,i2), ref[i0+kwk::dim<0>(v)*(i1+kwk::dim<1>(v)*i2)]);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(3_c, 4_c, 2_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)      );
  TTS_EQUAL ( w.size() , 24                 );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4,2));
  TTS_EXPECT( w.shape().is_fully_static     );
  TTS_EXPECT( (std::equal( w.begin(), w.end(), &ref[0])) );

  for(std::size_t i2 = 0;i2<kwk::dim<2>(w);++i2)
    for(std::size_t i1 = 0;i1<kwk::dim<1>(w);++i1)
      for(std::size_t i0 = 0;i0<kwk::dim<0>(w);++i0)
        TTS_EQUAL(w(i0,i1,i2), ref[i0+kwk::dim<0>(w)*(i1+kwk::dim<1>(w)*i2)]);
};

TTS_CASE( "Build a 3D view with constexpr shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  auto v = kwk::view{ kwk::source = ref, kwk::of_size(4_c, 3_c, 2_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)      );
  TTS_EQUAL ( v.size() , 24                 );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3,2));
  TTS_EXPECT( v.shape().is_fully_static     );
  TTS_EXPECT( (std::equal( v.begin(), v.end(), &ref[0])) );

  for(std::size_t i2 = 0;i2<kwk::dim<2>(v);++i2)
    for(std::size_t i1 = 0;i1<kwk::dim<1>(v);++i1)
      for(std::size_t i0 = 0;i0<kwk::dim<0>(v);++i0)
        TTS_EQUAL(v(i0,i1,i2), ref[i0+kwk::dim<0>(v)*(i1+kwk::dim<1>(v)*i2)]);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(3_c, 4_c, 2_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)      );
  TTS_EQUAL ( w.size() , 24                 );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4,2));
  TTS_EXPECT( w.shape().is_fully_static     );
  TTS_EXPECT( (std::equal( w.begin(), w.end(), &ref[0])) );

  for(std::size_t i2 = 0;i2<kwk::dim<2>(w);++i2)
    for(std::size_t i1 = 0;i1<kwk::dim<1>(w);++i1)
      for(std::size_t i0 = 0;i0<kwk::dim<0>(w);++i0)
        TTS_EQUAL(w(i0,i1,i2), ref[i0+kwk::dim<0>(w)*(i1+kwk::dim<1>(w)*i2)]);
};


TTS_CASE( "Build a 3D view with constexpr shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  auto v = kwk::view{ kwk::source = ref, kwk::of_size(4_c, 3_c, 2_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)      );
  TTS_EQUAL ( v.size() , 24                 );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3,2));
  TTS_EXPECT( v.shape().is_fully_static     );
  TTS_EXPECT( (std::equal( v.begin(), v.end(), &ref[0])) );

  for(std::size_t i2 = 0;i2<kwk::dim<2>(v);++i2)
    for(std::size_t i1 = 0;i1<kwk::dim<1>(v);++i1)
      for(std::size_t i0 = 0;i0<kwk::dim<0>(v);++i0)
        TTS_EQUAL(v(i0,i1,i2), ref[i0+kwk::dim<0>(v)*(i1+kwk::dim<1>(v)*i2)]);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(3_c, 4_c, 2_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)      );
  TTS_EQUAL ( w.size() , 24                 );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4,2));
  TTS_EXPECT( w.shape().is_fully_static     );
  TTS_EXPECT( (std::equal( w.begin(), w.end(), &ref[0])) );

  for(std::size_t i2 = 0;i2<kwk::dim<2>(w);++i2)
    for(std::size_t i1 = 0;i1<kwk::dim<1>(w);++i1)
      for(std::size_t i0 = 0;i0<kwk::dim<0>(w);++i0)
        TTS_EQUAL(w(i0,i1,i2), ref[i0+kwk::dim<0>(w)*(i1+kwk::dim<1>(w)*i2)]);
};
