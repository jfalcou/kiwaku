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

// Tets views of size 0
TTS_CASE( "Size 0: Build a 1D view with dynamic shape settings from a C array" )
{
  using namespace kwk::literals;

  // TODO: does not compile if the size of ref is zero.
  float ref[1] = {5};
  std::size_t sz = 0;
  auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz) };
  TTS_EQUAL ( sizeof(v), sizeof(void*)+sizeof(std::size_t) );
  TTS_EQUAL ( v.size() , 0ULL                              );
  TTS_EQUAL ( v.shape(), kwk::of_size(sz)                  );
  TTS_EXPECT_NOT( v.shape().is_fully_static                );
};

// TTS_CASE( "Size 0: Build a 1D view with dynamic shape settings from std::array" )
// {
//   using namespace kwk::literals;

//   std::array<float, 0> ref = {};
//   std::size_t sz = 0;
//   auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz) };
//   TTS_EQUAL ( sizeof(v), sizeof(void*)+sizeof(std::size_t) );
//   TTS_EQUAL ( v.size() , 0ULL                              );
//   TTS_EQUAL ( v.shape(), kwk::of_size(sz)                  );
//   TTS_EXPECT_NOT( v.shape().is_fully_static                );
// };

// TTS_CASE( "Size 0: Build a 1D view with dynamic shape settings from a ContiguousRange" )
// {
//   using namespace kwk::literals;

//   std::vector<float> ref = {};
//   std::size_t sz = 0;
//   auto v = kwk::view{ kwk::source = ref, kwk::of_size(sz) };
//   TTS_EQUAL ( sizeof(v), sizeof(void*)+sizeof(std::size_t) );
//   TTS_EQUAL ( v.size() , 0ULL                              );
//   TTS_EQUAL ( v.shape(), kwk::of_size(sz)                  );
//   TTS_EXPECT_NOT( v.shape().is_fully_static                );
// };

// TTS_CASE( "Size 0: Build a 1D view with dynamic shape settings from a pointer" )
// {
//   using namespace kwk::literals;

//   std::vector<float> ref = {};
//   std::size_t sz = 0;
//   auto v = kwk::view{ kwk::source = ref.data(), kwk::of_size(sz) };
//   TTS_EQUAL ( sizeof(v), sizeof(void*)+sizeof(std::size_t) );
//   TTS_EQUAL ( v.size() , 0ULL                              );
//   TTS_EQUAL ( v.shape(), kwk::of_size(sz)                  );
//   TTS_EXPECT_NOT( v.shape().is_fully_static                );
// };
