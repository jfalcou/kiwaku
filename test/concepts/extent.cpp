//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/concepts/extent.hpp>

TTS_CASE( "Checks concepts::joker behavior" )
{
  TTS_CONSTEXPR_EXPECT    ( kwk::concepts::joker<decltype(kwk::_)>);
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::joker<double>          );
  TTS_CONSTEXPR_EXPECT_NOT( kwk::concepts::joker<void>            );
};

TTS_CASE( "Checks concepts::extent behavior" )
{
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::extent<int, std::ptrdiff_t>)   );
  TTS_CONSTEXPR_EXPECT    ((kwk::concepts::extent<decltype(kwk::_), int>) );
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::extent<const char*, int>)      );
  TTS_CONSTEXPR_EXPECT_NOT((kwk::concepts::extent<void       , int>)      );
};
