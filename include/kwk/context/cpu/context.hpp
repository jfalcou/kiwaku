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
#include <kwk/detail/algorithm/for_until.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

namespace kwk
{
  struct cpu_context : base_context<cpu_context>
  {
    template<typename Func, auto... S>
    constexpr auto for_each(Func f, shape<S...> const& shp) const
    {
      return __::for_each(f, shp );
    }

    template<typename Func, concepts::container C0, concepts::container... Cs>
    constexpr auto for_each(Func f, C0&& c0, Cs&&... cs) const
    {
      this->for_each([&](auto... is) { return f(KWK_FWD(c0)(is...), KWK_FWD(cs)(is...)...); }, c0.shape() );
      return f;
    }

    template<typename Func, concepts::container Container>
    constexpr auto for_each_index(Func f, Container&& c) const
    {
      this->for_each ( [&](auto... is) { return f(KWK_FWD(c)(is...), is...); }
                    , c.shape()
                    );
      return f;
    }
  };

  inline constexpr cpu_context cpu = {};

}
