//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

#include "test.hpp"
#include "components/shape/shape.hpp"

TTS_CASE_TPL( "Building a nD shape with kwk::of_shape(a1,...,an)", sizes<10>)
<typename T>(::tts::type<T>)
{
  auto f  = []<std::size_t... Idx>(std::index_sequence<Idx...> const&)
            {
              return kwk::extent(3*(1+Idx)...);
            };

  test_nD(T{}, f(up_to<T>{}));
};
