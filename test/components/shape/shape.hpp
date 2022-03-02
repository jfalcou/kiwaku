//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/components/shape.hpp>

template<int N, typename Indexes = std::make_index_sequence<N>>  struct sizes;

template<int N, std::size_t... I> struct sizes<N, std::index_sequence<I...>>
{
  using types_list = tts::types<std::integral_constant<std::size_t,I+1>...>;
};

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
