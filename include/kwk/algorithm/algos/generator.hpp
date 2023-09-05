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
  template<concepts::container Out>
  constexpr auto fill(Out& out, auto value)
  {
    kwk::for_each([&](auto... is) { out(is...) = value; }, out.shape() );
  }

  template<typename Func, concepts::container Out>
  constexpr auto generate(Func f, Out& out)
  {
    kwk::for_each([&](auto... is) { out(is...) = f(is...); }, out.shape() );
  }

  template<concepts::container Out>
  constexpr auto iota(Out& out, auto value)
  {
    kwk::for_each([&](auto... is) { out(is...) = value++; }, out.shape() );
  }

  template<concepts::container Out>
  constexpr auto iota(Out& out)
  {
    using type = typename Out::value_type;
    kwk::iota(out,type{});
  }

  template<concepts::container Out>
  constexpr auto iota(Out& out, auto value, auto step)
  {
    kwk::for_each([&](auto... is) { out(is...) = value; value+=step; }, out.shape() );
  }
}
