//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/fixed.hpp>
#include <ostream>

namespace kwk
{
  template<auto Desc> struct shape;

  template<typename T> struct to
  {
    T value;
    constexpr to(T e) : value(e) {}

    friend std::ostream& operator<<(std::ostream& os, to t)
    {
      os << "to(";
      if constexpr(std::integral<decltype(t.value)>)  os << +t.value;
      else                                            os <<  t.value;
      return os<< ")";
    }
  };

  template<typename T> to(T) -> to<T>;

  template<auto Desc, typename T, std::size_t N>
  constexpr auto reshape( [[maybe_unused]] shape<Desc> const& sh
                        , to<T> t
                        , kumi::index_t<N> const&
                        ) noexcept
  {
    auto result = [&]()
    {
      if constexpr(concepts::extremum<T>) return t.value.size(sh.template extent<N>());
      else                                return t.value;
    }();

    return result;
  }
}
