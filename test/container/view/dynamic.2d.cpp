//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/container.hpp>
#include <kwk/algorithm/for_each.hpp>
#include <array>
#include <vector>

TTS_CASE( "Build a 2D view with dynamic shape settings from a C array" )
{
  using namespace kwk::literals;

  float ref[] = {1,2,3,4,5,6,7,8,9,10,11,12};

  std::ptrdiff_t sz = 4;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz,sz-1) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 12                                   );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3)                    );
  TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  kwk::for_each_index([&](auto e, auto i0, auto i1) { TTS_EQUAL(e, ref[i0+i1*sz]); }, v);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(sz-1,sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 12                                   );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4)                    );
  TTS_EXPECT_NOT( w.shape().is_fully_static                   );

  kwk::for_each_index([&](auto e, auto i0, auto i1) { TTS_EQUAL(e, ref[i0+i1*(sz-1)]); }, w);
};

TTS_CASE( "Build a 2D view with dynamic shape settings from std::array" )
{
  using namespace kwk::literals;

  std::array<float,12> ref = {1,2,3,4,5,6,7,8,9,10,11,12};

  std::ptrdiff_t sz = 4;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz,sz-1) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 12                                   );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3)                    );
  TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  kwk::for_each_index([&](auto e, auto i0, auto i1) { TTS_EQUAL(e, ref[i0+i1*sz]); }, v);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(sz-1,sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 12                                   );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4)                    );
  TTS_EXPECT_NOT( w.shape().is_fully_static                   );

  kwk::for_each_index([&](auto e, auto i0, auto i1) { TTS_EQUAL(e, ref[i0+i1*(sz-1)]); }, w);
};

TTS_CASE( "Build a 2D view with dynamic shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7,8,9,10,11,12};

  std::ptrdiff_t sz = 4;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz,sz-1) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 12                                   );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3)                    );
  TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  kwk::for_each_index([&](auto e, auto i0, auto i1) { TTS_EQUAL(e, ref[i0+i1*sz]); }, v);

  std::vector<float> const cref = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto w = kwk::view{ kwk::source = cref, kwk::of_size(sz-1,sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 12                                   );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4)                    );
  TTS_EXPECT_NOT( w.shape().is_fully_static                   );

  kwk::for_each_index([&](auto e, auto i0, auto i1) { TTS_EQUAL(e, ref[i0+i1*(sz-1)]); }, w);
};

TTS_CASE( "Build a 2D view with dynamic shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7,8,9,10,11,12};

  std::ptrdiff_t sz = 4;
  auto v = kwk::view{ kwk::source = ref.data(), kwk::of_size(sz,sz-1) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 12                                   );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3)                    );
  TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  kwk::for_each_index([&](auto e, auto i0, auto i1) { TTS_EQUAL(e, ref[i0+i1*sz]); }, v);

  std::vector<float> const cref = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto w = kwk::view{ kwk::source = cref.data(), kwk::of_size(sz-1,sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+2*sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 12                                   );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4)                    );
  TTS_EXPECT_NOT( w.shape().is_fully_static                   );

  kwk::for_each_index([&](auto e, auto i0, auto i1) { TTS_EQUAL(e, ref[i0+i1*(sz-1)]); }, w);
};
