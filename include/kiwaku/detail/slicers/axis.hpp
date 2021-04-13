//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_SLICERS_AXIS_HPP_INCLUDED
#define KIWAKU_DETAIL_SLICERS_AXIS_HPP_INCLUDED

#include <type_traits>
#include <cstddef>

namespace kwk::detail
{
  struct axis
  {
    axis& operator=(std::ptrdiff_t s) { size = s; return *this;}
    std::ptrdiff_t dims, size;
  };

  template<typename Shape>
  constexpr auto reshape( axis const& i, std::ptrdiff_t, Shape const& s ) noexcept
  {
    return s[i.dims];
  }

  inline constexpr auto origin( axis const& i ) noexcept
  {
    return 0;
  }

  template<typename Stride>
  inline auto restride( axis const&, std::ptrdiff_t i, Stride const& s) noexcept
  {
    return s[i];
  }

  // Traits for concepts
  template<typename>      struct is_unit_slicer;
  template<typename,int>  struct is_contiguous_slicer;

  template<>      struct is_unit_slicer<axis>         : std::false_type {};
  template<int D> struct is_contiguous_slicer<axis,D> : std::false_type {};
}

#endif
