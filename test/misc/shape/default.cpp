//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/shape.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/basic.hpp>

template<int N> using int_ = std::integral_constant<int,N>;

TTS_CASE( "Default constructed 0D shape behavior")
{
  auto sh = kwk::_0D();

  TTS_EQUAL(sh.nbdims() , 0   );
  TTS_EQUAL(sh.numel()  , 0   );
  TTS_EQUAL(sh.count()  , 0   );
  TTS_EQUAL(sh.size()   , 0ULL);

  TTS_EQUAL(sh.end() , sh.begin());
  TTS_EQUAL(sh.data(), nullptr   );
}

TTS_CASE_TPL( "Default constructed shape behavior"
            , int_<1>, int_<2>, int_<3>, int_<4>, int_<7>, int_<13>
            )
{
  auto sh = kwk::_nD<T::value>();

  TTS_EQUAL(sh.nbdims() , 1                                 );
  TTS_EQUAL(sh.numel()  , 0                                 );
  TTS_EQUAL(sh.count()  , T::value                          );
  TTS_EQUAL(sh.size()   , static_cast<std::size_t>(T::value));

  TTS_EQUAL(sh.front(), 0);
  if constexpr(T::value==1) TTS_EQUAL(sh.back() , 0);
  else                      TTS_EQUAL(sh.back() , 1);

  TTS_NOT_EQUAL ( sh.begin(), nullptr );
  TTS_EQUAL     (sh.end()  , sh.begin() + T::value );
  TTS_NOT_EQUAL ( sh.data(), nullptr );
}
