//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE( "Check as_dimension basic behavior" )
{
  using namespace kwk;

  TTS_EQUAL( kwk::as_dimension(kwk::_, 33)              , 33  );
  TTS_EQUAL( kwk::as_dimension(42, 0)                   , 42  );
  TTS_EQUAL( kwk::as_dimension(kwk::width = 63, 0)      , 63  );
  TTS_EQUAL( kwk::as_dimension(kwk::along<6> = 13, 0)   , 13  );
};

struct special
{
  friend constexpr auto tag_invoke(kwk::tags::as_dimension, special, auto) { return 11; }
};

TTS_CASE( "Check as_dimension customized behavior" )
{
  using namespace kwk;

  TTS_EQUAL( kwk::as_dimension(special{}, 0), 11);
};
