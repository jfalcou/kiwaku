//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/concepts/container.hpp>

namespace kwk
{
  template<typename Context>
  struct base_context
  {
    protected:
    Context&        self()        { return static_cast<Context&>(*this);       }
    Context const&  self() const  { return static_cast<Context const&>(*this); }

    public:

    static auto const&  in   (kwk::concepts::container auto const& c) { return c; }
    static auto&        out  (kwk::concepts::container auto& c)       { return c; }
    static auto&        inout(kwk::concepts::container auto& c)       { return c; }

    // template<typename Func, concepts::container In>
    // constexpr auto reduce(In const& in, Func f, auto init) const
    // {
    //   self().map ( [&](auto const& i) { init = f(init, i); }
    //                   , Context::in(in)
    //                   );
    //   return init;
    // }
  };
}
