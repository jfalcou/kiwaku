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
#include <kwk/container/shape.hpp>

TTS_CASE( "Small shape can be used to construct large shape" )
{
  auto small = kwk::shape{4,2};
  auto large = kwk::shape<kwk::_4D>(small);

  TTS_EQUAL( large.get<0>(), small.get<0>() );
  TTS_EQUAL( large.get<1>(), small.get<1>() );
  TTS_EQUAL( large.get<2>(), 1              );
  TTS_EQUAL( large.get<3>(), 1              );
};

TTS_CASE( "Small shape can be assigned to large shape" )
{
  auto                  small = kwk::shape{4,2};
  kwk::shape<kwk::_4D>  large;

  large = small;

  TTS_EQUAL( large.get<0>(), small.get<0>() );
  TTS_EQUAL( large.get<1>(), small.get<1>() );
  TTS_EQUAL( large.get<2>(), 1              );
  TTS_EQUAL( large.get<3>(), 1              );
};

TTS_CASE( "Large shape need to be explicitly converted to small shape" )
{
  auto large = kwk::shape{4,2,6,9};
  auto small = kwk::shape<kwk::_2D>{large};

  TTS_EQUAL( small.get<0>(), large.get<0>() );
  TTS_EQUAL( small.get<1>(), large.get<1>()*large.get<2>()*large.get<3>() );
};

TTS_CASE( "Large shape need to be explicitly assigned to small shape" )
{
  auto                  large = kwk::shape{4,2,6,9};
  kwk::shape<kwk::_2D>  small;

  small = kwk::shape<kwk::_2D>{large};

  TTS_EQUAL( small.get<0>(), large.get<0>() );
  TTS_EQUAL( small.get<1>(), large.get<1>()*large.get<2>()*large.get<3>() );
};
