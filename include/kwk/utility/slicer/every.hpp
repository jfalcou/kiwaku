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
#include <ostream>

namespace kwk
{
  template<auto Desc> struct shape;

  template<typename T> struct every
  {
    T value;
    constexpr every(T b) : value(b) {}

    friend std::ostream& operator<<(std::ostream& os, every b)
    {
      return os << "every(" << +b.value << ")";
    }
  };

  template<typename T> every(T) -> every<T>;

  template<auto Desc, typename T, std::size_t N>
  constexpr auto reshape( shape<Desc> const& sh
                        , every<T> e
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    auto fix = [](auto v, auto n) { return (v % n != 0) + v/n; };

    if constexpr (is_static_extent_v<N,Desc> && concepts::static_constant<T>)
      return fixed<(get<N>(Desc) == 0) ? 0 : fix(get<N>(Desc), T::value)>;
    else
      return (get<N>(sh) == 0) ? 0 : fix(get<N>(sh), e.value);
  }
}
