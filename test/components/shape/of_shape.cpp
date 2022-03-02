//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================

#include "test.hpp"
#include "components/shape/shape.hpp"

TTS_CASE( "Building a 0D shape with kwk::of_shape()" )
{
  test_0D(kwk::extent());
};

TTS_CASE_TPL( "Building a nD shape with kwk::of_shape(a1,...,an)"
            , nD<1>, nD<2>, nD<3>, nD<4>, nD<7>, nD<13>
            )
<typename T>(::tts::type<T>)
{
  auto f  = []<std::size_t... Idx>(std::index_sequence<Idx...> const&)
            {
              return kwk::extent(3*(1+Idx)...);
            };

  test_nD(T{}, f(up_to<T>{}));
};
