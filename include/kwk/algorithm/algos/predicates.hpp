//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <algorithm>
#include <kwk/concepts/container.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

//ALGO ::
/*
ends_with -> sous vue de k jusqu'à la fin
starts_with -> sous vue de k depuis le début (interprété 1D ?)
*/

namespace kwk
{
  template<typename Func, concepts::container In>
  constexpr auto all_of(In const& in, Func f)
  {
    return reduce(in, [f](auto a, auto e) {  return a && f(e); }, true );
  }

  template<typename Func, concepts::container In>
  constexpr auto any_of(In const& in, Func f)
  {
    return reduce(in, [f](auto a, auto e) {  return a || f(e); }, false );
  }

  template<typename Func, concepts::container In>
  constexpr auto none_of(In const& in, Func f)
  {
    return !any_of(in, f);
  }

  template< concepts::container In>
  constexpr auto count(In const& in, auto value)
  {
    return reduce(in, [value](auto a, auto e) {  return a + ((e==value) ? 1 : 0); }, std::size_t{} );
  }

  template<typename Func, concepts::container In>
  constexpr auto count_if(In const& in, Func f)
  {
    return reduce(in, [f](auto a, auto e) {  return a + (f(e) ? 1 : 0); }, std::size_t{} );
  }
}
