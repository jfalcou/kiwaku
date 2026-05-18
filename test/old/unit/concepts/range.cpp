//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/concepts/range.hpp>
#include <array>
#include <forward_list>
#include <list>
#include <vector>

TTS_CASE( "Checks concepts::range behavior" )
{
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::range<std::array<int,8>>)      );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::range<std::vector<float>>      );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::range<std::list<void*>>        );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::range<std::forward_list<void*>>);
};

TTS_CASE( "Checks concepts::contiguous_range behavior" )
{
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::contiguous_range<std::array<int,8>>)      );
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::contiguous_range<std::vector<float>>      );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::contiguous_range<std::list<void*>>        );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::contiguous_range<std::forward_list<void*>>);
};

TTS_CASE( "Checks concepts::contiguous_static_range behavior" )
{
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::contiguous_static_range<std::array<int,8>>)      );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::contiguous_static_range<std::vector<float>>      );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::contiguous_static_range<std::list<void*>>        );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::contiguous_static_range<std::forward_list<void*>>);
};
