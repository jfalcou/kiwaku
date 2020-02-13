//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_DYNAMIC_SHAPE_HPP_INCLUDED
#define KIWAKU_DETAIL_DYNAMIC_SHAPE_HPP_INCLUDED

#include <kiwaku/misc/shape.hpp>
#include <utility>
#include <cstddef>

namespace kwk::detail
{
  template<std::size_t Dimensions> struct dynamic_shape_option
  {
    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using shape_type = shape<Dimensions>;
    static constexpr bool is_dynamic_option = true;

    //==============================================================================================
    // Pseudo-constructor
    //==============================================================================================
    template<typename... D>
    constexpr auto operator()(D... s) const noexcept requires requires { shape_type{s...}; }
    {
      return shape_type{s...};
    }

    constexpr std::ptrdiff_t numel() const noexcept { return -1;}
  };
}

#endif
