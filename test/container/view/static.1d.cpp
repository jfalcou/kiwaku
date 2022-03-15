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

TTS_CASE( "Build a 1D view from a C array" )
{
  using namespace kwk::literals;

  float ref[7] = {1,2,3,4,5,6,7};

  auto v = kwk::view{ kwk::source = ref };

  TTS_EQUAL ( sizeof(v), sizeof(void*)    );
  TTS_EQUAL ( v.size() , 7                );
  TTS_EQUAL ( v.shape(), kwk::of_size(7)  );
  TTS_EXPECT( v.shape().is_fully_static   );
  TTS_EXPECT( (std::equal( v.begin(), v.end(), &ref[0])) );

  for(std::size_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(4_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)    );
  TTS_EQUAL ( w.size() , 4                );
  TTS_EQUAL ( w.shape(), kwk::of_size(4)  );
  TTS_EXPECT( w.shape().is_fully_static   );
  TTS_EXPECT( (std::equal( w.begin(), w.end(), &ref[0])) );

  for(std::size_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};

TTS_CASE( "Build a 1D view from std::array" )
{
  using namespace kwk::literals;

  std::array<float,7> ref = {1,2,3,4,5,6,7};

  auto v = kwk::view{ kwk::source = ref };

  TTS_EQUAL ( sizeof(v), sizeof(void*)    );
  TTS_EQUAL ( v.size() , 7                );
  TTS_EQUAL ( v.shape(), kwk::of_size(7)  );
  TTS_EXPECT( v.shape().is_fully_static   );
  TTS_EXPECT( (std::equal( v.begin(), v.end(), &ref[0])) );

  for(std::size_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  auto w = kwk::view{ kwk::source = ref, kwk::of_size(4_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)    );
  TTS_EQUAL ( w.size() , 4                );
  TTS_EQUAL ( w.shape(), kwk::of_size(4)  );
  TTS_EXPECT( w.shape().is_fully_static   );
  TTS_EXPECT( (std::equal( w.begin(), w.end(), &ref[0])) );

  for(std::size_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};

TTS_CASE( "Build a 1D view with constexpr shape settings from a ContiguousRange" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7};

  auto v = kwk::view{ kwk::source = ref, kwk::of_size(7_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)    );
  TTS_EQUAL ( v.size() , 7                );
  TTS_EQUAL ( v.shape(), kwk::of_size(7)  );
  TTS_EXPECT( v.shape().is_fully_static   );
  TTS_EXPECT( (std::equal( v.begin(), v.end(), ref.begin())) );

  for(std::size_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  std::vector<float> const cref = {1,2,3,4,5,6,7};

  auto w = kwk::view{ kwk::source = cref, kwk::of_size(5_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)    );
  TTS_EQUAL ( w.size() , 5                );
  TTS_EQUAL ( w.shape(), kwk::of_size(5)  );
  TTS_EXPECT( w.shape().is_fully_static   );
  TTS_EXPECT( (std::equal( w.begin(), w.end(), ref.begin())) );

  for(std::size_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};


TTS_CASE( "Build a 1D view with constexpr shape settings from a pointer" )
{
  using namespace kwk::literals;

  std::vector<float> ref = {1,2,3,4,5,6,7};

  auto v = kwk::view{ kwk::source = ref.data(), kwk::of_size(7_c) };

  TTS_EQUAL ( sizeof(v), sizeof(void*)    );
  TTS_EQUAL ( v.size() , 7                );
  TTS_EQUAL ( v.shape(), kwk::of_size(7)  );
  TTS_EXPECT( v.shape().is_fully_static   );
  TTS_EXPECT( (std::equal( v.begin(), v.end(), ref.begin())) );

  for(std::size_t i = 0;i<v.size();++i)
    TTS_EQUAL(v(i), ref[i]);

  std::vector<float> const cref = {1,2,3,4,5,6,7};

  auto w = kwk::view{ kwk::source = cref.data(), kwk::of_size(5_c) };

  TTS_EQUAL ( sizeof(w), sizeof(void*)    );
  TTS_EQUAL ( w.size() , 5                );
  TTS_EQUAL ( w.shape(), kwk::of_size(5)  );
  TTS_EXPECT( w.shape().is_fully_static   );
  TTS_EXPECT( (std::equal( w.begin(), w.end(), ref.begin())) );

  for(std::size_t i = 0;i<w.size();++i)
    TTS_EQUAL(w(i), ref[i]);
};
