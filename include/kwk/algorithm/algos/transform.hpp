//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <kwk/context/context.hpp>
#include <kwk/concepts/container.hpp>

namespace kwk
{
  template< typename Context, typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr void transform(Context& ctx,Func f, Out& out, C0&& c0, Cs&&... cs)
  {
    ctx.transform(f, out, KWK_FWD(c0), KWK_FWD(cs)...);
  }

  template< typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr void transform(Func f, Out& out, C0&& c0, Cs&&... cs)
  {
    transform(cpu, f, out, KWK_FWD(c0), KWK_FWD(cs)...);
  }
}
