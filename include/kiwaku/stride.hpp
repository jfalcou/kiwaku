//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_STRIDE_HPP_INCLUDED
#define KIWAKU_STRIDE_HPP_INCLUDED

#include <kiwaku/misc/stride.hpp>
#include <kiwaku/detail/options/stride_option.hpp>

namespace kwk
{
  //================================================================================================
  // Marker for unit stride
  //================================================================================================
  inline constexpr detail::unit_type const _1 = {};

  //================================================================================================
  // Imperative constructor
  //================================================================================================
  template<typename... V> constexpr auto with_stride(V... v) noexcept
  {
    return kwk::stride{v...};
  }
}

#endif
