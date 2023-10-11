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
  template<typename Context, typename Func, concepts::container In>
  constexpr auto all_of(Context const& ctx, In const& in, Func f)
  {
    return ctx.all_of(in, f);
  }

  template<typename Func, concepts::container In>
  constexpr auto all_of(In const& in, Func f)
  {
    return all_of(cpu, in, f);
  }

  template<typename Context, typename Func, concepts::container In>
  constexpr auto any_of(Context const& ctx, In const& in, Func f)
  {
    return ctx.any_of(in, f);
  }

  template<typename Func, concepts::container In>
  constexpr auto any_of(In const& in, Func f)
  {
    return any_of(cpu, in, f);
    // return reduce(in, [f](auto a, auto e) {  return a || f(e); }, false );
  }

  template<typename Context, typename Func, concepts::container In>
  constexpr auto none_of(Context const& ctx, In const& in, Func f)
  {
    return ctx.none_of(in, f);
    // return !any_of(in, f);
  }

  template<typename Func, concepts::container In>
  constexpr auto none_of(In const& in, Func f)
  {
    return none_of(cpu, in, f);
  }

  template<typename Context, concepts::container In>
  constexpr auto count(Context const& ctx, In const& in, auto value)
  {
    return ctx.count(in, value);
  }

  template<concepts::container In>
  constexpr auto count(In const& in, auto value)
  {
    return count(cpu, in, value);
  }

  template<typename Context, typename Func, concepts::container In>
  constexpr auto count_if(Context const& ctx, In const& in, Func f)
  {
    return ctx.count_if(in, f);
    // return reduce(in, [f](auto a, auto e) {  return a + (f(e) ? 1 : 0); }, std::size_t{} );
  }

  template<typename Func, concepts::container In>
  constexpr auto count_if(In const& in, Func f)
  {
    return count_if(cpu, in, f);
  }
}
