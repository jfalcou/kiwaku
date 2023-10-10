//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/context/context.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{
  template<typename Context, concepts::container Out, concepts::container In>
  constexpr auto copy(Context const& ctx, Out& out, In&& in)
  {
    ctx.copy(out, KWK_FWD(in));
    // kwk::transform([](auto in) { return in; }, out, KWK_FWD(in) );
  }

  template<concepts::container Out, concepts::container In>
  constexpr auto copy(Out& out, In&& in)
  {
    kwk::copy(cpu, out, KWK_FWD(in));
  }

  template<typename Context, typename Func, concepts::container Out, concepts::container In>
  constexpr auto copy_if(Context const& ctx, Func f, Out& out, In&& in)
  {
    ctx.copy_if(f, out, in);
    // kwk::for_each([&](auto... is) { if(f(in(is...))) out(is...) = KWK_FWD(in)(is...); }, out.shape() );
  }

  template<typename Func, concepts::container Out, concepts::container In>
  constexpr auto copy_if(Func f, Out& out, In&& in)
  {
    copy_if(cpu, f, out, in);
  }
}
