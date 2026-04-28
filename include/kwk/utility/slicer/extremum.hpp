//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/utility/fixed.hpp>
#include <kwk/utility/ratio.hpp>

namespace kwk
{
  template<typename Offset, typename Ratio> struct extremum_t
  {
    Offset o;
    Ratio r;

    constexpr auto offset() const noexcept { return o; }

    constexpr auto ratio() const noexcept { return r; }

    constexpr auto divisor() const noexcept { return r.denom; }

    constexpr auto factor() const noexcept { return r.num; }

    constexpr extremum_t(Offset of, Ratio ra) noexcept : o(of), r(ra) {}

    constexpr auto operator()(auto n) const noexcept { return (n * factor() + offset) / divisor(); }
  };

  template<typename O, typename R> extremum_t->extremum_t<O, R>;

  inline constexpr extremum_t end{fixed<0>, ratio{fixed<1>, fixed<1>}};
  inline constexpr extremum_t begin{fixed<0>, ratio{fixed<0>, fixed<1>}};
}
