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
#include <tts/tests/relation.hpp>
#include <tts/tests/basic.hpp>

template<typename Env, typename Shape>
inline void test_0D(Env& runtime, Shape const& sh)
{
  TTS_EQUAL(sh.nbdims() , 0   );
  TTS_EQUAL(sh.numel()  , 0   );
  TTS_EQUAL(sh.count()  , 0   );
  TTS_EQUAL(sh.size()   , 0ULL);
}

template<typename Env, typename Size, typename Shape>
inline void test_nD(Env& runtime, Size const&, Shape const& sh)
{
  TTS_EQUAL(sh.count(), Size::value                           );
  TTS_EQUAL(sh.size() , static_cast<std::size_t>(Size::value) );

  TTS_EQUAL(kwk::get<0>(sh)                   , 3             );
  TTS_EQUAL(kwk::get<Shape::static_size-1>(sh), 3*Size::value );
}

#endif
