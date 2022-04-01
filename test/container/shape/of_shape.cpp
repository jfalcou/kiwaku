//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

#include "test.hpp"
#include "container/shape/shape.hpp"

TTS_CASE_TPL( "Building a nD shape with kwk::of_shape(a1,...,an)", sizes<10>)
<typename T>(::tts::type<T>)
{
  auto f  = []<std::size_t... Idx>(std::index_sequence<Idx...> const&)
            {
              return kwk::of_size( 3*static_cast<std::ptrdiff_t>(1+Idx)...);
            };

  test_nD(T{}, f(up_to<T>{}));
};
