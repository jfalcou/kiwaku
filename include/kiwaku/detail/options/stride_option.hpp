//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_OPTIONS_STRIDE_OPTION_HPP_INCLUDED
#define KIWAKU_DETAIL_OPTIONS_STRIDE_OPTION_HPP_INCLUDED

#include <kiwaku/misc/stride.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <utility>
#include <cstddef>

namespace kwk::detail
{
  template<std::size_t Dimensions, typename IL> struct stride_option
  {
    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using stride_type = stride<Dimensions, index_list<0>>;

    static constexpr bool is_dynamic          = true;
    static constexpr bool is_unit             = true;
    static constexpr bool is_explicit         = false;
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

  //================================================================================================
  // RBR option global tag
  //================================================================================================
  struct stride_tag;
}

//================================================================================================
// Register a RBR keyword
//================================================================================================
namespace kwk::option
{
  inline constexpr auto stride  = ::rbr::keyword<kwk::detail::stride_tag>;
}

namespace rbr
{
  //================================================================================================
  // Register as RBR option
  //================================================================================================
  template<std::size_t D, typename IL>
  struct tag<kwk::detail::stride_option<D,IL>> : tag<kwk::detail::stride_tag> {};
}

#endif
