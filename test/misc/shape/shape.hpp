//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef SHAPE_SHAPE_HPP
#define SHAPE_SHAPE_HPP

#include <kiwaku/shape.hpp>

template<typename Shape> inline void test_0D(Shape const& sh)
{
  TTS_EQUAL(sh.nbdims() , 0 );
  TTS_EQUAL(sh.numel()  , 0 );
  TTS_EQUAL(sh.size()   , 0 );
}

template<typename Size, typename Shape> inline void test_nD(Size const&, Shape const& sh)
{
  TTS_EQUAL(sh.size(), Size::value  );

  TTS_EQUAL(kwk::get<0>(sh)                   , 3             );
  TTS_EQUAL(kwk::get<Shape::static_size-1>(sh), 3*Size::value );
}

#endif
