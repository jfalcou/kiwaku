//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/shape.hpp>
#include "container/shape/shape.hpp"

TTS_CASE_TPL( "Default constructed shape behavior", sizes<10>)
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

TTS_CASE_TPL( "Building a nD shape with kwk::shape{a1,...,an}", sizes<10>)
<typename T>(::tts::type<T>)
{
  auto f  = []<std::size_t... Idx>(std::index_sequence<Idx...> const&)
            {
              return kwk::shape{3*(1+Idx)...};
            };

  test_nD(T{}, f(up_to<T>{}));
};
