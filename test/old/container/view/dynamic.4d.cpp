//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier:   KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/container.hpp>
#include <algorithm>
#include <array>
#include <vector>

TTS_CASE( "Build a 4D view with dynamic shape settings from a C array" )
{
  using namespace kwk::literals;

  float ref[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                ,13,14,15,16,17,18,19,20,21,22,23,24
                };

  std::ptrdiff_t sz = 2;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz,sz+1,sz,sz) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+7*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 24                                     );
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2)                  );
  TTS_EXPECT_NOT( v.shape().is_fully_static                     );

  for(std::ptrdiff_t i3 = 0;i3<kwk::dim<3>(v);++i3)
    for(std::ptrdiff_t i2 = 0;i2<kwk::dim<2>(v);++i2)
      for(std::ptrdiff_t i1 = 0;i1<kwk::dim<1>(v);++i1)
        for(std::ptrdiff_t i0 = 0;i0<kwk::dim<0>(v);++i0)
          TTS_EQUAL(v(i0,i1,i2,i3), ref[i0+kwk::dim<0>(v)*(i1+kwk::dim<1>(v)*(i2+kwk::dim<2>(v)*i3))]);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(sz+1,sz,sz,sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+7*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 24                                     );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2)                  );
  TTS_EXPECT_NOT( w.shape().is_fully_static                     );

  for(std::ptrdiff_t i3 = 0;i3<kwk::dim<3>(w);++i3)
    for(std::ptrdiff_t i2 = 0;i2<kwk::dim<2>(w);++i2)
      for(std::ptrdiff_t i1 = 0;i1<kwk::dim<1>(w);++i1)
        for(std::ptrdiff_t i0 = 0;i0<kwk::dim<0>(w);++i0)
          TTS_EQUAL(w(i0,i1,i2,i3), ref[i0+kwk::dim<0>(w)*(i1+kwk::dim<1>(w)*(i2+kwk::dim<2>(w)*i3))]);
};

TTS_CASE( "Build a 4D view with dynamic shape settings from std::array" )
{
  using namespace kwk::literals;

  std::array<float,24> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                              ,13,14,15,16,17,18,19,20,21,22,23,24
                              };

  std::ptrdiff_t sz = 2;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz,sz+1,sz,sz) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+7*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 24                                     );
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2)                  );
  TTS_EXPECT_NOT( v.shape().is_fully_static                     );

  for(std::ptrdiff_t i3 = 0;i3<kwk::dim<3>(v);++i3)
    for(std::ptrdiff_t i2 = 0;i2<kwk::dim<2>(v);++i2)
      for(std::ptrdiff_t i1 = 0;i1<kwk::dim<1>(v);++i1)
        for(std::ptrdiff_t i0 = 0;i0<kwk::dim<0>(v);++i0)
          TTS_EQUAL(v(i0,i1,i2,i3), ref[i0+kwk::dim<0>(v)*(i1+kwk::dim<1>(v)*(i2+kwk::dim<2>(v)*i3))]);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(sz+1,sz,sz,sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+7*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 24                                     );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2)                  );
  TTS_EXPECT_NOT( w.shape().is_fully_static                     );

  for(std::ptrdiff_t i3 = 0;i3<kwk::dim<3>(w);++i3)
    for(std::ptrdiff_t i2 = 0;i2<kwk::dim<2>(w);++i2)
      for(std::ptrdiff_t i1 = 0;i1<kwk::dim<1>(w);++i1)
        for(std::ptrdiff_t i0 = 0;i0<kwk::dim<0>(w);++i0)
          TTS_EQUAL(w(i0,i1,i2,i3), ref[i0+kwk::dim<0>(w)*(i1+kwk::dim<1>(w)*(i2+kwk::dim<2>(w)*i3))]);
};

TTS_CASE( "Build a 4D view with dynamic shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  std::ptrdiff_t sz = 2;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz,sz+1,sz,sz) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+7*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 24                                     );
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2)                  );
  TTS_EXPECT_NOT( v.shape().is_fully_static                     );

  for(std::ptrdiff_t i3 = 0;i3<kwk::dim<3>(v);++i3)
    for(std::ptrdiff_t i2 = 0;i2<kwk::dim<2>(v);++i2)
      for(std::ptrdiff_t i1 = 0;i1<kwk::dim<1>(v);++i1)
        for(std::ptrdiff_t i0 = 0;i0<kwk::dim<0>(v);++i0)
          TTS_EQUAL(v(i0,i1,i2,i3), ref[i0+kwk::dim<0>(v)*(i1+kwk::dim<1>(v)*(i2+kwk::dim<2>(v)*i3))]);

  std::vector<float> const cref = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  auto w = kwk::view{ kwk::source = cref, kwk::of_size(sz+1,sz,sz,sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+7*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 24                                     );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2)                  );
  TTS_EXPECT_NOT( w.shape().is_fully_static                     );

  for(std::ptrdiff_t i3 = 0;i3<kwk::dim<3>(w);++i3)
    for(std::ptrdiff_t i2 = 0;i2<kwk::dim<2>(w);++i2)
      for(std::ptrdiff_t i1 = 0;i1<kwk::dim<1>(w);++i1)
        for(std::ptrdiff_t i0 = 0;i0<kwk::dim<0>(w);++i0)
          TTS_EQUAL(w(i0,i1,i2,i3), ref[i0+kwk::dim<0>(w)*(i1+kwk::dim<1>(w)*(i2+kwk::dim<2>(w)*i3))]);
};

TTS_CASE( "Build a 4D view with dynamic shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  std::ptrdiff_t sz = 2;
  auto v = kwk::view{ kwk::source = ref.data(), kwk::of_size(sz,sz+1,sz,sz) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+7*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 24                                     );
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2)                  );
  TTS_EXPECT_NOT( v.shape().is_fully_static                     );

  for(std::ptrdiff_t i3 = 0;i3<kwk::dim<3>(v);++i3)
    for(std::ptrdiff_t i2 = 0;i2<kwk::dim<2>(v);++i2)
      for(std::ptrdiff_t i1 = 0;i1<kwk::dim<1>(v);++i1)
        for(std::ptrdiff_t i0 = 0;i0<kwk::dim<0>(v);++i0)
          TTS_EQUAL(v(i0,i1,i2,i3), ref[i0+kwk::dim<0>(v)*(i1+kwk::dim<1>(v)*(i2+kwk::dim<2>(v)*i3))]);

  std::vector<float> const cref = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                                  ,13,14,15,16,17,18,19,20,21,22,23,24
                                  };

  auto w = kwk::view{ kwk::source = cref.data(), kwk::of_size(sz+1,sz,sz,sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+7*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 24                                     );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2)                  );
  TTS_EXPECT_NOT( w.shape().is_fully_static                     );

  for(std::ptrdiff_t i3 = 0;i3<kwk::dim<3>(w);++i3)
    for(std::ptrdiff_t i2 = 0;i2<kwk::dim<2>(w);++i2)
      for(std::ptrdiff_t i1 = 0;i1<kwk::dim<1>(w);++i1)
        for(std::ptrdiff_t i0 = 0;i0<kwk::dim<0>(w);++i0)
          TTS_EQUAL(w(i0,i1,i2,i3), ref[i0+kwk::dim<0>(w)*(i1+kwk::dim<1>(w)*(i2+kwk::dim<2>(w)*i3))]);
};
