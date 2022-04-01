//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <type_traits>
#include <cstddef>

namespace kwk::detail
{
  struct slicer { std::ptrdiff_t begin, step, end; };

  template<typename Shape>
  constexpr auto reshape( slicer const& i, std::ptrdiff_t, Shape const&) noexcept
  {
    return (i.end - i.begin)/i.step + 1;
  }

  inline constexpr auto origin( slicer const& i ) noexcept
  {
    return i.begin;
  }

  template<typename Stride>
  inline auto restride( slicer const& id, std::ptrdiff_t i, Stride const& s) noexcept
  {
    return s[i]*id.step;
  }

  // Traits for concepts
  template<typename>      struct is_unit_slicer;
  template<typename,int>  struct is_contiguous_slicer;

  template<>      struct is_unit_slicer<slicer>         : std::false_type {};
  template<int D> struct is_contiguous_slicer<slicer,D> : std::false_type {};
}
