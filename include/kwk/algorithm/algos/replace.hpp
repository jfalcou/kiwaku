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

namespace kwk
{
  template<typename Context, concepts::container Out>
  constexpr auto replace(Context& ctx, Out& out, auto old_value, auto new_value)
  {
    for_each(ctx, [&](auto... is) { if(out(is...) == old_value) KWK_FWD(out)(is...) = new_value; }, out.shape() );
  }
  template<concepts::container Out>
  constexpr auto replace(Out& out, auto old_value, auto new_value)
  {
    replace(cpu, out, old_value, new_value);
  }

  template<typename Context, typename Func, concepts::container Out>
  constexpr auto replace_if(Context& ctx, Out& out, Func f, auto new_value)
  {
    for_each(ctx, [&](auto... is) { if(f(out(is...))) KWK_FWD(out)(is...) = new_value; }, out.shape() );
  }
  template<typename Func, concepts::container Out>
  constexpr auto replace_if(Out& out, Func f, auto new_value)
  {
    replace_if(cpu, out, f, new_value);
  }
}
