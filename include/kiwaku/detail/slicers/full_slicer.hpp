//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_SLICERS_FULL_SLICER_HPP_INCLUDED
#define KIWAKU_DETAIL_SLICERS_FULL_SLICER_HPP_INCLUDED

#include <kiwaku/detail/slicers/unit_slicer.hpp>
#include <kiwaku/detail/slicers/slicer.hpp>
#include <kiwaku/detail/slicers/axis.hpp>
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

    constexpr auto operator[](std::ptrdiff_t d) const noexcept
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

#endif
