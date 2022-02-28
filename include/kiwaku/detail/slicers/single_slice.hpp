//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_SLICERS_SINGLE_INDEX_HPP_INCLUDED
#define KIWAKU_DETAIL_SLICERS_SINGLE_INDEX_HPP_INCLUDED

#include <type_traits>
#include <cstddef>

namespace kwk::detail
{
  template<typename Shape>
  constexpr auto reshape( std::ptrdiff_t const&, std::ptrdiff_t, Shape const& ) noexcept
  {
    return 1;
  }

  inline constexpr auto origin( std::ptrdiff_t const& i ) noexcept
  {
    return i;
  }

  template<typename Stride>
  inline auto restride( std::ptrdiff_t const& , std::ptrdiff_t i, Stride const& s) noexcept
  {
    return s[i];
  }

  // Traits for concepts
  template<typename>      struct is_unit_slicer;
  template<typename,int>  struct is_contiguous_slicer;

  template<typename T>  struct is_unit_slicer : std::is_integral<T> {};

  template<typename T, int D>
  struct is_contiguous_slicer : std::bool_constant<std::is_integral_v<T> && (D>1)> {};
}

#endif
