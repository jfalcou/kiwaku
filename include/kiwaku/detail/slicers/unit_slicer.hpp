//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_SLICERS_UNIT_SLICER_HPP_INCLUDED
#define KIWAKU_DETAIL_SLICERS_UNIT_SLICER_HPP_INCLUDED

#include <type_traits>
#include <cstddef>

namespace kwk::detail
{
  struct unit_slicer
  {
    std::ptrdiff_t begin, end;
  };

  template<typename Shape>
  constexpr auto reshape( unit_slicer const& i, std::ptrdiff_t, Shape const& ) noexcept
  {
    return (i.end - i.begin) + 1;
  }

  inline constexpr auto origin( unit_slicer const& i ) noexcept
  {
    return i.begin;
  }

  template<typename Stride>
  inline auto restride( unit_slicer const&, std::ptrdiff_t i, Stride const& s) noexcept
  {
    return s[i];
  }

  // Traits for concepts
  template<typename>      struct is_unit_slicer;
  template<typename,int>  struct is_contiguous_slicer;

  template<>      struct is_unit_slicer<unit_slicer>          : std::true_type  {};
  template<int D> struct is_contiguous_slicer<unit_slicer,D>  : std::false_type {};
}

#endif
