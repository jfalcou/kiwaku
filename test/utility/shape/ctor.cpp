//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>
#include "utility/shape/shape.hpp"

TTS_CASE_TPL( "Building a nD shape with kwk::kwk::shape{a1,...,an}", sizes<10>)
<typename T>(::tts::type<T>)
{
  auto f  = []<std::size_t... Idx>(std::index_sequence<Idx...> const&)
            {
              return kwk::shape( 3*static_cast<std::ptrdiff_t>(1+Idx)...);
            };

  auto sh = f(up_to<T>{});
  TTS_EQUAL(sh.order(), T::value  );

  for(int i =0;i<T::value;++i)
    TTS_EQUAL(sh[i], 3*(i+1) );
};
