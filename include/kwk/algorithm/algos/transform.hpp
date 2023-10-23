//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/context/context.hpp>
#include <kwk/concepts/container.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{
  template< typename Context, typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr auto transform(Context& ctx, Func f, Out& out, C0&& c0, Cs&&... cs)
  {
    ctx.transform(f, out, KWK_FWD(c0), KWK_FWD(cs)...);
  }

  template< typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr auto transform(Func f, Out& out, C0&& c0, Cs&&... cs)
  {
    // kwk::for_each([&](auto... is) { out(is...) = f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, out.shape() );
    kwk::transform(cpu, f, out, KWK_FWD(c0), KWK_FWD(cs)...);
  }
  
}
