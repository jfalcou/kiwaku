//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/misc/stride.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <utility>
#include <cstddef>

namespace kwk
{
  namespace detail
  {
    //============================================================================================
    // NTTP Indirect interface
    //============================================================================================
    template<std::size_t Dimensions, typename IL> struct stride_option
    {
      using stride_type = stride<Dimensions, index_list<0>>;

      static constexpr bool is_dynamic          = true;
      static constexpr bool is_unit             = true;
      static constexpr bool is_explicit         = false;
      static constexpr std::size_t static_size  = Dimensions;

      //============================================================================================
      // Pseudo-constructor
      //============================================================================================
      template<typename... D>
      constexpr auto operator()(D... s) const noexcept requires requires { stride_type{s...}; }
      {
        return stride_type{s...};
      }
    };
  }

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

//================================================================================================
// Register a RBR keyword
//================================================================================================
namespace kwk
{
  namespace detail { struct stride_tag; }
  namespace option { inline constexpr auto stride  = ::rbr::keyword<kwk::detail::stride_tag>; }
}

namespace rbr
{
  template<std::size_t D, typename IL>
  struct tag<kwk::detail::stride_option<D,IL>> : tag<kwk::detail::stride_tag> {};

  template<std::size_t D, typename IL>
  struct tag<kwk::stride<D,IL>> : tag<kwk::detail::stride_tag> {};
}
