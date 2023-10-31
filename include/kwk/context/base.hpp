//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{

template<typename Context>
struct base_context
{
  protected:
  Context&        self()        { return static_cast<Context&>(*this);       }
  Context const&  self() const  { return static_cast<Context const&>(*this); }

  public:

  template< typename Func, concepts::container Out
          , concepts::container C0, concepts::container... Cs
          >
  constexpr auto transform(Func f, Out& out, C0&& c0, Cs&&... cs) const
  {
    self().for_each([&](auto... is) { out(is...) = f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, out.shape() );
  }

  template<typename Func, concepts::container In>
  constexpr auto reduce(In const& in, Func f, auto init) const
  {
    self().for_each([&](auto... is) { init = f(init, in(is...)); }, in.shape() );
    return init;
  }

};

}
