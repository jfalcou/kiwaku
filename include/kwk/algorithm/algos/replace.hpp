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

namespace kwk
{
  template<typename Context, typename Func, concepts::container Out>
  constexpr void replace_if(Context& ctx, Out& out, Func f, auto new_value)
  {
    ctx.for_each([=](auto& o) { if(f(o)) o = new_value; }, Context::inout(out));
  }
  template<typename Func, concepts::container Out>
  constexpr void replace_if(Out& out, Func f, auto new_value)
  {
    replace_if(cpu, out, f, new_value);
  }

  template<typename Context, concepts::container Out>
  constexpr void replace(Context& ctx, Out& out, auto old_value, auto new_value)
  {
    replace_if(ctx, out, [old_value](auto const& x) { return x == old_value; }, new_value);
  }
  template<concepts::container Out>
  constexpr void replace(Out& out, auto old_value, auto new_value)
  {
    replace(cpu, out, old_value, new_value);
  }
}
