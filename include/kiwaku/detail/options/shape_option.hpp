//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_OPTIONS_SHAPE_OPTION_HPP_INCLUDED
#define KIWAKU_DETAIL_OPTIONS_SHAPE_OPTION_HPP_INCLUDED

#include <kiwaku/detail/options/stride_option.hpp>
#include <kiwaku/detail/options/options.hpp>
#include <kiwaku/misc/shape.hpp>
#include <utility>
#include <cstddef>

namespace kwk::detail
{
  template<std::size_t Dimensions> struct shape_option
  {
    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using shape_type  = shape<Dimensions>;
    using stride_type = typename shape_type::stride_type;
    using option_tag = shape_tag;

    static constexpr bool         is_dynamic  = true;
    static constexpr std::size_t  static_size = Dimensions;

    constexpr auto as_stride() const noexcept
    {
      return stride_option<Dimensions,detail::index_list<0>>{};
    }

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
