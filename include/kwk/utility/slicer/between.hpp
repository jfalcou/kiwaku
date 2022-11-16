//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/kumi.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/traits/extent.hpp>
#include <ostream>

namespace kwk
{
  template<auto Desc> struct shape;

  template<typename B, typename E> struct between
  {
    B begin;
    E end;
    between(B b, E e) : begin(b), end(e) {}

    friend std::ostream& operator<<(std::ostream& os, between b)
    {
      return os << "between(" << +b.begin << ", " << +b.end << ")";
    }
  };

  template<typename B, typename E> between(B, E) -> between<B, E>;

  template<auto Desc, typename B, typename E, std::size_t N>
  constexpr auto reshape( [[maybe_unused]] shape<Desc> const& sh
                        , between<B, E> b
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    if constexpr (requires{ B::value; } && requires{ E::value; })
    {
      return fixed<E::value - B::value>;
    }
    else
    {
      return b.end - b.begin;
    }
  }
}
