//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/context/base.hpp>
#include <kwk/detail/algorithm/for_each.hpp>

namespace kwk
{
  struct cpu_context : base_context<cpu_context>
  {
    template<typename Func, auto... S>
    constexpr auto map(Func f, shape<S...> const& shp) const
    {
      return __::for_each(f, shp );
    }

    template<typename Func, concepts::container C0, concepts::container... Cs>
    constexpr auto map(Func f, C0&& c0, Cs&&... cs) const
    {
      this->map([&](auto... is) { return f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, c0.shape() );
      return f;
    }

    template<typename Func, concepts::container Container>
    constexpr auto map_index(Func f, Container&& c) const
    {
      this->map([&](auto... is) { return f(KWK_FWD(c)(is...), is...); }, c.shape());
      return f;
    }
  };

  inline constexpr cpu_context cpu = {};

}
