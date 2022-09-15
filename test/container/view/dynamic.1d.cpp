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

TTS_CASE( "Build a 1D view with dynamic shape settings from a C array" )
{
  using namespace kwk::literals;

  float ref[7] = {1,2,3,4,5,6,7};

  std::ptrdiff_t sz = 7;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 7                                    );
  TTS_EQUAL ( v.shape(), kwk::of_size(7)                      );
  TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  for(std::ptrdiff_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  sz = 4;
  auto w = kwk::view{ kwk::source = ref, kwk::of_size(sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 4                                    );
  TTS_EQUAL ( w.shape(), kwk::of_size(4)                      );
  TTS_EXPECT_NOT( w.shape().is_fully_static                   );

  for(std::ptrdiff_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};

TTS_CASE( "Build a 1D view with dynamic shape settings from std::array" )
{
  using namespace kwk::literals;

  std::array<float,7> ref = {1,2,3,4,5,6,7};

  std::ptrdiff_t sz = 7;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 7                                    );
  TTS_EQUAL ( v.shape(), kwk::of_size(7)                      );
  TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  for(std::ptrdiff_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  sz = 4;
  auto w = kwk::view{ kwk::source = ref, kwk::of_size(sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 4                                    );
  TTS_EQUAL ( w.shape(), kwk::of_size(4)                      );
  TTS_EXPECT_NOT( w.shape().is_fully_static                   );

  for(std::ptrdiff_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};

TTS_CASE( "Build a 1D view with dynamic shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7};

  std::ptrdiff_t sz = 7;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 7                                    );
  TTS_EQUAL ( v.shape(), kwk::of_size(7)                      );
  TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  for(std::ptrdiff_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  sz = 5;
  std::vector<float> const cref = {1,2,3,4,5,6,7};

  auto w = kwk::view{ kwk::source = cref, kwk::of_size(sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 5                                    );
  TTS_EQUAL ( w.shape(), kwk::of_size(5)                      );
  TTS_EXPECT_NOT( w.shape().is_fully_static                   );

  for(std::ptrdiff_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};

TTS_CASE( "Build a 1D view with dynamic shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7};

  std::ptrdiff_t sz = 7;
  auto v = kwk::view{ kwk::source = ref.data(), kwk::of_size(sz) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)+sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( v.size() , 7                                    );
  TTS_EQUAL ( v.shape(), kwk::of_size(7)                      );
  TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  for(std::ptrdiff_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  std::vector<float> const cref = {1,2,3,4,5,6,7};

  sz = 5;
  auto w = kwk::view{ kwk::source = cref.data(), kwk::of_size(sz) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)+sizeof(std::ptrdiff_t) );
  TTS_EQUAL ( w.size() , 5                                    );
  TTS_EQUAL ( w.shape(), kwk::of_size(5)                      );
  TTS_EXPECT_NOT( w.shape().is_fully_static                   );

  for(std::ptrdiff_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};
