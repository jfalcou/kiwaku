//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/container/slicers/unit_slicer.hpp>
#include <kwk/container/slicers/slicer.hpp>
#include <kwk/container/slicers/axis.hpp>
#include <type_traits>
#include <cstddef>

namespace kwk::detail
{
  struct full_slicer
  {
    constexpr auto operator()(std::ptrdiff_t b, std::ptrdiff_t e) const noexcept
    {
      return unit_slicer{b,e};
    }

    constexpr auto operator()(std::ptrdiff_t b, std::ptrdiff_t s, std::ptrdiff_t e) const noexcept
    {
      return slicer{b,s,e};
    }

    constexpr auto operator[](std::size_t d) const noexcept
    {
      return axis{d,0};
    }
  };

  template<typename Shape>
  constexpr auto reshape( full_slicer const& i, std::ptrdiff_t, Shape const& s ) noexcept
  {
    return s[i];
  }

  inline constexpr auto origin( full_slicer const& ) noexcept
  {
    return 0;
  }

  template<typename Stride>
  inline auto restride( full_slicer const&, std::ptrdiff_t i, Stride const& s) noexcept
  {
    return s[i];
  }

  // Traits for concepts
  template<typename>      struct is_unit_slicer;
  template<typename,int>  struct is_contiguous_slicer;

  template<>      struct is_unit_slicer<full_slicer>          : std::true_type {};
  template<int D> struct is_contiguous_slicer<full_slicer,D>  : std::true_type {};
}
