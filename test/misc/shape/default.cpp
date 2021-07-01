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

TTS_CASE( "Default constructed 0D shape behavior")
{
  auto sh = kwk::shape<kwk::_0D>();

  TTS_EQUAL(sh.nbdims() , 0 );
  TTS_EQUAL(sh.numel()  , 0 );
  TTS_EQUAL(sh.size()   , 0 );
};

TTS_CASE_TPL( "Default constructed shape behavior"
            , int_<1>, int_<2>, int_<3>, int_<4>, int_<7>, int_<13>
            )
<typename T>(::tts::type<T>)
{
  auto sh = kwk::shape<kwk::_nD<T::value>>();

  TTS_EQUAL(sh.nbdims() , 1       );
  TTS_EQUAL(sh.numel()  , 0       );
  TTS_EQUAL(sh.size()   , T::value);

  TTS_EQUAL(kwk::get<0>(sh), 0);
  if constexpr(T::value==1) TTS_EQUAL(kwk::get<T::value-1>(sh) , 0);
  else                      TTS_EQUAL(kwk::get<T::value-1>(sh) , 1);
};
