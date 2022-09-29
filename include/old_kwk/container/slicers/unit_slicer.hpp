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
