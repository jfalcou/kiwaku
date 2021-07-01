//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include "misc/shape/shape.hpp"

TTS_CASE( "Building a 0D shape with kwk::of_shape()" )
{
  test_0D(kwk::of_shape());
};

TTS_CASE_TPL( "Building a nD shape with kwk::of_shape(a1,...,an)"
            , nD<1>, nD<2>, nD<3>, nD<4>, nD<7>, nD<13>
            )
<typename T>(::tts::type<T>)
{
  auto f  = []<std::size_t... Idx>(std::index_sequence<Idx...> const&)
            {
              return kwk::of_shape(3*(1+Idx)...);
            };

  test_nD(T{}, f(up_to<T>{}));
};
