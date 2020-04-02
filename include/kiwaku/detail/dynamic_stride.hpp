//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_DYNAMIC_STRIDE_HPP_INCLUDED
#define KIWAKU_DETAIL_DYNAMIC_STRIDE_HPP_INCLUDED

#include <kiwaku/misc/unit_stride.hpp>
#include <utility>
#include <cstddef>

namespace kwk::detail
{
  template<std::size_t Dimensions> struct dynamic_stride_option
  {
    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using stride_type = unit_stride<Dimensions>;
    using option_tag  = stride_option;

    static constexpr bool is_dynamic_option   = true;
    static constexpr bool is_unit_stride      = true;
    static constexpr std::size_t static_size  = Dimensions;

    //==============================================================================================
    // Pseudo-constructor
    //==============================================================================================
    template<typename... D>
    constexpr auto operator()(D... s) const noexcept requires requires { stride_type{s...}; }
    {
      return stride_type{s...};
    }
  };
}

#endif
