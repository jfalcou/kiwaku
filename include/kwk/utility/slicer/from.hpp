//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/values.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/utility/end.hpp>
#include <kwk/utility/fixed.hpp>
#include <ostream>

namespace kwk
{
  template<auto Desc> struct shape;

  template<typename T> struct from
  {
    T value;
    constexpr from(T b) : value(b) {}

    friend std::ostream& operator<<(std::ostream& os, from f)
    {
      os << "from(";
      if constexpr(std::integral<decltype(f.value)>)  os << +f.value;
      else                                            os << f.value;
      return os<< ")";
    }
  };

  template<typename T> from(T) -> from<T>;

  template<auto Desc, typename T, std::size_t N>
  constexpr auto reshape(shape<Desc> const& sh, from<T> f, kumi::index_t<N> const&) noexcept
  {
    const auto result = [&]()
    {
      if constexpr(concepts::extremum<T>) return f.value.size(sh.template extent<N>());
      else                                return f.value;
    }();

    return sh.template extent<N>() - result;
  }
}
