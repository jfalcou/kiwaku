//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <type_traits>
#include <kiwaku/detail/kumi.hpp>

namespace kwk
{
  template<auto N>
  inline constexpr std::integral_constant<decltype(N),N> fixed = {};

  namespace literals
  {
    using kumi::literals::b10;
    template<char... c> constexpr auto operator"" _c() noexcept { return fixed<b10<c...>()>; }
  }
}
