//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
// #include <kwk/context/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{
  template<typename Context, concepts::container Out>
  constexpr auto fill(Context& ctx, Out& out, auto value)
  {
    // TODO: but back kwk:: namespace before function call
    for_each(ctx, [&](auto... is) { out(is...) = value; }, out.shape() );
  }
  template<concepts::container Out>
  constexpr auto fill(Out& out, auto value)
  {
    // TODO: but back kwk:: namespace before function call
    fill(cpu, out, value);
  }


  template<typename Context, typename Func, concepts::container Out>
  constexpr auto generate(Context& ctx, Func f, Out& out)
  {
    // TODO: but back kwk:: namespace before function call
    for_each(ctx, [&](auto... is) { out(is...) = f(is...); }, out.shape() );
  }
  template<typename Func, concepts::container Out>
  constexpr auto generate(Func f, Out& out)
  {
    // TODO: but back kwk:: namespace before function call
    generate(cpu, f, out);
  }


  template<typename Context, concepts::container Out>
  constexpr auto iota(Context& ctx, Out& out, auto value)
  {
    // TODO: but back kwk:: namespace before function call
    // TODO: maybe call iota(ctx, out, value, step = 1) ?
    for_each(ctx, [&](auto... is) { out(is...) = value++; }, out.shape() );
  }
  template<concepts::container Out>
  constexpr auto iota(Out& out, auto value)
  {
    // TODO: but back kwk:: namespace before function call
    iota(cpu, out, value);
  }


  template<typename Context, concepts::container Out>
  constexpr auto iota(Context& ctx, Out& out)
  {
    // TODO: but back kwk:: namespace before function call
    using type = typename Out::value_type;
    iota(ctx, out, type{});
  }
  template<concepts::container Out>
  constexpr auto iota(Out& out)
  {
    // TODO: but back kwk:: namespace before function call
    iota(cpu, out);
  }


  template<typename Context, concepts::container Out>
  constexpr auto iota(Context& ctx, Out& out, auto value, auto step)
  {
    // TODO: but back kwk:: namespace before function call
    for_each(ctx, [&](auto... is) { out(is...) = value; value+=step; }, out.shape() );
  }
  template<concepts::container Out>
  constexpr auto iota(Out& out, auto value, auto step)
  {
    // TODO: but back kwk:: namespace before function call
    iota(cpu, out, value, step);
  }
}
