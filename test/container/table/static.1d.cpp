//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/container.hpp>
#include <algorithm>
#include <array>
#include <vector>

TTS_CASE( "Build a 1D table from a C array" )
{
  using namespace kwk::literals;

  float ref[5] = {1,2,3,4,5};

  auto v = kwk::table{ kwk::source = ref };

  TTS_EQUAL ( v.size() , 5                      );
  TTS_EQUAL ( sizeof(v), sizeof(ref[0])*v.size());
  TTS_EQUAL ( v.shape(), kwk::of_size(5)        );
  TTS_EXPECT( v.shape().is_fully_static         );

  TTS_NOT_EQUAL( &v(0), &ref[0] );
  for(std::ptrdiff_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  auto w = kwk::table{ kwk::source = ref, kwk::of_size(4_c) };

  TTS_EQUAL ( w.size() , 4                      );
  TTS_EQUAL ( sizeof(w), sizeof(ref[0])*w.size());
  TTS_EQUAL ( w.shape(), kwk::of_size(4)        );
  TTS_EXPECT( w.shape().is_fully_static         );

  TTS_NOT_EQUAL( &w(0), &ref[0] );
  for(std::ptrdiff_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};

TTS_CASE( "Build a 1D table from std::array" )
{
  using namespace kwk::literals;

  std::array<float,7> ref = {1,2,3,4,5,6,7};

  auto v = kwk::table{ kwk::source = ref };

  TTS_EQUAL ( v.size() , 7                      );
  TTS_EQUAL ( sizeof(v), sizeof(ref[0])*v.size());
  TTS_EQUAL ( v.shape(), kwk::of_size(7)        );
  TTS_EXPECT( v.shape().is_fully_static         );

  TTS_NOT_EQUAL( &v(0), &ref[0] );
  for(std::ptrdiff_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  auto w = kwk::table{ kwk::source = ref, kwk::of_size(4_c) };

  TTS_EQUAL ( w.size() , 4                      );
  TTS_EQUAL ( sizeof(w), sizeof(ref[0])*w.size());
  TTS_EQUAL ( w.shape(), kwk::of_size(4)        );
  TTS_EXPECT( w.shape().is_fully_static         );

  TTS_NOT_EQUAL( &w(0), &ref[0] );
  for(std::ptrdiff_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};

TTS_CASE( "Build a 1D table with constexpr shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7};

  auto v = kwk::table{ kwk::source = ref, kwk::of_size(7_c) };

  TTS_EQUAL ( v.size() , 7                        );
  TTS_EQUAL ( sizeof(v), sizeof(ref[0]) * v.size());
  TTS_EQUAL ( v.shape(), kwk::of_size(7)          );
  TTS_EXPECT( v.shape().is_fully_static           );

  TTS_NOT_EQUAL( &v(0), &ref[0] );
  for(std::ptrdiff_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  std::vector<float> const cref = {1,2,3,4,5,6,7};

  auto w = kwk::table{ kwk::source = cref, kwk::of_size(5_c) };

  TTS_EQUAL ( w.size() , 5                          );
  TTS_EQUAL ( sizeof(w), sizeof(cref[0]) * w.size() );
  TTS_EQUAL ( w.shape(), kwk::of_size(5)            );
  TTS_EXPECT( w.shape().is_fully_static             );

  TTS_NOT_EQUAL( &w(0), &cref[0] );
  for(std::ptrdiff_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};

TTS_CASE( "Build a 1D table with constexpr shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7};

  auto v = kwk::table{ kwk::source = ref.data(), kwk::of_size(7_c) };

  TTS_EQUAL ( v.size() , 7                );
  TTS_EQUAL ( sizeof(v), sizeof(ref[0])*v.size());
  TTS_EQUAL ( v.shape(), kwk::of_size(7)  );
  TTS_EXPECT( v.shape().is_fully_static   );

  TTS_NOT_EQUAL( &v(0), &ref[0] );
  for(std::ptrdiff_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  std::vector<float> const cref = {1,2,3,4,5,6,7};

  auto w = kwk::table{ kwk::source = cref.data(), kwk::of_size(5_c) };

  TTS_EQUAL ( w.size() , 5                );
  TTS_EQUAL ( sizeof(w), sizeof(cref[0])*w.size());
  TTS_EQUAL ( w.shape(), kwk::of_size(5)  );
  TTS_EXPECT( w.shape().is_fully_static   );

  TTS_NOT_EQUAL( &w(0), &cref[0] );
  for(std::ptrdiff_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};
