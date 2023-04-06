//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE( "Check to_descriptor basic behavior" )
{
  using namespace kwk;

  TTS_EQUAL( kwk::to_descriptor(kwk::_)               , kwk::_            );
  TTS_EQUAL( kwk::to_descriptor(42)                   , kwk::_            );
  TTS_EQUAL( kwk::to_descriptor(kwk::fixed<37>)       , 37                );
  TTS_EQUAL( kwk::to_descriptor(kwk::width = 63)      , kwk::width        );
  TTS_EQUAL( kwk::to_descriptor(kwk::along<6> = 13)   , kwk::along<6>     );
  TTS_EQUAL( kwk::to_descriptor(kwk::along<3> = 13_c) , kwk::along<3>[13] );
};

struct special
{
  friend constexpr auto tag_invoke(kwk::tags::to_descriptor, special) { return 11; }
};

TTS_CASE( "Check to_descriptor customized behavior" )
{
  using namespace kwk;

  TTS_EQUAL( kwk::to_descriptor(special{}), 11);
};
