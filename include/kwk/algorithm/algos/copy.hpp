//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/context/context.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{

  template<typename Context, concepts::container Out, concepts::container In>
  constexpr auto copy(Context& ctx, Out& out, In const& in)
  {
    kwk::transform(ctx, [](auto in) { return in; }, out, KWK_FWD(in) );
  }

  template<concepts::container Out, concepts::container In>
  constexpr auto copy(Out& out, In const& in)
  {
    kwk::copy(cpu, out, in);
  }


  template<typename Context, typename Func, concepts::container Out, concepts::container In>
  constexpr auto copy_if(Context& ctx, Func f, Out& out, In const& in)
  {
    // kwk::for_each(ctx, [&](auto... is) { if(f(in(is...))) out(is...) = KWK_FWD(in)(is...); }, out.shape() );
    ctx.map([f](auto& o, auto const& i) { if(f(i)) o = i; }, ctx.out(out), ctx.in(in));
  }

  template<typename Func, concepts::container Out, concepts::container In>
  constexpr auto copy_if(Func f, Out& out, In const& in)
  {
    kwk::copy_if(cpu, f, out, in);
  }
}
