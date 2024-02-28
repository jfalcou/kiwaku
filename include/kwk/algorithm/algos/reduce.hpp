//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
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
  // TODO: Trouver les bons algos en SYCL ?

  // Reduce is not a required part of Contexts (unlike map)
  // A custom overloaded reduce(my_context, ...) is required to use a custom context.
  // TODO reduce: soit on fait ctx.reduce, soit dans le contexte
  // sycl on réécrit la fonction reduce avec sycl_context.
  // transform and reduce are members of base_context
  template<typename Context, typename Func, concepts::container In>
  constexpr auto reduce(Context& ctx, In const& in, Func f, auto init)
  {
    ctx.map( [&](auto const& i) { init = f(init, i); }
                , ctx.in(in)
                );
    return init;
    // return ctx.reduce(in, f, init);
    // for_each(ctx, [&](auto... is) { init = f(init, in(is...)); }, in.shape() );
    // return init;
  }
  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f, auto init)
  {
    return kwk::reduce(cpu, in, f, init);
  }

  template<typename Context, typename Func, concepts::container In>
  constexpr auto reduce(Context& ctx, In const& in, Func f)
  {
    return kwk::reduce(ctx, in, f, typename In::value_type{});
  }
  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f)
  {
    return kwk::reduce(cpu, in, f);
  }

  template<typename Context, concepts::container In>
  constexpr auto reduce(Context& ctx, In const& in)
  {
    return kwk::reduce(ctx, in, [](auto a, auto e) { return a+e; });
  }
  template<concepts::container In>
  constexpr auto reduce(In const& in)
  {
    return kwk::reduce(cpu, in);
  }
}
