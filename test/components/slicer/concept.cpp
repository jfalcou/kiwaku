//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/misc/slicers.hpp>
#include <kwk/concept/slicer.hpp>

TTS_CASE( "kwk::_ is a contiguous, unit slicer" )
{
  TTS_EXPECT( (kwk::concepts::contiguous_slicer<decltype(kwk::_),0>) );
  TTS_EXPECT( (kwk::concepts::contiguous_slicer<decltype(kwk::_),7>) );
  TTS_EXPECT( kwk::concepts::unit_slicer<decltype(kwk::_)>           );
};

TTS_CASE( "kwk::_(b,e) is a non-contiguous, unit slicer" )
{
  TTS_EXPECT_NOT( (kwk::concepts::contiguous_slicer<decltype(kwk::_(0,7)),0>) );
  TTS_EXPECT_NOT( (kwk::concepts::contiguous_slicer<decltype(kwk::_(0,7)),7>) );
  TTS_EXPECT( kwk::concepts::unit_slicer<decltype(kwk::_(0,7))>               );
};

TTS_CASE( "kwk::_(b,s,e) is a non-contiguous, non-unit slicer" )
{
  TTS_EXPECT_NOT( (kwk::concepts::contiguous_slicer<decltype(kwk::_(0,2,7)),0>) );
  TTS_EXPECT_NOT( (kwk::concepts::contiguous_slicer<decltype(kwk::_(0,2,7)),7>) );
  TTS_EXPECT_NOT( kwk::concepts::unit_slicer<decltype(kwk::_(0,2,7))>           );
};

TTS_CASE( "kwk::_[d] is a non-contiguous, non-unit slicer" )
{
  TTS_EXPECT_NOT( (kwk::concepts::contiguous_slicer<decltype(kwk::_[1]),0>) );
  TTS_EXPECT_NOT( (kwk::concepts::contiguous_slicer<decltype(kwk::_[1]),7>) );
  TTS_EXPECT_NOT( kwk::concepts::unit_slicer<decltype(kwk::_[1])>           );
};

TTS_CASE( "Any raw index is a maybe-contiguous, unit slicer" )
{
  TTS_EXPECT_NOT( (kwk::concepts::contiguous_slicer<decltype(1),0>) );
  TTS_EXPECT( (kwk::concepts::contiguous_slicer<decltype(2),7>)     );
  TTS_EXPECT( kwk::concepts::unit_slicer<decltype(3)>               );
};
