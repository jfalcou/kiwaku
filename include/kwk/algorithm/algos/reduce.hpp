//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

//ALGO:
/*
min_location, max_location, minmax_location -> reduce avec min ou max et conserver l'index -> renvoi valeur et indice
lambla = std::min std::max
min, max, minmax -> valeurs
*/

namespace kwk
{
  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f, auto init)
  {
    kwk::for_each([&](auto... is) { init = f(init, in(is...)); }, in.shape() );
    return init;
  }

  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f)
  {
    return kwk::reduce(in, f, typename In::value_type{});
  }

  template<concepts::container In>
  constexpr auto reduce(In const& in)
  {
    return kwk::reduce(in, [](auto a, auto e) { return a+e; });
  }
}
