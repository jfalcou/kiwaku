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
  template<typename Context, typename Func, concepts::container In>
  constexpr auto reduce(Context const& ctx, In const& in, Func f, auto init)
  {
    // kwk::for_each([&](auto... is) { init = f(init, in(is...)); }, in.shape() );
    return ctx.reduce(in, f, init);
  }

  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f, auto init)
  {
    return kwk::reduce(cpu, in, f, init);
  }

  template<typename Context, typename Func, concepts::container In>
  constexpr auto reduce(Context const& ctx, In const& in, Func f)
  {
    return ctx.reduce(in, f, typename In::value_type{});
  }
  
  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f)
  {
    return kwk::reduce(cpu, in, f);
  }

  template<typename Context, concepts::container In>
  constexpr auto reduce(Context const& ctx, In const& in)
  {
    return reduce(ctx, in, [](auto a, auto e) { return a+e; });
  }

  template<concepts::container In>
  constexpr auto reduce(In const& in)
  {
    return kwk::reduce(cpu, in);
  }
}
