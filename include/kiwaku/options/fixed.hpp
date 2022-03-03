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
  namespace detail
  {
    template<typename T>      struct  to_int { using type = T; };
    template<typename T, T N> struct  to_int<std::integral_constant<T,N>> { using type = T; };
    template<typename T>      using   to_int_t = typename to_int<T>::type;
  }

  /**
    @ingroup utility
    @brief Provides a short-cut to define a `std::integral_constant`instance from a literal integer
  **/
  template<auto N>
  inline constexpr std::integral_constant<decltype(N),N> fixed = {};

  namespace literals
  {
    using kumi::literals::b10;

    /**
      @ingroup utility
      @brief User-defined literal suffix for compile-time constant
    **/
    template<char... c> constexpr auto operator"" _c() noexcept { return fixed<b10<c...>()>; }
  }
}
