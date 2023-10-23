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

  // transform and reduce are members of base_context
  template<typename Context, typename Func, concepts::container In>
  constexpr auto reduce(Context& ctx, In const& in, Func f, auto init)
  {
    return ctx.reduce(in, f, init);
    // for_each(ctx, [&](auto... is) { init = f(init, in(is...)); }, in.shape() );
    // return init;
  }
  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f, auto init)
  {
    return reduce(cpu, in, f, init);
  }

  template<typename Context, typename Func, concepts::container In>
  constexpr auto reduce(Context& ctx, In const& in, Func f)
  {
    return reduce(ctx, in, f, typename In::value_type{});
  }
  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f)
  {
    return reduce(cpu, in, f);
  }

  template<typename Context, concepts::container In>
  constexpr auto reduce(Context& ctx, In const& in)
  {
    return reduce(ctx, in, [](auto a, auto e) { return a+e; });
  }
  template<concepts::container In>
  constexpr auto reduce(In const& in)
  {
    return reduce(cpu, in);
  }
}
