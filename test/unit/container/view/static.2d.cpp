//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/container.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <array>
#include <vector>

TTS_CASE( "Build a 2D view from a C array" )
{
  using namespace kwk::literals;

  float ref[] = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto v = kwk::view{ kwk::source = ref, kwk::of_size(4_c, 3_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)    );
  TTS_EQUAL ( v.size() , 12               );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3));
  TTS_EXPECT( v.shape().is_fully_static   );

  kwk::for_each([i=0ULL,&ref](auto e) mutable { TTS_EQUAL(e, ref[i++]); }, v);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(3_c, 4_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)    );
  TTS_EQUAL ( w.size() , 12               );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4));
  TTS_EXPECT( w.shape().is_fully_static   );

  kwk::for_each([i=0ULL,&ref](auto e) mutable { TTS_EQUAL(e, ref[i++]); }, w);
};

TTS_CASE( "Build a 2D view from std::array" )
{
  using namespace kwk::literals;

  std::array<float,12> ref = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto v = kwk::view{ kwk::source = ref, kwk::of_size(4_c, 3_c)};

  TTS_EQUAL ( sizeof(v), sizeof(void*)    );
  TTS_EQUAL ( v.size() , 12               );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3));
  TTS_EXPECT( v.shape().is_fully_static   );

  kwk::for_each([i=0ULL,&ref](auto e) mutable { TTS_EQUAL(e, ref[i++]); }, v);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(3_c, 4_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)    );
  TTS_EQUAL ( w.size() , 12               );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4));
  TTS_EXPECT( w.shape().is_fully_static   );

  kwk::for_each([i=0ULL,&ref](auto e) mutable { TTS_EQUAL(e, ref[i++]); }, w);
};

TTS_CASE( "Build a 2D view with constexpr shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto v = kwk::view{ kwk::source = ref, kwk::of_size(4_c, 3_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)    );
  TTS_EQUAL ( v.size() , 12               );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3));
  TTS_EXPECT( v.shape().is_fully_static   );

  kwk::for_each([i=0ULL,&ref](auto e) mutable { TTS_EQUAL(e, ref[i++]); }, v);

  std::vector<float> const cref = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto w = kwk::view{ kwk::source = cref, kwk::of_size(3_c, 4_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)    );
  TTS_EQUAL ( w.size() , 12               );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4));
  TTS_EXPECT( w.shape().is_fully_static   );

  kwk::for_each([i=0ULL,&ref](auto e) mutable { TTS_EQUAL(e, ref[i++]); }, w);
};


TTS_CASE( "Build a 2D view with constexpr shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto v = kwk::view{ kwk::source = ref.data(), kwk::of_size(4_c, 3_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)    );
  TTS_EQUAL ( v.size() , 12               );
  TTS_EQUAL ( v.shape(), kwk::of_size(4,3));
  TTS_EXPECT( v.shape().is_fully_static   );

  kwk::for_each([i=0ULL,&ref](auto e) mutable { TTS_EQUAL(e, ref[i++]); }, v);

  std::vector<float> const cref = {1,2,3,4,5,6,7,8,9,10,11,12};

  auto w = kwk::view{ kwk::source = cref.data(), kwk::of_size(3_c, 4_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)    );
  TTS_EQUAL ( w.size() , 12               );
  TTS_EQUAL ( w.shape(), kwk::of_size(3,4));
  TTS_EXPECT( w.shape().is_fully_static   );

  kwk::for_each([i=0ULL,&ref](auto e) mutable { TTS_EQUAL(e, ref[i++]); }, w);
};
