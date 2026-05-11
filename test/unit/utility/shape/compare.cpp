//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>

TTS_CASE("Equality comparison between shapes")
{
  using kwk::shape;

  TTS_EQUAL(shape{3}, (shape{3}));
  TTS_EQUAL((shape{3, 5}), (shape{3, 5}));
  TTS_EQUAL((shape{3, 5, 7}), (shape{3, 5, 7}));
  TTS_EQUAL((shape{3, 5, 7, 9}), (shape{3, 5, 7, 9}));
};

TTS_CASE("Inequality comparison between shapes")
{
  using kwk::shape;

  TTS_NOT_EQUAL(shape{3}, shape{7});
  TTS_NOT_EQUAL(shape{3}, (shape{1, 3}));
  TTS_NOT_EQUAL(shape{3}, (shape{1, 1, 3}));
  TTS_NOT_EQUAL(shape{3}, (shape{1, 1, 1, 3}));

  TTS_NOT_EQUAL((shape{3, 5}), (shape{15}));
  TTS_NOT_EQUAL((shape{3, 5}), (shape{5, 3}));
  TTS_NOT_EQUAL((shape{3, 5}), (shape{1, 3, 5}));
  TTS_NOT_EQUAL((shape{3, 5}), (shape{1, 1, 3, 5}));

  TTS_NOT_EQUAL((shape{3, 5, 7}), (shape{105}));
  TTS_NOT_EQUAL((shape{3, 5, 7}), (shape{15, 7}));
  TTS_NOT_EQUAL((shape{3, 5, 7}), (shape{1, 3, 5, 7}));

  TTS_NOT_EQUAL((shape{3, 5, 7, 9}), (shape{945}));
  TTS_NOT_EQUAL((shape{3, 5, 7, 9}), (shape{105, 9}));
  TTS_NOT_EQUAL((shape{3, 5, 7, 9}), (shape{15, 7, 9}));
};

TTS_CASE("Inequality comparison between squeezed shapes")
{
  using kwk::shape;
  using namespace kwk::literals;

  TTS_EQUAL(shape{3}, kwk::squeeze(shape{1_c, 3}));
  TTS_EQUAL(shape{3}, kwk::squeeze(shape{3, 1_c}));
  TTS_NOT_EQUAL(shape{3}, kwk::squeeze(shape{3, 1, 1_c}));
  TTS_NOT_EQUAL(shape{3}, kwk::squeeze(shape{1_c, 1, 3}));

  TTS_NOT_EQUAL((shape{3, 3}), kwk::squeeze(shape{3, 1, 3}));
  TTS_EQUAL((shape{3, 3}), kwk::squeeze(shape{3, 1_c, 3}));
};

// Should filling with leading ones be implicit or a function ?
/*
using namespace kwk::literals;

TTS_CASE( "Small shape can be used to construct large shape" )
{
  auto small = kwk::shape<_2D>{4,2};
  auto large = kwk::broadcast_to(small, kwk::shape<_4D>{});

  TTS_EQUAL( large, (kwk::shape{1,1,get<0>(small),get<1>(small)}) );
};

TTS_CASE( "Small shape can be assigned to large shape" )
{
  auto      small = kwk::_2D{4,2};
  kwk::_4D  large;

  large = small;

  TTS_EQUAL( large, kwk::of_size(1,1,small[0],small[1]) );
};

TTS_CASE( "Large shape need to be explicitly converted to small shape" )
{
  auto large = kwk::_4D{4,2,6,9};
  auto small = kwk::_2D{large};

  TTS_EQUAL( small[0], large[0]*large[1]*large[2] );
  TTS_EQUAL( small[1], large[3] );
};

TTS_CASE( "Large shape need to be explicitly assigned to small shape" )
{
  auto    large = kwk::_4D{4,2,6,9};
  kwk::_2D small;

  small = kwk::_2D{large};

  TTS_EQUAL( small[0], large[0]*large[1]*large[2] );
  TTS_EQUAL( small[1], large[3] );
};
*/
