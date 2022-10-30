//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/container.hpp>
#include <algorithm>
#include <array>
#include <vector>

TTS_CASE( "Build a 4D table with dynamic shape settings from a C array" )
{
  using namespace kwk::literals;

  float ref[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                ,13,14,15,16,17,18,19,20,21,22,23,24
                };

  std::ptrdiff_t sz = 2;
  auto v = kwk::table{ kwk::source = ref, kwk::of_size(sz,sz+1,sz,sz) };

  TTS_EQUAL ( sizeof(v), 72ULL                );
  TTS_EQUAL ( v.size() , 24                   );
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2));
  TTS_EXPECT_NOT( v.shape().is_fully_static   );

  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+sz*(i1+(sz+1)*(i2+sz*i3))]);
                      }
                      , v
                      );

  auto w = kwk::table{ kwk::source = ref, kwk::of_size(sz+1,sz,sz,sz) };

  TTS_EQUAL ( sizeof(w), 72ULL                );
  TTS_EQUAL ( w.size() , 24                   );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2));
  TTS_EXPECT_NOT( w.shape().is_fully_static   );

  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+(sz+1)*(i1+sz*(i2+sz*i3))]);
                      }
                      , w
                      );
};

TTS_CASE( "Build a 4D table with dynamic shape settings from std::array" )
{
  using namespace kwk::literals;

  std::array<float,24> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                              ,13,14,15,16,17,18,19,20,21,22,23,24
                              };

  std::ptrdiff_t sz = 2;
  auto v = kwk::table{ kwk::source = ref, kwk::of_size(sz,sz+1,sz,sz) };

  TTS_EQUAL ( sizeof(v), 72ULL                );
  TTS_EQUAL ( v.size() , 24                   );
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2));
  TTS_EXPECT_NOT( v.shape().is_fully_static   );

  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+sz*(i1+(sz+1)*(i2+sz*i3))]);
                      }
                      , v
                      );

  auto w = kwk::table{ kwk::source = ref, kwk::of_size(sz+1,sz,sz,sz) };

  TTS_EQUAL ( sizeof(w), 72ULL                );
  TTS_EQUAL ( w.size() , 24                   );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2));
  TTS_EXPECT_NOT( w.shape().is_fully_static   );

  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+(sz+1)*(i1+sz*(i2+sz*i3))]);
                      }
                      , w
                      );
};

TTS_CASE( "Build a 4D table with dynamic shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  std::ptrdiff_t sz = 2;
  auto v = kwk::table{ kwk::source = ref, kwk::of_size(sz,sz+1,sz,sz) };

  TTS_EQUAL ( sizeof(v), 72ULL                );
  TTS_EQUAL ( v.size() , 24                   );
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2));
  TTS_EXPECT_NOT( v.shape().is_fully_static   );

  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+sz*(i1+(sz+1)*(i2+sz*i3))]);
                      }
                      , v
                      );

  std::vector<float> const cref = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  auto w = kwk::table{ kwk::source = cref, kwk::of_size(sz+1,sz,sz,sz) };

  TTS_EQUAL ( sizeof(w), 72ULL                );
  TTS_EQUAL ( w.size() , 24                   );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2));
  TTS_EXPECT_NOT( w.shape().is_fully_static   );

  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+(sz+1)*(i1+sz*(i2+sz*i3))]);
                      }
                      , w
                      );
};

TTS_CASE( "Build a 4D table with dynamic shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref  = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                            ,13,14,15,16,17,18,19,20,21,22,23,24
                            };

  std::ptrdiff_t sz = 2;
  auto v = kwk::table{ kwk::source = ref.data(), kwk::of_size(sz,sz+1,sz,sz) };

  TTS_EQUAL ( sizeof(v), 72ULL                );
  TTS_EQUAL ( v.size() , 24                   );
  TTS_EQUAL ( v.shape(), kwk::of_size(2,3,2,2));
  TTS_EXPECT_NOT( v.shape().is_fully_static   );

  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+sz*(i1+(sz+1)*(i2+sz*i3))]);
                      }
                      , v
                      );

  std::vector<float> const cref = { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12
                                  ,13,14,15,16,17,18,19,20,21,22,23,24
                                  };

  auto w = kwk::table{ kwk::source = cref.data(), kwk::of_size(sz+1,sz,sz,sz) };

  TTS_EQUAL ( sizeof(w), 72ULL                );
  TTS_EQUAL ( w.size() , 24                   );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,2,2,2));
  TTS_EXPECT_NOT( w.shape().is_fully_static   );

  kwk::for_each_index([&](auto e, auto i0, auto i1, auto i2, auto i3)
                      {
                        TTS_EQUAL(e, ref[i0+(sz+1)*(i1+sz*(i2+sz*i3))]);
                      }
                      , w
                      );
};
