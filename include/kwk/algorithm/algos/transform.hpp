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

namespace kwk
{
  // Transform is not a required part of contexts anymore
  template< typename Context, typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr void transform(Context& ctx, Func f, Out& out, C0 const& c0, Cs const&... cs)
  {
    ctx.map ( [f](auto& o, auto const& i0, auto const&... in) { o = f(i0, in...); }
            , ctx.out(out), ctx.in(c0), ctx.in(cs)...
            );
  }

  template< typename Context, typename Func, typename Out
          , typename In0, typename... Ins
          >
  constexpr void transform_proxy(Context& ctx, Func f, Out& out, In0& in0, Ins&... ins)
  {
    ctx.map ( [f](auto& o, auto const& i0, auto const&... in) { o = f(i0, in...); }
            , out, in0, ins...
            );
  }

  // Only takes one container, equivalent to using the same container with the stl,
  // and equivalent to: kwk::transform(ctx, f, view1, view1)
  // But since SYCL does not support multiple accessors to the same buffers,
  // I just used an inout accessor.
  template< typename Context, typename Func, concepts::container InOut>
  constexpr void transform_inplace(Context& ctx, Func f, InOut& inout)
  {
    ctx.map ( [f](auto& o) { o = f(o); }
            , ctx.inout(inout)
            );
  }

  template< typename Context, typename Func, typename InOut>
  constexpr void transform_inplace_proxy(Context& ctx, Func f, InOut& inout)
  {
    ctx.map ( [f](auto& o) { o = f(o); }
            , inout
            );
  }

  template< typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr void transform(Func f, Out& out, C0&& c0, Cs&&... cs)
  {
    kwk::transform(cpu, f, out, KWK_FWD(c0), KWK_FWD(cs)...);
  }
}
