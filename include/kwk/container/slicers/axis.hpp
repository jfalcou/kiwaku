//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <type_traits>
#include <cstddef>

namespace kwk::detail
{
  struct axis
  {
    axis& operator=(std::size_t s) { size = s; return *this;}
    std::size_t     dims;
    std::ptrdiff_t  size;
  };

  template<typename Shape>
  constexpr auto reshape( axis const& i, std::ptrdiff_t, Shape const& s ) noexcept
  {
    return s[i.dims];
  }

  inline constexpr auto origin( axis const& ) noexcept
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
