//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include "tts.hpp"
#include <kwk/concepts/axis.hpp>
#include <kwk/settings.hpp>

TTS_CASE( "Checks concepts::axis behavior" )
{
  TTS_CONSTEXPR_EXPECT(kwk::concepts::axis<decltype(kwk::along<3>)>);
  TTS_CONSTEXPR_EXPECT(kwk::concepts::axis<decltype(kwk::height)>);
  TTS_CONSTEXPR_EXPECT(kwk::concepts::axis<decltype(kwk::axis<"phase">)>);
};

TTS_CASE( "Checks concepts::dynamic_axis behavior" )
{
  TTS_CONSTEXPR_EXPECT(kwk::concepts::dynamic_axis<decltype(kwk::along<3>)>);
  TTS_CONSTEXPR_EXPECT(kwk::concepts::dynamic_axis<decltype(kwk::height)>);
  TTS_CONSTEXPR_EXPECT(kwk::concepts::dynamic_axis<decltype(kwk::axis<"phase">)>);

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::dynamic_axis<decltype(kwk::along<3>[7])>);
  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::dynamic_axis<decltype(kwk::height[9])>);
  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::dynamic_axis<decltype(kwk::axis<"phase">[11])>);
};

TTS_CASE( "Checks concepts::static_axis behavior" )
{
  TTS_CONSTEXPR_EXPECT(kwk::concepts::static_axis<decltype(kwk::along<3>[7])>);
  TTS_CONSTEXPR_EXPECT(kwk::concepts::static_axis<decltype(kwk::height[9])>);
  TTS_CONSTEXPR_EXPECT(kwk::concepts::static_axis<decltype(kwk::axis<"phase">[11])>);

  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::static_axis<decltype(kwk::along<3>)>);
  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::static_axis<decltype(kwk::height)>);
  TTS_CONSTEXPR_EXPECT_NOT(kwk::concepts::static_axis<decltype(kwk::axis<"phase">)>);
};
