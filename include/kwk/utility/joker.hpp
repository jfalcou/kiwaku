//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <ostream>

namespace kwk
{
  struct axis
  {
    axis& operator=(std::size_t s) { size = s; return *this;}
    std::size_t     dims;
    std::ptrdiff_t  size;
  };

  struct joker
  {
    friend std::ostream& operator<<(std::ostream& os, joker const&) { return os << "_"; }
    constexpr auto operator[](std::size_t d) const noexcept { return axis{d,0}; }
  };

  template<typename T>  struct is_joker               : std::false_type {};
  template<>            struct is_joker<joker>        : std::true_type {};
  template<>            struct is_joker<joker&>       : std::true_type {};
  template<>            struct is_joker<joker const>  : std::true_type {};
  template<>            struct is_joker<joker const&> : std::true_type {};

  template<typename T>  inline constexpr bool is_joker_v = is_joker<T>::value;

  inline constexpr joker _ = {};
}


